#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 100
#define MAXCODE 100
#define INF 9999999

typedef struct {
    char ch;
    int weight;
    int parent, lchild, rchild;
} HTNode;

typedef struct {
    char bits[MAXCODE];
} HCode;

HTNode ht[2 * MAXN];
HCode hcode[MAXN];

int n = 27; // 空格 + A-Z

// ===================== 自动频度表 =====================
void AutoInit() {
    char chars[] = {
        ' ', 'A','B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    int freq[] = {
        186,64,23,22,32,103,21,15,47,57,1,5,32,20,
        20,56,19,2,50,51,55,30,10,11,2,21,2
    };

    for (int i = 1; i <= n; i++) {
        ht[i].ch = chars[i - 1];
        ht[i].weight = freq[i - 1];
        ht[i].parent = ht[i].lchild = ht[i].rchild = 0;
    }
}

// ===================== 找最小两个节点 =====================
void selectMin(int end, int *s1, int *s2) {
    int min1 = INF, min2 = INF;
    *s1 = *s2 = -1;

    for (int i = 1; i <= end; i++) {
        if (ht[i].parent == 0) {
            if (ht[i].weight < min1) {
                min2 = min1; *s2 = *s1;
                min1 = ht[i].weight; *s1 = i;
            } else if (ht[i].weight < min2) {
                min2 = ht[i].weight; *s2 = i;
            }
        }
    }
}

// ===================== 初始化哈夫曼树 =====================
void Init() {
    AutoInit();

    int m = 2 * n - 1;
    int s1, s2;

    for (int i = n + 1; i <= m; i++) {
        ht[i].weight = 0;
        ht[i].parent = ht[i].lchild = ht[i].rchild = 0;
        ht[i].ch = '#';
    }

    for (int i = n + 1; i <= m; i++) {
        selectMin(i - 1, &s1, &s2);
        ht[s1].parent = ht[s2].parent = i;
        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[i].weight = ht[s1].weight + ht[s2].weight;
    }

    // ===== 生成编码 =====
    char cd[MAXCODE];

    for (int i = 1; i <= n; i++) {
        int start = MAXCODE - 1;
        cd[start] = '\0';

        int c = i;
        int p = ht[i].parent;

        while (p != 0) {
            if (ht[p].lchild == c)
                cd[--start] = '0';
            else
                cd[--start] = '1';

            c = p;
            p = ht[p].parent;
        }

        strcpy(hcode[i].bits, &cd[start]);
    }

    FILE *fp = fopen("huffman.dat", "wb");
    fwrite(ht, sizeof(HTNode), m + 1, fp);
    fwrite(hcode, sizeof(HCode), n + 1, fp);
    fclose(fp);

    printf("✔ 哈夫曼树初始化完成\n");
}

// ===================== 读取树 =====================
void Load() {
    FILE *fp = fopen("huffman.dat", "rb");
    if (!fp) {
        printf("❌ 请先执行 I 初始化\n");
        exit(0);
    }
    fread(ht, sizeof(HTNode), 2 * MAXN, fp);
    fread(hcode, sizeof(HCode), MAXN, fp);
    fclose(fp);
}

// ===================== 编码 =====================
void Coding() {
    Load();

    FILE *in = fopen("tobetrans.dat", "r");
    FILE *out = fopen("codefile", "w");

    if (!in) {
        printf("❌ 缺少 tobetrans.dat\n");
        return;
    }

    char text[1000];
    fgets(text, sizeof(text), in);

    for (int i = 0; text[i]; i++) {
        if (text[i] == '\n') continue;

        for (int j = 1; j <= n; j++) {
            if (ht[j].ch == text[i]) {
                fprintf(out, "%s", hcode[j].bits);
                break;
            }
        }
    }

    fclose(in);
    fclose(out);

    printf("✔ 编码完成\n");
}

// ===================== 解码 =====================
void Decoding() {
    Load();

    FILE *in = fopen("codefile", "r");
    FILE *out = fopen("textfile", "w");

    if (!in) {
        printf("❌ 没有codefile\n");
        return;
    }

    char code[10000];
    fscanf(in, "%s", code);

    int root = 2 * n - 1;
    int p = root;

    for (int i = 0; code[i]; i++) {
        if (code[i] == '0') p = ht[p].lchild;
        else p = ht[p].rchild;

        if (!ht[p].lchild && !ht[p].rchild) {
            fputc(ht[p].ch, out);
            p = root;
        }
    }

    fclose(in);
    fclose(out);

    printf("✔ 解码完成\n");
}

// ===================== code打印 =====================
void PrintCode() {
    FILE *fp = fopen("codefile", "r");
    FILE *out = fopen("codeprint", "w");

    if (!fp) {
        printf("❌ 没有codefile\n");
        return;
    }

    char c;
    int cnt = 0;

    while ((c = fgetc(fp)) != EOF) {
        printf("%c", c);
        fprintf(out, "%c", c);

        cnt++;
        if (cnt % 50 == 0) {
            printf("\n");
            fprintf(out, "\n");
        }
    }

    fclose(fp);
    fclose(out);

    printf("\n✔ codefile已打印\n");
}

// ===================== ⭐ 树输出（关键修复） =====================
void PrintTreeToFile(int root, int depth, FILE *fp) {
    if (!root) return;

    PrintTreeToFile(ht[root].rchild, depth + 1, fp);

    for (int i = 0; i < depth; i++)
        fprintf(fp, "    ");

    if (!ht[root].lchild && !ht[root].rchild)
        fprintf(fp, "%c(%d)\n", ht[root].ch, ht[root].weight);
    else
        fprintf(fp, "*(%d)\n", ht[root].weight);

    PrintTreeToFile(ht[root].lchild, depth + 1, fp);
}

// ===================== T功能 =====================
void TreePrint() {
    int root = 2 * n - 1;

    FILE *fp = fopen("treeprint", "w");
    if (!fp) {
        printf("❌ 无法创建treeprint\n");
        return;
    }

    fprintf(fp, "===== Huffman Tree =====\n\n");
    PrintTreeToFile(root, 0, fp);

    fclose(fp);

    printf("✔ treeprint生成成功\n");
}

// ===================== 主函数 =====================
int main() {
    char op;

    while (1) {
        printf("\n====================\n");
        printf("I 初始化\nC 编码\nD 解码\nP 打印code\nT 打印树\nQ 退出\n");
        printf("输入：");

        scanf(" %c", &op);

        switch (op) {
            case 'I': case 'i': Init(); break;
            case 'C': case 'c': Coding(); break;
            case 'D': case 'd': Decoding(); break;
            case 'P': case 'p': PrintCode(); break;
            case 'T': case 't': TreePrint(); break;
            case 'Q': case 'q': return 0;
            default: printf("❌ 输入错误\n");
        }
    }
}