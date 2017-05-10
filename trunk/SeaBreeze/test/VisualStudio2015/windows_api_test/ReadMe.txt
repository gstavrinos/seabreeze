========================================================================
    CONSOLE APPLICATION : windows_api_test Project Overview
========================================================================

AppWizard has created this windows_api_test application for you.

This file contains a summary of what you will find in each of the files that
make up your windows_api_test application.


windows_api_test.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

windows_api_test.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

windows_api_test.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named windows_api_test.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

gpio, i2c etc. need a loopback connector to be properly tested. That is not currently available, so 
any test mearly exercises the command call. i2c is not tested for this reason.

/////////////////////////////////////////////////////////////////////////////
