/***************************************************//**
 * @file    NativeUSBMacOSX.c
 * @date    June 2011
 * @author  Ocean Optics, Inc.
 *
 * This is an implementation of the USB interface for
 * MacOSX.
 *
 * LICENSE:
 *
 * SeaBreeze Copyright (C) 2014, Ocean Optics Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *******************************************************/

#define __CARBONSOUND_R__
#define __CARBONSOUND__

#include "common/globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mach/mach.h>
#include <CoreFoundation/CFNumber.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/usb/usb.h>

#include "native/usb/NativeUSB.h"

/* Constants and macro definitions */
#define MAX_USB_DEVICES                127  /* As per USB spec */
#define SET_ERROR_CODE(code) do { if(NULL != errorCode) { *errorCode = code; }  } while(0)

typedef IOUSBDeviceInterface197 cIOUSBDeviceInterface;

/* struct definitions */
typedef struct {
    long deviceID;  /* Unique ID for device.  Assigned by this driver */
    io_service_t usbDeviceRef;
    cIOUSBDeviceInterface **dev;
    IOUSBInterfaceInterface **intf;
    IOUSBConfigurationDescriptorPtr confDesc;
    unsigned char endpointCount;       /* Length of pipeToEndpointMap */
    unsigned char *pipeToEndpointMap;  /* Index = pipe, value = endpoint */
} __usb_interface_t;

typedef struct {
    long deviceID;  /* Unique ID for device.  Assigned by this driver. */
    __usb_interface_t *handle;    /* Pointer to USB interface instance */
    long busLocation;      /* Reported by IOKit for a particular device */
    unsigned short vendorID;
    unsigned short productID;
    unsigned char valid;    /* Whether this struct is valid */
    unsigned char mark;     /* Used to determine if device is still present */
} __device_instance_t;

/* Global variables (mostly static lookup tables) */
static __device_instance_t __enumerated_devices[MAX_USB_DEVICES] = { { 0 } };
static int __enumerated_device_count = 0;   /* To keep linear searches short */
static long __last_assigned_deviceID = 0;   /* To keep device IDs unique */

/* Function prototypes */
void __clear_halt(cIOUSBDeviceInterface **dev, unsigned char endpoint);
int __get_endpoint(__usb_interface_t *handle, int pipe_index);
__device_instance_t *__lookup_device_instance_by_ID(long deviceID);
__device_instance_t *__lookup_device_instance_by_location(long busLocation);
__device_instance_t *__add_device_instance(long busLocation,
                                           int vendorID, int productID);
void __purge_unmarked_device_instances(int vendorID, int productID);
void __close_and_dealloc_usb_interface(__usb_interface_t *usb);
void __setup_endpoint_map(__usb_interface_t *usb);
unsigned char __get_pipe_for_endpoint(__usb_interface_t *usb, unsigned char ep);

/* This function will iterate over the known devices and attempt to match
 * the given ID.  It might be more efficient for the sake of this search
 * to store the device instances in a linked list so that no invalid elements
 * need to be searched through, but that just has the effect of shifting the
 * time burden to when a device must be removed from the list.  This lookup
 * is most likely to be called when the calling code is trying to open a device,
 * at which point they more or less expect to have to wait a little while.
 * Not that searching over this array will take very long...
 */
__device_instance_t *__lookup_device_instance_by_ID(long deviceID) {
    int i;
    int valid;
    /* The __enumerated_device_count is used to end the search once it is
     * known that there are no more devices to be found.
     */
    for(    i = 0, valid = 0;
        i < MAX_USB_DEVICES && valid < __enumerated_device_count;
        i++) {
        if(0 != __enumerated_devices[i].valid) {
            if(__enumerated_devices[i].deviceID == deviceID) {
                return &(__enumerated_devices[i]);
            }
            valid++;
        }
    }
    return NULL;
}

__device_instance_t *__lookup_device_instance_by_location(long busLocation) {
    int i;
    int valid;
    /* The __enumerated_device_count is used to end the search once it is
     * known that there are no more devices to be found.
     */
    for(    i = 0, valid = 0;
        i < MAX_USB_DEVICES && valid < __enumerated_device_count;
        i++) {
        if(0 != __enumerated_devices[i].valid) {
            if(__enumerated_devices[i].busLocation == busLocation) {
                return &(__enumerated_devices[i]);
            }
            valid++;
        }
    }
    return NULL;
}

