# Using Debug Info from `pmdsky-debug`
- [Using Debug Info from `pmdsky-debug`](#using-debug-info-from-pmdsky-debug)
  - [Ghidra](#ghidra)
    - [Symbols](#symbols)
      - [Using the built-in import script](#using-the-built-in-import-script)
      - [Using the custom `pmdsky-debug` import script](#using-the-custom-pmdsky-debug-import-script)
      - [Using the NTRGhidra import script](#using-the-ntrghidra-import-script)
    - [C headers (types and function signatures)](#c-headers-types-and-function-signatures)
      - [Applying types manually](#applying-types-manually)
      - [Enums](#enums)
        - [Workaround for enums in struct fields](#workaround-for-enums-in-struct-fields)
      - [Bitfields](#bitfields)
    - [Subsequent imports](#subsequent-imports)
  - [No$GBA](#nogba)

## Ghidra
Ghidra can load both symbols and C source code. These instructions assume a specific Ghidra setup. See the preceding steps in [Setting up Ghidra for _Pokémon Mystery Dungeon: Explorers of Sky_](ghidra-setup.md) first.

### Symbols
There are two options for importing `pmdsky-debug` symbols into Ghidra: with the built-in import script (easier) or with a custom one from `pmdsky-debug` (better). Using the built-in script requires slightly less setup, but won't import symbol descriptions, whereas the custom `pmdsky-debug` importer will create plate comments containing the symbol descriptions. The two options are otherwise equivalent.

If you used [NTRGhidra](https://github.com/pedro-javierf/NTRGhidra) to set up your project, you must use a special import script. See [Using the NTRGhidra import script](#using-the-ntrghidra-import-script).

#### Using the built-in import script
1. Download `symbols-ghidra.zip` from the [latest release package](https://github.com/UsernameFodder/pmdsky-debug/releases/latest) and extract the archive.
2. In the Ghidra code browser, open the script manager (Window > Script Manager in the menu, or by clicking the image in the top toolbar).
3. Search for "ImportSymbolsScript.py".

   ![ImportSymbolsScript.py](images/ghidra-import-symbols-script.png)

4. When you run the script, it will open a file picker window. Run the import script on each of the `.ghidra` files in the `pmdsky-debug` archive that correspond to a binary you've loaded into the Ghidra program (pick the version subdirectory that matches your ROM). **Do _not_ blanket-load every single file**, as this will cause incorrect symbols to be applied (due to [overlays](overlays.md)), which is difficult to correct later.

   Alternatively, you can concatenate the contents of all the relevant `.ghidra` files into a single, combined `.ghidra` file and just run the import script once on that file. For example, a combined `.ghidra` file for a North American ROM might look like this:

   ![Example Ghidra symbol table](images/ghidra-symbol-file.png)

5. You should now see symbol names in the code listing and the decompiler:

   ![Symbol names in Ghidra](images/ghidra-symbols.png)

#### Using the custom `pmdsky-debug` import script
1. Download `symbols-json.zip` from the [latest release package](https://github.com/UsernameFodder/pmdsky-debug/releases/latest) and extract the archive.
2. In the Ghidra code browser, open the script manager (Window > Script Manager in the menu, or by clicking the image in the top toolbar).
3. Add [`import_symbols_json.py`](../tools/ghidra_scripts/import_symbols_json.py) to the Ghidra Script Manager. There are two ways to do this:
    1. [Preferred] Add a script directory containing the file (e.g., the `pmdsky-debug/tools/ghidra_scripts` directory) with the "Manage Script Directories" button.

       ![Manage Script Directories](images/ghidra-manage-script-directories.png)

    2. Create a new script with the "Create New Script" button and manually copy the code into the editor (select "Python" for the script type and name it `import_symbols_json.py`).

       ![Create New Script](images/ghidra-create-new-script.png)

4. Search for the "import_symbols_json.py" script you just added and run it. When you run the script, it will open a file picker window. Run the import script on each of the `.json` files in the `pmdsky-debug` archive that correspond to a binary you've loaded into the Ghidra program (pick the version subdirectory that matches your ROM). **Do _not_ blanket-load every single file**, as this will cause incorrect symbols to be applied (due to [overlays](overlays.md)), which is difficult to correct later.
5. You should now see symbol names and descriptions in the code listing and the decompiler:

   ![Symbol names and descriptions in Ghidra](images/ghidra-symbols-with-descriptions.png)

#### Using the NTRGhidra import script
Importing into an NTRGhidra project is essentially the same as [using the custom `pmdsky-debug` import script](#using-the-custom-pmdsky-debug-import-script), with the following differences:
1. Use [`import_symbols_ntr_ghidra.py`](../tools/ghidra_scripts/import_symbols_ntr_ghidra.py) rather than `import_symbols_json.py`.
2. When the script opens the file picker window, select the version subdirectory within the `symbols-json` archive that matches your ROM (e.g., `symbols-json/NA/` for the North American version), rather than a specific JSON file. The script will import symbols from all the relevant files at once.

### C headers (types and function signatures)
Importing C headers is best done after importing symbols, so make sure you've followed the steps in the [previous section](#symbols) before proceeding with this section.

1. Download `headers.zip` from the [latest release package](https://github.com/UsernameFodder/pmdsky-debug/releases/latest) and extract the archive.
2. Open the C parser in the menu with File > Parse C Source...
3. Configure the appropriate settings:
    1. For the parse configuration, select `generic_clib_32.prf` (`clib.prf` should work too). I haven't tested all of the default profiles, but this one is most aligned with the `pmdsky-debug` C headers.
    2. Remove all the existing "Source files to parse" by highlighting them all and hitting the red 'x' button (`pmdsky-debug`'s headers are standalone, so this would just be extra cruft). Then hit the green '+' button and select one of `pmdsky_na.h` (North American), `pmdsky_eu.h` (European), or `pmdsky_jp.h` (Japanese) from the `pmdsky-debug` headers package, depending on your game version. After this step, the variant of `pmdsky_*.h` should be the only file in the list.
    3. Leave the parse options as the defaults.
4. Click "Parse to Program" and the confirmations in the popups that follow to begin parsing. Some notes:
    1. Annoyingly, sometimes the "Parsing C" loading window can pop up before you've hit all the confirmations, and actually obscure a confirmation window. Make sure you check that there's nothing behind this window (otherwise you'll be waiting forever).
    2. The load time shouldn't take more than a few seconds to a minute.

       ![Parsing C loading window](images/ghidra-parsing-c.png)

5. You should now be able to view and edit imported types and function signatures in the data type manager (Window > Data Type Manager, if it's not already open).

   ![Ghidra data type manager](images/ghidra-data-types.png)

6. Right click on your project's archive in the data type manager (the one labeled with the program name you specified when setting up the project; "dungeon_mode" in the above image) and select "Apply Function Data Types". This will match the function signatures from the C headers with the function symbols you imported in the [previous section](#symbols), and apply parameter names and types to them.
7. You should now see additional parameter information (types and names) in the decompiler. Additionally, Ghidra should automatically propagate type information to other variables within functions where they can be inferred. Be aware that some registers in the assembly (in the code listing) will be bound to a parameter name for the duration of the function, even if the registers are repurposed throughout the function's execution.

   ![Parameter information in Ghidra](images/ghidra-parameters.png)

#### Applying types manually
Sometimes data types won't be assigned automatically in the decompiler, and you'll need to do it manually to get the full benefit of the imported types. In the decompiler, you can right-click on a variable and select "Retype Variable" (or sometimes "Retype Global") and specify the name of a type to manually apply to the variable in the decompiled code. This is most useful for struct pointers that haven't been automatically typed, since this allows the decompiler to show struct field accesses by name (e.g., `some_struct->some_named_field`) rather than as raw pointer operations (e.g., `*(some_pointer + some_offset)`).

![Retyping a variable in Ghidra](images/ghidra-retype-variable.png)

Similarly, you can also apply types to data labels in the code listing by right-clicking and selecting "Data" > "Choose Data Type...".

![Retyping a data label in Ghidra](images/ghidra-retype-label.png)

#### Enums
There are two ways to make use of enums: types and equates.

In some cases, the Ghidra decompiler can use type information to automatically resolve a constant value to its appropriate enum label. For example, in the `IsGoldenChamber` function, Ghidra automatically resolves the raw value of `3` and displays the more useful `MISSION_EXPLORE_WITH_CLIENT` (from `enum mission_type`).

![Ghidra can automatically resolve enum labels](images/ghidra-enum-labels.png)

However, automatic resolution might not always be possible. For example, the raw value of `2` in the `IsGoldenChamber` was not resolved. If the value of interest is stored in a local variable, you can try [applying the enum type manually](#applying-types-manually). Otherwise, you can use a Ghidra "equate". _Equates_ are a feature that allows you to associate an instance of a constant value with a name or enum label. You can assign an equate by right-clicking on the numeric value and selecting "Set Equate...", then selecting the enum you want to use to interpret the value. For example, in `IsGoldenChamber` the `2` should be interpreted using `enum mission_subtype_explore`. After applying the equate, the decompiler will show the enum label `MISSION_EXPLORE_GOLDEN_CHAMBER` rather than the raw value of `2`. You can follow the same procedure in the listing if you want equates to show up in the raw assembly instructions.

![Ghidra equates](images/ghidra-equates.png)

##### Workaround for enums in struct fields
One common case where automatic enum resolution fails is when the enum is a bitfield within a struct (see the [`headers/` README](../headers/README.md) for why things are done this way). Since the Ghidra decompiler [does not yet support bitfields](#bitfields), these fields won't be recognized or resolved to enum labels. Until this feature is added, you can use equates as described in the previous section.

Alternatively, if you really prefer automatic resolution, another workaround is to manually define a new, properly-sized enum (in the data type manager: Window > Data Type Manager) by copying the existing enum and changing the `Size` field in the editor, then editing the struct definition to use the properly-sized, non-bitfield enum for the relevant field. For example, you could create a new enum called `item_id_16` with a size of 2 bytes, then change `struct item::id` to be of type `item_id_16` rather than a bitfield of type `item_id` (which is normally defined to be 4 bytes). After doing this, the Ghidra decompiler should be able to automatically resolve enum labels when `struct item::id` is used (you can also use the resized enums as [types for certain data labels](#applying-types-manually)). Be aware that this may cause some conflicts with [subsequent debug info imports](#subsequent-imports).

#### Bitfields
_Explorers of Sky_ makes frequent use of bitfields. Ghidra supports bitfields in struct definitions, but they unfortunately don't yet work in the decompiler (support for them is "on the books", see [ghidra#2462](https://github.com/NationalSecurityAgency/ghidra/issues/2462) and [ghidra#647](https://github.com/NationalSecurityAgency/ghidra/issues/647)). This means that some struct fields might appear as undefined in the decompiler, even though they are defined in the struct definition. For example, the room flags bitfield at offset 0xD of `struct floor_properties` will show up as the Ghidra-default `field_0xd` in the decompiler.

![Bitfields currently won't show up in the Ghidra decompiler](images/ghidra-bitfields.png)

This is something to be aware of while reading decompiled code. If a field looks undefined, don't forget to check the actual struct definition in the data type manager or the C headers to see if it's a bitfield. If this really bothers you, you can redefine that field (in the data type manager: Window > Data Type Manager) to a normal integer field with a name like `flags`. Be aware that this may cause some conflicts with [subsequent debug info imports](#subsequent-imports).

### Subsequent imports
As `pmdsky-debug` is updated, you might want to import the latest debug information into your project. Ghidra is pretty good about checking for repeated information when you import things, so if you've already imported a previous version of the debug info, you should be able to just follow the above steps for importing symbols and headers with the latest `pmdsky-debug` package to add incremental changes. (Although if there are changes to existing things in the debug info rather than just new additions, you might have to do some manual cleanup afterwards.)

## No$GBA
The No$GBA debugger supports loading symbol names from a `.sym` file.

1. Download `symbols-sym.zip` from the [latest release package](https://github.com/UsernameFodder/pmdsky-debug/releases/latest) and extract the archive.
2. Locate the ROM file that you will be running in No$GBA, and create a text file in the same folder with the same name, but with the `.sym` extension. For example, if your ROM was called `pmd-eos.nds`, you should create a text file called `pmd-eos.sym`, and the folder should look like this in the file explorer:

   ![Example ROM file with its accompanying symbol table](images/nocash-files.png)

3. Locate which subdirectories you will have to use, which depends on the version of your ROM. You will need the files from all the subdirectories starting with your ROM's version identifier. For example, if you're working with an North American ROM, you'll need the files under the `NA` and `NA-ITCM` subdirectories.
4. Figure out which [overlays](overlays.md) will be loaded during the gameplay that you're debugging with No$GBA. Copy the contents of all the corresponding `.sym` files from each one of the relevant subdirectories into the `.sym` file you just created, concatenating the lines of the different files together. Make sure to start copying from the `xx-ITCM` subdirectory first. Along with the overlays, you'll always want to include the contents of `arm9_xx.sym`, and if relevant you might also want to include the contents of `ram_xx.sym`. For example, a `.sym` file for a North American ROM might look like this:

   ![Example symbol table](images/nocash-symfile.png)

4. Load the ROM in No\$GBA! The debugger will automatically detect the `.sym` file and load it, and symbol name annotations will appear alongside the assembly instructions when debugging:

   ![Symbol names in the No$GBA debugger](images/nocash-symbols.png)
