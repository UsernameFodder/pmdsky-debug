// Imports pmdsky-debug symbols into an NTRGhidra project
// @author UsernameFodder
// @category Import

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSpace;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.DuplicateNameException;

public class ImportSymbolsNtrGhidra extends GhidraScript {

    private static final String COMMENT_TAG = "=== imported description ===\n";
    private static final Pattern OVERLAY_FILE_PATTERN = Pattern.compile("overlay(\\d+)");

    private static class SymbolEntry {
        String type;
        String name;
        Object address;
        String description;
        List<String> aliases;

        public List<String> getAliases() {
            return aliases != null ? aliases : Collections.emptyList();
        }
    }

    @Override
    public void run() throws Exception {
        Map<String, AddressSpace> addrSpaceMap = buildAddressSpaceMap();

        File dir = askDirectory("Select directory containing symbol JSON files", "Import");
        File[] files = dir.listFiles();
        if (files == null) {
            printerr("Selected directory is invalid or empty.");
            return;
        }

        Arrays.sort(files, (f1, f2) -> f1.getName().compareTo(f2.getName()));

        for (File file : files) {
            if (!shouldImport(file.getName())) {
                continue;
            }

            AddressSpace addrSpace;
            try {
                addrSpace = getFileAddrSpace(file.getName(), addrSpaceMap);
            } catch (IllegalArgumentException e) {
                println("Could not find address space for " + file.getName() + ", skipping");
                continue;
            }

            List<SymbolEntry> symbols = loadSymbols(file);
            for (SymbolEntry symbol : symbols) {
                processSymbol(symbol, addrSpace);
            }
        }
    }

    private Map<String, AddressSpace> buildAddressSpaceMap() {
        Map<String, AddressSpace> map = new HashMap<>();
        AddressSpace[] spaces = currentProgram.getAddressFactory().getAddressSpaces();
        for (AddressSpace space : spaces) {
            map.put(space.getName(), space);
        }
        return map;
    }

    private boolean shouldImport(String filename) {
        return filename.contains("arm") || filename.contains("overlay");
    }

    private AddressSpace getFileAddrSpace(String filename, Map<String, AddressSpace> addrSpaceMap) throws Exception {
        Matcher matcher = OVERLAY_FILE_PATTERN.matcher(filename);
        if (matcher.lookingAt()) {
            // Convert to integer to strip off any leading zeroes
            int overlayNum = Integer.parseInt(matcher.group(1));
            Pattern overlayTargetPattern = Pattern.compile("overlay_0*" + overlayNum);

            for (Map.Entry<String, AddressSpace> entry : addrSpaceMap.entrySet()) {
                if (overlayTargetPattern.matcher(entry.getKey()).matches()) {
                    return entry.getValue();
                }
            }
            throw new IllegalArgumentException(filename);
        }

        AddressSpace ramSpace = addrSpaceMap.get("ram");
        if (ramSpace != null) {
            return ramSpace;
        }
        throw new IllegalArgumentException(filename);
    }

    private List<SymbolEntry> loadSymbols(File file) throws IOException {
        try (BufferedReader reader = Files.newBufferedReader(file.toPath())) {
            Gson gson = new Gson();
            TypeToken<List<SymbolEntry>> symbolListType = new TypeToken<List<SymbolEntry>>() {};
            return gson.fromJson(reader, symbolListType.getType());
        }
    }

    private void processSymbol(SymbolEntry symbol, AddressSpace addrSpace) throws Exception {
        Address address = parseAddress(symbol.address, addrSpace);
        if (address == null) {
            printerr("Skipping symbol with invalid address: " + symbol.name);
            return;
        }

        if ("function".equals(symbol.type)) {
            handleFunction(address, symbol.name);
        } else {
            println("Created primary label " + symbol.name + " at address " + address);
            createLabel(address, symbol.name, true);
        }

        for (String alias : symbol.getAliases()) {
            println("Created label " + alias + " at address " + address);
            createLabel(address, alias, false);
        }

        if (symbol.description != null && !symbol.description.isEmpty()) {
            updatePlateComment(address, symbol.description);
        }
    }

    private void handleFunction(Address address, String name) throws Exception {
        FunctionManager functionManager = currentProgram.getFunctionManager();
        Function func = functionManager.getFunctionAt(address);

        if (func == null) {
            createFunction(address, name);
            println("Created function " + name + " at address " + address);
        } else {
            String oldName = func.getName();
            try {
                func.setName(name, SourceType.USER_DEFINED);
            } catch (DuplicateNameException e) {
                // This can happen if name already exists, but is not the
                // primary label, which can happen if it was overridden by a
                // different label. In this case, just set it as the primary.
                createLabel(address, name, true);
            }
            println("Renamed function " + oldName + " to " + name + " at address " + address);
        }
    }

    private Address parseAddress(Object addrObj, AddressSpace addrSpace) {
        if (addrObj == null) {
            return null;
        }

        long offset;
        if (addrObj instanceof Number) {
            offset = ((Number) addrObj).longValue();
        } else {
            String addrStr = addrObj.toString().trim();
            if (addrStr.startsWith("0x") || addrStr.startsWith("0X")) {
                offset = Long.parseLong(addrStr.substring(2), 16);
            } else {
                offset = Long.parseLong(addrStr);
            }
        }
        return addrSpace.getAddressInThisSpaceOnly(offset);
    }

    private void updatePlateComment(Address address, String description) {
        String existingComment = getPlateComment(address);
        String updatedComment;

        if (existingComment == null || existingComment.isEmpty()) {
            updatedComment = COMMENT_TAG + description;
        } else {
            String[] parts = existingComment.split(COMMENT_TAG);
            if (parts.length == 1) {
                // Append description if the comment tag was not found
                updatedComment = parts[0] + "\n\n" + COMMENT_TAG + description;
            } else {
                // Replace old description while preserving the comment prefix
                updatedComment = parts[0] + COMMENT_TAG + description;
            }
        }
        setPlateComment(address, updatedComment);
    }
}