__device_instance_t *__add_device_instance(long busLocation,
                                           int vendorID, int productID) {
    int i;

    /* First need to find an empty slot to store this device descriptor */
    for(i = 0; i < MAX_USB_DEVICES; i++) {
        if(0 == __enumerated_devices[i].valid) {
            /* Found an empty slot */
            __enumerated_devices[i].valid = 1;
            __enumerated_devices[i].deviceID = __last_assigned_deviceID++;
            __enumerated_devices[i].busLocation = busLocation;
            __enumerated_devices[i].vendorID = vendorID;
            __enumerated_devices[i].productID = productID;
            __enumerated_device_count++;
            return &(__enumerated_devices[i]);
        }
    }
    return NULL;
}

void __purge_unmarked_device_instances(int vendorID, int productID) {
    int new_count = 0;
    int valid = 0;
    int i;
    __device_instance_t *device;

    for(i = 0; i < MAX_USB_DEVICES && valid < __enumerated_device_count; i++) {
        device = &(__enumerated_devices[i]);
        if(0 == device->valid) {
            continue;
        }
        /* From here down, the device entry was thought to be valid */
        valid++;

        /* Now check whether it was marked as still being present.  Note that
         * this search is limited just to the type of device that was being
         * probed when this call was made -- otherwise, any devices that were
         * not being probed for just now would be unmarked and would be
         * discarded.
         */
        if(0 == device->mark
           && (vendorID == device->vendorID)
           && (productID == device->productID)) {
            /* Not marked, so it needs to be purged */
            if(NULL != device->handle) {
                /* Clean up the device since it seems to have been disconnected */
                __close_and_dealloc_usb_interface(device->handle);
            }
            /* Wipe the structure completely */
            memset(&__enumerated_devices[i], (int)0, sizeof(__device_instance_t));
        } else {
            /* Device was valid or a different type so it survived this pass */
            __enumerated_devices[i].mark = 0;
            /* Keep track of how many survived */
            new_count++;
        }
    }
    __enumerated_device_count = new_count;
}

/* This will take an open USB device and read out the endpoint descriptors
 * to determine what the mapping of endpoints to pipes is for the device.
 */
void __setup_endpoint_map(__usb_interface_t *usb) {
    unsigned char count = 0;
    unsigned int i;
    IOReturn flag;

    flag = (*usb->intf)->GetNumEndpoints(usb->intf, &count);
    if(kIOReturnSuccess != flag) {
        usb->endpointCount = 0;
        usb->pipeToEndpointMap = NULL;
        return;
    }

    usb->endpointCount = count;
    usb->pipeToEndpointMap = (unsigned char *)calloc(count, sizeof(unsigned char));
    for(i = 0; i < count; i++) {
        usb->pipeToEndpointMap[i] = __get_endpoint(usb, i);
    }
}

/* This will determine the pipe that is associated with the given
 * endpoint number.  This uses the map that is created by
 * __setup_endpoint_map().
 */
unsigned char __get_pipe_for_endpoint(__usb_interface_t *usb, unsigned char ep) {
    int i;

    if(NULL == usb || NULL == usb->pipeToEndpointMap) {
        return 0xFF;
    }

    for(i = 0; i < usb->endpointCount; i++) {
        if(usb->pipeToEndpointMap[i] == ep) {
            return i + 1;
        }
    }

    return 0xFF;
}

/* This is a helper function to look up the endpoint number of a given "pipe",
 * where the "pipe" is simply the index of the endpoint.  This is needed to
 * translate between the endpoint number being passed down from above and the
 * pipe numbers needed by IOKit.
 */
int __get_endpoint(__usb_interface_t *usb, int pipe_index) {
    unsigned char direction;
    unsigned char transferType;
    unsigned char number;
    unsigned short maxPacketSize;
    unsigned char interval;
    IOReturn flag;

    flag = (*usb->intf)->GetPipeProperties(usb->intf, pipe_index + 1,
                                           &direction, &number, &transferType,
                                           &maxPacketSize, &interval);

    if(kIOReturnSuccess != flag) {
        return -1;
    }

    if(1 == direction) {
        number += 0x80;
    }

    return number;
}

