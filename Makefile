CC=gcc

all: disassembler_assembler

disassembler_assembler: disassemble_assemble.c utils.c utils.h
	$(CC) -o disassemble_assemble disassemble_assemble.c utils.c utils.h

