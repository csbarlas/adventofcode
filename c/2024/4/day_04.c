#include "stdio.h"
#include "ctype.h"
#include "string.h"

#define LETTER_ARRAY_MAX_SIZE 150
#define NUM_DIRECTIONS 8
#define VECTOR_SIZE 2

/* Globals */
char letterBuffer[LETTER_ARRAY_MAX_SIZE][LETTER_ARRAY_MAX_SIZE];
int directionList[NUM_DIRECTIONS][VECTOR_SIZE] = {
    {0, -1}, /* 0, N */
    {1, -1}, /* 1, NE */ 
    {1, 0}, /* 2, E */
    {1, 1}, /* 3, SE */
    {0, 1}, /* 4, S */
    {-1, 1}, /* 5, SW */
    {-1, 0}, /* 6, W */
    {-1, -1} /* 7, NW */
};

int readFileIntoBuffer(char* filename);
void findMatches();
int sweepSearchAroundLetterPartOne(size_t x, size_t y);
int searchForCrossMASPartTwo(size_t x, size_t y);
char charFromVector(size_t x, size_t y, int vec[2]);
int areCharactersMASCrossMatch(char c1, char c2);

int main(int argc, char** argv) {
    if (argc < 2) return -1;
    readFileIntoBuffer(argv[1]);
    findMatches();
    return 0;
}

int readFileIntoBuffer(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    char currentChar = '\0';
    int currentRow = 0;
    int currentCol = 0;
    while ((currentChar = fgetc(file)) != EOF) {
        if (currentChar == '\n') {
            currentRow += 1;
            currentCol = 0;
            continue;
        }

        if (isspace(currentChar)) continue;

        letterBuffer[currentRow][currentCol] = currentChar;
        currentCol += 1;
    }

    if (fclose(file) != 0) {
        return -1;
    }

    return 0;
}

void findMatches() {
    int partOneMatches = 0, partTwoMatches = 0;
    size_t x = 0, y = 0;

    for(y = 0; y < LETTER_ARRAY_MAX_SIZE; y++) {
        for(x = 0; x < LETTER_ARRAY_MAX_SIZE; x++) {
            char currentLetter = letterBuffer[y][x];
            if (currentLetter == 'X') {
                partOneMatches += sweepSearchAroundLetterPartOne(x, y);
            }
            else if (currentLetter == 'A') {
                partTwoMatches += searchForCrossMASPartTwo(x, y);
            }
        }
    }

    printf("Number of XMAS matches: %d\n", partOneMatches);
    printf("Number of X-MAS matches: %d\n", partTwoMatches);
}

int sweepSearchAroundLetterPartOne(size_t x, size_t y) {
    if (letterBuffer[y][x] != 'X') return 0;
    int nearMatches = 0;

    size_t i;
    for (i = 0; i < NUM_DIRECTIONS; i++) {
        char word[5] = {'X'}; /* XMAS + null terminator */
        size_t wordPtr = 1;
        int xVec = directionList[i][0];
        int yVec = directionList[i][1];
        int currX = x + xVec;
        int currY = y + yVec;

        while (wordPtr < 4 && currX >= 0 && currY >= 0) {
            if (letterBuffer[currY][currX] == '\0') break;

            word[wordPtr] = letterBuffer[currY][currX];
            wordPtr += 1;
            currX += xVec;
            currY += yVec;
        }
        word[4] = '\0';
        
        if (strncmp("XMAS", word, 4) == 0) {
            nearMatches += 1;
        }
    }

    return nearMatches;
}

int searchForCrossMASPartTwo(size_t x, size_t y) {
    if (letterBuffer[y][x] != 'A') return 0;
    int matches = 0;

    int* northEast = directionList[1];
    int* southEast = directionList[3];
    int* southWest = directionList[5];
    int* northWest = directionList[7];
    
    char topLeftChar = charFromVector(x, y, northWest);
    char bottomRightChar = charFromVector(x, y, southEast);
    char topRightChar = charFromVector(x, y, northEast);
    char bottomLeftChar = charFromVector(x, y, southWest);
    matches += areCharactersMASCrossMatch(topRightChar, bottomLeftChar) && areCharactersMASCrossMatch(topLeftChar, bottomRightChar);

    return matches;
}

char charFromVector(size_t x, size_t y, int* vec) {
    char theChar = '\0';
    int currY = 0, currX = 0;
    if ((currY = y + vec[1]) >= 0 && (currX = x + vec[0]) >= 0) {
        theChar = letterBuffer[currY][currX];
    }
    return theChar;
}

int areCharactersMASCrossMatch(char c1, char c2) {
    return ((c1 == 'M' && c2 == 'S') || (c1 == 'S' && c2 == 'M'));
}