/* This will attempt to free up all resources associated with an open
 * USB descriptor.  This also deallocates the provided pointer.
 */
void __close_and_dealloc_usb_interface(__usb_interface_t *usb) {
    if(NULL == usb) {
        return;
    }

    if(NULL != usb->intf) {
        (*usb->intf)->USBInterfaceClose(usb->intf);
        (*usb->intf)->Release(usb->intf);
    }
    if(NULL != usb->dev) {
        (*usb->dev)->USBDeviceClose(usb->dev);
        (*usb->dev)->Release(usb->dev);
    }

    IOObjectRelease(usb->usbDeviceRef);
    /* FIXME: does this need to release handle->confDesc? */

    if(NULL != usb->pipeToEndpointMap) {
        free(usb->pipeToEndpointMap);
    }

    free(usb);
}

int
USBProbeDevices(int vendorID, int productID, unsigned long *output,
                int max_devices) {
    /* Local variables */
    mach_port_t masterPort;
    io_service_t usbDeviceRef;
    IOCFPlugInInterface **iodev = NULL;
    cIOUSBDeviceInterface **dev = NULL;
    kern_return_t err;
    CFMutableDictionaryRef matchingDictionary = 0;
    SInt32 idVendor;
    SInt32 idProduct;
    SInt32 score;
    CFNumberRef numberRef;
    io_iterator_t iterator = 0;
    int i = 0;
    int matched;
    int valid = 0;
    __device_instance_t *instance;
    UInt32 busLocation = 0;

    idVendor = vendorID;
    idProduct = productID;

    /* Attempt to create a master port; throw an IOException if we can't */
    if(IOMasterPort(MACH_PORT_NULL, &masterPort) != kIOReturnSuccess) {
        return -1;
    }

    /* Attempt to create a matching dictionary; throw an IOException if we can't */
    matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);
    if(NULL == matchingDictionary) {
        /* Failed to get the dictionary, so clean up and bail out. */
        goto error1;
    }

    /* Attempt to create a vendor reference; throw an exception if we can't */
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type,
                               &idVendor);
    if(NULL == numberRef) {
        /* Failed to get vendor reference, so clean up and bail out. */
        goto error1;
    }

    /* Attempt to create a reference; throw an IOException if we can't */
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBVendorID), numberRef);
    CFRelease(numberRef);
    numberRef = 0;
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type,
                               &idProduct);
    if(NULL == numberRef) {
        /* Failed to get product reference, so clean up and bail out */
        goto error1;
    }

    /* Attempt to create an iterator; throw an IOException if we can't */
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBProductID), numberRef);
    CFRelease(numberRef);
    numberRef = 0;
    if(kIOReturnSuccess != IOServiceGetMatchingServices(masterPort,
                                                        matchingDictionary,
                                                        &iterator)) {
        /* Failed to get an iterator */
        goto error1;
    }

    if(0 == iterator) {
        /* No devices found */
        goto error2;    /* FIXME: does this try to release a null iterator? */
    }

    /* Search using the iterator for the indicated device */
    while(0 != (usbDeviceRef = IOIteratorNext(iterator))) {
        /* The iterator contains all devices matching the VID and PID.
         * Go through these and determine whether they are already known
         * (i.e. they exist in the cache) and if not then add them to the cache.
         * This will then need to build an array of longs out of the cache.
         * Note that every device on the bus is assigned a unique location
         * as an integer by IOKit, and this will be used to distinguish between
         * multiple instances of the same kind of device.
         */

        /* Try to create an interface for the device.  This is necessary to
         * determine its unique location on the bus.
         */
        err = IOCreatePlugInInterfaceForService(usbDeviceRef,
                                                kIOUSBDeviceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &iodev, &score);
        if(0 != err || NULL == iodev) {
            /* Cannot create plugin interface for service */
            goto error3;
        }
        err = (*iodev)->QueryInterface(iodev,
                                       CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID),
                                       (LPVOID*)&dev);
        if(0 != err || NULL == dev) {
            /* Cannot query interface */
            goto error4;
        }

        /* Get the location ID from the operating system.  This is unique
         * for a particular spot on the bus.
         */
        if(kIOReturnSuccess != (*dev)->GetLocationID(dev, &busLocation)) {
            /* Got an unexpected error, so bail out. */
            goto error5;
        }

        /* Try to match the location with the set of devices already known */
        instance = __lookup_device_instance_by_location(busLocation);
        if(NULL == instance) {
            /* This device was not in the cache so it must be added */
            instance = __add_device_instance(busLocation, vendorID, productID);
            if(NULL == instance) {
                /* Could not add the device -- this should not be possible,
                 * but fail gracefully regardless.
                 */
                goto error5;
            }
        }
        instance->mark = 1;     /* Preserve this since it was just seen */

        /* Free up the resources that were needed to get this far for this device */
        (*dev)->Release(dev);
        (*iodev)->Release(iodev);
        IOObjectRelease(usbDeviceRef);
    }

    IOObjectRelease(iterator);
    iterator = 0;

    /* Purge any devices that are cached but that no longer exist. */
    __purge_unmarked_device_instances(vendorID, productID);

    /* Count up how many of this type of device are known */
    for(    i = 0, matched = 0, valid = 0;
        i < MAX_USB_DEVICES && valid < __enumerated_device_count;
        i++) {
        if(0 != __enumerated_devices[i].valid) {
            valid++;
            if(__enumerated_devices[i].vendorID == vendorID
                    && __enumerated_devices[i].productID == productID) {
                matched++;
            }
        }
    }

    for(    i = 0, valid = 0;
            i < MAX_USB_DEVICES && valid < matched && valid < max_devices;
            i++) {
        if(0 != __enumerated_devices[i].valid
           && __enumerated_devices[i].vendorID == vendorID
           && __enumerated_devices[i].productID == productID) {
            output[valid] = __enumerated_devices[i].deviceID;
            valid++;
        }
    }

    return valid;

    /* If anything goes wrong above, there are a lot of different ways that the
     * accumulated state may need to be unwound.  goto is generally not the
     * preferred way to do anything, but it does greatly simplify exception
     * handling.
     */
