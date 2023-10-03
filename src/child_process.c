#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {    
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.
    // DONE by Stephen and RoberT, CHECKED by ______ 

    // Store arg values 
    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int n = atoi(argv[3]);
    int child_id = atoi(argv[4]);

    if((child_id >= n-1) && (child_id <= (2*n)-1)) {
        char block_hash[SHA256_BLOCK_SIZE * 2 + 1]; // for storing hashed data after hashing block_id.txt content
        char block_filename[PATH_MAX]; // the string "output/blocks/block_id.txt"
        int block_id = child_id - (n-1); // to figure out which block_id.txt to use for hashing
        sprintf(block_filename, "%s/%d.txt", blocks_folder, block_id);
        hash_data_block(block_hash, block_filename);

        // write hashed data to a string
        char hash_output_name[PATH_MAX];
        sprintf(hash_output_name, "%s/%d.out", hashes_folder, child_id);

        // create child_id.out in output/hashes and write data from hash_output_name to the file
        FILE* hashfd; 
        if ((hashfd = fopen(hash_output_name, "w+")) == NULL) { // check if opening the file returns NULL
            perror("Couldn't open file to write hash \n");
            exit(-1);
        }
        fprintf(hashfd, "%s", block_hash);
        fclose(hashfd);
        exit(0);
    }

    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    // DONE by Stephen, CHECKED by RoberT
    else {
        // spawn child process for left child
        execl("./child_process", blocks_folder, hashes_folder, n, (2*child_id + 1), NULL);
        // spawn child process for right child
        execl("./child_process", blocks_folder, hashes_folder, n, (2*child_id + 2), NULL);
    }

    // TODO: Wait for the two child processes to finish
    // DONE by Stephen, CHECKED by RoberT 
    while(wait(NULL) > 0);

    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.
    // DONE by _____, CHECKED by ______ (In progress by Stephen and RoberT)

    // store file path names for left and right children hash files 
    char left_path[PATH_MAX]; // output/hashes/left_child_id.out
    char right_path[PATH_MAX]; // output/hashes/right_child_id.out
    sprintf(left_path, "%s/%d.out", hashes_folder, (2*child_id + 1));
    sprintf(right_path, "%s/%d.out", hashes_folder, (2*child_id + 2));

    // open children hash files for reading
    FILE* fd_left; 
    FILE* fd_right;
    if(((fd_left = fopen(left_path, "r")) == NULL) || ((fd_right = fopen(right_path, "r")) == NULL)) {
        perror("Error opening one of the children hash files \n");
        exit(-1);
    }

    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];

    // read data from children hash .out files and store in strings  
    fread(left_hash, sizeof(char), SHA256_BLOCK_SIZE * 2 + 1, fd_left);
    fread(right_hash, sizeof(char), SHA256_BLOCK_SIZE * 2 + 1, fd_right);

    //close fd_left and fd_right
    fclose(fd_left);
    fclose(fd_right);

    // compute hash of the concatenation of the two children hashes and store in result_hash
    compute_dual_hash(result_hash, left_hash, right_hash);

    // store file path name for output/hashes/process_id.out
    char hash_output_name[PATH_MAX];
    sprintf(hash_output_name, "%s/%d.out", hashes_folder, child_id);
    
    // write result_hash to current_process_id.out file
    FILE* hashfd;
    if ((hashfd = fopen(hash_output_name, "w+")) == NULL) {
        perror("Couldn't open file to write hash \n");
        exit(-1);
    }
    fprintf(hashfd, "%s", result_hash);
    fclose(hashfd);

    return 0;
}

