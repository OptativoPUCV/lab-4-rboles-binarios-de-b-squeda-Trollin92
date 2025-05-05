#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

                              // junte las funciones para poder ver las structs de treenode y treemap
int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}
TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}
TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *) malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}
void insertTreeMap(TreeMap* tree, void* key, void* value) {
    if (tree == NULL || key == NULL || value == NULL) return;
    if (searchTreeMap(tree, key) != NULL) return;
    TreeNode* newNode = createTreeNode(key, value);
    if (newNode == NULL) return;
    TreeNode* parent = NULL;
    TreeNode* current = tree->root;
    while (current != NULL) {
        parent = current;
        current = tree->lower_than(key, current->pair->key) ? current->left : current->right;
    }
    newNode->parent = parent;
    if (parent == NULL) tree->root = newNode;
    else if (tree->lower_than(key, parent->pair->key)) parent->left = newNode;
    else parent->right = newNode;

    tree->current = newNode;
}
TreeNode * minimum(TreeNode * x) {
    if (x == NULL) return NULL;
    
    while (x->left != NULL) {
        x = x->left;
    }
    
    return x;
}
void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) tree->root = NULL;
        else if (node->parent->left == node) node->parent->left = NULL;
        else node->parent->right = NULL;
    } 
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) tree->root = child;
        else if (node->parent->left == node) node->parent->left = child;
        else node->parent->right = child;
        child->parent = node->parent;
    } 
    else {
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        removeNode(tree, minNode);
        return;
    }

    free(node->pair);
    free(node);
}
void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}
Pair* searchTreeMap(TreeMap* tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    
    tree->current = tree->root;
    while (tree->current != NULL) {
        if (!tree->lower_than(tree->current->pair->key, key) && !tree->lower_than(key, tree->current->pair->key)) {
            return tree->current->pair;
        }
        if (tree->lower_than(tree->current->pair->key, key)) {
            tree->current = tree->current->right;
        } else {
            tree->current = tree->current->left;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode * current = tree->root;
    TreeNode * candidato = NULL;
    while(current != NULL){
        if (is_equal(tree, current->pair->key, key)) {
            tree->current = current;
            return current->pair;
        }
        if (!tree->lower_than(current->pair->key, key)) {
            candidato = current;
            current = current->left;
        } else {
            current = current->right;
        } 
    }
    if (candidato != NULL) {
        tree->current = candidato;
        return candidato->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root->left;
    if (aux == NULL) return NULL;
    aux = minimum(tree->root->left);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode* aux = tree->current;
    if (aux == NULL) return NULL;

    if (aux->right != NULL) {
        aux = minimum(aux->right);
        return aux;
    }

    TreeNode *parent = aux->parent;
    while (parent != NULL && tree->current == parent->right) {
        tree->current = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    if (parent != NULL) return parent->pair;

    return NULL;
}