error2:
    IOObjectRelease(iterator);
error1:
    mach_port_deallocate(mach_task_self(), masterPort);
    return -1;

error5:
    (*dev)->Release(dev);
error4:
    (*iodev)->Release(iodev);
error3:
    IOObjectRelease(usbDeviceRef);
    IOObjectRelease(iterator);
    return -1;
}

void *
USBOpen(unsigned long deviceID, int *errorCode) {
    /* Local variables */
    io_service_t usbInterfaceRef;
    mach_port_t masterPort;
    io_service_t usbDeviceRef;
    IOCFPlugInInterface **iodev = NULL;
    IOUSBInterfaceInterface **intf = NULL;
    cIOUSBDeviceInterface **dev = NULL;
    kern_return_t err;
    CFMutableDictionaryRef matchingDictionary=0;
    SInt32 idVendor;
    SInt32 idProduct;
    int score;
    CFNumberRef numberRef;
    UInt8 numConf;
    io_iterator_t iterator = 0;
    IOUSBConfigurationDescriptorPtr confDesc;
    IOUSBFindInterfaceRequest interfaceRequest;
    __device_instance_t *instance;
    UInt32 busLocation = 0;
    __usb_interface_t *retval;

    /* Try to look up the deviceID provided by the caller.  If this does
     * not exist, then there is nothing else to do.
     */
    instance = __lookup_device_instance_by_ID(deviceID);
    if(NULL == instance) {
        /* The device ID was not found.  The caller must only provide IDs that
         * have previously been provided by the USBProbeDevices() function
         * (which is wrapped by getDeviceIDs()).
         */
        goto error0;
    }

    if(NULL != instance->handle) {
        /* If there is already a device handle then the device has been opened.
         * It is illegal to try to open a device twice without first closing it.
         */
        goto error0;
    }

    /* The Vendor and Product IDs were cached when the deviceID was created.
     * By using these cached values, there is some protection against the caller
     * requesting devices at positions that do not match up.
     */
    idVendor = instance->vendorID;
    idProduct = instance->productID;

    /* Attempt to create a master port */
    if(IOMasterPort(MACH_PORT_NULL, &masterPort) != kIOReturnSuccess) {
        goto error0;
    }

    /* Attempt to create a matching dictionary */
    matchingDictionary = IOServiceMatching(kIOUSBDeviceClassName);
    if(NULL == matchingDictionary) {
        /* Failed to get the dictionary, so clean up and bail out */
        goto error1;
    }

    /* Attempt to create vendor reference */
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type,
                               &idVendor);
    if(NULL == numberRef) {
        /* Failed to get vendor reference, so clean up and bail out */
        goto error1;
    }

    /* Attempt to create a reference */
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBVendorID), numberRef);
    CFRelease(numberRef);
    numberRef = 0;
    numberRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberSInt32Type,
                               &idProduct);
    if(NULL == numberRef) {
        /* Failed to get product reference, so clean up and bail out */
        goto error1;
    }

    /* Attempt to create an interator */
    CFDictionaryAddValue(matchingDictionary, CFSTR(kUSBProductID), numberRef);
    CFRelease(numberRef);
    numberRef = 0;
    if(kIOReturnSuccess != IOServiceGetMatchingServices(masterPort,
                                                        matchingDictionary,
                                                        &iterator)) {
        /* Failed to get an iterator */
        goto error1;
    }

    if(0 == iterator) {
        /* No devices found */
        goto error2;  /* FIXME: does this try to release a null iterator? */
    }

    /* Search using the iterator for the indicated device */
    while(0 != (usbDeviceRef = IOIteratorNext(iterator))) {
        /* The iterator contains all devices matching the VID and PID.
         * However, the deviceID refers to a specific, unique location.
         * Only a device that matches the location, VID, and PID will be opened.
         * This prevents any confusion in case the user has plugged a different
         * kind of device into the port that was associated with the deviceID.
         * This code can't tell if one device was swapped for another at the
         * same location and VID/PID, but if it was never opened, then it
         * doesn't really matter -- the driver just wants a device of that type
         * and does not care specifically which one yet.  Once the device is
         * opened, its unique handle will prevent any ambiguity caused by
         * swapping devices.
         */

        /* Try to create an interface for the device.  This is necessary to
         * determine its unique location on the bus.
         */
        err = IOCreatePlugInInterfaceForService(usbDeviceRef,
                                                kIOUSBDeviceUserClientTypeID,
                                                kIOCFPlugInInterfaceID,
                                                &iodev,
                                                &score);
        if(0 != err || 0 == iodev) {
            /* Cannot create plugin interface for service */
            goto error3;
        }
        err = (*iodev)->QueryInterface(iodev,
                                       CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID),
                                       (LPVOID *)&dev);
        if(0 != err || 0 == dev) {
            /* Cannot query interface */
            goto error4;
        }

        /* Check whether this is really the same device
         * that the deviceID refers to.  If not, it will be necessary to
         * keep iterating.
         */
        if(kIOReturnSuccess != (*dev)->GetLocationID(dev, &busLocation)) {
            /* Got an unexpected error, so bail out. */
            goto error5;
        }

        if(busLocation != instance->busLocation) {
            /* This just means that the device wasn't exactly the one the
             * caller wanted.  This isn't an error state.
             */
            (*dev)->Release(dev);
            (*iodev)->Release(iodev);
            IOObjectRelease(usbDeviceRef);
            continue;
        }

        /* Try to open the device */
        err = (*dev)->USBDeviceOpen(dev);
        if(0 != err) {
            /* Cannot open the device */
            goto error5;
        }

        err = (*dev)->GetNumberOfConfigurations(dev, &numConf);
        if(0 != err) {
            goto error5;
        }

        err = (*dev)->GetConfigurationDescriptorPtr(dev, 0, &confDesc);
        if(0 != err) {
            /* Failed to get the 0th configuration */
            goto error5;
        }

        err = (*dev)->SetConfiguration(dev, confDesc->bConfigurationValue);
        if(0 != err) {
            goto error5;
        }
        interfaceRequest.bInterfaceClass = kIOUSBFindInterfaceDontCare;
        interfaceRequest.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
        interfaceRequest.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
        interfaceRequest.bAlternateSetting = kIOUSBFindInterfaceDontCare;
        IOObjectRelease(iterator);
        iterator = 0;
        err = (*dev)->CreateInterfaceIterator(dev, &interfaceRequest,
                                              &iterator);
        if(0 != err) {
            IOObjectRelease(usbDeviceRef);
            (*iodev)->Release(iodev);
            (*dev)->Release(dev);
            goto error0;
        }
        (*iodev)->Release(iodev);
        usbInterfaceRef = IOIteratorNext(iterator);
        if(0 != usbInterfaceRef) {
            IOCFPlugInInterface **iodev1;
            err = IOCreatePlugInInterfaceForService(usbInterfaceRef,
                                                    kIOUSBInterfaceUserClientTypeID,
                                                    kIOCFPlugInInterfaceID,
                                                    &iodev1, &score);
            if(0 != err || 0 == iodev1) {
                (*iodev1)->Release(iodev1);
                (*dev)->Release(dev);
                goto error3;
            }
            err = (*iodev1)->QueryInterface(iodev1,
                                            CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID),
                                            (LPVOID *)&intf);
            if(0 != err || 0 == intf) {
                (*iodev1)->Release(iodev1);
                (*dev)->Release(dev);
                goto error3;
            }

            (*iodev1)->Release(iodev1);
            err = (*intf)->USBInterfaceOpen(intf);
            if(0 != err) {
                (*dev)->Release(dev);
                goto error3;
            }

            /* Try to clear and reset all endpoints.  These are expected
             * to fail for the endpoints not defined by the interface, but
             * that will not cause any problems.
             */
            (*intf)->ClearPipeStall(intf, 1);
            (*intf)->ClearPipeStall(intf, 2);
            (*intf)->ClearPipeStall(intf, 3);
            (*intf)->ClearPipeStall(intf, 4);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x01);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x02);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x04);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x82);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x86);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x87);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x81);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x07);
            __clear_halt((cIOUSBDeviceInterface **)dev, 0x88);
        }
        IOObjectRelease(iterator);

        retval = (__usb_interface_t *)calloc(sizeof(__usb_interface_t), 1);
        retval->usbDeviceRef = usbDeviceRef;
        retval->dev = dev;
        retval->confDesc = confDesc;
        retval->intf = intf;
        retval->deviceID = instance->deviceID;

        __setup_endpoint_map(retval);
        /* Keep a reference to the handle for later */
        instance->handle = retval;

        SET_ERROR_CODE(OPEN_OK);

        return (void *)retval;
    }
    numberRef = 0;
    IOObjectRelease(iterator);
    iterator = 0;
    SET_ERROR_CODE(NO_DEVICE_FOUND);
    return (void *)NULL;

