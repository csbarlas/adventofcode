#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_INIT_COUNT 100

struct LocationIDBuffer {
    int* buffer;
    size_t size;
    size_t count;
} typedef LocationIDBuffer;

int readDataToBuffer(char* filename);
int initBuffers();
void addToBuffer(int num, LocationIDBuffer* buffer);
void sortBuffer(LocationIDBuffer* buffer);
void printBuffer(LocationIDBuffer* buffer);
int findDistance();
long findSimularityScore();

LocationIDBuffer leftList, rightList;

int main(int argc, char** argv) {
    /* read file into buffer */
    if (argc < 2) {
        printf("Please specify input file\n");
        return -1;
    }

    char* filename = argv[1];
    if (readDataToBuffer(filename)) {
        return -1;
    }

    /* sort buffers */
    sortBuffer(&leftList);
    printf("Left buffer sorted:\n");
    printBuffer(&leftList);

    sortBuffer(&rightList);
    printf("Right buffer sorted:\n");
    printBuffer(&rightList);

    /* run calculation pt. 1 */
    printf("Total distance is: %d\n", findDistance());

    /* pt 2 */
    printf("Simularity score is: %ld\n", findSimularityScore());
}

int readDataToBuffer(char* filename) {
    printf("Opening %s...\n", filename);
    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        printf("Failed to open %s\n", filename);
        return -1;
    }

    if (initBuffers()) {
        return -1;
    }

    int leftNum, rightNum;
    while(fscanf(inputFile, "%d%d", &leftNum, &rightNum) == 2) {
        addToBuffer(leftNum, &leftList);
        addToBuffer(rightNum, &rightList);
    }

    if (fclose(inputFile)) {
        printf("Error closing file\n");
        return -1;
    }
    printf("File closed\n");
    printf("Left list count: %lu\nRight list count: %lu\n", leftList.count, rightList.count);

    return 0;
}

int initBuffer(LocationIDBuffer* buffer) {
    buffer->buffer = (int*) calloc(BUFFER_INIT_COUNT, sizeof(int));

    if (buffer->buffer == NULL) {
        return -1;
    }

    buffer->size = BUFFER_INIT_COUNT;
    buffer->count = 0;
    return 0;
}

int initBuffers() {
    if (initBuffer(&leftList) == -1 || initBuffer(&rightList) == -1) {
        printf("Error initializing buffers\n");
        free(leftList.buffer);
        free(rightList.buffer);
        return -1;
    }

    return 0;
}

int expandBuffer(LocationIDBuffer* buffer) {
    size_t newSize = buffer->size * 2;
    printf("Expanding buffer capacity from %lu to %lu\n", buffer->size, newSize);

    int* tempBuffer = (int *) calloc(newSize, sizeof(int));
    if (tempBuffer == NULL) {
        return -1;
    }

    memcpy(tempBuffer, buffer->buffer, sizeof(int) * (buffer->count));

    buffer->buffer = tempBuffer;
    buffer->size = newSize;
    return 0;
}

void addToBuffer(int num, LocationIDBuffer* buffer) {
    if (buffer->count == buffer->size) {
        if (expandBuffer(buffer)) {
            printf("Failed to expand buffer\n");
            return;
        }
    }

    buffer->buffer[buffer->count] = num;
    buffer->count += 1;
}

void sortBuffer(LocationIDBuffer* buffer) {
    size_t i, j;
    for (i = 0; i < buffer->count - 1; i++) {
        size_t smallestIndex = i;
        for (j = i + 1; j < buffer->count; j++) {
            if (buffer->buffer[j] < buffer->buffer[smallestIndex]) {
                smallestIndex = j;
            }
        }

        int temp = buffer->buffer[i];
        buffer->buffer[i] = buffer->buffer[smallestIndex];
        buffer->buffer[smallestIndex] = temp;
    }
}

void printBuffer(LocationIDBuffer* buffer) {
    size_t i;
    for (i = 0; i < buffer->count - 1; i++) {
        printf("%d ", buffer->buffer[i]);
    }

    printf("%d\n", buffer->buffer[buffer->count - 1]);
}

int findDistance() {
    if (leftList.count != rightList.count) {
        return 0;
    }

    size_t i;
    int accum = 0;
    for (i = 0; i < leftList.count; i++) {
        int res = abs(leftList.buffer[i] - rightList.buffer[i]);
        printf("%lu. distance between %d and %d: %d\n", i, leftList.buffer[i], rightList.buffer[i], res);
        accum += abs(leftList.buffer[i] - rightList.buffer[i]);
    }

    return accum;
}

long findSimularityScore() {
    size_t i;
    long accum = 0;
    for (i = 0; i < leftList.count; i++) {
        int leftNum = leftList.buffer[i];
        int occurances = 0;

        size_t j;
        for (j = 0; j < rightList.count; j++) {
            if (rightList.buffer[j] == leftNum) {
                occurances += 1;
            }

            if (rightList.buffer[j] > leftNum) {
                break;
            }
        }

        accum = accum + (leftNum * occurances);
    }

    return accum;
}
