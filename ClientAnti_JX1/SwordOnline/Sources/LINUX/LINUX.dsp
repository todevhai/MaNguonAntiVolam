# Microsoft Developer Studio Project File - Name="LINUX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LINUX - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LINUX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LINUX.mak" CFG="LINUX - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LINUX - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LINUX - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SwordOnline/Sources/LINUX", TNUAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LINUX - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "LINUX - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".\core" /I ".\engine" /I ".\\" /I ".\lua" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_SERVER" /D "_STANDALONE" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib shlwapi.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin\server\linux.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LINUX - Win32 Release"
# Name "LINUX - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Core\CoreServerShell.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\CoreShell.h
# End Source File
# Begin Source File

SOURCE=.\Core\KBasPropTbl.CPP
# End Source File
# Begin Source File

SOURCE=.\Core\KBuySell.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KCore.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KFaction.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KGMCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KGMProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KItemChangeRes.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KItemGenerator.CPP
# End Source File
# Begin Source File

SOURCE=.\Core\KItemList.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KItemSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KMagicDesc.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KMission.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KMissle.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KMissleMagicAttribsData.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KMissleSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNewProtocolProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpc.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcAI.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcAttribModify.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcDeathCalcExp.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcFindPath.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcGold.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KObj.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KObjSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerDBFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerFaction.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerMenuState.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerPK.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTask.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTeam.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTong.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTong.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KProtocol.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KProtocolProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillList.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSkills.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillSpecial.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSubWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KSubWorldSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KTaskFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KWeatherMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Core\KWorldMsg.cpp
# End Source File
# End Group
# Begin Group "Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Engine\KAutoMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KBinTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KFilePath.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KIniFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KLinkArray.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KList.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KLittleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KLuaScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KLuaScriptSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KLubCmpl_Blocker.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMath.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemClass.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemClass1.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KPakFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KRandom.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptList.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptValueSet.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KSG_MD5_String.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KSG_StringProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KSortScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KStepLuaScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrList.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KTabFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KTabFileCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KThread.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\KWin32.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\md5.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\md5.h
# End Source File
# Begin Source File

SOURCE=.\Engine\ScriptFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\Engine\Text.cpp
# End Source File
# End Group
# Begin Group "lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lua\lapi.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\lauxlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\lbaselib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lcode.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\ldblib.c
# End Source File
# Begin Source File

SOURCE=.\lua\ldebug.c
# End Source File
# Begin Source File

SOURCE=.\lua\ldo.c
# End Source File
# Begin Source File

SOURCE=.\lua\lfunc.c
# End Source File
# Begin Source File

SOURCE=.\lua\lgc.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\liolib.c
# End Source File
# Begin Source File

SOURCE=.\lua\llex.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\lmathlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\lmem.c
# End Source File
# Begin Source File

SOURCE=.\lua\lobject.c
# End Source File
# Begin Source File

SOURCE=.\lua\lparser.c
# End Source File
# Begin Source File

SOURCE=.\lua\lstate.c
# End Source File
# Begin Source File

SOURCE=.\lua\lstring.c
# End Source File
# Begin Source File

SOURCE=.\lua\baselib\lstrlib.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltable.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltests.c
# End Source File
# Begin Source File

SOURCE=.\lua\ltm.c
# End Source File
# Begin Source File

SOURCE=.\lua\lundump.c
# End Source File
# Begin Source File

SOURCE=.\lua\lvm.c
# End Source File
# Begin Source File

SOURCE=.\lua\lzio.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\GameServer.cpp
# End Source File
# Begin Source File

SOURCE=.\KSG_EncodeDecode.cpp
# End Source File
# Begin Source File

SOURCE=.\KSOServer.cpp
# End Source File
# Begin Source File

SOURCE=.\package.cpp
# End Source File
# Begin Source File

SOURCE=.\xbuffer.h
# End Source File
# Begin Source File

SOURCE=.\zbuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\zclient.cpp
# End Source File
# Begin Source File

SOURCE=.\zport.cpp
# End Source File
# Begin Source File

SOURCE=.\zserver.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Core\CoreObjGenreDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\CoreServerDataDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\CoreServerShell.h
# End Source File
# Begin Source File

SOURCE=.\Core\CoreUseNameDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\GameDataDef.h
# End Source File
# Begin Source File

SOURCE=.\IClient.h
# End Source File
# Begin Source File

