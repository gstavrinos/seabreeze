[README.TXT]

OBPTester

This is a Windows 7 executable that provides a graphical user interface for a large
portion of commands specified by the Ocean Optics Binary Protocol (OBP). Currently the
utility supports commands sent via USB and RS-232 with infrastructure provided for 
future TCP/IP support. 

The utility is intended to be used to debug, troubleshoot and exercise Ocean Optics OBP
spectrometers.



Assumptions:

- Only spectrometers that use OBP should be used with the utility. Trying to use other
	instruments, which use another protocol, could cause unpredictable or damaging 
	results in the spectrometer. 

History:

1.0 - Original application provided by Doug Andersson
1.1 - USB (winUSBNet) added by Kirk Clendinning
