#include <stdio.h>
#include <string.h>

#define MAX_MAP_DIMENSION 150
#define MAP_GUARD_CHAR '^'
#define MAP_OBSTACLE_CHAR '#'
#define MAP_EMPTY_SPACE_CHAR '.'

typedef enum {
    TYPE_OUT_OF_BOUNDS = 0,
    TYPE_EMPTY = 1,
    TYPE_OBSTACLE = 2,
    TYPE_GUARD = 3,
    TYPE_VISITED = 4
} MapEntity;

struct GuardData {
    size_t x, y;
    int xVec, yVec;
    size_t visitedTiles;
} typedef GuardData;

char map[MAX_MAP_DIMENSION][MAX_MAP_DIMENSION];
char originalMap[MAX_MAP_DIMENSION][MAX_MAP_DIMENSION];
char visitedObstaclesMap[MAX_MAP_DIMENSION][MAX_MAP_DIMENSION];
GuardData guardData = {0, 0, 0, -1, 0}; /* initialize facing up */
GuardData guardDataSafeCopy = {0, 0, 0, -1, 0};

void run(char* filename);
void importData(char* filename);
MapEntity getEntityType(char input);
void printMap(char currentMap[MAX_MAP_DIMENSION][MAX_MAP_DIMENSION]);
void runPartOne();
void runPartTwo();
void turnGuardRight();
void moveGuardForwardOneStep();
int runGuardSim(int isPartTwo);

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Missing filename as argument\n");
        return -1;
    }

    run(argv[1]);
    
    return 0;
}

void run(char* filename) {
    importData(filename);
    runPartOne();
    runPartTwo();
}

void importData(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char tempLine[MAX_MAP_DIMENSION];
    size_t y;
    for (y = 0; y < MAX_MAP_DIMENSION; y++) {
        if (fgets(tempLine, MAX_MAP_DIMENSION, file) != NULL) {
            size_t lineLength = strnlen(tempLine, MAX_MAP_DIMENSION);

            size_t x;
            for (x = 0; x < lineLength; x++) {
                MapEntity entityType = getEntityType(tempLine[x]);
                map[y][x] = entityType;

                if (entityType == TYPE_GUARD) {
                    guardData.x = x;
                    guardData.y = y;
                    printf("The guard is at %lu, %lu\n", x, y);
                }
            }
        }
    }

    memcpy(originalMap, map, MAX_MAP_DIMENSION * MAX_MAP_DIMENSION);
    guardDataSafeCopy = guardData;

    if (fclose(file) != 0) {
        printf("Error closing file!\n");
        return;
    }
}

MapEntity getEntityType(char input) {
    if (input == MAP_GUARD_CHAR) {
        return TYPE_GUARD;
    } else if (input == MAP_OBSTACLE_CHAR) {
        return TYPE_OBSTACLE;
    } else if (input == MAP_EMPTY_SPACE_CHAR) {
        return TYPE_EMPTY;
    } else {
        return TYPE_OUT_OF_BOUNDS;
    }
}

void printMap(char currentMap[MAX_MAP_DIMENSION][MAX_MAP_DIMENSION]) {
    size_t y;
    for (y = 0; y < MAX_MAP_DIMENSION; y++) {
        size_t x;
        for (x = 0; x < MAX_MAP_DIMENSION; x++) {
            MapEntity type = currentMap[y][x];

            if (type == TYPE_OUT_OF_BOUNDS) continue;

            printf("%d", type);
        }

        if (currentMap[y][0] != TYPE_OUT_OF_BOUNDS) printf("\n");
    }
}

void runPartOne() {
    runGuardSim(0);
    printf("The guard visited %lu distinct locations!\n", guardData.visitedTiles);
}

void runPartTwo() {
    guardData = guardDataSafeCopy;
    memcpy(map, originalMap, MAX_MAP_DIMENSION * MAX_MAP_DIMENSION);

    int loops = 0;
    size_t x, y;
    for (y = 0; y < MAX_MAP_DIMENSION; y++) {
        for (x = 0; x < MAX_MAP_DIMENSION; x++) {
            MapEntity typeAtPointer = map[y][x];

            if (typeAtPointer == TYPE_EMPTY) {
                map[y][x] = TYPE_OBSTACLE;
            } else {
                continue;
            }

            loops += runGuardSim(1);

            memcpy(map, originalMap, MAX_MAP_DIMENSION * MAX_MAP_DIMENSION);
            memset(visitedObstaclesMap, 0, MAX_MAP_DIMENSION * MAX_MAP_DIMENSION);
            guardData = guardDataSafeCopy;
        }
    }

    printf("The guard had %d unique loops!\n", loops);
}

int runGuardSim(int isPartTwo) {
    int didLoop = 0;
    MapEntity tileTypeAtGuardPos = TYPE_GUARD;
    while (tileTypeAtGuardPos != TYPE_OUT_OF_BOUNDS) {
        /* check what is in front of us */
        size_t nextX = guardData.x + guardData.xVec;
        size_t nextY = guardData.y + guardData.yVec;

        MapEntity nextType = map[nextY][nextX];
        if (nextType == TYPE_OBSTACLE) {
            if (isPartTwo) {
                if (visitedObstaclesMap[nextY][nextX] == 1) {
                    /* we have visited this obstable before and there is a loop */

                    /* BUG: we need to track direction too*/
                    didLoop = 1;
                    break;
                } else {
                    visitedObstaclesMap[nextY][nextX] = 1;
                }
            }
            
            /* if obstacle turn 90 deg to the right */
            turnGuardRight();
        } else {
             /* otherwise take a step and add to total */
            moveGuardForwardOneStep();
        }

        /* update loop variable */
        tileTypeAtGuardPos = map[guardData.y][guardData.x];
    }

    return didLoop;
}

void turnGuardRight() {
    int oldXVec = guardData.xVec;
    guardData.xVec = -1 * guardData.yVec;
    guardData.yVec = oldXVec;
    /* newx = -y */
    /* newy = x */
}

void moveGuardForwardOneStep() {
    MapEntity oldType = map[guardData.y][guardData.x];
    map[guardData.y][guardData.x] = TYPE_VISITED;
    guardData.x = guardData.x + guardData.xVec;
    guardData.y = guardData.y + guardData.yVec;
    if (oldType != TYPE_VISITED) guardData.visitedTiles++;
}