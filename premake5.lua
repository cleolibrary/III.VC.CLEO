function setupDebugger(gameDirVar, gameExeName, trgDir)
	debugcommand ("$(" .. gameDirVar .. ")\\" .. gameExeName)
    debugdir ("$(" .. gameDirVar .. ")")
    postbuildcommands { "\
if defined " .. gameDirVar .. " ( \r\n\
taskkill /IM " .. gameExeName .. " /F /FI \"STATUS eq RUNNING\" \r\n\
xcopy /Y \"$(TargetPath)\" \"$(" .. gameDirVar .. ")" .. trgDir .. "\" \r\n\
)" }      
end

workspace "III.VC.CLEO"
   configurations { "Release", "Debug" }
   platforms { "Win32" }
   architecture "x32"
   location "build"
   objdir ("build/obj")
   buildlog ("build/log/%{prj.name}.log")
   
   kind "SharedLib"
   language "C++"
   cdialect "C17"
   cppdialect "C++latest"
   targetdir "bin"
   targetextension ".asi"
   characterset ("MBCS")
   staticruntime "On"		

   defines { "_CRT_SECURE_NO_WARNINGS",  "_CRT_NON_CONFORMING_SWPRINTFS" }
   defines { "rsc_CompanyName=\"CLEO\"" }
   defines { "rsc_LegalCopyright=\"MIT License\""} 
   defines { "rsc_InternalName=\"%{prj.name}\"", "rsc_ProductName=\"%{prj.name}\"", "rsc_OriginalFilename=\"%{prj.name}.asi\"" }
   defines { "rsc_FileDescription=\"https://cleo.li\"" }
   defines { "rsc_UpdateUrl=\"https://github.com/cleolibrary/III.VC.CLEO\"" }
   
   files { "source/III.VC.CLEO/*.h", "source/III.VC.CLEO/*.cpp" }
   files { "source/III.VC.CLEO/*.def" }
   files { "Resources/*.rc" }
   includedirs { "external/injector/include" }
   
   pbcommands = { 
      "setlocal EnableDelayedExpansion",
      --"set \"path=" .. (gamepath) .. "\"",
      "set file=$(TargetPath)",
      "FOR %%i IN (\"%file%\") DO (",
      "set filename=%%~ni",
      "set fileextension=%%~xi",
      "set target=!path!!filename!!fileextension!",
      "if exist \"!target!\" copy /y \"!file!\" \"!target!\"",
      ")" }
   
   filter "configurations:Debug*"
      defines "DEBUG"
      symbols "On"

   filter "configurations:Release*"
      defines "NDEBUG"
      optimize "On"

project "III.CLEO"
   defines { "CLEO_III" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "")
project "VC.CLEO"
   defines { "CLEO_VC" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "")

   
   
   
   
   
workspace "CLEO_SDK"
   configurations { "Release", "Debug" }
   platforms { "Win32" }
   architecture "x32"
   location "build"
   objdir ("build/obj")
   buildlog ("build/log/%{prj.name}.log")
   
   kind "SharedLib"
   language "C++"
   cdialect "C17"
   cppdialect "C++latest"
   targetdir "bin/cleo/cleo_plugins"
   targetextension ".cleo"
   characterset ("MBCS")
   staticruntime "On"
   
   defines { "_CRT_SECURE_NO_WARNINGS",  "_CRT_NON_CONFORMING_SWPRINTFS" }
   defines { "rsc_CompanyName=\"CLEO\"" }
   defines { "rsc_LegalCopyright=\"MIT License\""} 
   defines { "rsc_InternalName=\"%{prj.name}\"", "rsc_ProductName=\"%{prj.name}\"", "rsc_OriginalFilename=\"%{prj.name}.asi\"" }
   defines { "rsc_FileDescription=\"https://cleo.li\"" }
   defines { "rsc_UpdateUrl=\"https://github.com/cleolibrary/III.VC.CLEO\"" }
   
   includedirs { "source/CLEO_SDK" }
   libdirs { "bin" }
   
   filter "configurations:Debug*"
      defines "DEBUG"
      symbols "On"

   filter "configurations:Release*"
      defines "NDEBUG"
      optimize "On"


project "III.ClipboardControl"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/ClipboardControl/*.h", "source/CLEO_SDK/demo_plugins/ClipboardControl/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "\\cleo\\cleo_plugins")
project "III.FileSystemOperations"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.h", "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "\\cleo\\cleo_plugins")
project "III.IniFiles"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/IniFiles/*.h", "source/CLEO_SDK/demo_plugins/IniFiles/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "\\cleo\\cleo_plugins")
project "III.IntOperations"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/IntOperations/*.h", "source/CLEO_SDK/demo_plugins/IntOperations/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "\\cleo\\cleo_plugins")
project "III.MemoryModule"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/MemoryModule/*.h", "source/CLEO_SDK/demo_plugins/MemoryModule/*.cpp", "source/CLEO_SDK/demo_plugins/MemoryModule/*.c" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_III_DIR", "gta3.exe", "\\cleo\\cleo_plugins")
project "VC.ClipboardControl"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/ClipboardControl/*.h", "source/CLEO_SDK/demo_plugins/ClipboardControl/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "\\cleo\\cleo_plugins")
project "VC.FileSystemOperations"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.h", "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "\\cleo\\cleo_plugins")
project "VC.IniFiles"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/IniFiles/*.h", "source/CLEO_SDK/demo_plugins/IniFiles/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "\\cleo\\cleo_plugins")
project "VC.IntOperations"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/IntOperations/*.h", "source/CLEO_SDK/demo_plugins/IntOperations/*.cpp" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "\\cleo\\cleo_plugins")
project "VC.MemoryModule"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/MemoryModule/*.h", "source/CLEO_SDK/demo_plugins/MemoryModule/*.cpp", "source/CLEO_SDK/demo_plugins/MemoryModule/*.c" }
   files { "Resources/*.rc" }
   setupDebugger("GTA_VC_DIR", "gta-vc.exe", "\\cleo\\cleo_plugins")