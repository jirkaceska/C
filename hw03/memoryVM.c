#include <stdlib.h>

//***********************************************
//              IMPORTANT NOTICE                *
//***********************************************
//  If you need to add more headers, which      *
//  you CAN, add them AFTER THIS LINE           *
#include <string.h>
//  but BEFORE the following one:               *
#include "memoryVM.h"
//  otherwise your code MAY FAIL TO COMPILE!    *
//                                              *
//  You have been warned.                       *
//***********************************************


// Here goes your glorious implementation.
typedef struct
{
    size_t refCount;
    size_t childrenCount;
} VMBlock_t;

VMBlock_t *getVMBlock(void *memory)
{
    return (VMBlock_t *)((size_t *)memory - 2);
}

void *getMemory(VMBlock_t *VMBlock)
{
    return (void *)(VMBlock + 1);
}

bool initVM(void)
{
    return true;
}

void clearVM(void)
{
    return;
}

void *allocate(size_t size)
{
    size_t memLen = 2 * sizeof(size_t) + size;
    VMBlock_t *VMBlock = malloc(memLen);
    if (VMBlock == NULL) {
        return NULL;
    }
    memset(VMBlock, 0, memLen);

    VMBlock->refCount = 1;
    return getMemory(VMBlock);
}

void *allocateArray(size_t elementSize, size_t elementCount)
{
    void *arr = allocate(elementSize * elementCount);
    if (arr == NULL) {
        return NULL;
    }

    VMBlock_t *VMBlock = getVMBlock(arr);
    VMBlock->childrenCount = elementCount;
    return getMemory(VMBlock);
}

void **allocateArray2D(size_t elementSize, size_t subarrays, size_t *elementCounts)
{
    void **outerArr = allocateArray(sizeof(*outerArr), subarrays);
    if (outerArr == NULL) {
        return NULL;
    }

    VMBlock_t *VMBlock = getVMBlock(outerArr);
    VMBlock->childrenCount = 0;

    while (VMBlock->childrenCount < subarrays) {
        size_t elementCount = elementCounts == NULL ? subarrays : elementCounts[VMBlock->childrenCount];
        void *innerArr = allocateArray(elementSize, elementCount);

        if (innerArr == NULL) {
            releaseArray2D(outerArr);
            return NULL;
        }
        outerArr[VMBlock->childrenCount] = innerArr;
        VMBlock->childrenCount++;
    }

    return outerArr;
}

void acquire(void *memory)
{
    VMBlock_t *VMBlock = getVMBlock(memory);
    VMBlock->refCount++;
}

void acquireArray2D(void **array)
{
    VMBlock_t *VMBlock = getVMBlock(array);
    acquire(array);

    for (size_t i = 0; i < VMBlock->childrenCount; i++) {
        acquire(array[i]);
    }
}

bool release(void *memory)
{
    VMBlock_t *VMBlock = getVMBlock(memory);
    VMBlock->refCount--;

    if (VMBlock->refCount == 0) {
        free(VMBlock);
        return true;
    }
    return false;
}

bool releaseArray2D(void **array)
{
    VMBlock_t *VMBlock = getVMBlock(array);

    for (size_t i = 0; i < VMBlock->childrenCount; i++) {
        release(array[i]);
    }
    return release(array);
}
