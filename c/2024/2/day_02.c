#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INIT_REPORT_LIST_SIZE 10

struct ReportRow {
    int levels[15];
    size_t count;
} typedef ReportRow;

struct ReportList {
    ReportRow* reports;
    size_t count;
    size_t capacity;
} typedef ReportList;

enum LEVEL_ORDER {
    UNDEF,
    ASC,
    DEC
};

int readData(char* filename, ReportList* dest);
int expandReport(ReportList* report, size_t newCapacity);
void printReport(ReportList* report);
void addLevelToReport(int level, int reportIndex, ReportList* list);
int calculateSafeReports(ReportList* list);
int isReportSafe(ReportRow* row);
void printReportRow(ReportRow* row);
ReportRow** generateRowPermutations(ReportRow* row);

int main(int argc, char** argv) {
    /* verify args */
    if (argc < 2) {
        printf("Please specify input file\n");
        return -1;
    }

    char* filename = argv[1];
    ReportList data = {NULL, 0, 0};
    if (expandReport(&data, INIT_REPORT_LIST_SIZE)) return -1;

    /* read file */
    if (readData(filename, &data)) {
        printf("Error executing file operations on %s", filename);
        return -1;
    }
    printReport(&data);
    
    /* determine safety of reports */
    int numSafe = calculateSafeReports(&data);
    printf("Number of safe reports: %d\n", numSafe);
}

int readData(char* filename, ReportList* dest) {
    FILE* inputFile = fopen(filename, "r");
    if (inputFile == NULL) {
        return -1;
    }

    char lineBuffer[50];
    while (fgets(lineBuffer, 50, inputFile) != NULL) {
        int reportRowIndex = dest->count;
        int bytes_read = 0;
        int num = 0;
        char* pointerInLine = lineBuffer;

        while (sscanf(pointerInLine, "%d%n", &num, &bytes_read) == 1) {
            addLevelToReport(num, reportRowIndex, dest);
            //printReportRow(&dest->reports[reportRowIndex]);
            pointerInLine += bytes_read;
        }

        dest->count += 1;
    }

    if (fclose(inputFile)) return -1;

    return 0;
}

int expandReport(ReportList* report, size_t newCapacity) {
    size_t oldCapacity = report->capacity;
    printf("Expanding report from %lu to %lu\n", oldCapacity, newCapacity);

    if (report->reports == NULL) {
        printf("init\n");
        /* initialize the struct */
        ReportRow* rows = calloc(newCapacity, sizeof(ReportRow));
        if (rows == NULL) return -1;

        memset(rows, 0, sizeof(ReportRow) * newCapacity);
        report->reports = rows;
        report->capacity = newCapacity;
    } else {
        /* use realloc */
        printf("realloc\n");
        ReportRow* expanded = realloc(report->reports, newCapacity * sizeof(ReportRow));
        if (expanded == NULL) return -1;

        memset(expanded + (sizeof(ReportRow) * oldCapacity), 0, (newCapacity - oldCapacity) * sizeof(ReportRow));
        report->reports = expanded;
        report->capacity = newCapacity;
    }
    
    printf("Report now has capacity: %lu from request capacity %lu\n", report->capacity, newCapacity);
    return 0;
}

void printReport(ReportList* report) {
    printf("Report count: %lu, capacity: %lu\n", report->count, report->capacity);

    size_t i;
    for (i = 0; i < report->count; i++) {
        printf("Printing row %d of %d\n", i + 1, report->count);
        ReportRow* row = &report->reports[i];
        printReportRow(row);
    }
}

void printReportRow(ReportRow* row) {
    size_t j;
    for (j = 0; j < row->count - 1; j++) {
        printf("%d ", row->levels[j]);
    }
    printf("%d\n", row->levels[row->count - 1]);
}

void addLevelToReport(int level, int reportIndex, ReportList* list) {
    if (list->count == list->capacity) {
        if (expandReport(list, list->capacity * 2)) return;
    }

    ReportRow* row = &list->reports[reportIndex];
    row->levels[row->count] = level;
    row->count++;
}

int calculateSafeReports(ReportList* list) {
    size_t i;
    int accum = 0;
    for (i = 0; i < list->count; i++) {
        ReportRow* row = &list->reports[i];

        if (isReportSafe(row) == 1) {
            printf("%lu. Report SAFE\n", i);
            accum += 1;
        } else {
            printf("%lu. Report UNSAFE\n", i);
            ReportRow** permutations = generateRowPermutations(row);
            if (permutations == NULL) {
                printf("generateRowPermutations returned no data\n");
            } else {
                size_t j;
                for (j = 0; j < row->count; j++) {
                    ReportRow* selected = permutations[j];
                    if (isReportSafe(selected) == 1) {
                        printf("Row permutation is valid, adding to accum");
                        accum += 1;
                        break;
                    }
                }
            }
        }
    }

    return accum;
}

int isReportSafe(ReportRow* row) {
    /* printf("Calculating safety on row with %d levels\n", row->count); */
    printReportRow(row);
    int isSafe = 1;
    enum LEVEL_ORDER order = UNDEF;
    size_t i;
    for (i = 1; i < row->count; i++) {
        if (order == UNDEF) {
            int last = row->levels[i - 1];
            int current = row->levels[i];

            if (last == current) {
                isSafe = 0;
            } else if (last < current) {
                order = ASC;
            } else {
                order = DEC;
            }
        }

        int last = row->levels[i - 1];
        int current = row->levels[i];

        if (order == ASC && last > current) {
            isSafe = 0;
        } else if (order == DEC && current > last) {
            isSafe = 0;
        }

        int diff = abs(current - last);
        if (diff == 0 || diff > 3) {
            isSafe = 0;
        }

        if (isSafe == 0) {
            break;
        }
    }

    return isSafe;
}

ReportRow** generateRowPermutations(ReportRow* row) {
    printf("Generating %lu permutations", row->count);

    ReportRow** reports = malloc(row->count * sizeof(ReportRow*));
    if (reports == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < row->count; i++) {
        ReportRow* newRow = (ReportRow*) calloc(1, sizeof(ReportRow));
        if (newRow == NULL) {
            printf("generateReportPermutations could not alloc a ReportRow");
            return NULL;
        }

        newRow->count = row->count - 1;
        size_t newRowIndex = 0;
        size_t j;
        for (j = 0; j < row->count; j++) {
            if (j == i) continue;

            newRow->levels[newRowIndex] = row->levels[j];
            newRowIndex++;
        }

        reports[i] = newRow;
    }

    return reports;
}