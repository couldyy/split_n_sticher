#include "utils.h"

void to_lower_ascii(char* str)
{
	int i = 0;
	while(str[i] != '\0'){
		if(str[i] >= 65 && str[i] <= 90)
			str += 32;
		i++;
	}
}


disassemble_type get_disassemble_type(char* input, int *parts, unsigned long long *part_size)
{
	to_lower_ascii(input);
	int len = strlen(input);
	char data_unit[2];
	long double input_num;
	data_unit[0] = input[len - 3];
	data_unit[1] = input[len - 2];
	if(!strcmp("gb", data_unit))
	{
		input[len - 3] = '\0';
		input_num = strtold(input, NULL);
		*part_size = (unsigned long long)(input_num * GB);
		return BY_PART_SIZE;
	}
	else if(!strcmp("mb", data_unit))
	{
		input[len - 3] = '\0';
		input_num = strtold(input, NULL);
		*part_size = (unsigned long long)(input_num * MB);
		return BY_PART_SIZE;
	
	}
	else if(!strcmp("kb", data_unit))
	{
		input[len - 3] = '\0';
		input_num = strtold(input, NULL);
		*part_size = (unsigned long long)(input_num * KB);
		return BY_PART_SIZE;
	
	}
	else if(!strcmp("b", &data_unit[1]))
	{
		input[len - 2] = '\0';
		input_num = strtold(input, NULL);
		*part_size = (unsigned long long)input_num;
		return BY_PART_SIZE;
	}
	else if((input_num = strtold(input, NULL)) > 0)
	{
		*parts = (int)input_num;
		return BY_PART_NUMBER;
	}
	return NONE;
}

char* get_extension(const char* path)
{
    int len = 0, dot_pos = 0;
        
    while (path[len] != '\0') {
        if (path[len] == '.')
            dot_pos = len;
        len++;
    }
    int new_size = len - dot_pos;
    char* extension = (char*)malloc(new_size*sizeof(char));
    for (size_t i = dot_pos+1, j =0; i < len; i++,j++)
    {      
        extension[j] = path[i];
    }
    extension[new_size-1] = '\0';
    return extension;
}

char* get_filename(char* path)
{
    int pos = -1;
    int len = strlen(path);
    int new_size = 0;
    /*
    if(path[len-1] == '\n'){
        path[len-1] = '\0';     //if '\n' is stored in the end of str
                                //test it later when uploading multiple paths
        len -=1 ;
    }*/
    #ifdef __linux__
    char delimeter = '/';
    #elif _WIN32
    char delimeter = '\\';
    #endif
    for(int i = len; i >= 0; i--, new_size++){
        if(path[i] == delimeter){
            pos = i+1;
            break;
        }
    }

    if(pos != -1){
        char* filename = (char*)malloc(sizeof(char) * (new_size+1));
        for(int i = 0; i < new_size; i++, pos++){
            filename[i] = path[pos];
        }
        filename[new_size] = '\0';
        return filename;

    }
    else
        return path;
}

void drag_and_drop_linux(char ***paths, int *path_count)
{
    char buff[4096];
    printf("Drag&Drop files here:\n");
    //scanf("%4095s",buff); //get filepaths user dropped
    fgets(buff, sizeof(buff), stdin);

    size_t len = strlen(buff);
    if (len > 0 && buff[strlen(buff) - 1] == '\n') {
        buff[len - 1] = '\0';
    }
    char *str = strdup(buff);

    const char *start = str;
    const char *end;
    *path_count = 0;  // Initialize path count

    *paths = NULL;  // Initialize the paths array to NULL

    while ((start = strchr(start, '\'')) != NULL) {
        start++;  // Move past the starting quote
        end = strchr(start, '\'');
        if (end) {
            // Calculate the length of the path
            int length = end - start;

            // Reallocate memory for the new path
            *paths = realloc(*paths, (*path_count + 1) * sizeof(char *));
            if (*paths == NULL) {
                perror("Failed to reallocate memory");
                exit(EXIT_FAILURE);
            }

            // Allocate memory for the new path and copy it
            (*paths)[*path_count] = malloc((length + 1) * sizeof(char));
            if ((*paths)[*path_count] == NULL) {
                perror("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }

            strncpy((*paths)[*path_count], start, length);
            (*paths)[*path_count][length] = '\0';  // Null-terminate the string
            (*path_count)++;

            start = end + 1;  // Move past the ending quote
        } else {
            break;  // No closing quote found
        }
    }
    free(str);
}

void drag_and_drop_windows(char*** paths, int* path_count)
{
    char buff[4096];
    printf("Drag&Drop files here:\n");
    //scanf("%4095s",buff); //get filepaths user dropped
    fgets(buff, sizeof(buff), stdin);
    //if(buff[strlen(buff) - 1] == '\n')
    //  printf("char is '\\n'");
    size_t len = strlen(buff);
    if (len > 0 && buff[strlen(buff) - 1] == '\n')
	{
        buff[len - 1] = '\0';
    }
   // printf("%s", buff);
    char* str = strdup(buff);
    char* theRest = str;
    char* token = "\"";
    // get number of files, create an array of paths and return it;

    *path_count = 0;
    char* path;

    //printf("part: %s, iteration %d\n", path_size, *size);
    while((path = strtok_r(theRest, token, &theRest)) != NULL)
	{
        if(*path_count < 1)
            *paths = (char**)malloc((*path_count+1) * sizeof(char*));
        else
            *paths = realloc(*paths, (*path_count+1) * sizeof(char *));
        (*paths)[*path_count] = strdup(path);
        *path_count += 1;
        //path_size = strtok_r(buff, token, &buff);
        printf("%s\n", path);
    }
    free(str);
}

void free_paths_array(char*** paths, int path_count)
{
    for(int i = 0; i < path_count; i++)
	{
        free((*paths)[i]);
    }
    free(*paths);
}

unsigned long str_sum(const char* str)
{
    int i = 0;
    unsigned long sum = 0;
    while(str[i] != '\0'){
        sum += (int)str[i];
        i++;
    }
    return sum;
    
}

void insertionSort(char*** arr, int n)
{
    for (int i = 1; i < n; ++i) {
        char* key = (*arr)[i];
        int j = i - 1;

        /* Move elements of arr[0..i-1], that are
greater than key, to one position ahead
           of their current position */
        while (j >= 0 && (str_sum((*arr)[j]) > str_sum(key))) {
            (*arr)[j + 1] = (*arr)[j];
            j = j - 1;
        }
        (*arr)[j + 1] = key;
    }
}
