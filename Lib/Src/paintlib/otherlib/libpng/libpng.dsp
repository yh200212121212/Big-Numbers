# Microsoft Developer Studio Project File - Name="libpng" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libpng - Win32 Debug Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libpng.mak" CFG="libpng - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libpng - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "libpng - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libpng - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpng___Win32_Release"
# PROP BASE Intermediate_Dir "libpng___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\MTRelease\libpng.lib"

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libpng___Win32_Debug"
# PROP BASE Intermediate_Dir "libpng___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_CRT_SECURE_NO_WARNINGS" /D PNG_DEBUG=1 /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\..\MTDebug\libpng.lib"

!ELSEIF  "$(CFG)" == "libpng - Win32 Release Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libpng___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "libpng___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_Unicode"
# PROP Intermediate_Dir "Release_Unicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\..\..\zlib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_WARNINGS" /YX"stdafx.h" /FD /c
# ADD BASE RSC /l 0x406 /d "NDEBUG"
# ADD RSC /l 0x406 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\MTRelease\libpng.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\MTURelease\libpng.lib"

!ELSEIF  "$(CFG)" == "libpng - Win32 Debug Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libpng___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "libpng___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Unicode"
# PROP Intermediate_Dir "Debug_Unicode"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_CRT_SECURE_NO_WARNINGS" /D PNG_DEBUG=1 /FR /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\zlib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_CRT_SECURE_NO_WARNINGS" /D PNG_DEBUG=1 /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x406 /d "_DEBUG"
# ADD RSC /l 0x406 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\..\MTDebug\libpng.lib"
# ADD LIB32 /nologo /out:"..\..\..\..\MTUDebug\libpng.lib"

!ENDIF 

# Begin Target

# Name "libpng - Win32 Release"
# Name "libpng - Win32 Debug"
# Name "libpng - Win32 Release Unicode"
# Name "libpng - Win32 Debug Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\png.c
# End Source File
# Begin Source File

SOURCE=.\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\pngget.c
# End Source File
# Begin Source File

SOURCE=.\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\pngread.c
# End Source File
# Begin Source File

SOURCE=.\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\pngset.c
# End Source File
# Begin Source File

SOURCE=.\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\pngwutil.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\png.h
# End Source File
# Begin Source File

SOURCE=.\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\pngdebug.h
# End Source File
# Begin Source File

SOURCE=.\pnginfo.h
# End Source File
# Begin Source File

SOURCE=.\pnglibconf.h
# End Source File
# Begin Source File

SOURCE=.\pngpriv.h
# End Source File
# Begin Source File

SOURCE=.\pngstruct.h
# End Source File
# Begin Source File

SOURCE=.\pngusr.h
# End Source File
# End Group
# End Target
# End Project
