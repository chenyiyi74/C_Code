#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10000  // 最大结点数

int main() {
    int N, M;
    printf("请输入结点数和边数：");
    scanf("%d %d", &N, &M);

    // 动态分配邻接矩阵
    int indegree[MAX_NODES] = {0};

    // 读取边并统计入度
    for (int i = 0; i < M; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        indegree[v]++;
    }

    // 构造结点数组
    typedef struct {
        int id;
        int indeg;
    } Node;

    Node nodes[MAX_NODES];
    for (int i = 1; i <= N; i++) {
        nodes[i-1].id = i;
        nodes[i-1].indeg = indegree[i];
    }

    // 按入度降序排序
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - i - 1; j++) {
            if (nodes[j].indeg < nodes[j+1].indeg) {
                Node temp = nodes[j];
                nodes[j] = nodes[j+1];
                nodes[j+1] = temp;
            }
        }
    }

    // 输出前10个大V
    printf("\n入度排名前10的大V结点：\n");
    int top = (N < 10) ? N : 10;
    for (int i = 0; i < top; i++) {
        printf("结点 %d 的入度：%d\n", nodes[i].id, nodes[i].indeg);
    }

    return 0;
}