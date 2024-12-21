## 2.1.1

- Added support for chinese text in fxt files. ([#16](https://github.com/cleolibrary/III.VC.CLEO/issues/16)) ([#18](https://github.com/cleolibrary/III.VC.CLEO/issues/18))
- Opcode 0605 (GET_WEAPONTYPE_FOR_MODEL) supports negative values for the parameter model id. ([#17](https://github.com/cleolibrary/III.VC.CLEO/issues/17))
- Support short string parameters in opcodes. ([#29](https://github.com/cleolibrary/III.VC.CLEO/issues/29))
- Added opcodes 0AF8 (SET_CLEO_ARRAY), 0AF9 (GET_CLEO_ARRAY), 0AFA (GET_CLEO_ARRAY_OFFSET), 0AFB (GET_CLEO_ARRAY_SCRIPT).
- Added CLEO4 opcodes 0AB7 (GET_CAR_NUMBER_OF_GEARS), 0AB8 (GET_CAR_CURRENT_GEAR), 0ABD (IS_CAR_LIGHTS_ON), 0ABE (IS_CAR_ENGINE_ON), 0ABF (CLEO_SET_CAR_ENGINE_ON), 0AE4 (DOES_DIRECTORY_EXIST), 0AE5 (CREATE_DIRECTORY), 0AE6 (
FIND_FIRST_FILE), 0AE7 (FIND_NEXT_FILE), 0AE8 (FIND_CLOSE).
- A some other minor fixes and improvements.

## 2.1.0

- Added opcode 0DD5 (get_platform).
- Partially fixed 0ADD (spawn car like a cheat) for GTA3. Now opcode can spawn not only the tank. Still no support for models with id greater than 127.
- Fixed 0ADB to return GXT label instead of car name (same as in Cleo SA).
- Crash fix in 0AF4 read_string_from_ini_file. Fixed some internal data-type sizes.
- Version numbering changed from "x.x.x.x" to "x.x.x". Introduced readme and changelog files.
- Updated build tools and scripts.
- Added C-style exports.
