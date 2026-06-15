#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_WORDS 1000   // 最多单词数
#define MAX_DOCS 100      // 每个单词最多关联文档数
#define MAX_FILES 100     // 最多文件数

// 倒排索引结构
typedef struct {
    char word[50];                // 单词
    char docs[MAX_DOCS][100];     // 包含该单词的文档列表
    int docCount;                 // 文档数量
} Index;

Index indexTable[MAX_WORDS];
int wordCount = 0;  // 单词总数
char allFiles[MAX_FILES][100];
int fileCount = 0;

// 查找单词在索引表中的位置
int findWord(char* word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(indexTable[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

// 添加单词到索引表
int addWord(char* word) {
    strcpy(indexTable[wordCount].word, word);
    indexTable[wordCount].docCount = 0;
    return wordCount++;
}

// 添加文档到单词
void addDocToWord(char* word, char* fname) {
    int idx = findWord(word);
    if (idx == -1) {
        idx = addWord(word);
    }
    
    // 检查是否已存在
    for (int i = 0; i < indexTable[idx].docCount; i++) {
        if (strcmp(indexTable[idx].docs[i], fname) == 0) {
            return;
        }
    }
    
    strcpy(indexTable[idx].docs[indexTable[idx].docCount], fname);
    indexTable[idx].docCount++;
}

// 处理一个文件
void indexFile(char* fname) {
    FILE* fp = fopen(fname, "r");
    if (!fp) return;
    
    char word[100];
    while (fscanf(fp, "%s", word) != EOF) {
        addDocToWord(word, fname);
    }
    fclose(fp);
    printf("  已索引: %s\n", fname);
}

// 搜索
void search(char* word) {
    int idx = findWord(word);
    if (idx == -1) {
        printf("未找到\n");
        return;
    }
    
    printf("找到: ");
    for (int i = 0; i < indexTable[idx].docCount; i++) {
        printf("%s ", indexTable[idx].docs[i]);
    }
    printf("\n");
}

int main() {
    DIR* dir = opendir(".");
    struct dirent* entry;
    
    printf("=== MARS搜索引擎 ===\n扫描中...\n");
    
    while ((entry = readdir(dir))) {
        char* dot = strrchr(entry->d_name, '.');
        if (dot && strcmp(dot, ".txt") == 0) {
            strcpy(allFiles[fileCount++], entry->d_name);
            indexFile(entry->d_name);
        }
    }
    closedir(dir);
    
    printf("\n共索引 %d 个文件, %d 个单词\n", fileCount, wordCount);
    
    char word[50];
    while (1) {
        printf("\n请输入关键词(exit退出): ");
        scanf("%s", word);
        if (strcmp(word, "exit") == 0) break;
        search(word);
    }
    
    return 0;
}