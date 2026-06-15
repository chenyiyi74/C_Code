#include <stdio.h>
#include <string.h>
#include <ctype.h>
// 加密函数
void caesar_enc(char m[], int k, char c[]) {
    int i;
    for (i = 0; m[i] != '\0'; i++) {
        if (isalpha(m[i])) {
            char base = isupper(m[i]) ? 'A' : 'a';
            c[i] = (m[i] - base + k) % 26 + base;
        } else {
            c[i] = m[i];
        }
    }
    c[i] = '\0';
}
// 解密函数
void caesar_dec(char c[], int k, char m[]) {
    int i;
    for (i = 0; c[i] != '\0'; i++) {
        if (isalpha(c[i])) {
            char base = isupper(c[i]) ? 'A' : 'a';
            m[i] = (c[i] - base - k + 26) % 26 + base;
        } else {
            m[i] = c[i];
        }
    }
    m[i] = '\0';
}

int main() {
    char text[200], result[200];
    int k;
    printf("凯撒密码:\n");
    printf("请输入消息: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    printf("请输入密钥(整数): ");
    scanf("%d", &k);
    getchar();
    caesar_enc(text, k, result);
    printf("加密结果: %s\n", result);
    caesar_dec(result, k, result);
    printf("解密结果: %s\n", result);
    return 0;
}