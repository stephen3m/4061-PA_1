#include <stdio.h>
#include "utils.h"
#include "print_tree.h"

// ##### DO NOT MODIFY THESE VARIABLES #####
char *blocks_folder = "output/blocks";
char *hashes_folder = "output/hashes";
char *visualization_file = "output/visualization.txt";


int main(int argc, char* argv[]) {
    if (argc != 3) {
        // N is the number of data blocks to split <file> into (should be a power of 2)
        // N will also be the number of leaf nodes in the merkle tree
        printf("Usage: ./merkle <file> <N> \n");
        return 1;
    }

    // TODO: Read in the command line arguments and validate them
    // DONE by Stephen, CHECKED by RoberT
    char *input_file = argv[1];
    int n = atoi(argv[2]);
    if(!(n && (!(n & (n - 1))))) {
        printf("N should be a power of 2 \n");
        return 1;
    }

    // ##### DO NOT REMOVE #####
    setup_output_directory(blocks_folder, hashes_folder);

    // TODO: Implement this function in utils.c
    // DONE by ____, CHECKED by ______ (IN PROGRESS by Stephen)
    // tested with t1/t2 by RoberT, seems to work good
    partition_file_data(input_file, n, blocks_folder);

    // TODO: Start the recursive merkle tree computation by spawning first child process (root)
    // DONE by Stephen, CHECKED by ______
    fork();

    // ##### DO NOT REMOVE #####
    #ifndef TEST_INTERMEDIATE
        // Visually display the merkle tree using the output in the hashes_folder
        print_merkle_tree(visualization_file, hashes_folder, n);
    #endif

    return 0;
}