const { readFileSync, writeFileSync } = require("fs");
const { EOL } = require("os");
const { GITHUB_REF_NAME } = process.env;
const addJobOutput = require('./add-job-output');

if (GITHUB_REF_NAME) {
  const version = GITHUB_REF_NAME.startsWith("v") ? GITHUB_REF_NAME.slice(1) : GITHUB_REF_NAME;
  addJobOutput("version", version);

  // update CleoVersion.h to replace version numbers
  const cleoH = readFileSync("source/III.VC.CLEO/CleoVersion.h", { encoding: "utf-8" });

  const [, main, major, minor] = version.match(/(\d+)\.(\d+)\.(\d+).*/);

  const newCleoH = cleoH
    .replace(/#define\s+CLEO_VERSION_MAIN\s+.*/, `#define CLEO_VERSION_MAIN    ${main}`)
    .replace(/#define\s+CLEO_VERSION_MAJOR\s+.*/, `#define CLEO_VERSION_MAJOR   ${major}`)
    .replace(/#define\s+CLEO_VERSION_MINOR\s+.*/, `#define CLEO_VERSION_MINOR   ${minor}`);
  writeFileSync("source/III.VC.CLEO/CleoVersion.h", newCleoH, { encoding: "utf-8" });
}

const changelog = readFileSync("CHANGELOG.md", { encoding: "utf-8" });
writeFileSync("changes.txt", getChanges().join(EOL), { encoding: "utf-8" });


function getChanges() {
  const ref = GITHUB_REF_NAME || "latest";
  const result = [
    `## Download Instructions`,
    ``,
    `### GTA III`,
    `- [III.CLEO ${ref}](https://github.com/cleolibrary/III.VC.CLEO/releases/download/${ref}/III.CLEO-${ref}.zip)`,
    ``,
    `### GTA Vice City`,
    `- [VC.CLEO ${ref}](https://github.com/cleolibrary/III.VC.CLEO/releases/download/${ref}/VC.CLEO-${ref}.zip)`,
    ``,
    `## Installation`,
    `- Unzip the archive to the game directory.`,
    ``,
    `## CHANGELOG`,
  ];

  const lines = changelog.split(/\r?\n/);
  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.trimStart().startsWith("## ")) {
      for (let j = i + 1; j < lines.length; j++) {
        const nextLine = lines[j];
        if (nextLine.trimStart().startsWith("## ")) {
          return result;
        }
        result.push(nextLine);
      }
    }
  }

  return result;
}
