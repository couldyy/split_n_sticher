#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum data_units_multipler{
	KB = 1024,
	MB = 1048576,		//1024 * 1024
	GB = 1073741824		//1024 * 1024 * 1024
}data_units_multipler;

typedef enum disassemble_type{
	NONE,
	BY_PART_SIZE,
	BY_PART_NUMBER,
}disassemble_type;


void to_lower_ascii(char* str);
disassemble_type get_disassemble_type(char* input, int *parts, unsigned long long *part_size);
char* get_extension(const char* path);
char* get_filename(char* path);

void drag_and_drop_linux(char ***paths, int *path_count);
void drag_and_drop_windows(char*** paths, int* path_count);
void free_paths_array(char*** paths, int path_count);

unsigned long str_sum(const char* str);
void insertionSort(char*** arr, int n);

#endif