/* If anything goes wrong above, there are a lot of different ways that the
 * accumulated state may need to be unwound.  goto is generally not the
 * preferred way to do anything, but it does greatly simplify exception
 * handling.
 */
error2:
    IOObjectRelease(iterator);
error1:
    mach_port_deallocate(mach_task_self(), masterPort);
error0:
    SET_ERROR_CODE(NO_DEVICE_FOUND);
    return (void *)NULL;

error5:
    (*dev)->Release(dev);
error4:
    (*iodev)->Release(iodev);
error3:
    IOObjectRelease(usbDeviceRef);
    IOObjectRelease(iterator);
    SET_ERROR_CODE(NO_DEVICE_FOUND);
    return (void *)NULL;
}

int
USBClose(void *deviceHandle) {
    /* Local variables */
    __usb_interface_t *usb;
    __device_instance_t *device;

    if(NULL == deviceHandle) {
        return CLOSE_ERROR;
    }

    usb = (__usb_interface_t *)deviceHandle;

    device = __lookup_device_instance_by_ID(usb->deviceID);
    if(NULL != device) {
        /* This had an extra reference to the handle so free it up */
        device->handle = NULL;
    }

    __close_and_dealloc_usb_interface(usb);
    return CLOSE_OK;
}

int
USBWrite(void *deviceHandle, unsigned char endpoint, char *data, int numberOfBytes) {
    /* Local variables */
    IOReturn flag;
    __usb_interface_t *usb;
    unsigned char pipe;

    if(0 == deviceHandle) {
        return WRITE_FAILED;
    }

    usb = (__usb_interface_t *)deviceHandle;
    pipe = __get_pipe_for_endpoint(usb, endpoint);

    if(0xFF == pipe) {
        return WRITE_FAILED;
    }

    flag = (*usb->intf)->WritePipe(usb->intf, pipe, data, numberOfBytes);
    if(kIOReturnSuccess != flag) {
        return WRITE_FAILED;
    }

    return numberOfBytes;
}


