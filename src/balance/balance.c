#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack {
    char * stackArr;
    int capacity;
    int top;    // to point on the top of the stack
};
void push(struct Stack * stack, char data) {
    // if stack is full, return
    if( (stack->top+1) == stack->capacity ) {
        return;
    }
    stack->top++;
    stack->stackArr[ stack->top ] = data;
}

// method to check if stack is empty
int isEmpty( struct Stack * stack ) {
    return stack->top == -1;
}

// method to pop from the stack
char pop(struct Stack * stack) {
    if( isEmpty(stack) )
        return 0;
    char stackTop = stack->stackArr[stack->top];
    stack->top--;
    return stackTop;
}

int main(int argc, char *argv[]) {

    if( argc == 1 ) {
        exit(EXIT_FAILURE);
    }
    char * string = argv[1];
    // create a stack
    struct Stack * stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = strlen(string);
    stack->top = -1;
    stack->stackArr = (char *)malloc(sizeof(char) * stack->capacity);

    // iterate the string
    int i;
    for( i=0; i<strlen(string); i++ ) {

        char c = string[i];
        switch (c)
        {
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
            break;
        default:
            continue;
        }

        if( c=='{' || c=='[' || c=='(' ) {
            push(stack,c);
        }
        else {
            char top = pop(stack);
            // if stack is empty 
            if( top == 0 ){
                printf("%d: %c\n",i,c);
                exit(EXIT_FAILURE);
            }
            
            if( c=='}' ) {
                if( top != '{' ) {
                    printf("%d: %c\n",i,c);
                    exit(EXIT_FAILURE);
                }
            }
            else if( c==']' ){
                if( top != '[' ) {
                    printf("%d: %c\n",i,c);
                    exit(EXIT_FAILURE);
                }
            }
            else if( c==')' ) {
                if( top != '(' ) {
                    printf("%d: %c\n",i,c);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    if( !isEmpty(stack)) {
        printf("open: ");
        while( !isEmpty(stack) ) {
            char top = pop(stack);
            switch(top) {
                case '{':
                    printf("}");
                    break;
                case '[':
                    printf("]");
                    break;
                case '(':
                    printf(")");
            }
        } 
        printf("\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
