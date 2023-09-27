#include "utils.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder) {
    // Use fseek() and ftell() to determine the size of the file
    FILE* fp = fopen(input_file, "a+");
    fseek(fp, 0, SEEK_END);
    int file_len = ftell(fp);

    // Calculate file sizes (how many bytes each file should contain)
    // file_size_1 is for first N-1 files; file_size_2 is for last file
    int file_size_1 = floor(file_len/N);
    int file_size_2 = floor(file_len/N) + (file_len % N);

    // Create first N-1 files in the folder "output/blocks/" (will be created when setup function is called)
    char file_path[1024];
    FILE* fp_block_file;
    char input_file_content[file_size_1 + 1];
    fseek(fp, 0, SEEK_SET); //Set input_file position to beginning
    for(int i = 0; i < N-1; i++) {
        sprintf(file_path, "%s%d.txt", "output/blocks/", i);
        // Create a file with the name i.txt in output/blocks/ 
        fp_block_file = fopen(file_path, "w+"); 
        // Read file_size_1 bytes from input_file and store in string
        fgets(input_file_content, file_size_1, fp);
        fseek(fp, file_size_1, SEEK_SET); // move position up file_size_1 bytes
        // Write string to i.txt
        fputs(input_file_content, fp_block_file);
        fclose(fp_block_file);
    }
    
    // Create last file in the folder "output/blocks/"
    sprintf(file_path, "%s%d.txt", "output/blocks/", N-1);
    fp_block_file = fopen(file_path, "w+");
    char input_file_content_2[file_size_2 + 1];
    fgets(input_file_content_2, file_size_2, fp);
    fputs(input_file_content_2, fp_block_file);
    fclose(fp_block_file);

    fclose(fp);


    // Stephen question: do we need to check if fp is NULL? 
    //if( fp == NULL )  {
    //       perror ("Error opening file");
    //       return(-1);
    //    }
    // Also should I use "r" instead of "a+" even though both work?
}


// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder) {
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0) {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    } else {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0) {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0) {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0) {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}
