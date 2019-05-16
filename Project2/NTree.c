/**
 * Project 2: Offspring
 * @file: NTree.c
 * Description: .c file to NTree.h. This file contains
 * an N-Ary tree, which is a binary tree that can contain
 * any amount of chilren nodes.
 * @author Samuel Tregea
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Queue.h"

struct NTree_S {
    char * name;            ///< name of the person
    struct NTree_S ** children;    ///< dynamic collection of children
    size_t child_count;        ///< number of children
};

#define _TREE_IMPL
#include "NTree.h"

/**
 * Helper function that will place the tree's nodes into a queue.
 * @param node : the tree node (and children) that will be placed into a queue.
 * @param queue : the queue that will contain the node and all of its children.
 */
void treeQue(NTree node, QueueADT queue){
    // if there is nothing in the tree, exit function
    if (node == NULL) {
        return;
    }
    
    que_insert(queue, node);
    
    // recursively place children into the queue
    for(unsigned i = 0; i < node->child_count; i++)
        treeQue(node->children[i], queue);
}

NTree searchTree(NTree tree, char * name){
    
    /// The Queue to hold the tree
    QueueADT queue = que_create();
    
    // insert the tree into the queue
    treeQue(tree, queue);
    
    while(!que_empty(queue)){
        
        NTree node = que_remove(queue); // front + dequeue
        
        
        if(node == NULL || name == NULL)
            break;
        
        
        // if the data is found
        if(strcmp(node->name, name) == 0){
            que_destroy(queue);
            return node;
        }
        
    }
    
    // if the node was not found
    que_destroy(queue);
    
    return NULL;
}

/**
 * Helper function that will free the contents
 * of a node.
 * @param node : the node to be free'd
 */
void free_node(NTree node){
    free(node->children);
    free(node->name);
    free(node);
}

void destroy_tree(NTree tree){
    
    /// The Queue to hold the tree's nodes
    QueueADT queue;
    
    if(tree != NULL){
        
        // initializing the queue
        queue = que_create();
        
        // placing entire tree into queue
        treeQue(tree, queue);
        
        while (!que_empty(queue)) {
            
            /// NTree pointer that will be used to free the
            /// nodes of the N-Ary tree.
            NTree node_to_be_freed = que_remove(queue); //front + dequeue
            
            free_node(node_to_be_freed);
            
        }
        
        que_destroy(queue);
        
    }
}

void print_node(NTree node){
    
    printf("%s", node->name);
    fflush(stdin);
    
    // if there are no children
    if (node->child_count == 0) {
        printf(" had no offspring.\n");
        fflush(stdin);
    }
    
    // the node has 1 or more children
    else{
        
        printf(" had");
        fflush(stdin);
        
        // the node has 2 or more childre
        if (node->child_count >= 2) {
            
            // print through n-1 children
            for(unsigned i = 0; i < (node->child_count-1); i++){
                printf(" %s,", node->children[i]->name);
                fflush(stdin);
            }
            
            // print the last child
            printf(" and %s.\n", node->children[(node->child_count)-1]->name);
            fflush(stdin);
            
        }
        // the node has only 1 child
        else{
            printf(" %s.\n", node->children[0]->name);
            fflush(stdin);
        }
    }
}

void print_tree(NTree tree, char * name){
    
    /// The node that will be used to point to
    /// the root node or a specified node.
    NTree node;
    
    // if the tree is null and the name is null
    // case enters when tree is empty and a user enters
    // a new-line or "empty string"
    if(tree == NULL && name == NULL)
    {
        fprintf(stderr, "error: empty tree.\n");
        fflush(stderr);
        return;
    }
    
    // if a user didn't enter a name.
    if(name == NULL)
        // pointing the node to the root node.
        node = tree;
    else
        // pointing the node to a specified node.
        node = searchTree(tree, name);
    
    if(node == NULL){
        fprintf(stderr, "error: \'%s\' not found.\n", name);
        fflush(stderr);
        return;
    }
    
    QueueADT queue = que_create();

    que_insert(queue, node);
    
    while(!que_empty(queue)){
        
        // front + dequeue
        node = que_remove(queue);
        
        if(node == NULL)
            break;
        
        // add to queue
        else{
            
            // print out the nodes children
            print_node(node);
            
            for(unsigned i = 0; i < node->child_count; i++){
                if(node->children[i] != NULL){
                    que_insert(queue, node->children[i]);
                    
                }
            }
            
        }
    }
    
    que_destroy(queue);
}

