# Nintendo DS Overlays
## What is an overlay?
Like many other embedded systems, the Nintendo DS is quite memory-constrained (it only has 4 MiB of RAM). In order to reduce RAM usage, NDS games like _Explorers of Sky_ use a technique called [overlaying](https://en.wikipedia.org/wiki/Overlay_(programming)).

A program that uses overlays is split up into multiple binary files, with each file containing a just a subset of the instructions needed to run the program as a whole. When the program needs to run code from a certain overlay, it will load that overlay into memory for execution. When an overlay is no longer needed, it will be unloaded (and possibly swapped for a different overlay) to free up space for other purposes.

Overlaying is particularly useful if the program has certain chunks of code that are used very infrequently, or if it has several distinct modes of execution that will never be run at the same time. For example, EoS has separate overlays for controlling various menu interfaces for things like the Kecleon Shop and Kangaskhan Storage; this code isn't used very often, so keeping it loaded all the time would just waste space. It also has separate overlays for controlling "ground mode" (when you're walking around freely in the overworld or watching cutscenes) and "dungeon mode" (when you're exploring a Mystery Dungeon); there's quite a lot of code for these two modes, but since they never run at the same time, the game can save quite a lot of space by only loading one at a time (in fact, these two overlays have the same load address, so the game quite literally swaps between the two binaries when switching modes).

## NDS overlay specifics
NDS overlays are stored as special files in a ROM's file system (the [NitroFS](https://problemkaputt.de/gbatek.htm#dscartridgenitroromandnitroarcfilesystems)) (you can access them by [unpacking the ROM](ghidra-setup.md#unpack-the-rom)). Each overlay has a specific load address assigned to it: when an overlay is loaded, it will always be loaded at its assigned location in memory. The load addresses for EoS overlays have already been documented; you can see them in their respective files in [`symbols/`](../symbols) (the top-level `address` field) (or in the [Project Pokémon technical documentation](https://projectpokemon.org/home/docs/mystery-dungeon-nds/pok%C3%A9mon-mystery-dungeon-explorers-r78/), although it only lists addresses for the North American version). If you're wondering where these addresses come from, they're directly encoded in the [overlay tables](https://problemkaputt.de/gbatek.htm#dscartridgenitroromandnitroarcfilesystems) in the ROM itself.

_Side note: The phrase "load an overlay" is quite literal and means the same thing as loading any other binary. If you look at RAM (for example, DeSmuME and No$GBA both have memory viewers on Windows) at an overlay's load address after it's been loaded, you'll see the exact same bytes you would see if you opened the overlay binary in a hex editor._

Of course, overlays can't load themselves. When an NDS game is booted up, one of the first things the system does is load the main ARM9 binary (often called `arm9.bin`) into memory, which contains the "core" code needed to run the game (actually, there's also a second ARM7 binary for the NDS's second CPU, but you don't need to worry about it in most cases when working with EoS). This core code includes instructions to load and unload other overlays, as needed by the game. Unlike the overlays, the ARM9 binary is never unloaded while the game is running.

## _Explorers of Sky_ overlay specifics
Overlays add some extra complexity when reverse engineering NDS games like EoS, because it means you need to know which overlays are relevant to the part of the game you're trying to research. It also complicates setup for reverse engineering tools like Ghidra. Fortunately, the purposes of many EoS overlays (especially the important ones) have already been documented; you can see descriptions of each overlay in their respective files in [`symbols/`](../symbols) (the top-level `description` field) (or in the [Project Pokémon technical documentation](https://projectpokemon.org/home/docs/mystery-dungeon-nds/pok%C3%A9mon-mystery-dungeon-explorers-r78/), although many of its descriptions are incomplete).

For convenience, here are the relevant overlays in the two most common cases:

- Dungeon mode (while exploring a Mystery Dungeon):
    - overlay 29 (dungeon mode)
    - overlay 10 (game data/mechanics)
    - overlay 31 (dungeon menu)
- Ground mode (while walking around in the overworld or during cutscenes):
    - overlay 11 (script engine)
    - overlay 10 (game data/mechanics)

In EoS, the game can has 3 "slots" for loading overlays, which means there can be a maximum of 3 overlays loaded at once (this may or may not be general to all NDS games; I'm not sure). You can read more about how this works in [`arm9.yml`](../symbols/arm9.yml), in the descriptions of the `LOADED_OVERLAY_GROUP_*` symbols. You can also look at the values at `LOADED_OVERLAY_GROUP_*` in a memory viewer if you ever need to determine exactly which overlays are loaded at a certain point in the game.
