#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_helper.h"

/*
 * Declaration of exit codes
 */
const int SUCCESS = 0;
const int NO_ARGS = 1;
const int INVALID_ARGS = 2;
const int MISSING_HASH = 3;
const int MISSING_ADDRES = 4;
const int INVALID_FILE_PATH = 5;
const int ARGUMENT_CONFLICT = 6;

/*
 * Auxiliary variables for args, hashes etc.
 */
const int ALL_VALID_ARGS_COUNT = 6;
const int VALID_HASHES_COUNT = 4;
typedef enum {
    HEX = 4,
    FARG
} ARG_INDEXES;
const char *allowedArgs[6] = { "-xor", "-c16", "-c32", "-md5", "-hex", "-f" };
const char *hashDescriptors[4] = { "XOR", "CRC-16", "CRC-32", "MD5" };

typedef struct
{
    const char *info;
    void *hCtx;
    void (*hashCount)();
    int length;
    bool used;
} validArg;

int getPosInArgs(const int argc, const char *argv[], const char *searchedArg)
{
    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], searchedArg)) {
            return i;
        }
    }
    return -1;
}

int getHashFromCtx(void *context, const char *arg)
{
    if (!strcmp(hashDescriptors[0], arg))
        return (int)((xor_context *)context)->xorH;
    if (!strcmp(hashDescriptors[1], arg))
        return (int)((crc16_context *)context)->crc;
    if (!strcmp(hashDescriptors[2], arg))
        return (int)~((crc32_context *)context)->crc;
    return 0;
}

bool containsFileAddress(char *argv[], const int i, validArg validArgs[])
{
    return i > 1 && !strcmp(argv[i - 1], allowedArgs[FARG]) && !validArgs[FARG].info;
}

bool atLeastOneHashArg(validArg validArgs[])
{
    for (int i = 0; i < VALID_HASHES_COUNT; i++) {
        if (validArgs[i].used) {
            return true;
        }
    }
    return false;
}

void trySetArgUsed(validArg validArgs[], int validArgPos, char *arg, void **contexts, void (*hashCountFunc[4])())
{
    if (validArgs[validArgPos].used) {
        fprintf(stderr, "Argument '%s' set up multiple times!", arg);
        exit(ARGUMENT_CONFLICT);
    }
    validArgs[validArgPos].used = true;

    for (int i = 0; i < VALID_HASHES_COUNT; i++) {
        if (!strcmp(allowedArgs[i], arg)) {
            validArgs[validArgPos].hCtx = contexts[i];
            validArgs[validArgPos].info = hashDescriptors[i];
            validArgs[validArgPos].hashCount = hashCountFunc[i];
            validArgs[validArgPos].length = 2 << i;
        }
    }
}

void loadArgs(const int argc, char *argv[], validArg validArgs[], void **contexts, void (*hashCountFunc[4])())
{
    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) > 255) {
            fprintf(stderr, "'%s' is too long argument\n", argv[i]);
            exit(INVALID_ARGS);
        }
        int validArgPos = getPosInArgs(ALL_VALID_ARGS_COUNT, allowedArgs, argv[i]);
        bool isFileAddress = containsFileAddress(argv, i, validArgs);

        if (validArgPos == -1 && !isFileAddress) {
            fprintf(stderr, "You have provided invalid argument '%s'!\n", argv[i]);
            exit(INVALID_ARGS);
        } else if (isFileAddress) {
            validArgs[FARG].info = argv[i];
        } else {
            trySetArgUsed(validArgs, validArgPos, argv[i], contexts, hashCountFunc);
        }
    }
}

void initTest(const int argc, validArg validArgs[])
{
    if (argc == 1) {
        fprintf(stderr, "You must provide at least one argument!\n");
        exit(NO_ARGS);
    }

    if (!atLeastOneHashArg(validArgs)) {
        fprintf(stderr, "You must provide at least one hash argument!\n");
        exit(MISSING_HASH);
    }

    if (validArgs[FARG].used && !validArgs[FARG].info) {
        fprintf(stderr, "You must provide path to hashed file along with -f argument!\n");
        exit(MISSING_ADDRES);
    }
}

bool tryOpen(validArg fileArg, FILE **file)
{
    if (fileArg.used) {
        *file = fopen(fileArg.info, "rb");
        if (!*file) {
            fprintf(stderr, "Requested file '%s' was not found or you don't have access rights to read it!\n", fileArg.info);
            exit(INVALID_FILE_PATH);
        }
        return true;
    }
    *file = stdin;
    return false;
}

void initHashes(void **contexts, void (*hashInitFunc[4])())
{
    for (int i = 0; i < VALID_HASHES_COUNT; i++) {
        hashInitFunc[i](contexts[i]);
    }
}

void countHashes(FILE *file, unsigned int *len, validArg validArgs[])
{
    unsigned char buffer[512] = "";
    size_t bytesRead;

    do {
        bytesRead = fread(buffer, sizeof(char), sizeof(buffer), file);
        (*len) += bytesRead;

        for (int i = 0; i < VALID_HASHES_COUNT; i++) {
            if (validArgs[i].used) {
                validArgs[i].hashCount(validArgs[i].hCtx, buffer, bytesRead);
            }
        }
    } while (bytesRead == sizeof(buffer));
}

void printMD5(MD5_CTX *context)
{
    unsigned char result[16] = "";
    MD5_Final(result, context);
    for (int i = 0; i < 16; i++) {
        printf("%02x", result[i]);
    }
}

void printFormattedHashes(validArg validArgs[])
{
    for (int i = 0; i < VALID_HASHES_COUNT; i++) {
        if (validArgs[i].used) {
            printf("%s: ", validArgs[i].info);
            if (!strcmp("MD5", validArgs[i].info)) {
                printMD5(validArgs[i].hCtx);
            } else {
                if (validArgs[HEX].used) {
                    printf("0x%0*x", validArgs[i].length, getHashFromCtx(validArgs[i].hCtx, validArgs[i].info));
                } else {
                    printf("%u", getHashFromCtx(validArgs[i].hCtx, validArgs[i].info));
                }
            }
            printf("\n");
        }
    }
}

int main(const int argc, char *argv[])
{
    validArg validArgs[6] = { { 0 } };
    xor_context xorCTX;
    crc16_context c16CTX;
    crc32_context c32CTX;
    MD5_CTX md5CTX;
    void *contexts[4] = { &xorCTX, &c16CTX, &c32CTX, &md5CTX };
    void (*hashInitFunc[4])() = { xor_init, crc16_init, crc32_init, MD5_Init };
    void (*hashCountFunc[4])() = { xor_update, crc16_update, crc32_update, MD5_Update };
    FILE *fileToHash = NULL;
    unsigned int len = 0;

    initHashes(contexts, hashInitFunc);
    loadArgs(argc, argv, validArgs, contexts, hashCountFunc);
    initTest(argc, validArgs);
    bool opened = tryOpen(validArgs[FARG], &fileToHash);

    countHashes(fileToHash, &len, validArgs);

    printf("Length: %u bytes\n", len);
    printFormattedHashes(validArgs);

    if (opened) {
        fclose(fileToHash);
    }
    return SUCCESS;
}
