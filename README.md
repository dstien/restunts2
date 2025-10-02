![restunts](assets/logo.svg)

An attempt to refurbish the [Stunts reverse engineering project](https://github.com/4d-stunts/restunts) with free, contemporary tooling and measures to improve correctness of the ported code.

## Progress

- [X] Convert IDA database to Ghidra.
- [X] Set up Ghidra Server for collaborative analysis.
- [X] Ghidra script to generate Watcom Assembler code.
- [X] Recreate working, debuggable game executable with Watcom Linker.
- [X] Build-time override functions ported to C.
- [ ] Import C functions from the original [restunts](https://github.com/4d-stunts/restunts) project, switch to [fixed-width integers](https://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types) and add [unit tests](https://en.wikipedia.org/wiki/Unit_testing).
- [X] Enforce consistent coding style with *clang-format*.
- [ ] Continuous integration for all targets.
- [ ] Find a clean solution for improving the game and adding modern platforms while preserving original behaviour with minimal code duplication or `#ifdef` spaghetti.
- [ ] Find a lossless, text-based format for backing up the Ghidra analysis database in git.
- [ ] Synchronise analysis with other versions of Stunts and 4D [Sports] Driving.

## Building

A recreation of the original Stunts 1.1 executable can be built using GNU Make and [Open Watcom 2](https://github.com/open-watcom/open-watcom-v2) on Unix-like systems:
```
$ make
```
This produces four 16-bit real-mode DOS executables in `build/dos16`:
* `restunts.exe`: Combines our ported C functions with the original code.
* `restunto.exe`: Uses solely the original code. It is functionally identical to the retail version, but not binary identical.
* `test.exe`: Runs unit tests for the ported C code.
* `testo.exe`: Runs unit tests on the original code. If these tests fails, the tests are wrong.

Builds are debug-enabled by default. Release builds can be made with `make DEBUG=0` or running `wstrip` on the executable.

The ported code can also be compiled for x86_64 with *clang*:
```
$ make linux64
```
This produces the unit test executable `build/linux64/test-linux64`, which ensures that the code is portable and preserves the original code's behaviour.

## Debugging

The Watcom Debugger `wd` can be used to interactively debug the game in a DOS environment.

Copy or symlink the executables to a directory with the game data files and start the debugger:
```
C:\STUNTS> wd restunts.exe
```
Inside the debugger you may want to set the source file paths, adjust settings or UI layout, and save the configuration to a file that can be loaded on startup. This example starts the debugger in VESA 80x50 mode, loads a stored configuration, and debugs `restunts.exe` with the Roland MT-32 sound driver:
```
C:\STUNTS> wd -co=80 -li=50 -invoke=restunts.dbg restunts.exe /smt
```
Watcom also supports remote debugging. `LDFLAGS` can be set for the `dos16` make target to produce debug data in DWARF or CodeView format for use with other debuggers. See the [Open Watcom Linker User's Guide](https://open-watcom.github.io/open-watcom-v2-wikidocs/lguide.pdf) for details.

Since code and data alignments are preserved, `restunts.exe` can be debugged side by side with `restunto.exe` or the retail version for comparison.

## Analysis

Analysis of the original machine code is done with [Ghidra](https://www.nsa.gov/ghidra). The analysis database consists of binary files that are unsuitable for merging in git, but Ghidra provides its own server component for multi-user collaboration.

### Opening the Ghidra project
1. In the main Ghidra project window click *File → New Project...*.
2. Select *Shared Project* and click *Next*.
3. In the *Server Name* field enter `re.stunts.no`, leave *Port Number* with the default value `13100` and click *Next*.
4. When prompted for authentication, you can chose to use an account or continue anonymously.
    * Anonymous connections are read-only.
    * If you want to participate in the analysis, open an issue or contact [@dstien](https://github.com/dstien) by other means to have an account created. Provide a username and an e-mail address where the password can be sent.
    * If the username field is not editable, see the [Troubleshooting](#troubleshooting) section.
5. Choose the existing repository `restunts` and continue.
6. You can use the `ghidra` folder of your cloned *restunts* repository as the *Project Directory*.
7. In the project window, double-click `bb11_mcga.exe` to open it in the default tool, *CodeBrowser*.

The `restunts` project can be now be opened like local projects. `bb11_mcga.exe` is the main analaysis target.

### Setting up the Ghidra script

1. In the Ghidra *CodeBrowser*, select *Tools → Script Manager*
2. Click the *Manage Script Directories* bullet-list icon on the *Script Manager* toolbar.
3. Click the plus icon and add the `ghidra` folder of your cloned *restunts* repository.
4. The *Script Manager* should refresh automatically. Check *In Tool* for the newly found `restunts-export.py` script.
5. The script is now available from *Tools → restunts export* or by using the `Ctrl+Alt+R` keyboard shortcut. You can assign a different key binding in the *Script Manager*.

Running the *restunts export* script will regenerate the files in the `src/asm` directory to reflect the current analysis.

When renaming public symbols, be mindful of their use in ported C code, which must be updated accordingly. Changes in the analysis can also affect what code is produced. The build will fail if a segment size changes.

To account for issues we can't handle properly in Ghidra, we use four *patch tags* in analysis comments that the exporter script interprets:
* Pre-comment tags
    * `<REPLACE>code</REPLACE>` replaces the subsequent instruction with the content of the tag. Ensure that the replacement code compiles to the same number of bytes as the original instruction.
    * `<INSERT>code</INSERT>` inserts the content of the tag before the subsequent instruction. Useful for multi-line inserts combined with the `<DELETE>` tag at the end of every line that should be replaced.
* End-of-line tags
    * `<NOP>` replaces every byte in the original instruction with a `nop` opcode, effectively ignoring the original instruction while preserving alignment. Useful for "commenting out" a function call and observing the effects.
    * `<DELETE>` removes the instruction. Must be combined with another patch to make up for the code size change.

## Porting code to C

1. Implement the function in a file in `src/c`. When adding a new file, include it in `SRC` in `src/Makefile`.
2. Add the function name to `PORTED_FUNCS_BY_FILE` in `ghidra/resunts-export.py` and run the export from Ghidra.
3. Write unit tests in `src/test` to ensure the code is portable and the behaviour is consistent across platforms.
4. Rebuild and test.
5. Run `make check-fmt` and apply automatic formatting with `clang-format -i FILENAME`.

## Troubleshooting

<details>
<summary><b>Ghidra won't let you change the username when connecting</b></summary>

Try launching Ghidra with:
```
$ env JAVA_TOOL_OPTIONS='-Duser.name=<USERNAME>' ghidra
```
This workaround will prevent you from opening other Ghidra projects because they are tied to your local system account name. Either start Ghidra as normal to work on other projects, or edit the owner in each project's `<PROJECT>.rep/project.prp` file.
* NationalSecurityAgency/ghidra#7454
</details>

<details>
<summary><b>Watcom Debugger won't start under DOS</b></summary>

Install the older [Open Watcom 1.9](https://github.com/open-watcom/open-watcom-1.9/releases/tag/ow1.9) in your DOS environment. The 2.0 project has regressions with DOS debugging:
* open-watcom/open-watcom-v2#1396
* open-watcom/open-watcom-v2#1417
</details>

## Contact

The [Stunts Forum](https://forum.stunts.hu/index.php?board=90.0) is the community's primary platform for communication and coordination.

## Acknowledgements

The [restunts](https://github.com/4d-stunts/restunts) project was started in 2009 by [@clvn](https://github.com/clvn), who figured out all the cunning tricks to rebuild the executable with new code, did the majority of the analysis and the porting of code to C. Other valuable contributors include [@AlbertoMarnetto](https://github.com/AlbertoMarnetto),  [@duplode](https://github.com/duplode), [@dstien](https://github.com/dstien), [@LowLevelMahn](https://github.com/LowLevelMahn).
