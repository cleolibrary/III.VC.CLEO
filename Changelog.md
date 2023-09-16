## 2.1.0

- Added opcode 0DD5 (get_platform).
- Partially fixed 0ADD (spawn car like a cheat) for GTA3. Now opcode can spawn not only the tank. Still no support for models with id greater than 127.
- Fixed 0ADB to return GXT label instead of car name (same as in Cleo SA).
- Crash fix in 0AF4 read_string_from_ini_file. Fixed some internal data-type sizes.
- Version numbering changed from "x.x.x.x" to "x.x.x". Introduced readme and changelog files.
- Updated build tools and scripts.
- Added C-style exports.
