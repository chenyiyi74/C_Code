#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 10000
#define TABLE_SIZE 10007

// ================== 电话记录 ==================
typedef struct {
    int id;
    char name[30];
    char address[50];
    char phone[20];
} Record;

// ================== 链地址法 ==================
typedef struct Node {
    Record data;
    struct Node* next;
} Node;

// ================== 哈希表 ==================
Node* hashTable_chain[TABLE_SIZE];
Record hashTable_linear[TABLE_SIZE];
int used[TABLE_SIZE];

// ================== 哈希函数1：DJB2（字符串） ==================
int hash_djb2(const char* str) {
    unsigned long hash = 5381;
    while (*str)
        hash = ((hash << 5) + hash) + (*str++);
    return hash % TABLE_SIZE;
}

// ================== 哈希函数2：手机号简单取模 ==================
int hash_phone(const char* phone) {
    long long sum = 0;
    for (int i = 0; phone[i]; i++)
        sum = (sum * 10 + (phone[i] - '0')) % TABLE_SIZE;
    return sum % TABLE_SIZE;
}

// ================== 初始化 ==================
void init() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable_chain[i] = NULL;
        used[i] = 0;
    }
}

// ================== 插入（链地址） ==================
void insert_chain(Record r) {
    int index = hash_djb2(r.name);

    Node* node = (Node*)malloc(sizeof(Node));
    node->data = r;
    node->next = hashTable_chain[index];
    hashTable_chain[index] = node;
}

// ================== 插入（线性探测） ==================
void insert_linear(Record r) {
    int index = hash_djb2(r.name);

    while (used[index])
        index = (index + 1) % TABLE_SIZE;

    hashTable_linear[index] = r;
    used[index] = 1;
}

// ================== 查找（链地址） ==================
int search_chain(char* key, int* cmp) {
    int index = hash_djb2(key);
    Node* p = hashTable_chain[index];

    while (p) {
        (*cmp)++;
        if (strcmp(p->data.name, key) == 0)
            return 1;
        p = p->next;
    }
    return 0;
}

// ================== 查找（线性探测） ==================
int search_linear(char* key, int* cmp) {
    int index = hash_djb2(key);

    while (used[index]) {
        (*cmp)++;
        if (strcmp(hashTable_linear[index].name, key) == 0)
            return 1;
        index = (index + 1) % TABLE_SIZE;
    }
    return 0;
}

// ================== 读取文件 ==================
int load_file(Record arr[]) {
    FILE* fp = fopen("input.dat", "r");
    if (!fp) {
        printf("❌ 文件打开失败！请检查 input.dat 是否在同目录\n");
        return 0;
    }

    int n = 0;
    while (fscanf(fp, "%d %s %s %s",
        &arr[n].id,
        arr[n].name,
        arr[n].address,
        arr[n].phone) == 4) {
        n++;
    }

    fclose(fp);

    printf("✔ 成功读取 %d 条数据\n", n);
    return n;
}

// ================== ASL分析 ==================
void test_asl(Record arr[], int n) {
    int total_chain = 0, total_linear = 0;

    for (int i = 0; i < n; i++) {
        int c1 = 0, c2 = 0;

        search_chain(arr[i].name, &c1);
        search_linear(arr[i].name, &c2);

        total_chain += c1;
        total_linear += c2;
    }

    printf("\n===== ASL 性能分析 =====\n");
    printf("链地址法 ASL = %.2f\n", (double)total_chain / n);
    printf("线性探测 ASL = %.2f\n", (double)total_linear / n);
}

// ================== 主函数 ==================
int main() {
    Record arr[MAXN];

    init();

    int n = load_file(arr);
    if (n == 0) {
        printf("程序结束\n");
        return 0;
    }

    // 建表
    for (int i = 0; i < n; i++) {
        insert_chain(arr[i]);
        insert_linear(arr[i]);
    }

    printf("\n请输入要查询的用户名：");

    char key[30];
    scanf("%s", key);

    int cmp = 0;

    if (search_chain(key, &cmp))
        printf("✔ 链地址法：找到（比较次数=%d)\n", cmp);
    else
        printf("❌ 链地址法：未找到\n");

    cmp = 0;

    if (search_linear(key, &cmp))
        printf("✔ 线性探测：找到（比较次数=%d)\n", cmp);
    else
        printf("❌ 线性探测：未找到\n");

    test_asl(arr, n);

    printf("\n程序结束\n");
    return 0;
}