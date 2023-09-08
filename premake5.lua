workspace "III.VC.CLEO"
   configurations { "Release", "Debug", "Release_xp" }
   platforms { "Win32" }
   architecture "x32"
   location "build"
   objdir ("build/obj")
   buildlog ("build/log/%{prj.name}.log")
   
   kind "SharedLib"
   language "C++"
   targetdir "bin"
   targetextension ".asi"
   characterset ("MBCS")
   flags { "StaticRuntime" }
   
   defines { "rsc_CompanyName=\"CLEO\"" }
   defines { "rsc_LegalCopyright=\"MIT License\""} 
   defines { "rsc_InternalName=\"%{prj.name}\"", "rsc_ProductName=\"%{prj.name}\"", "rsc_OriginalFilename=\"%{prj.name}.asi\"" }
   defines { "rsc_FileDescription=\"https://cleo.li\"" }
   defines { "rsc_UpdateUrl=\"https://github.com/cleolibrary/III.VC.CLEO\"" }
   
   files { "source/III.VC.CLEO/*.h", "source/III.VC.CLEO/*.cpp" }
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

   function setpaths (gamepath, exepath, scriptspath)
      scriptspath = scriptspath or "scripts/"
      if (gamepath) then
         cmdcopy = { "set \"path=" .. gamepath .. scriptspath .. "\"" }
         table.insert(cmdcopy, pbcommands)
         postbuildcommands (cmdcopy)
         debugdir (gamepath)
         if (exepath) then
            debugcommand (gamepath .. exepath)
            dir, file = exepath:match'(.*/)(.*)'
            debugdir (gamepath .. (dir or ""))
         end
      end
      targetdir ("bin" .. scriptspath)
   end
   
   configuration "Release_xp"
      toolset "v141_xp"
      buildoptions { "/Zc:threadSafeInit-" }
   
   filter "configurations:Debug*"
      defines "DEBUG"
      symbols "On"

   filter "configurations:Release*"
      defines "NDEBUG"
      optimize "On"

project "III.CLEO"
   defines { "CLEO_III" }
   setpaths("Z:/WGTA/gta3sc_test/Grand Theft Auto III/", "gta3.exe", "")
project "VC.CLEO"
   defines { "CLEO_VC" }
   setpaths("Z:/WGTA/gta3sc_test/Grand Theft Auto Vice City/", "gta-vc.exe", "")

   
   
   
   
   
workspace "CLEO_SDK"
   configurations { "Release", "Debug", "Release_xp" }
   platforms { "Win32" }
   architecture "x32"
   location "build"
   objdir ("build/obj")
   buildlog ("build/log/%{prj.name}.log")
   
   kind "SharedLib"
   language "C++"
   targetdir "bin/CLEO/CLEO_PLUGINS"
   targetextension ".cleo"
   characterset ("MBCS")
   flags { "StaticRuntime" }
   
   defines { "rsc_CompanyName=\"CLEO\"" }
   defines { "rsc_LegalCopyright=\"MIT License\""} 
   defines { "rsc_InternalName=\"%{prj.name}\"", "rsc_ProductName=\"%{prj.name}\"", "rsc_OriginalFilename=\"%{prj.name}.asi\"" }
   defines { "rsc_FileDescription=\"https://cleo.li\"" }
   defines { "rsc_UpdateUrl=\"https://github.com/cleolibrary/III.VC.CLEO\"" }
   
   includedirs { "source/CLEO_SDK" }
   libdirs { "bin" }
   
   configuration "Release_xp"
      toolset "v141_xp"
      buildoptions { "/Zc:threadSafeInit-" }
   
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
project "III.FileSystemOperations"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.h", "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.cpp" }
   files { "Resources/*.rc" }
project "III.IniFiles"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/IniFiles/*.h", "source/CLEO_SDK/demo_plugins/IniFiles/*.cpp" }
   files { "Resources/*.rc" }
project "III.IntOperations"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/IntOperations/*.h", "source/CLEO_SDK/demo_plugins/IntOperations/*.cpp" }
   files { "Resources/*.rc" }
project "III.MemoryModule"
   defines { "CLEO_III" }
   files { "source/CLEO_SDK/demo_plugins/MemoryModule/*.h", "source/CLEO_SDK/demo_plugins/MemoryModule/*.cpp", "source/CLEO_SDK/demo_plugins/MemoryModule/*.c" }
   files { "Resources/*.rc" }
project "VC.ClipboardControl"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/ClipboardControl/*.h", "source/CLEO_SDK/demo_plugins/ClipboardControl/*.cpp" }
   files { "Resources/*.rc" }
project "VC.FileSystemOperations"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.h", "source/CLEO_SDK/demo_plugins/FileSystemOperations/*.cpp" }
   files { "Resources/*.rc" }
project "VC.IniFiles"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/IniFiles/*.h", "source/CLEO_SDK/demo_plugins/IniFiles/*.cpp" }
   files { "Resources/*.rc" }
project "VC.IntOperations"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/IntOperations/*.h", "source/CLEO_SDK/demo_plugins/IntOperations/*.cpp" }
   files { "Resources/*.rc" }
project "VC.MemoryModule"
   defines { "CLEO_VC" }
   files { "source/CLEO_SDK/demo_plugins/MemoryModule/*.h", "source/CLEO_SDK/demo_plugins/MemoryModule/*.cpp", "source/CLEO_SDK/demo_plugins/MemoryModule/*.c" }
   files { "Resources/*.rc" }