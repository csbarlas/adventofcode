#include <stddef.h>
#include <stdio.h>
#include <regex.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int readFileIntoBuffer(char* filename, char** buffer);
long extract(char* buffer);

int main(int argc, char** argv) {
    /* read string into char buffer */
    if (argc < 2) {
        printf("Please specify input file\n");
        return -1;
    }

    char* filename = argv[1];
    char* buffer;
    if (readFileIntoBuffer(filename, &buffer)) {
        printf("Error opening input file\n");
        return -1;
    }
    /*printf("%s", buffer);*/

    /* compile and execute regex */
    long finalValue = extract(buffer);
    printf("Final value: %ld\n", finalValue);

    return 0;
}

int readFileIntoBuffer(char* filename, char** buffer) {
    struct stat st;
    u_int64_t filesize = 0;

    if (stat(filename, &st) == 0) {
        filesize = st.st_size;
    } else {
        return -1;
    }

    const size_t string_length = filesize * sizeof(char) + 1;
    char* tempBuffer = malloc(string_length);
    printf("Allocated %llu\n", filesize);
    if (tempBuffer == NULL) {
        return -1;
    }
    *buffer = tempBuffer;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    char lineBuffer[4096];
    size_t totalChars = 0;
    while(fgets(lineBuffer, filesize, file) != NULL) {
        size_t lineLength = strlen(lineBuffer);
        strncat(tempBuffer, lineBuffer, string_length - 1 - totalChars);
        totalChars += lineLength;
    }

    printf("Total chars read: %lu\n", totalChars);

    if (fclose(file)) {
        return -1;
    }

    return 0;
}

long extract(char* buffer) {
    regex_t pattern;

    /* "mul\\((\\d+),(\\d+)\\)" */
    /* "mul((d+),(d+))" */
    /* "mul\\((d+),(d+)\\)" */
    if (regcomp(&pattern, "don't\\(\\)|do\\(\\)|mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED)) {
        printf("Error compiling regex");
        return 0;
    }

    long ret = 0;
    char* bufferStartPtr = buffer;
    char* bufferPtr = buffer;
    regmatch_t match[pattern.re_nsub + 1];
    int allowInstructions = 1;
    printf("BEGIN\n");
    while (regexec(&pattern, bufferPtr, pattern.re_nsub + 1, match, 0) == 0) {
        char* temp = calloc(50, sizeof(char));
        char* diff = bufferStartPtr + (bufferPtr - bufferStartPtr) + match->rm_so;
        strncpy(temp, diff, match->rm_eo - match->rm_so);

        int skipCheck = 0;
        if (strncmp(temp, "don't()", 7) == 0) {
            printf("STOP: %s\n", temp);
            allowInstructions = 0;
            skipCheck = 1;
        } else if (strncmp(temp, "do()", 4) == 0) {
            printf("BEGIN: %s\n", temp);
            allowInstructions = 1;
            skipCheck = 1;
        }

        if (allowInstructions && !skipCheck) {
          char *num1 = calloc(10, sizeof(char));
          char *num2 = calloc(10, sizeof(char));
          regmatch_t numMatch = match[1];
          char *num1Diff =
              bufferStartPtr + (bufferPtr - bufferStartPtr) + numMatch.rm_so;
          strncpy(num1, num1Diff, numMatch.rm_eo - numMatch.rm_so);

          numMatch = match[2];
          char *num2Diff =
              bufferStartPtr + (bufferPtr - bufferStartPtr) + numMatch.rm_so;
          strncpy(num2, num2Diff, numMatch.rm_eo - numMatch.rm_so);

          int num1Conv = atoi(num1);
          int num2Conv = atoi(num2);

          printf("full match: %s\n", temp);
          printf("\tnum1: %s, num2: %s\n", num1, num2);
          ret += (num1Conv * num2Conv);
          free(num1);
          free(num2);
        }
        bufferPtr += match->rm_eo;

        free(temp);

    }

    regfree(&pattern);
    return ret;
}