NTree create_node(char * name){
    /// The newly created node for the tree
    NTree new_node = malloc( sizeof(  struct NTree_S ) );
    
    // creating space for the children
    new_node->children = malloc(sizeof(struct NTree_S));
    
    // allocating memory space and assigning string
    new_node->name = malloc(strlen(name)+1);
    strcpy(new_node->name, name);
    
    // setting values to 0
    new_node->child_count = 0;
    
    return new_node;
}

NTree add_child(NTree tree, char * parent_name, char * child_name){
    
    // if empty, add parent as root and the child as the child node
    if(tree == NULL && child_name == NULL){
        tree = create_node(parent_name);
        return tree;
    }
    // if empty,
    if(tree == NULL){
        tree = create_node(parent_name);
        tree->children[tree->child_count] = create_node(child_name);
        tree->child_count++;
        return tree;
    }
    
    /// NTree pointer that will point to a pre-existing node
    /// or will be a newly created node depening on the case.
    NTree parent = NULL; // needed to free -- was initially causes memory leaks
    
    // checking if the child node exists
    if(child_name != NULL){
        //   NTree parent = NULL;
        // if the parent was found
        if ((parent = searchTree(tree, parent_name)) != NULL) {
            
            // checking if child belongs to parent
            for(unsigned i = 0; i < parent->child_count; i++)
                // if the child was found, return original tree
                if(strcmp(parent->children[i]->name, child_name) == 0)
                    return tree;
            
            // reallocating memory space for additional children
            parent->children = realloc(parent->children, sizeof(NTree)*(parent->child_count+1));
            
            // adding the child to the parents node
            parent->children[parent->child_count] = create_node(child_name);
            parent->child_count++; // add child to list if !NULL
        }
        // if the child was the parent node
        else if (strcmp(tree->name, child_name) == 0) {
            
            // placing the parent as the root node
            if((parent = create_node(parent_name)) != NULL){
                parent->children[0] = tree;
                parent->child_count = 1;
                return parent;
            }
            
        }
        else{
            fprintf(stderr, "error: \'%s\' is not in the tree and \'%s\' is not the root.\n", parent_name,child_name);
            fflush(stderr);
            free(parent); // missing this line of code was the source of all my tears
        }
        
    }
    
    return tree;
}

size_t size_of_tree(NTree tree, char * name){
    
    /// NTree pointer to be used if a user enters a name for a
    /// a specified node.
    NTree specific_node;
    
    /// The Queue that will contain all tree nodes.
    QueueADT queue = que_create();

    /// size_t variable that will contain the size of the tree.
    size_t size = 0;

    if (tree != NULL && name == NULL) {
        
        // add all nodes to a queue
        treeQue(tree, queue);
        
        size = sizeQue(queue);
    }
    else if( tree != NULL){
        
        specific_node = searchTree(tree, name);
        
        // if the node exists
        if (specific_node != NULL) {
            treeQue(specific_node, queue);
            size = sizeQue(queue);
        }
        
    }
    
    // destroying the queue to prevent memory leaks
    que_destroy(queue);
    
    return size; // return the size of the queue
}


/**
 * Compare between two numbers.
 * @return the larger number
 */
size_t max(size_t num1, size_t num2){
    return (((num1) > (num2)) ? (num1) : (num2));
}

/**
 * Calculate the height of the tree.
 * @param tree : the node to be used to calculate it's depth.
 */
size_t calculate_height(NTree tree){
    
    /// variable that will contain the height of the tree
    size_t height = 0;
    
    if (tree == NULL) {
        return 0;
    }
    else if( tree->child_count == 0)
        return 0;
    
    else{
        
        for(unsigned i = 0; i < tree->child_count; i++)
            if(tree->children[i] != NULL) // recursivel calculate the height
                height = max(height, calculate_height(tree->children[i]));
    }
    
    return height+1;
}

size_t height_of_tree(NTree tree, char * name){
    /// the height of the tree
    size_t height = 0;
    
    // if the tree is empty, return 0
    if(tree == NULL){
        return 0;
    }
    
    // print entire height
    else if(name == NULL){
        height = calculate_height(tree);
    }
    else{
        
        // searching for the specified node.
        NTree node = searchTree(tree, name);
        
        // if the node is found, calculate the height,
        // otherwise, set height to -1.
        if(node != NULL)
            height = calculate_height(node);
        else{
            height = -1;
        }
        
    }
    return height;
}
