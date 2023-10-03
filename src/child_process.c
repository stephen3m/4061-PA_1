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
    // DONE by Stephen, CHECKED by ______ 

    // Store arg values 
    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int n = atoi(argv[3]);
    int child_id = atoi(argv[4]);

    if((child_id >= n-1) && (child_id <= (2*n)-1)) {
        char block_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char block_filename[PATH_MAX];
        int block_id = child_id - (n-1); // To figure out which block_id.txt to use for hashing
        sprintf(block_filename, "%s/%d.txt", blocks_folder, block_id);
        hash_data_block(block_hash, block_filename);

        // write data to hash file
        char hash_output_name[PATH_MAX];
        sprintf(hash_output_name, "%s%d.out", hashes_folder, child_id);
        int hashfd;

        if (hashfd = fopen(hash_output_name, 'w+')) {
            perror("Couldn't open file to write hash \n");
            exit(-1);
        }
        sprintf(hashfd, block_hash);
        exit(0);
    }

    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 
    // DONE by _____, CHECKED by ______ (In progress by Stephen)
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

    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];

    // Retrieve left (2*child_id + 1) and right (2*child_id + 2) hash from hashes_folder
    // Open the respective files: E.g. for left, it would be ("%d",2*child_id + 1).out
    // Next, we want to read all of the data from the respective .out files into left_hash and right_hash
    char left_path[PATH_MAX];
    char right_path[PATH_MAX]; 
    sprintf(left_path, "%s%d.out", hashes_folder, (2*child_id + 2));
    sprintf(right_path, "%s%d.out", hashes_folder, (2*child_id + 1));

    int fd_left, fd_right;
    if((fd_left = fopen(left_path, 'r')) || (fd_right = fopen(right_path, 'r'))) {
        perror("Error opening left or right hash outputs \n");
        exit(-1);
    }

    fread(left_hash, sizeof(char), SHA256_BLOCK_SIZE * 2 + 1, fd_left);
    fread(right_hash, sizeof(char), SHA256_BLOCK_SIZE * 2 + 1, fd_right);

    //*Don't think what I have below is correct* // pretty sure it's correct, it's what the documentation says
    // To compute and output the hash of the concatenation of the two hashes, we use compute_dual_hash, 
    // and the result is stored in result_hash.
    compute_dual_hash(result_hash, left_hash, right_hash);

    // write data to hash file
    char hash_output_name[PATH_MAX];
    sprintf(hash_output_name, "%s%d.out", hashes_folder, child_id);
    int hashfd;

    if (hashfd = fopen(hash_output_name, 'w+')) {
        perror("Couldn't open file to write hash \n");
        exit(-1);
    }
    sprintf(hashfd, result_hash);
    
    return 0;
}

