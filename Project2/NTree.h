/**
 * Project 2: Offspring
 * @file: NTree.h
 * Description: Header file to NTree.c, a N-Ary tree.
 * @author Samuel Tregea
 */

#ifndef NTree_h
#define NTree_h

#include <stdio.h>
#include "Queue.h"

#ifndef _NTREE_IMPL_
typedef struct NTree_S * NTree;

#endif

/**
 * Search the tree using breadth first search
 * @param tree : the tree to search.
 * @param name : the name of the node to search for.
 * @return a node and it's children, NULL if not found.
 */
NTree searchTree(NTree tree, char * name);

/**
 * Free's all memory of the tree and its offspring
 * @param tree : the tree to be destroyed.
 */
void destroy_tree(NTree tree);

/**
 * Helper function to print_tree().
 * Function will print out a specified node and its children.
 * Function is used in the driver when a user enters 'find [name]'.
 * @param node : the node to print.
 */
void print_node(NTree node);

/**
 * Print out entire tree, or from name downward.
 * @param tree : the tree to print.
 * @param name : the specidic node to print.
 */
void print_tree(NTree tree, char * name);

/**
 * Create a node
 * @param name : the name of the node
 * @return a new node
 */
NTree create_node(char * name);

/**
 * Finds the names parent, adds the child, and then
 * returns the possibly revised tree node pointer with the child added to the
 * parent's set of children.
 * @param tree : the tree to search through
 * @param parent_name : the name of the parent node
 * @param child_name : the name of the child node
 */
NTree add_child(NTree tree, char * parent_name, char * child_name);

/**
 * Retrieve the size of the tree and all of its subnodes.
 * @param tree : the tree to count.
 * @param name : the name of a specified node to count.
 * @return the number of nodes in the tree
 */
size_t size_of_tree(NTree tree, char * name);

/**
 * Retrieve the height of a tree.
 * @param tree : the tree to be used to calculate height.
 * @param name : the name of the node to calculate the height.
 * @return the height of a tree, -1 if a node doesn't exist.
 */
size_t height_of_tree(NTree tree, char * name);

#endif /* NTree_h */
