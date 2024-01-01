# Imports pmdsky-debug symbols into an NTRGhidra project
# @author UsernameFodder
# @category Data

import json
import os
import re
import ghidra.program.model.symbol.SourceType as SourceType

COMMENT_TAG = "=== imported description ===\n"

functionManager = currentProgram.getFunctionManager()

globalNsBody = currentProgram.globalNamespace.body
addrSpaceMap = {r.addressSpace.name: r.addressSpace for r in globalNsBody}


def shouldImport(filename):
    return "arm" in filename or "overlay" in filename


def getFileAddrSpace(filename):
    match = re.match("overlay(\d+)", os.path.basename(filename))
    if match:
        overlayNum = int(match.group(1))
        # This is the naming scheme NTRGhidra uses for overlays
        return "overlay_{}".format(overlayNum)
    return "ram"  # Default address space name


jythonDir = askDirectory("Select directory containing symbol JSON files", "Import")
filenames = sorted(
    [
        os.path.join(jythonDir.absolutePath, f)
        for f in os.listdir(jythonDir.absolutePath)
        if shouldImport(f)
    ]
)

for fname in filenames:
    addrSpace = addrSpaceMap[getFileAddrSpace(fname)]
    with open(fname, "r") as f:
        symbols = json.load(f)

    for s in symbols:
        stype = s["type"]
        name = s["name"]
        address = s["address"]
        if type(address) != int:
            address = int(address, 0)
        address = addrSpace.getAddressInThisSpaceOnly(address)
        description = s.get("description")

        if stype == "function":
            func = functionManager.getFunctionAt(address)

            if func is None:
                func = createFunction(address, name)
                print("Created function {} at address {}".format(name, address))
            else:
                old_name = func.getName()
                func.setName(name, SourceType.USER_DEFINED)
                print(
                    "Renamed function {} to {} at address {}".format(
                        old_name, name, address
                    )
                )
        else:
            print("Created primary label {} at address {}".format(name, address))
            createLabel(address, name, True)
        for alias in s.get("aliases", []):
            print("Created label {} at address {}".format(alias, address))
            createLabel(address, alias, False)

        if description:
            comment = getPlateComment(address)
            if not comment:
                comment = COMMENT_TAG + description
            else:
                commentParts = comment.split(COMMENT_TAG)
                if len(commentParts) == 1:
                    commentParts[0] += "\n\n"
                    commentParts.append("")
                commentParts[1] = description
                comment = COMMENT_TAG.join(commentParts)
            setPlateComment(address, comment)
