#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    // if there are no arguments
    if( argc == 1 ) {
        return 0;
    }

    char * string = argv[1];
    // strlen - used to find the length 
    int length = strlen(string);

    if( length == 0 ){
        return 0;
    }
    int i;
    for( i=0; i<length; i++ ) {
        string[i] = toupper(string[i]);
    }
    printf("%s!!\n", string);
    return 0;
}
