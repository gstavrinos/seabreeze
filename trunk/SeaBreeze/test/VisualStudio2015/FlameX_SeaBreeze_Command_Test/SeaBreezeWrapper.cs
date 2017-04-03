// SeaBreezeWrapper.cs
//
// Wrap the SeaBreezeWrapper C interface with C# bindings.
//
// For documentation on these functions, see include/api/SeaBreezeWrapper.h

using System.Runtime.InteropServices;

public partial class SeaBreezeWrapper
{
    public const int SLOT_LENGTH = 15;
    public const int ERROR_SUCCESS = 0;

    // NOTE: To Debug SeaBreeze.dll set the full absolute path to your debug build of SeaBreeze.dll
    // Remember to build seabreeze with the /DEBUG:FULL symbol.
    public const string DLL = @"Z:\\Linux_Share\seabreeze_trunk\os-support\windows\VisualStudio2015\x64\Debug\SeaBreeze.dll";
    
    // for a relase version of the software use
    //const string DLL = @"SeaBreeze.dll";

    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern double seabreeze_read_tec_temperature               (int index, ref int errorCode); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern float  seabreeze_read_irrad_collection_area         (int index, ref int errorCode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_close_spectrometer                 (int index, ref int errorCode); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_api_version_string             (ref byte buffer, int len);
    // wish list
    //[DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_eeprom_slot_count              (int index, ref int errorCode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_electric_dark_pixel_indices    (int index, ref int errorCode, ref int indices, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_error_string                   (int errorCode, ref byte buffer, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_error_string_maximum_length    ();
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_formatted_spectrum             (int index, ref int errorCode, ref double buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_formatted_spectrum_length      (int index, ref int errorCode); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_serial_number                  (int index, ref int errorCode, ref byte buffer, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern byte   seabreeze_get_serial_number_max_length       (int index, ref int errorCode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_model_string_maximum_length    ();
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_model                          (int index, ref int errorCode, ref byte buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_unformatted_spectrum           (int index, ref int errorCode, ref byte buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_unformatted_spectrum_length    (int index, ref int errorCode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_usb_descriptor_string          (int index, ref int errorCode, int id, ref byte buffer, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_wavelengths                    (int index, ref int errorCode, ref double wavelengths, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_has_irrad_collection_area          (int index, ref int errorCode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_open_spectrometer                  (int index, ref int errorCode); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_read_eeprom_slot                   (int index, ref int errorCode, int slot_number, ref byte buffer, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_write_irrad_collection_area        (int index, ref int errorCode, ref float area);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_read_irrad_calibration             (int index, ref int errorCode, ref float buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_read_usb                           (int index, ref int errorCode, byte endpoint, ref byte buffer, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_set_continuous_strobe_period_microsec (int index, ref int errorCode, ushort strobeID, ulong strobePeriodMicroseconds); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_write_eeprom_slot                  (int index, ref int errorCode, int slot_number, ref byte buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_write_irrad_calibration            (int index, ref int errorCode, ref float buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_write_usb                          (int index, ref int errorCode, byte endpoint, ref byte buffer, int length); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern long   seabreeze_get_min_integration_time_microsec  (int index, ref int errorCode);
    // not found
   // [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_override_eeprom_slot_count         (int index, ref int errorCode, int count);
    // not found
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_integration_time_microsec      (int index, ref int errorCode, ulong integration_time_micros); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_shutter_open                   (int index, ref int errorCode, byte opened); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_strobe_enable                  (int index, ref int errorCode, byte strobe_enable); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_tec_enable                     (int index, ref int errorCode, byte tec_enable); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_tec_fan_enable                 (int index, ref int errorCode, byte tec_fan_enable); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_tec_temperature                (int index, ref int errorCode, double temperature_degrees_celsius); 
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_trigger_mode                   (int index, ref int errorCode, int mode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_light_source_count             (int index, ref int errorCode, int mode);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_light_source_enable            (int index, ref int errorCode, int mode, int light_index, byte enable);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_light_source_intensity         (int index, ref int error_code, int light_index, double intensity);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern uint   seabreeze_get_number_of_pixels               (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_optical_dark_pixel_indices     (int index, ref int error_code, ref int indices, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern int    seabreeze_get_active_pixel_indices           (int index, ref int error_code, ref int indices, int length);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_shutdown                           ();
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void   seabreeze_set_acquisition_delay_microsec     (int index, ref int errorCode, ulong delay_usec);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern ulong  seabreeze_get_buffer_element_count           (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern ulong  seabreeze_get_buffer_capacity                (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern byte   seabreeze_get_buffering_enable               (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern uint    seabreeze_get_consecutive_sample_count      (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern ulong  seabreeze_get_buffer_capacity_maximum        (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern ulong  seabreeze_get_buffer_capacity_minimum        (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void  seabreeze_set_buffer_capacity                 (int index, ref int error_code, ulong capacity);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void  seabreeze_set_buffering_enable                (int index, ref int error_code, byte isEnabled);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void seabreeze_set_consecutive_sample_count         (int index, ref int error_code, uint consecutiveSampleCount);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void  seabreeze_clear_buffer                       (int index, ref int error_code);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void seabreeze_set_verbose                          (int flag);
    [DllImport(DLL, CallingConvention = CallingConvention.Cdecl)] public static extern void seabreeze_set_logfile                          (ref byte pathname, int length);


}
