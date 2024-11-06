# file_asmmebler_disassembler
Simple program to disassmeble file to parts and then assemble it back
## Build
To build just simply do:
```
git clone https://github.com/couldyy/file_asmmebler_disassembler.git
cd file_asmmebler_disassembler
make
```
No external dependancies required, except `mingw` for Windows users.

## How to use
To start program for assembling files, run:
```./disassemble_assemble -a```
And to start program for disassembling files, run:
```./disassemble_assemble -d```
You can see similar inforamtion if you run:
```./disassemble_assemble --help```
After that just follow instructions in program.

> [!WARNING]
> Make sure there is no "(*number*)" in filenames, otherwise ot will not open this file for some reason
> Example -> filename.jpg **(1)**, should be -> filename.jpg

### ToDo:
- [ ] Fix bug with filenames
- [ ] Add support for opening all files in directory