int
USBRead(void *deviceHandle, unsigned char endpoint, char * data, int numberOfBytes) {
    IOReturn flag;
    __usb_interface_t *usb;
    unsigned int bytesRead = numberOfBytes;
    unsigned char pipe;

    if(NULL == deviceHandle) {
        return READ_FAILED;
    }

    usb = (__usb_interface_t *)deviceHandle;

    pipe = __get_pipe_for_endpoint(usb, endpoint);
    if(0xFF == pipe) {
        return READ_FAILED;
    }

    flag = (*usb->intf)->ReadPipe(usb->intf, pipe, data, &bytesRead);
    if(kIOReturnSuccess != flag) {
        return READ_FAILED;
    }

    return bytesRead;
}


void
USBResetPipe(void *deviceHandle, unsigned char endpoint) {
    __usb_interface_t *usb = NULL;
    unsigned char pipe;

    if(NULL == deviceHandle) {
        return;
    }

    pipe = __get_pipe_for_endpoint(usb, endpoint);
    if(0xFF == pipe) {
        return;
    }

    usb = (__usb_interface_t *)deviceHandle;

    (*usb->intf)->ResetPipe(usb->intf, pipe);
}


int USBGetDeviceDescriptor(void *deviceHandle, struct USBDeviceDescriptor *desc) {
    __usb_interface_t *usb;
    unsigned char uc = 0;
    unsigned short us = 0;
    if(NULL == deviceHandle || NULL == desc) {
        return -1;
    }

    usb = (__usb_interface_t *)deviceHandle;

    /* Unfortunately, IOKit does not appear to provide a nice way of reading
     * out the entire device descriptor as-is.  This fills in the fields as
     * well as it can, but some of these are going to end up with default
     * values.  It may be possible to issue a command to the USB device
     * on endpoint 0 and try to read back the raw descriptor, but that level
     * of detail may not be necessary here.
     */
    desc->bLength = 136;
    desc->bDescriptorType = kUSBDevice;
    desc->bcdUSB = 0;  /* FIXME: is there any way to get this value in IOKit? */
    (*usb->dev)->GetDeviceClass(usb->dev, &uc);
    desc->bDeviceClass = uc;
    (*usb->dev)->GetDeviceSubClass(usb->dev, &uc);
    desc->bDeviceSubClass = uc;
    (*usb->dev)->GetDeviceProtocol(usb->dev, &uc);
    desc->bDeviceProtocol = uc;
    desc->bMaxPacketSize0 = 64;  /* FIXME: can this be read through IOKit? */
    (*usb->dev)->GetDeviceVendor(usb->dev, &us);
    desc->idVendor = us;
    (*usb->dev)->GetDeviceProduct(usb->dev, &us);
    desc->idProduct = us;
    (*usb->dev)->GetDeviceReleaseNumber(usb->dev, &us);
    desc->bcdDevice = us;
    desc->iManufacturer = 0; /* FIXME: can this be read through IOKit? */
    desc->iSerialNumber = 0; /* FIXME: can this be read through IOKit? */
    (*usb->dev)->GetNumberOfConfigurations(usb->dev, &uc);
    desc->bNumConfigurations = uc;

    return 0;
}