SOURCE=.\IServer.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KAutoMutex.h
# End Source File
# Begin Source File

SOURCE=.\Core\KBasPropTbl.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KBinsTree.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KBinTree.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KBinTreeNode.h
# End Source File
# Begin Source File

SOURCE=.\Core\KBuySell.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KCache.h
# End Source File
# Begin Source File

SOURCE=.\Core\KCore.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KDebug.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KEngine.h
# End Source File
# Begin Source File

SOURCE=.\Core\KFaction.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KFile.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KFilePath.h
# End Source File
# Begin Source File

SOURCE=.\Core\KGMCommand.h
# End Source File
# Begin Source File

SOURCE=.\Core\KGmProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KIndexNode.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KIniFile.h
# End Source File
# Begin Source File

SOURCE=.\Core\KInventory.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KITabFile.h
# End Source File
# Begin Source File

SOURCE=.\Core\KItem.h
# End Source File
# Begin Source File

SOURCE=.\Core\KItemChangeRes.h
# End Source File
# Begin Source File

SOURCE=.\Core\KItemGenerator.h
# End Source File
# Begin Source File

SOURCE=.\Core\KItemList.h
# End Source File
# Begin Source File

SOURCE=.\Core\KItemSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLinkArray.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLinkArrayTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KList.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLittleMap.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLuaScript.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLuaScriptSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KLubCmpl_Blocker.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMagicAttrib.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMagicDesc.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMath.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemBase.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemClass.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemClass1.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemManager.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMemStack.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMessage.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMission.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMissionArray.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMissle.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMissleMagicAttribsData.h
# End Source File
# Begin Source File

SOURCE=.\Core\KMissleSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KMutex.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNewProtocolProcess.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KNode.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpc.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcAI.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcAttribModify.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcDeathCalcExp.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcFindPath.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcGold.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcSet.h
# End Source File
# Begin Source File

SOURCE=.\Core\KNpcTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Core\KObj.h
# End Source File
# Begin Source File

SOURCE=.\Core\KObjSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KPakData.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KPakFile.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerChat.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KPlayerDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerFaction.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerMenuState.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerPK.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerSet.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTask.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTeam.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPlayerTrade.h
# End Source File
# Begin Source File

SOURCE=.\Core\KPolygon.h
# End Source File
# Begin Source File

SOURCE=.\Core\KProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Core\KProtocolDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\KProtocolProcess.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KRandom.h
# End Source File
# Begin Source File

SOURCE=.\Core\KRegion.h
# End Source File
# Begin Source File

SOURCE=.\Core\KRelayProtocol.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KScript.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptCache.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptList.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KScriptValueSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KSG_MD5_String.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KSG_StringProcess.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillList.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillManager.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSkills.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSkillSpecial.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KSortScript.h
# End Source File
# Begin Source File

SOURCE=.\KSOServer.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KStepLuaScript.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrBase.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrList.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KStrNode.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSubWorld.h
# End Source File
# Begin Source File

SOURCE=.\Core\KSubWorldSet.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KTabFile.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KTabFileCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Core\KTaskFuns.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KThread.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KTimer.h
# End Source File
# Begin Source File

SOURCE=.\KTransferUnit.h
# End Source File
# Begin Source File

SOURCE=.\Core\KWeatherMgr.h
# End Source File
# Begin Source File

SOURCE=.\Engine\KWin32.h
# End Source File
# Begin Source File

SOURCE=.\Core\KWorldMsg.h
# End Source File
# Begin Source File

SOURCE=.\Engine\LhaLib.h
# End Source File
# Begin Source File

SOURCE=.\Engine\LuaFuns.h
# End Source File
# Begin Source File

SOURCE=.\Engine\LuaLib.h
# End Source File
# Begin Source File

SOURCE=.\Core\MsgGenreDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\MyAssert.H
# End Source File
# Begin Source File

SOURCE=.\Core\Scene\ObstacleDef.h
# End Source File
# Begin Source File

SOURCE=.\package.h
# End Source File
# Begin Source File

SOURCE=.\S3DBInterface.h
# End Source File
# Begin Source File

SOURCE=.\Core\Scene\SceneDataDef.h
# End Source File
# Begin Source File

SOURCE=.\Core\Skill.h
# End Source File
# Begin Source File

SOURCE=.\Core\SkillDef.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Core\TaskDef.h
# End Source File
# Begin Source File

SOURCE=.\Engine\Text.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
