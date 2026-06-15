#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 10000
#define MAX_TEXT_LEN 100000

typedef struct {
    char word[MAX_WORD_LEN];
    int countA;
    int countB;
} WordFreq;

WordFreq vocab[MAX_WORDS];
int vocab_size = 0;

// 转小写
void to_lower(char *str) {
    for (; *str; str++) {
        *str = tolower((unsigned char)*str);
    }
}

// 从文件读取全部文本
int read_text_from_file(const char *filename, char *buffer, int max_len) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("错误：无法打开文件 %s\n", filename);
        return 0;
    }
    int index = 0;
    char c;
    while ((c = fgetc(fp)) != EOF && index < max_len - 1) {
        buffer[index++] = c;
    }
    buffer[index] = '\0';
    fclose(fp);
    return 1;
}

// 分词并统计词频
void tokenize_and_count(const char *text, int isTextA) {
    char copy[MAX_TEXT_LEN];
    strncpy(copy, text, sizeof(copy)-1);
    copy[sizeof(copy)-1] = '\0';

    char *token = strtok(copy, " \n\t,.!?;:'\"()");
    while (token != NULL) {
        to_lower(token);
        int found = -1;

        for (int i = 0; i < vocab_size; i++) {
            if (strcmp(vocab[i].word, token) == 0) {
                found = i;
                break;
            }
        }

        if (found == -1 && vocab_size < MAX_WORDS) {
            strncpy(vocab[vocab_size].word, token, MAX_WORD_LEN - 1);
            vocab[vocab_size].word[MAX_WORD_LEN - 1] = '\0';
            vocab[vocab_size].countA = 0;
            vocab[vocab_size].countB = 0;
            found = vocab_size++;
        }

        if (found != -1) {
            if (isTextA) vocab[found].countA++;
            else vocab[found].countB++;
        }

        token = strtok(NULL, " \n\t,.!?;:'\"()");
    }
}

// 计算余弦相似度
double cosine_similarity() {
    double dot_product = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (int i = 0; i < vocab_size; i++) {
        dot_product += vocab[i].countA * vocab[i].countB;
        normA += vocab[i].countA * vocab[i].countA;
        normB += vocab[i].countB * vocab[i].countB;
    }

    if (normA == 0 || normB == 0) {
        return 0.0;
    }

    return dot_product / (sqrt(normA) * sqrt(normB));
}

int main() {
    char textA[MAX_TEXT_LEN], textB[MAX_TEXT_LEN];
    char fileA[100], fileB[100];

    printf("请输入第一篇文章的文件名（如 short1.txt）：");
    scanf("%s", fileA);
    printf("请输入第二篇文章的文件名（如 short2.txt）：");
    scanf("%s", fileB);

    // 读取文件
    if (!read_text_from_file(fileA, textA, MAX_TEXT_LEN) ||
        !read_text_from_file(fileB, textB, MAX_TEXT_LEN)) {
        return 1;
    }

    // 重置词表
    vocab_size = 0;
    memset(vocab, 0, sizeof(vocab));

    // 分词统计
    tokenize_and_count(textA, 1);
    tokenize_and_count(textB, 0);

    // 计算相似度
    double similarity = cosine_similarity();

    printf("\n===== 相似度计算结果 =====\n");
    printf("文件1：%s\n", fileA);
    printf("文件2：%s\n", fileB);
    printf("余弦相似度：%.4f\n", similarity);
    printf("（范围：0~1，越接近1越相似）\n");

    return 0;
}