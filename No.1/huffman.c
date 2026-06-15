#include <stdio.h>
#include <stdlib.h>
#include <string.h>`

//固定二进制三位编码
const char* fixedCode(char c) {
    switch (c) {
        case 'A': return "000";
        case 'B': return "001";
        case 'C': return "010";
        case 'D': return "011";
        case 'E': return "100";
        default: return NULL;
    }
}

//哈夫曼编码
const char* huffmanCode(char c) {
    switch (c) {
        case 'A': return "1000";
        case 'B': return "101";
        case 'C': return "0";
        case 'D': return "11";
        case 'E': return "1001";
        default: return NULL;
    }
}

int main(int argc, char* argv[]) {
    char inFile[256];
    char outFile[256];
    
    // 获取输入文件名
    if (argc >= 2) {
        strcpy(inFile, argv[1]);
    } else {
        printf("请输入要压缩的文件名: ");
        scanf("%s", inFile);
    }
    
    // 生成输出文件名
    size_t len = strlen(inFile);
    if (len > 4) {
        strncpy(outFile, inFile, len - 4);
        outFile[len - 4] = '\0';
        strcat(outFile, ".zipe");
    } else {
        // 输入文件名太短，直接加.zipe
        strcpy(outFile, inFile);
        strcat(outFile, ".zipe");
    }
    
    FILE* in = fopen(inFile, "r");
    if (!in) {
        printf("无法打开文件: %s\n", inFile);
        return 1;
    }
    
    // 读取文件内容
    fseek(in, 0, SEEK_END);
    long fileSize = ftell(in);
    rewind(in);
    
    char* content = (char*)malloc(fileSize + 1);
    fread(content, 1, fileSize, in);
    content[fileSize] = '\0';
    fclose(in);
    
    printf("原文件内容: %s\n", content);
    printf("原文件字符数: %ld 个\n", fileSize);
    
    char* originalBinary = (char*)malloc(fileSize * 4 + 1);
    originalBinary[0] = '\0';
    
    for (int i = 0; i < fileSize; i++) {
        const char* code = fixedCode(content[i]);
        if (code) {
            strcat(originalBinary, code);
        }
    }
    printf("原始二进制码:\n%s\n", originalBinary);
    printf("原始二进制码位数: %zu 位\n", strlen(originalBinary));
    printf("原始二进制码字节数: %.1f 字节\n\n", strlen(originalBinary) / 8.0);
    //哈夫曼压缩
    char* compressedBinary = (char*)malloc(fileSize * 5 + 1);
    compressedBinary[0] = '\0';
    
    for (int i = 0; i < fileSize; i++) {
        const char* code = huffmanCode(content[i]);
        if (code) {
            strcat(compressedBinary, code);
        }
    }
    printf("压缩后二进制码:\n%s\n", compressedBinary);
    printf("压缩后二进制码位数: %zu 位\n", strlen(compressedBinary));
    printf("压缩后二进制码字节数: %.1f 字节\n\n", strlen(compressedBinary) / 8.0);
    
    //写入输出文件
    FILE* out = fopen(outFile, "w");
    fprintf(out, "%s", compressedBinary);
    fclose(out);
    
    //压缩率计算
    long originalBits = strlen(originalBinary);
    long compressedBits = strlen(compressedBinary);
    float compressionRate = (float)compressedBits / originalBits * 100;
    float savedSpace = 100 - compressionRate;
    
    printf("原始编码位数: %ld 位\n", originalBits);
    printf("哈夫曼编码位数: %ld 位\n", compressedBits);
    printf("压缩率: %.2f%% \n", compressionRate);
    printf("节省空间: %.2f%%\n", savedSpace);
    printf("压缩文件已保存为: %s\n", outFile);
    
    free(content);
    free(originalBinary);
    free(compressedBinary);
    
    return 0;
}