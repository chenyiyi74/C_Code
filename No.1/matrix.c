#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_col_order(char key[], int order[]) {
    int len = strlen(key);
    for (int i = 0; i < len; i++) {
        order[i] = key[i] - '0' - 1;
    }
}
// 加密函数
void mat_enc(char m[], char k[], char c[]) {
    int keylen = strlen(k);
    int len = strlen(m);
    int rows = (len + keylen - 1) / keylen;
    char matrix[rows][keylen];
    int idx = 0;
    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < keylen; col++) {
            if (idx < len)
                matrix[r][col] = m[idx++];
            else
                matrix[r][col] = 'X';
        }
    }
    int order[keylen];
    get_col_order(k, order);
    idx = 0;
    for (int i = 0; i < keylen; i++) {
        int col = order[i];
        for (int r = 0; r < rows; r++) {
            c[idx++] = matrix[r][col];
        }
    }
    c[idx] = '\0';
}
// 解密函数
void mat_dec(char c[], char k[], char m[]) {
    int keylen = strlen(k);
    int len = strlen(c);
    int rows = len / keylen;
    int order[keylen];
    get_col_order(k, order);
    char matrix[rows][keylen];
    int idx = 0;
    for (int i = 0; i < keylen; i++) {
        int col = order[i];
        for (int r = 0; r < rows; r++) {
            matrix[r][col] = c[idx++];
        }
    }
    idx = 0;
    for (int r = 0; r < rows; r++) {
        for (int col = 0; col < keylen; col++) {
            m[idx++] = matrix[r][col];
        }
    }
    m[idx] = '\0';
    int last = strlen(m) - 1;
    while (last >= 0 && m[last] == 'X') {
        m[last] = '\0';
        last--;
    }
}
int main() {
    char text[200], key[100], result[200];
    printf("矩阵换位密码\n");
    printf("请输入消息: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    printf("请输入密钥: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;

    mat_enc(text, key, result);
    printf("加密结果: %s\n", result);
    mat_dec(result, key, result);
    printf("解密结果: %s\n", result);
    return 0;
}