int
USBGetInterfaceDescriptor(void *deviceHandle, struct USBInterfaceDescriptor *desc) {
    __usb_interface_t *usb;
    unsigned char uc = 0;
    if(NULL == deviceHandle || NULL == desc) {
        return -1;
    }

    usb = (__usb_interface_t *)deviceHandle;

    desc->bLength = 0; /* FIXME: can this be read through IOKit? */
    desc->bDescriptorType = kUSBInterfaceDesc;
    (*usb->intf)->GetInterfaceNumber(usb->intf, &uc);
    desc->bInterfaceNumber = uc;
    (*usb->intf)->GetAlternateSetting(usb->intf, &uc);
    desc->bAlternateSetting = uc;
    (*usb->intf)->GetNumEndpoints(usb->intf, &uc);
    desc->bNumEndpoints = uc;
    (*usb->intf)->GetInterfaceClass(usb->intf, &uc);
    desc->bInterfaceClass = uc;
    (*usb->intf)->GetInterfaceSubClass(usb->intf, &uc);
    desc->bInterfaceSubClass = uc;
    (*usb->intf)->GetInterfaceProtocol(usb->intf, &uc);
    desc->bInterfaceProtocol = uc;
    desc->iInterface = 0; /* FIXME: can this be read through IOKit? */

    return 0;
}


