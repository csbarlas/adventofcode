#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ORDERING_RULE_PAGE_NUM 99

int findCorrectUpdateMiddleNumberSum(char* filename);
int runRulesOnLine(char* str);
int doForwardPagesPassRuleCheck(int* pages, int numPages, size_t index, int pageToSearch);

struct Node {
    struct Node* next;
    int value;
} typedef Node;

/* Globals */
Node* ruleMap[MAX_ORDERING_RULE_PAGE_NUM];

int main(int argc, char** argv) {
    if (argc < 2) return -1;
    int sum = findCorrectUpdateMiddleNumberSum(argv[1]);
    printf("The middle number sum is %d\n", sum);
    return 0;
}

int findCorrectUpdateMiddleNumberSum(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return -1;

    char line[100];
    int sum = 0;
    int processRuleMode = 0;
    while (fgets(line, 100, file) != NULL) {
        if (processRuleMode) {
            printf("running rules on: %s\n", line);
            sum += runRulesOnLine(line);
        } else {
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

    size_t i;
    for (i = 0; i < MAX_ORDERING_RULE_PAGE_NUM; i++) {
        Node* rule = ruleMap[i];
        while (rule != NULL) {
            Node* next = rule->next;
            free(rule);
            rule = next;
        }
    }

    if (fclose(file) != 0) {
        return -1;
    }

    return sum;
}

int runRulesOnLine(char* str) {
    int pages[50];
    int middleNum = 0;
    int numPages = 0;
    char* token = strtok(str, ",");
    while (token != NULL) {
        int page = atoi(token);
        pages[numPages] = page;
        numPages += 1;
        token = strtok(NULL, ",");
    }

    size_t i;
    int passedRuleCheck = 1;
    for (i = 0; i < numPages; i++) {
        int num = pages[i];
        Node* rule = ruleMap[num];
        while (rule != NULL) {
            passedRuleCheck = passedRuleCheck && doForwardPagesPassRuleCheck(pages, numPages, i, rule->value);
            rule = rule->next;
        }
    }

    if (passedRuleCheck) {
        size_t middleIndex = numPages / 2;
        middleNum = pages[middleIndex];
    }

    return middleNum;
}

int doForwardPagesPassRuleCheck(int* pages, int numPages, size_t index, int pageToSearch) {
    size_t i;
    for (i = index + 1; i < numPages; i++) {
        int pageToCheck = pages[i];
        if (pageToCheck == pageToSearch) {
            return 0;
        }
    }

    return 1;
}
