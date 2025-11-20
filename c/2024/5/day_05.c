#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERING_RULE_PAGE_NUM 99

struct Node {
    struct Node* next;
    int value;
} typedef Node;

struct RuleCheckResult {
    int passed;
    size_t index; /* index of where the other page is, if rule not passed */
} typedef RuleCheckResult;

void run(char* filename);
int evaluatePartOneRules(char* str);
int evaluatePartTwoRules(char* str);
RuleCheckResult* doForwardPagesPassRuleCheck(int* pages, int numPages, size_t index, int pageToSearch);
void freeRuleMap();
void extractNPages(char* input, int* pages, int n, int* pagesRead);

/* Globals */
Node* ruleMap[MAX_ORDERING_RULE_PAGE_NUM];

int main(int argc, char** argv) {
    if (argc < 2) return -1;
    run(argv[1]);
    return 0;
}

void run(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return;

    char line[100];
    int partOneSum = 0, partTwoSum = 0;
    int processRuleMode = 0;
    while (fgets(line, 100, file) != NULL) {
        if (processRuleMode) {
            char lineCopy[100];
            strncpy(lineCopy, line, 100);
            partOneSum += evaluatePartOneRules(line);
            partTwoSum += evaluatePartTwoRules(lineCopy);
        } else {
            /* extract the rules into data structures */
            if (strncmp(line, "\n", 1) == 0) {
                processRuleMode = 1;
                continue;
            } else {
                int p1 = 0, p2 = 0;
                if (sscanf(line, "%d|%d", &p1, &p2) == 2) {
                    Node* node = calloc(1, sizeof(Node));
                    node->value = p1;
                    node->next = ruleMap[p2];
                    ruleMap[p2] = node;
                }
            }
        }
    }

    freeRuleMap();

    if (fclose(file) != 0) {
        return;
    }

    printf("Part 1. The middle number sum is %d\n", partOneSum);
    printf("Part 2. The middle number sum is %d\n", partTwoSum);
}

int evaluatePartOneRules(char* str) {
    int pages[50];
    int middleNum = 0;
    int numPages = 0;

    extractNPages(str, pages, 50, &numPages);

    size_t i;
    int passedRuleCheck = 1;
    for (i = 0; i < numPages; i++) {
        int num = pages[i];
        Node* rule = ruleMap[num];
        while (rule != NULL) {
            RuleCheckResult* res = doForwardPagesPassRuleCheck(pages, numPages, i, rule->value);
            passedRuleCheck = passedRuleCheck && (res->passed);
            rule = rule->next;
        }
    }

    if (passedRuleCheck) {
        size_t middleIndex = numPages / 2;
        middleNum = pages[middleIndex];
    }

    return middleNum;
}

int evaluatePartTwoRules(char* str) {
    int pages[50];
    int middleNum = 0;
    int numPages = 0;
    
    extractNPages(str, pages, 50, &numPages);

    size_t i = 0;
    int passedRuleCheck = 1;
    int didCorrectOrder = 0;
    while (i < numPages) {
        int num = pages[i];
        Node* rule = ruleMap[num];
        int didSwap = 0;

        while (rule != NULL) {
            RuleCheckResult* res = doForwardPagesPassRuleCheck(pages, numPages, i, rule->value);
            if (!res->passed) {
                /* swap to satisfy rule and re-run from beginning */
                int temp = pages[i];
                pages[i] = pages[res->index];
                pages[res->index] = temp;
                didSwap = 1;
                didCorrectOrder = 1;
                break;
            }

            passedRuleCheck = passedRuleCheck && res->passed;
            rule = rule->next;
        }

        if (didSwap) {
            i = 0;
            passedRuleCheck = 1;
            continue;
        }

        i++;
    }

    if (passedRuleCheck && didCorrectOrder) {
        size_t middleIndex = numPages / 2;
        middleNum = pages[middleIndex];
    }

    return middleNum;
}

RuleCheckResult* doForwardPagesPassRuleCheck(int* pages, int numPages, size_t index, int pageToSearch) {
    RuleCheckResult* res = calloc(1, sizeof(RuleCheckResult));
    res->passed = 1;

    size_t i;
    for (i = index + 1; i < numPages; i++) {
        int pageToCheck = pages[i];
        if (pageToCheck == pageToSearch) {
            res->passed = 0;
            res->index = i;
            return res;
        }
    }

    return res;
}

void freeRuleMap() {
    size_t i;
    for (i = 0; i < MAX_ORDERING_RULE_PAGE_NUM; i++) {
        Node* rule = ruleMap[i];
        while (rule != NULL) {
            Node* next = rule->next;
            free(rule);
            rule = next;
        }
    }
}

void extractNPages(char* input, int* pages, int n, int* pagesRead) {
    char* token = strtok(input, ",");
    while (token != NULL && *pagesRead < n) {
        int page = atoi(token);
        int pageNum = *pagesRead;
        pages[pageNum] = page;
        *pagesRead += 1;
        token = strtok(NULL, ",");
    }
}