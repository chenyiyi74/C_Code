#include <stdio.h>
#include <string.h>
#include <ctype.h>
// 加密函数
void vige_enc(char m[], char k[], char c[]) {
    int i, j = 0;
    int klen = strlen(k);
    for (i = 0; m[i] != '\0'; i++) {
        if (isalpha(m[i])) {
            char base = isupper(m[i]) ? 'A' : 'a';
            char kbase = isupper(k[j % klen]) ? 'A' : 'a';
            int shift = (k[j % klen] - kbase) % 26;
            c[i] = (m[i] - base + shift) % 26 + base;
            j++;
        } else {
            c[i] = m[i];
        }
    }
    c[i] = '\0';
}
// 解密函数
void vige_dec(char c[], char k[], char m[]) {
    int i, j = 0;
    int klen = strlen(k);
    for (i = 0; c[i] != '\0'; i++) {
        if (isalpha(c[i])) {
            char base = isupper(c[i]) ? 'A' : 'a';
            char kbase = isupper(k[j % klen]) ? 'A' : 'a';
            int shift = (k[j % klen] - kbase) % 26;
            m[i] = (c[i] - base - shift + 26) % 26 + base;
            j++;
        } else {
            m[i] = c[i];
        }
    }
    m[i] = '\0';
}
int main() {
    char text[200], key[100], result[200];
    printf("维吉尼亚密码\n");
    printf("请输入消息: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = 0;
    printf("请输入密钥: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;
    vige_enc(text, key, result);
    printf("加密结果: %s\n", result);
    vige_dec(result, key, result);
    printf("解密结果: %s\n", result);
    return 0;
}