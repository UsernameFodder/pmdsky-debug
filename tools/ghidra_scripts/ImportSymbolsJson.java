// Imports symbols from a JSON file
// @author UsernameFodder
// @category Import

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.Collections;
import java.util.List;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.DuplicateNameException;

public class ImportSymbolsJson extends GhidraScript {

    private static final String COMMENT_TAG = "=== imported description ===\n";

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
        File jsonFile = askFile("Select symbol JSON file", "Import");

        List<SymbolEntry> symbols = loadSymbols(jsonFile);
        for (SymbolEntry symbol : symbols) {
            processSymbol(symbol);
        }
    }

    private List<SymbolEntry> loadSymbols(File file) throws IOException {
        try (BufferedReader reader = Files.newBufferedReader(file.toPath())) {
            Gson gson = new Gson();
            TypeToken<List<SymbolEntry>> symbolListType = new TypeToken<List<SymbolEntry>>() {};
            return gson.fromJson(reader, symbolListType.getType());
        }
    }

    private void processSymbol(SymbolEntry symbol) throws Exception {
        Address address = parseAddress(symbol.address);
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

    private Address parseAddress(Object addrObj) {
        if (addrObj == null) {
            return null;
        }

        if (addrObj instanceof Number) {
            return toAddr(((Number) addrObj).longValue());
        }

        String addrStr = addrObj.toString().trim();
        if (addrStr.startsWith("0x") || addrStr.startsWith("0X")) {
            return toAddr(Long.parseLong(addrStr.substring(2), 16));
        }
        return toAddr(Long.parseLong(addrStr));
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
