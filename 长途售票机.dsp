# Microsoft Developer Studio Project File - Name="��;��Ʊ��" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=��;��Ʊ�� - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "��;��Ʊ��.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "��;��Ʊ��.mak" CFG="��;��Ʊ�� - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "��;��Ʊ�� - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "��;��Ʊ�� - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "��;��Ʊ�� - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "��;��Ʊ�� - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "��;��Ʊ�� - Win32 Release"
# Name "��;��Ʊ�� - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Project\main.c
# End Source File
# Begin Source File

SOURCE=.\Project\NormalData.c
# End Source File
# Begin Source File

SOURCE=.\Project\shell.c
# End Source File
# Begin Source File

SOURCE=.\Project\ydt.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Project\data_type.h
# End Source File
# Begin Source File

SOURCE=.\Project\main.h
# End Source File
# Begin Source File

SOURCE=.\Project\shell.h
# End Source File
# Begin Source File

SOURCE=.\Project\ydt.h
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Project\ymodem.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\air_manager.pbd
# End Source File
# Begin Source File

SOURCE="..\..\\��;��Ʊ��\Project\EPT-A9L"
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\D\main.d
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\main.o
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\D\NormalData.d
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\NormalData.o
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Debug\NormalData.obj
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Debug\NormalData.sbr
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\POS.vpj
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\POS.vpw
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\POS.vpwhist
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\POS.vtg
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\PosInfo.txt
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\D\shell.d
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\shell.o
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket20121205ver4
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket_printer
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket_printer.map
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket_printer.pkg
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket_printer0.0.5.pkg
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ticket_printer0.0.6.pkg
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Debug\vc60.idb
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Debug\vc60.pdb
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\D\ydt.d
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\o\ydt.o
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.dsp
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.dsw
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.ncb
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.nps
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.opt
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\Debug\��;��Ʊ��.pch
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.plg
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.vpj
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.vpw
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.vpwhist
# End Source File
# Begin Source File

SOURCE=..\..\\��;��Ʊ��\��;��Ʊ��.vtg
# End Source File
# End Target
# End Project
