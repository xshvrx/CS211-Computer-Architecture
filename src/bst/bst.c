#include <stdio.h>
#include <stdlib.h>

struct BSTNode {
    int data;
    struct BSTNode * left;
    struct BSTNode * right;
};

struct BSTNode * createNode(int data) {
    struct BSTNode * node = malloc(sizeof(struct BSTNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int search(struct BSTNode * node, int data) {
    if( node == NULL ) {
        return 0;
    } 
    if( data == node->data ) {
        return 1;
    }
    else if( data > node->data ) {
        return search(node->right, data);
    }
    else {
        return search(node->left, data);
    }
}

//  node is printed as a(, followed bythe left sub-tree, the item for that node, the right subtree, and), without spaces.
void print(struct BSTNode * node){
    if( node ) {
        printf("(");
        print(node->left);
        printf("%d", node->data);
        print(node->right);
        printf(")");
    }
}

struct BSTNode * insert(struct BSTNode * node, int data){
    if( node == NULL ) {
        node = createNode(data);
        return node;
    }

    // 

    // this is where the root is not null
    if( data > node->data ) {
        // go right
        node->right = insert(node->right, data);
    }
    else if( data < node->data ) {
        // go left
        node->left = insert(node->left, data);
    }
    else {
        // the same value exists
    }
    return node;
}

struct BSTNode * findMax(struct BSTNode * node) {
    struct BSTNode * curr = node;
    while( curr->right ) {
        curr = curr->right;
    }
    return curr;
}

struct BSTNode * delete(struct BSTNode * node, int data){
    if( data > node->data ) {
        node->right = delete(node->right,data);
    }
    else if( data < node->data ) {
        node->left = delete(node->left,data);
    }
    else {
        // this is the node where this data is to be deleted

        // if this node has no child
        if( node->left == NULL && node->right == NULL ) {
            free(node);
            return NULL;
        }

        // if this node has left child only - that is the right child is null
        if( node->right == NULL ) {
            struct BSTNode * newChild = node->left;
            free(node);
            return newChild;
        }

        // if this node has right child only - that is the left child is null
        if( node->left == NULL ) { 
            struct BSTNode * newChild = node->right;
            free(node);
            return newChild;
        }

        // this is the case where both the children are not null

        // get the maximum from the left subtree
        struct BSTNode * maxInLeft = findMax(node->left);
        node->data = maxInLeft->data;
        node->left = delete(node->left, maxInLeft->data);   
    }
    return node;
}

void freeBST(struct BSTNode * node) {
    if (node)
    free(node->left);
    free(node->right);
    free(node);
}

int main(int argc, char *argv[]) {

    struct BSTNode * root = NULL;

    char command[2];
    while( scanf("%s", command) != -1 ) {
        if( command[0] == 'i' ) {
            int data;
            scanf("%d", &data);
            if( search(root,data) ) {
                printf("not inserted\n");
            }
            else {
                root = insert(root,data);
                printf("inserted\n");
            }
        }
        else if( command[0] == 's' ) {
            int data;
            scanf("%d", &data);
            if( search(root,data)) {
                printf("present\n");
            }
            else {
                printf("absent\n");
            }
        }
        else if( command[0] == 'p' ){
            print(root);
            printf("\n");
        }
        else if( command[0] == 'd' ){
            int data;
            scanf("%d", &data);
            if( search(root,data)) {
                // delete it
                root = delete(root,data);
                printf("deleted\n");
            }
            else {
                printf("absent\n");
            }
        }
    }
    freeBST(root);
    return 0;
}
