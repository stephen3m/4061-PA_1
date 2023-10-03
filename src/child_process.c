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
    int n = argv[3];
    int child_id = argv[4];

    if((child_id >= n-1) && (child_id <= (2*n)-1)) {
        char block_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char *block_filename;
        int block_id = child_id - (n-1) // To figure out which block_id.txt to use for hashing
        sprintf(block_filename, "%s/%d.txt", blocks_folder, block_id);
        
        hash_data_block(block_hash, block_filename);
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
    // DONE by Stephen, CHECKED by ______ 
    while(wait(NULL) > 0);

    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.
    // DONE by _____, CHECKED by ______ (In progress by Stephen)

    char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char right_hash[SHA256_BLOCK_SIZE * 2 + 1];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];
    //Retrieve left (2*child_id + 1) and right (2*child_id + 2) hash from hashes_folder
    //Open the respective files: E.g. for left, it would be ("%d",2*child_id + 1).out
    //Next, we want to read all of the data from the respective .out files into left_hash and right_hash

    //*Don't think what I have below is correct*
    //To compute and output the hash of the concatenation of the two hashes, we use compute_dual_hash, 
    //and the result is stored in result_hash
    compute_dual_hash(result_hash, left_hash, right_hash);
    
}

