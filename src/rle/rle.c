#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // if no comannd line arguments 
    if( argc == 1 ) {
        return 0;
    }
    char * string = argv[1];
    int length = strlen(string);
    int csLength = 0;
    int i = 0;
    int charCount = 0;
    int error = 0;
    while( i<length ) {
        char c = string[i];
        if( isdigit(c) ) {
            error = 1;
            break;
        }
        int j = i;
        csLength++;
        while( j<length && c == string[j] ) {
            j++;
        }
        charCount = j-i;
        csLength++;
        while( charCount >= 10 ) {
            csLength++;
            charCount = charCount/10;
        }
        i = j;
    }
    if( error ) {
        printf("error\n");
    }
    else if( csLength > length ) {
        printf("%s\n",string);
    }
    else {
        char * cString = (char *)malloc(sizeof(char) * (csLength + 1));
        int index = 0;
        i = 0;
        while( i<length ) {
            char c = string[i];
            int j = i;
            while( j<length && c == string[j] ) {
                j++;
            }
            charCount = j-i;
            i = j;
            cString[index++] = c;
            char strCount[20];
            sprintf(strCount, "%d",charCount);
            for( j=0; j<strlen(strCount); j++ ) {
                cString[index++] = strCount[j];
            }
        }
        cString[index] = '\0';
        printf("%s\n", cString);
        free(cString);
    }
    return 0;
}
