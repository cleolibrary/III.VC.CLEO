## 2.1.1

- Added support for chinese text in fxt files. ([#16](https://github.com/cleolibrary/III.VC.CLEO/issues/16)) ([#18](https://github.com/cleolibrary/III.VC.CLEO/issues/18))
- Opcode 0605 (GET_WEAPONTYPE_FOR_MODEL) supports negative values for the parameter model id. ([#17](https://github.com/cleolibrary/III.VC.CLEO/issues/17))
- Support short string parameters in opcodes. ([#29](https://github.com/cleolibrary/III.VC.CLEO/issues/29))
- Added opcodes 0AF8 (SET_CLEO_ARRAY), 0AF9 (GET_CLEO_ARRAY), 0AFA (GET_CLEO_ARRAY_OFFSET), 0AFB (GET_CLEO_ARRAY_SCRIPT).
- Added CLEO4 opcodes 0AB7 (GET_CAR_NUMBER_OF_GEARS), 0AB8 (GET_CAR_CURRENT_GEAR), 0ABD (IS_CAR_LIGHTS_ON), 0ABE (IS_CAR_ENGINE_ON), 0ABF (CLEO_SET_CAR_ENGINE_ON), 0AE4 (DOES_DIRECTORY_EXIST), 0AE5 (CREATE_DIRECTORY), 0AE6 (
FIND_FIRST_FILE), 0AE7 (FIND_NEXT_FILE), 0AE8 (FIND_CLOSE).
- A some other minor fixes and improvements.

## 2.1.0

- Added opcode **0DD5 ([get_platform](https://library.sannybuilder.com/#/gta3/CLEO/0DD5))**
- Added opcode **2002 ([cleo_return_with](https://library.sannybuilder.com/#/gta3/CLEO/2002))**
- Added opcode **2003 ([cleo_return_fail](https://library.sannybuilder.com/#/gta3/CLEO/2003))**
- 'argument count' parameter of **0AB1 (cleo_call)** is now optional. `cleo_call @LABEL args 0` can be written as `cleo_call @LABEL`
- 'argument count' parameter of **0AB2 (cleo_return)** is now optional. `cleo_return 0` can be written as `cleo_return`
- SCM functions **(0AB1)** now keep their own GOSUB's call stack
- correct support of condition result of opcode **0AB1 (cleo_call)** used in multi conditional 'if' statements.
- Partially fixed 0ADD (spawn car like a cheat) for GTA3. Now opcode can spawn not only the tank. Still no support for models with id greater than 127.
- Fixed 0ADB to return GXT label instead of car name (same as in Cleo SA).
- Crash fix in 0AF4 read_string_from_ini_file. Fixed some internal data-type sizes.
- Version numbering changed from "x.x.x.x" to "x.x.x". Introduced readme and changelog files.
- Updated build tools and scripts.
- Added C-style exports.
