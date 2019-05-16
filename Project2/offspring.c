/**
 * Project 2: Offspring
 * @file offspring.c
 * Description: The driver class to Project 2: "Offspring".
 * this which builds a descendant tree of the generations of
 * offspring from one person. The program takes one optional
 * command line argument. If there is an argument, it reads the
 * file to load an offspring tree based on the file's content.
 * Otherwise, it begins running with an empty (NULL) tree.
 * @author Samuel Tregea
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "trimit.h"
#include "Queue.h"
#include "NTree.h"

#define MAX 1024
#define TRUE 1
#define FALSE 0

/**
 * Read in input from a file and add it to a tree.
 * @param fileName : the name of the file to read from.
 * @return a pre-filled n-ary tree.
 */
NTree readFile(const char * fileName){
    /// File pointer that will be used to open a file
    FILE * file;
    
    /// Pointer to be used as a tokenizer of the user input
    char *token = NULL;
    
    /// Pointers that will contain the names of the parent and child nodes
    char * parent = NULL, * child = NULL;
    char output[MAX] = {'\0'};
    
    /// The tree to modify
    NTree tree = NULL;
    
    // checking to see if the node exists.
    if((file=fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "error: '(null)' not found\n");
        fclose(file);
        return NULL;
    }
    else{
        while(!feof(file)){
            
             if (feof(file)){
                fprintf(stderr, "error: No newline at end of file\n");
                fclose(file);
                return NULL;
            }
            
            fgets(output,sizeof(output),file);
            token = strtok(output, ",");
            parent = token;
            token = strtok(NULL, ",\n");
            
            if (strcmp(parent, "\n") == 0){
                //do nothing
            }
            else{
                // continue reading the rest of the line
                while (token != NULL) {
                    child = trim(token);
                    
                    // if child is empty
                    if (strcmp(child, "\n") == 0){
                        //do nothing
                    }
                    else{
                        token = strtok(NULL, ",\n");
                        tree = add_child(tree, parent, child);
                    }
                }
            }
        }

    }

    fclose(file);
    
    return tree;
}

/**
 * Print out a help menu
 */
void help(){
    printf("User Commands for offspring:\n");
    printf("add\tparent_name , child_name # find parent and add child.\n");
    printf("find\t[name] # search and print name and children if name is found.\n");
    printf("print\t[name] # breadth first traversal of offspring from name.\n");
    printf("size\t[name] # count members in the [sub]tree.\n");
    printf("height\t[name] # return the height of [sub]tree.\n");
    printf("init\t# delete current tree and restart with an empty tree.\n");
    printf("help\t# print this information.\n");
    printf("quit\t#delete current tree and end program.\n");
    fflush(stdin);
}

/**
 * Have the user interact with the N-Ary tree.
 */
void commandInput(NTree tree){
    /// array that will hold the user input
    char input[MAX] = {'\0'};
    
    /// pointer that will be used to look at a tokenized string
    char *token;
    
    /// boolean that will be used to keep the program running
    bool running = TRUE;
    
    /// pointer that will be used to store the name of a node
    char *name = NULL;
    
    /// pointers that will store the name of a parent and child node
    char *parent= NULL, *child = NULL;
    
    while(running){
        printf("offspring> ");
        fflush(stdin);

        fgets(input, MAX, stdin);
        token = strtok(input, " .\n");
        
        if (token == NULL) {
            fprintf(stderr,"error: empty input.\n");
            fflush(stderr);
        }
        /// add a child to a parent on the tree
        else if((strcmp(token, "add") == 0)){
            token = strtok(NULL, ",");
            if (token == NULL) {
                
                while (token!=NULL) {
                    token = strtok(NULL, ",");
                }
                fprintf(stderr, "Usage: \'add parent name , child name\'\n");
                fflush(stderr);
            }
            else{
                parent = trim(token);
                token = strtok(NULL, ",\n");
                
                if (token == NULL && tree == NULL) { // if there is no root and user enters one name
                    tree = add_child(tree, parent, NULL);
                }
                else if(token == NULL && tree != NULL){
                    fprintf(stderr, "error: cannot add \'%s\' since \'%s\' is not a root node.\n", parent,parent);
                    fflush(stderr);

                }
                while (token != NULL) {
                    child = trim(token);
                    
                    // if child is empty
                    if (strcmp(child, "\n") == 0){
                        //do nothing
                    }
                    else{
                        tree = add_child(tree, parent, child);
                        token = strtok(NULL, ".,\n");
                    }
                }
            }

        }
        // print out a specific node and its children
        else if((strcmp(token, "find") == 0)){
            token = strtok(NULL, ",.\n");
            
            // empty string
            if (token == NULL) {
                print_tree(tree, "");
            }
            else{
                name = trim(token);
                NTree node = searchTree(tree, name);
                
                if(node != NULL)
                    print_node(node);
                else{
                    fprintf(stderr, "error: \'%s\' not found\n", name);
                    fflush(stderr);
                }
            }
            
        }
        // print out the children and parent of a tree
        else if((strcmp(token, "print") == 0)){
            token = strtok(NULL, ",.\n");
            
            if (token == NULL) {
                print_tree(tree, NULL);
            }
            else{
                name = trim(token);
                print_tree(tree, name);
            }
        }
        // find out how many nodes are within the tree
        else if((strcmp(token, "size") == 0)){
            token = strtok(NULL, ",.\n");
            
            if (token == NULL) {
                // print size of entire tree
                printf("size: %d\n", (int) size_of_tree(tree,NULL));
                fflush(stdin);
            }
            else{
                // print size of sub tree
                name = token;
                printf("size: %d\n", (int) size_of_tree(tree, name));
                fflush(stdin);
            }
        }
        // find the height of the tree
        else if((strcmp(token, "height") == 0)){
            token = strtok(NULL, ",.\n");
            
            // print entire tree
            if (token == NULL){
                printf("height: %d\n", (int) height_of_tree(tree, NULL));
                fflush(stdin);
            }
            // if name is given, print out specified node.
            else{
                name = token;
                printf("height: %d\n", (int) height_of_tree(tree, name));
                fflush(stdin);
            }
        }
        // clear the tree of it's contents
        else if((strcmp(token, "init") == 0)){
            destroy_tree(tree);
            tree = NULL;
        }
        // print out a help menu
        else if((strcmp(token, "help") == 0)){
            help();
        }
        // exit program
        else if((strcmp(token, "quit") == 0)){
            destroy_tree(tree);
            printf("\n");
            fflush(stdin);
            running = FALSE;
        }
        // if an invalid command
        else{
            fprintf(stderr,"Invalid command. Please enter again.\n");
            fflush(stderr);
        }

    }
}


int main(int argc, char * argv[]){

    /// The N-ary Tree to be used for the program.
    NTree tree;

    // if there is a file given, read that first.
    if (argc > 1) {
        tree = readFile(argv[1]);
    }else{
    // otherwise, set to null
        tree = NULL;
    }
    
    commandInput(tree);
    fflush(stdin);
    return EXIT_SUCCESS;
}
