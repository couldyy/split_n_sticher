#include "utils.h"


void assemble_files(char ***paths, int *path_count);
void disassemble_file(char ***paths, int *path_count);

#ifdef _WIN32
#include <stddef.h>


size_t strlcpy(char *dest, const char *src, size_t size)
    {
	size_t ret = strlen(src);

	if (size) {
	    size_t len = (ret >= size) ? size - 1 : ret;
	    memcpy(dest, src, len);
	    dest[len] = '\0';
	}
	return ret;
}

#endif

int main(int argc, char** argv){
	if(argc != 2){
		printf("Incorrect arguments!\nSee dissassemble_assemble --help\n");
		return -1;
	}
	else{
		char buff[10];
		char** paths;
		int path_count = 0;
		if(!strcmp("-a", argv[1])){
			printf("Started in assasemble mode\n");
#ifdef _WIN32
			drag_and_drop_windows(&paths, &path_count)	;
#else

			drag_and_drop_linux(&paths, &path_count); 
#endif
			insertionSort(&paths, path_count);
#ifdef DEBUG
			for(int i = 0; i < path_count; i++)
				printf("%d. %s\n", i, paths[i]);
#endif
			assemble_files(&paths, &path_count);
		}
		else if(!strcmp("-d", argv[1])){
#ifdef _WIN32
			drag_and_drop_windows(&paths, &path_count)	;
#else

			drag_and_drop_linux(&paths, &path_count); 
#endif
			disassemble_file(&paths, &path_count);
		}
		else if(!strcmp("--help", argv[1])){
			printf("dissassemble_assemble -a	-- run program to assemble files\n");
			printf("dissassemble_assemble -d	-- run program to disassemble files\n");
		}
		free_paths_array(&paths, path_count);
	}

	return 0;
}



void disassemble_file(char ***paths, int *path_count){
	if(*path_count <= 0)
		return;


	for(int i = 0; i < *path_count; i++){

		FILE* file_in = fopen((*paths)[i], "rb");
		if(file_in == NULL){
			printf("Error opening file \"%s\"\n", (*paths)[i]);
			return;
		}

		char* filename_in = get_filename((*paths)[i]);
		
		//get file extension and write
		char* extension = get_extension((*paths)[i]);
		int extension_len = strlen(extension);
		strtok(filename_in, ".");
		char filename_extension[1024];
		snprintf(filename_extension, sizeof(filename_extension), "extension_%s", filename_in);
		FILE* file_extension = fopen(filename_extension, "wb");
		fwrite(&extension_len, sizeof(int), 1, file_extension);
		fwrite(extension, sizeof(char), extension_len, file_extension);
		fclose(file_extension);
		free(extension);

		//get file size
		fseek(file_in, 0L, SEEK_END);
		long filesize = ftell(file_in);
		rewind(file_in);

		int parts = 0;
		unsigned long long part_size = 0;
		disassemble_type type = NONE;
		char userinput_buff[256];
		do{
			printf("\nIf you want to split file to parts with specific filesize,\nenter filesize in following pattern {number}{data unit} [Gb, Mb, Kb, b]\nExmaple: 2Mb	or	0.6Mb	or	1024Kb\n");
			printf("If you want to split file to a specific number of parts, just enter number of parts WITHOUT DATA UNITS.\nExample: 3\n");
			printf("Consider that filesize is - %f Mb( %ld bytes )\n", (double)filesize/ 1024.0f / 1024.0f, filesize);
			printf("Input: ");
			fgets(userinput_buff, sizeof(userinput_buff), stdin);
			type = get_disassemble_type(userinput_buff, &parts, &part_size); 

		}while(type == NONE);
		

		if(type == BY_PART_SIZE)
		{
			parts = filesize / part_size;
			printf("This will generate %d files\nYou sure you want to proceed? [Y/n]: ", parts);
			char answer[2];
			fgets(answer, sizeof(answer), stdin);
			if(strcmp("Y", answer))		//if user input in NOT "Y"
			{
			printf("Stopping...\n");
			free(filename_in);	
			fclose(file_in);
			return;
			}
		}
		else if(type == BY_PART_NUMBER)
			part_size = filesize / parts;
		else
		{
			printf("Error occured while getting type of disassembling\nStopping...\n");	
			free(filename_in);
			
			fclose(file_in);
			return;
		}
		printf("Starting disassembling file \"%s\" ... \n", filename_in);
		for(int i = 0; i < parts; i++){
			char part_filename[1024];
			snprintf(part_filename,sizeof(part_filename), "%s_%s%d", filename_in, "part", i+1);
			printf("\nstarting writing file \"%s\" ... ", part_filename);
			FILE *part_file = fopen(part_filename, "wb");

			int bytes_read = 0;
			int bytes_left = part_size;
			char buff[1024];
			if(i == parts-1){
				while((bytes_read = fread(buff,sizeof(char), sizeof(buff), file_in)) > 0){
					fwrite(buff, sizeof(char), bytes_read, part_file);
				}
			}
			else{
				while(bytes_left > 0 && (bytes_read = fread(buff,sizeof(char), sizeof(buff), file_in)) > 0){
					fwrite(buff, sizeof(char), bytes_read, part_file);
					bytes_left -= bytes_read;
				}	
			}

			fclose(part_file);
			printf("done");
		}
		printf("\nFinished disassembling file \"%s\" \n", filename_in);
		free(filename_in);
		
		fclose(file_in);
	}

}


void assemble_files(char ***paths, int *path_count){
	if(*path_count <= 0)
		return;
	
	char out_filename[1024];

	
	FILE* extension_file = NULL;

	char string_compare[] = "extension_"; 
	char filename_buff[sizeof(string_compare)];

	//find file with extension
	for(int i = 0; i < *path_count; i++){
		char* name = get_filename((*paths)[i]);
		strlcpy(filename_buff, name, sizeof(filename_buff));
		free(name);
		if(!strcmp(filename_buff, string_compare)){
			extension_file = fopen((*paths)[i], "rb");

			//delete this file from array
			free((*paths)[i]);
			for(int j = i; j < *path_count-1; j++){
				(*paths)[j] = (*paths)[j+1];
			}
			*path_count -= 1;
			break;
		}
	}

	char* filename = get_filename((*paths)[0]);
	//read extension and construct filename
	if(extension_file != NULL){
		int extension_len;
		fread(&extension_len, sizeof(int), 1, extension_file);
		char* extension = (char*)malloc(sizeof(char) * extension_len);
		fread(extension, sizeof(char), extension_len, extension_file);
		strtok(filename, ".");
		snprintf(out_filename, sizeof(out_filename), "%s.%s", filename, extension);
		fclose(extension_file);
		free(extension);
	}
	else{
		snprintf(out_filename,sizeof(out_filename), "%s_%s", filename, "out");
	}


	FILE* output_file = fopen(out_filename, "wb");
	if(output_file == NULL){
		free(filename);
		printf("Error opening file \"%s\"!\n", out_filename);
		return;
	}
	free(filename);
	printf("Starting assembling \"%s\" ...\n", out_filename);
	for(int i = 0; i < *path_count; i++){
		FILE* file_in = fopen((*paths)[i], "rb");
		char buff[1024];
		int bytes_read = 0;
		while((bytes_read = fread(buff,sizeof(char), sizeof(buff), file_in)) > 0){
			fwrite(buff, sizeof(char), bytes_read, output_file);
		}
		fclose(file_in);
		printf("assembled part %d\n", i+1);
	}
	fclose(output_file);
	printf("\nFinished assembling!\n");
}



