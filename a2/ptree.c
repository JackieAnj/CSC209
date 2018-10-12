#include <stdio.h>
// Add your other system includes here.
#include <sys/stat.h>
#include <stdlib.h>
#include "ptree.h"

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif


/*
 * Creates a PTree rooted at the process pid.
 * The function returns 0 if the tree was created successfully 
 * and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */
int generate_ptree(struct TreeNode **root, pid_t pid) {
    // Here's a way to generate a string representing the name of
    // a file to open. Note that it uses the PROC_ROOT variable.

    char procfile[MAX_PATH_LENGTH + 1];
    if (sprintf(procfile, "%s/%d/exe", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }

    //return 1 if exe file does not exist
    struct stat exe_buffer;
    if (lstat(procfile, &exe_buffer) < 0) {
	return 1;
    } else {
	//allocate memory for root
 	//set its pid to the given argument
	*root = malloc(sizeof(struct TreeNode));
	(*root)->pid = pid;
	//find the filename that contains the name of the node
	char name_file[MAX_PATH_LENGTH + 1];
	if (sprintf(name_file, "%s/%d/cmdline", PROC_ROOT, pid) < 0) {
	    (*root)->name = NULL;
	} else {
	    //open the file if it exists
	    //extract the name in the file and set it as the node's name
	    FILE * node_name;
	    node_name = fopen(name_file, "r");
	    char *name = malloc(MAX_PATH_LENGTH + 1);
	    fgets(name, MAX_PATH_LENGTH, node_name);
	    (*root)->name = name;
	    fclose(node_name);
	} 

	//find the filename that contains the children of the node
        char children[MAX_PATH_LENGTH + 1];
	if (sprintf(children, "%s/%d/task/%d/children", PROC_ROOT, pid, pid) < 0) {
	    (*root)->child = NULL;
	} else {
	    //open the file if it exists
	    FILE * kids;
	    kids = fopen(children,"r");

	    int *child_pid = malloc(sizeof(int));
	    if(fscanf(kids, "%d", child_pid) !=EOF){
		//if there is at least one element in the file
		//generate a new node with the child's pid using recursion
		//set the new node as the child of the root node
	        struct TreeNode ** child_node = malloc(sizeof(struct TreeNode));
	        generate_ptree(child_node, *child_pid);
	        (*root)->child = *child_node;
	    
		//allocate memory for potential siblings of the child node
		int *sibling_pid = malloc(sizeof(int));
		//make a pointer to the current child node to add a sibling node to
		struct TreeNode ** cur_child = child_node;

	        while(fscanf(kids, "%d", sibling_pid) != EOF){
		    //while there is more elements in the childen file
		    //generate new nodes using recursion
		    //assign the node as the sibling node of the cur_child node
	            struct TreeNode ** sibling_node = malloc(sizeof(struct TreeNode));
	            generate_ptree(sibling_node, *sibling_pid);
	            (*cur_child)->sibling = *sibling_node;
		    //set cur_child to the sibling node for the next loop
		    cur_child = sibling_node;
	        }
	    }
	    fclose(kids);
	}
    }


    return 0;
}

/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
    // Here's a way to keep track of the depth (in the tree) you're at
    // and print 2 * that many spaces at the beginning of the line.
    static int depth = 0;
    if ((max_depth == 0) || (depth <= max_depth)){
        printf("%*s", depth * 2, "");
        if (root != NULL) {
	    //if root exists and max_depth is not reached
	    //print the root information
	    printf("%d", root->pid);
	    if ((root->name) != NULL) {
	        printf("%s%s\n", ": ", root->name);
	    } else {
		//if the node does not contain a name
		//print a newline character
		printf("\n");
	    }
	    //if root has a child increase depth by 1
	    //and recrusive call the function on its child node
	    if((root->child) != NULL) {
	        depth ++;
	        print_ptree(root->child, max_depth);
	    }
	    //if root has a sibling recursive call the function
	    //on its sibling
	    if ((root->sibling) != NULL) {
	        print_ptree(root->sibling, max_depth);
	    } else {
		//if root does not have a sibling
		//revert depth by 1 to print the sibling
		//of the root's parent node
		depth--;
	    }
        }
    } else if (max_depth != 0) {
	//if max depth down a branch is reached
	//revert depth by one to print the next branch
	depth--;
    }

}
