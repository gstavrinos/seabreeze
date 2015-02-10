@echo off
REM ***************************************************************************
REM This file was created to solve a problem on Win64/cygwin when attempting to
REM link gcc/g++ compiled source files with a VisualStudio created SeaBreeze.dll.
REM The error manifests as:
REM ../../lib/seabreeze.lib: error adding symbols: File in wrong format
REM
REM The solution is to use the cygwin utility "dlltool.exe" to generate a gcc/ld
REM compatible SeaBreeze.dll.  The dlltool requires a SeaBreeze.def file
REM containing the symbols exported from SeaBreeze.dll.  The Visual C++ utility
REM "dumpbin.exe" is used to extract the symbols.  This batch file creates a
REM SeaBreeze.def file from the exported symbols and calls dlltool to create the
REM gcc/ld compatible SeaBreeze.lib.  The original VisualStudio SeaBreeze.lib
REM is preserved as SeaBreeze-VS.lib.
REM
REM NOTE: This batch file MUST be executed from a cygwin terminal window
REM ./winlibfix.bat
REM
REM ***************************************************************************
REM //
REM // Generate the SeaBreeze.def header text
echo ; SeaBreeze.def : Declares the module parameters for the DLL. > ../../../lib/SeaBreeze.def
echo ; Auto-Generated from winlibfix.bat >> ../../../lib/SeaBreeze.def
echo. >> ../../../lib/SeaBreeze.def
echo LIBRARY      "SeaBreeze" >> ../../../lib/SeaBreeze.def
echo. >> ../../../lib/SeaBreeze.def
echo EXPORTS >> ../../../lib/SeaBreeze.def
echo     ; Explicit exports go here >> ../../../lib/SeaBreeze.def
echo     ; Auto-Generated from winlibfix.bat via dumpbin.exe /exports >> ../../../lib/SeaBreeze.def
REM //
REM // Extract the exported symbols with dumpbin output piped to sed
REM // and append the symbol list to SeaBreeze.def 
REM // Note: dumpbin must be in the path and is found in the Visual C++ bin
REM //       directory (under the Visual Studio directory)
REM //       For example:
REM //       C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin
REM //       In cygwin:
REM //       /cygdrive/c/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin
REM //       Be sure to open a new cygwin terminal AFTER setting the path in Windows
REM // Note: ";tx;d;:x" at the end of the sed command deletes unmatched lines
REM //       The rest of the sed command is the regex to extract the export symbol
REM //       from each matching line (unmatched lines are deleted from stream)
dumpbin /exports ..\..\..\lib\SeaBreeze.dll | sed 's/^\s\+[0-9]\+\s\+[0-9A-F]\+\s\+[0-9A-F]\+\s\+\([a-z_A-Z@$?0-9]\+\).*/    \1/;tx;d;:x' >> ../../../lib/SeaBreeze.def
REM //
REM // Copy the original VisualStudio generated SeaBreeze.lib to SeaBreeze-VS.lib
REM // Note: only copied if SeaBreeze-VS.lib doesn't already exist
if not exist ..\..\..\lib\SeaBreeze-VS.lib cp ../../../lib/SeaBreeze.lib ../../../lib/SeaBreeze-VS.lib
REM //
REM // Generate the gcc/ld compatible SeaBreeze.lib using exports in SeaBreeze.def
dlltool --def ../../../lib/SeaBreeze.def --dllname SeaBreeze.dll --output-lib ../../../lib/SeaBreeze.lib
