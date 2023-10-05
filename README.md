**Project group number:** PA 27

**Group member names and x500s:**
1. Stephen Ma, ma000094
2. Robert Wang, wan00379
3. Robert Tan, tan00222

**The name of the CSELabs computer that you tested your code on:**
csel-kh1250-21.cselabs.umn.edu

**Members’ individual contributions:**  
Stephen Ma:
* Outline and write code in merkle.c
* Outline and write code for partition_file_data function in utils.c
* Create pseudocode for child_process.c
* Outline and write code for child_process.c
* Write README.md

Robert Wang:
* Check over partition_file_data function in utils.c
* Check over merkle.c and fix issues
* Check over child_process.c and fix issues
* Run tests using make command to ensure code works

Robert Tan:
* Check over merkle.c and fix issues
* Write code for child_process.c
* Check over child_process.c and fix issues
* Run tests using make command to ensure code works

**How you designed your program for creating the process tree**  
***Pseudocode for child_process.c:***  
read in args and store in variables  
if current process is a leaf node (n-1 <= child_id <= 2n-2):  
  hash content from (child_id-(n-1)).txt using hash_data_block and store in char array  
  create corresponding .out file and write the hash to it  
  return  
else if the current process is a non-leaf node:  
  spawn child process for left child (2 * child_id+1) by calling execl after using fork
  spawn child process for right child (2 * child_id+2) by calling execl after using fork
wait for both child processes to finish executing  
read hash content from left child process' .out file and store in char array  
read hash content from right child process' .out file and store in char array  
call compute_dual_hash to compute the hash of the concatenation of the two hashes and store in char array
write the combined hash in char array to its own .out file  
return
  