int USBGetEndpointDescriptor(void *deviceHandle, int endpointIndex,
                             struct USBEndpointDescriptor *desc) {
    __usb_interface_t *usb;
    unsigned char direction;
    unsigned char transferType;
    unsigned char number;
    unsigned short maxPacketSize;
    unsigned char interval;
    IOReturn flag;

    if(NULL == deviceHandle || NULL == desc) {
        return -1;
    }

    usb = (__usb_interface_t *)deviceHandle;

    flag = (*usb->intf)->GetPipeProperties(usb->intf, endpointIndex + 1,
                                           &direction, &number, &transferType,
                                           &maxPacketSize, &interval);
    if(kIOReturnSuccess != flag) {
        return -1;
    }

    if(1 == direction) {
        number += 0x80;
    }

    desc->bLength = 7;
    desc->bDescriptorType = kUSBEndpointDesc;
    desc->bEndpointAddress = number;
    desc->bmAttributes = (direction << 7) | transferType;
    desc->wMaxPacketSize = maxPacketSize;
    desc->bInterval = interval;

    return 0;
}

int
USBGetStringDescriptor(void *deviceHandle, unsigned int string_index,
                       char *buffer, int maxLength) {
    /* Local variables */
    char buf[500];
    IOUSBDevRequest devreq;
    devreq.bmRequestType = 0x80; /* get string descriptor */
    devreq.bRequest = kUSBRqGetDescriptor; /* get descriptor */
    devreq.wValue = (string_index & 0x00FF) | ((kUSBStringDesc & 0x00FF) << 8); /* of type string, index */
    devreq.wIndex = 0x409; /* language ID */
    devreq.wLength = 500; /* length of available buffer */
    devreq.pData = buf; /* point to buffer */
    int i = 2;
    int j = 0;
    cIOUSBDeviceInterface** devint;
    __usb_interface_t *usb;

    if(0 == deviceHandle) {
        return -1;
    }

    usb = (__usb_interface_t *)deviceHandle;
    devint = usb->dev;

    memset(buffer, (int)0, maxLength);  /* Initialize user buffer */

    (*devint)->DeviceRequest(devint, &devreq); /* make device request */
    while(i < 500 && buf[i] > 0 && j < maxLength) {
        /* go through string, and stop if reach end, or if ASCII > 128 encountered */
        buffer[j++] = buf[i];
        i += 2;
    }

    return j;
}

void __clear_halt(cIOUSBDeviceInterface **dev, unsigned char endpoint) {
    IOUSBDevRequest request;

    if(NULL == dev) {
        return;
    }

    /* There is some good information on this EndPoint 0 transaction here:
     * http://www.beyondlogic.org/usbnutshell/usb6.shtml
     */

    request.bmRequestType = 2;  /* Clear feature endpoint */
    request.bRequest = 1;       /* Clear feature */
    request.wValue = 0;         /* Endpoint halt */
    request.wIndex = endpoint;  /* Endpoint number (not pipe) */
    request.wLength = 0;        /* No extended payload */
    request.pData = NULL;       /* No extended payload */
    (*dev)->DeviceRequest(dev, &request);
}
