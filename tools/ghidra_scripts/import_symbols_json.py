# Imports symbols from a JSON file
# @author UsernameFodder
# @category Data

import json
import ghidra.program.model.symbol.SourceType as SourceType

COMMENT_TAG = "=== imported description ===\n"
functionManager = currentProgram.getFunctionManager()

jython_file = askFile("Select symbol JSON file", "Importing symbols...")
with open(jython_file.absolutePath, "r") as f:
    symbols = json.load(f)

for s in symbols:
    stype = s["type"]
    name = s["name"]
    address = s["address"]
    if type(address) != int:
        address = int(address, 0)
    address = toAddr(address)
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
        print("Created label {} at address {}".format(name, address))
        createLabel(address, name, False)

    if description:
        comment = getPlateComment(address)
        if not comment:
            comment = COMMENT_TAG + description
        else:
            comment_parts = comment.split(COMMENT_TAG)
            if len(comment_parts) == 1:
                comment_parts[0] += "\n\n"
                comment_parts.append("")
            comment_parts[1] = description
            comment = COMMENT_TAG.join(comment_parts)
        setPlateComment(address, comment)
