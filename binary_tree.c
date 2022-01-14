#include <stdio.h>
#include <stdlib.h>

typedef struct node {

    int data;
    struct node * left, * right;

} node;

typedef node * p_node;

p_node create_tree(int x, p_node left, p_node right) {

    //Creates an integer binary tree, with x at the root. Returns the root node. 

    p_node root = malloc(sizeof(node));
    root->data = x;
    root->left = left;
    root->right = right;
    return root;
}

p_node search_node(p_node root, int x) {

    //Returns the node whose data is x. If none is found, the function returns NULL

    p_node left;

    if (root == NULL || root->data == x) { //if the root is null or has the searched data
        return root;
    }

    left = search_node(root->left, x);

    if (left != NULL) { //if we find the searched data in the left sub-tree
        return left;
    }

    return search_node(root->right, x); //if we dont find it in the left sub-tree, then we look right
}

int number_nodes(p_node root) {

    //Returns the number of nodes in the tree whose root node is given

    if (root == NULL) {
        return 0;
    }
    return 1 + number_nodes(root->left) + number_nodes(root->right);
}

int height(p_node root) {

    int h_left, h_right;

    if (root == NULL) {
        return 0;
    }

    h_left = height(root->left);
    h_right = height(root->right);

    return 1 + (h_left > h_right ? h_left : h_right); //height is equal to 1 + the largest height found in the subtrees
}

p_node insert(p_node root, int key) {

    //Inserts a node with the key value in a binary search tree and returns the root of the new tree

    p_node new;

    if (root == NULL) { //If we reached a leaf of the tree:

        new = malloc(sizeof(node)); 
        new->left = new->right = NULL;
        new->data = key;

        return new; //we return the new node. If the tree was empty, the new node will be the root
    }

    //If we still did not reach a leaf, the function will find the position of the new node and create it:

    if (key < root->data) {
        root->left = insert(root->left, key); 
    }
    else {
        root->right = insert(root->right, key);
    }

    return root; //the root of the tree is returned as it remains the same
}

//Depth-first searches:

void pre_order(p_node root) {

    if (root != NULL) {

        printf("%d ", root->data); //we firstly print the tree's root
        pre_order(root->left); //then we print the whole left subtree, starting by it's root
        pre_order(root->right); //finally, we print the whole right subtree, starting by it's root

    }
}

void pos_order(p_node root) {

    if (root != NULL) {

        pos_order(root->left);
        pos_order(root->right);
        printf("%d ", root->data); //the root is printed only after the subtrees
    }
}

void inorder(p_node root) {

    if (root != NULL) {

        inorder(root->left); //firstly, we print the left subtree
        printf("%d ", root->data); //then, we print the root
        inorder(root->right); //and finally, the right subtree
    }
}