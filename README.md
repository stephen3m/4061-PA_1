Project group number: PA 27

Group member names and x500s:
1. Stephen Ma, ma000094
2. Robert Wang, wan00379
3. Robert Tan, tan00222

The name of the CSELabs computer that you tested your code on:
csel-kh1250-21.cselabs.umn.edu

Any changes you made to the Makefile or existing files that would affect grading: N/A

Plan outlining individual contributions for each member of your group:__
Stephen Ma:
* Outline and write code in merkle.c
* Outline and write code for partition_file_data function in utils.c
* Create pseudocode for child_process.c
* Outline and write code for child_process.c__
Robert Wang:
* Check over merkle.c and fix issues
* Write code for child_process.c
* Check over child_process.c and fix issues__
Robert Tan:
* Check over merkle.c and fix issues
* Write code for child_process.c
* Check over child_process.c and fix issues
* Run tests using make command to ensure code works

Plan on how you are going to implement the process tree component of creating the Merkle tree:
(high-level pseudocode would be acceptable/preferred for this part)__
**Pseudocode for child_process.c:**
read in args and store in variables
if current process is a leaf node (n-1 <= child_id <= 2n-1):
  hash content from (child_id-(n-1)).txt using hash_data_block and store in char array
  create corresponding .out file and write the hash to it
  return
else if the current process is a non-leaf node:
  spawn child process for left child (2 * child_id+1) by calling execl
  spawn child process for right child (2 * child_id+2) by calling execl
wait for both child processes to finish executing 
read content from left child process' .out file and store in char array
read content from right child process' .out file and store in char array
call compute_dual_hash to compute the hash of the concatenation of the two hashes
write the combined hash to its own .out file
return
  


