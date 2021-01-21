#include <stdio.h>
#include <stdlib.h>

#include "dataDef.h"
#include "memoryVM.h"
#include "memoryVMObject.h"

#define UNUSED(VAR) ((void) VAR)

/*
 * This file is not submitted as your homework.
 *
 * You should only use it to test and play with your implementation.
 */
void printRefInt(int *ptr)
{
    printf("PTR: 0x%p, REF: %i, %i\n", (void *)ptr, *(ptr - 2), *ptr);
}

void printRefChar(char *ptr)
{
    printf("PTR: 0x%p, REF: %i, CHILD: %i, %c\n", (void *)ptr, *(ptr - 8), *(ptr - 4), *ptr);
}

void printRefString(char *ptr)
{
    printf("PTR: 0x%p, REF: %i, CHILD: %i, %s\n", (void *)ptr, *(ptr - 8), *(ptr - 4), ptr);
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    int *number = allocate(sizeof(int));
    printRefInt(number);
    *number = 5;
    printRefInt(number);
    acquire(number);
    printRefInt(number);
    printf("RELEASE: %i\n", release(number));
    printRefInt(number);
    printf("RELEASE: %i\n\n", release(number));

    char *letter = allocate(sizeof(char));
    printRefChar(letter);
    *letter = 'G';
    printRefChar(letter);
    acquire(letter);
    printRefChar(letter);
    printf("RELEASE: %i\n", release(letter));
    printRefChar(letter);
    printf("RELEASE: %i\n\n", release(letter));

    char *string = allocateArray(sizeof(char), 5);
    printRefString(string);
    string[0] = 'A';
    string[1] = 'H';
    string[2] = 'O';
    string[3] = 'J';
    string[4] = '\0';
    printRefString(string);
    acquire(string);
    printRefString(string);
    printf("RELEASE: %i\n", release(string));
    printRefString(string);
    printf("RELEASE: %i\n\n", release(string));

    size_t lengths[2] = {5, 6};
    char **stringArr = allocateArray2D(sizeof(char), 2, lengths);
    printRefString(stringArr[0]);
    printRefString(stringArr[1]);
    stringArr[0][0] = 'A';
    stringArr[0][1] = 'H';
    stringArr[0][2] = 'O';
    stringArr[0][3] = 'J';
    stringArr[0][4] = '\0';
    stringArr[1][0] = 'S';
    stringArr[1][1] = 'V';
    stringArr[1][2] = 'E';
    stringArr[1][3] = 'T';
    stringArr[1][4] = 'E';
    stringArr[1][5] = '\0';
    printRefString(stringArr[0]);
    printRefString(stringArr[1]);
    acquireArray2D(stringArr);

    printRefString(stringArr[0]);
    printRefString(stringArr[1]);
    printf("RELEASE: %i\n", releaseArray2D(stringArr));
    printRefString(stringArr[0]);
    printRefString(stringArr[1]);
    printf("RELEASE: %i\n\n", releaseArray2D(stringArr));

    return EXIT_SUCCESS;
}
