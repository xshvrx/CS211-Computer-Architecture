#include <stdio.h>
#include <stdlib.h>

// the definitions
struct Node {
    int item;
    struct Node * next;
}; 

struct Node * createNode(int item){
    struct Node * node = (struct Node* )malloc(sizeof(struct Node));
    node->item = item;
    node->next = NULL;
    return node;
}

void printList(struct Node * head) {
    struct Node * curr = head;
    while( curr != NULL ) {
        printf("%d", curr->item);
        if( curr->next ) {
            printf(" ");
        }
        curr = curr->next;
    }
    printf("\n");
}

int search(struct Node * head, int item) {
    struct Node * curr = head;
    while( curr != NULL ) {
        if( curr->item == item ) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

int countNodes(struct Node * head){
    struct Node * curr = head;
    int count = 0;
    while( curr != NULL ) {
        count++;
        curr = curr->next;  
    }
    return count;
}

struct Node * insert(struct Node * head, int item){

    if( search(head,item) ){
        return head;
    }

    if( head == NULL ) {
        head = createNode(item);
    }
    else {
        struct Node * newNode = createNode(item);
        newNode->next = head;
        head = newNode;
        struct Node * curr = head;
        while( curr->next ) {
            if( curr->next->item < curr->item ) {
                int temp = curr->next->item;
                curr->next->item = curr->item;
                curr->item = temp;
            }
            curr = curr->next;
        }
    }
    return head;
}

struct Node * delete(struct Node * head, int item ){

    if( !search(head,item) ){
        return head;
    }
    // if the node to delete is at the head
    if( head->item == item ) {
        struct Node * temp = head;
        head = head->next;
        free(temp);
    }
    else {

        struct Node * prev = head;
        struct Node * curr = head->next;
        while( curr ) {
            if( curr->item == item ) {
                struct Node * n = curr->next;
                free(curr);
                prev->next = n;
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    return head;
}

void freeList(struct Node * head) {
    while( head ){
        struct Node * t = head;
        head = head->next;
        free(t);
    } 
}

int main(int argc, char *argv[]){
    char command[2];
    struct Node * head = NULL;
    while( scanf("%s", command) != -1 ) {
        // printf("Command is: %s\n", command);
        int i;
        scanf("%d", &i);
        if( command[0] == 'i' ) {
            head = insert(head,i);
        }
        else if( command[0] == 'd' ) {
            head = delete(head,i);
        }
        printf("%d :", countNodes(head));
        if( countNodes(head) > 0 ){
            printf(" ");
        }
        printList(head);
    }
	freeList(head);
    
    return 0; 
}
