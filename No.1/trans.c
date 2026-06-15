#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    RGBQUAD palette[256];
    unsigned char *pData, *pNewData;
    FILE *fp;
    int width, height, lineByte, newLineByte, i, j;

    fp = fopen("flower.bmp", "rb");
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
    fread(palette, sizeof(RGBQUAD), 256, fp);

    width = bi.biWidth;
    height = bi.biHeight;
    lineByte = ((width * 8 + 31) / 32) * 4;
    pData = (unsigned char*)malloc(lineByte * height);
    fread(pData, 1, lineByte * height, fp);
    fclose(fp);

    // 水平镜像
    pNewData = (unsigned char*)malloc(lineByte * height);
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            pNewData[i * lineByte + (width - 1 - j)] = pData[i * lineByte + j];
    fp = fopen("flower_horizontal.bmp", "wb");
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(palette, sizeof(RGBQUAD), 256, fp);
    fwrite(pNewData, 1, lineByte * height, fp);
    fclose(fp);

    // 垂直镜像
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            pNewData[(height - 1 - i) * lineByte + j] = pData[i * lineByte + j];
    fp = fopen("flower_vertical.bmp", "wb");
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(palette, sizeof(RGBQUAD), 256, fp);
    fwrite(pNewData, 1, lineByte * height, fp);
    fclose(fp);

    // 转置
    int newWidth = height, newHeight = width;
    newLineByte = ((newWidth * 8 + 31) / 32) * 4;
    unsigned char *pTransData = (unsigned char*)malloc(newLineByte * newHeight);
    for (i = 0; i < height; i++)
        for (j = 0; j < width; j++)
            pTransData[j * newLineByte + i] = pData[i * lineByte + j];
    bi.biWidth = newWidth;
    bi.biHeight = newHeight;
    bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + newLineByte * newHeight;
    fp = fopen("flower_transpose.bmp", "wb");
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
    fwrite(palette, sizeof(RGBQUAD), 256, fp);
    fwrite(pTransData, 1, newLineByte * newHeight, fp);
    fclose(fp);

    free(pData);
    free(pNewData);
    free(pTransData);
    printf("图片生成完毕！");
    return 0;
}