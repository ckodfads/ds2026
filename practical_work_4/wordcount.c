#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 10000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} KeyValue;
KeyValue result_list[MAX_WORDS];
int result_count = 0;

void reducer(char *word) {
    for (int i = 0; i < result_count; i++) {
        if (strcmp(result_list[i].word, word) == 0) {
            result_list[i].count++;
            return;
        }
    }

    if (result_count < MAX_WORDS) {
        strcpy(result_list[result_count].word, word);
        result_list[result_count].count = 1;
        result_count++;
    }
}

void mapper(char *text) {
    char *token = strtok(text, " \t\n\r.,;");
    while (token != NULL) {
        for(int i = 0; token[i]; i++){
            token[i] = tolower(token[i]);
        }
        
        reducer(token);
        
        token = strtok(NULL, " \t\n\r.,;");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("File open error");
        return 1;
    }

    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0;
        mapper(buffer);
    }

    fclose(fp);

    printf("--- MapReduce WordCount Result ---\n");
    for (int i = 0; i < result_count; i++) {
        printf("%s: %d\n", result_list[i].word, result_list[i].count);
    }

    return 0;
}