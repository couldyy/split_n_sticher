# Split'N'Sticher
Simple program to split file to parts and then assemble it back

> [!WARNING]
> Due to the way getting filesize is implemented (long ftell()), maximum filesize is size of **long** type, which means **on 32-bit systems max size is 2gb**. This will probably be fixed in future

## Build
To build just simply do:
```
git clone https://github.com/couldyy/split_n_sticher.git
cd split_n_sticher
make
```
No external dependancies required, except `mingw` for Windows users.

## How to use
To start program for assembling files, run:
```
./splitNsticher -a
```
And to start program for disassembling files, run:
```
./splitNsticher -d
```
You can see similar inforamtion if you run:
```
./splitNsticher --help
```
After that just follow instructions in program.

> [!WARNING]
> Make sure there is no "(*number*)" in filenames, otherwise it will not open this file for some reason
> 
> Example -> filename**(1)**.jpg , should be -> filename.jpg

### ToDo:
- [ ] Fix bug with filenames
- [ ] Add support for opening all files in directory
- [ ] Fix max filesize
- [ ] Add capability if choosing directory to assemble in 

