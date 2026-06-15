#include <stdio.h>
#include <string.h>

#define MAX 20

int n;                      // 顶点数
int edge[MAX][MAX];         // 邻接矩阵
int earliest[MAX];          // 最早发生时间
int latest[MAX];            // 最晚发生时间
int inDegree[MAX];          // 入度

// 拓扑排序并计算最早时间
int topologicalSort(int order[]) {
    int queue[MAX], front = 0, rear = 0;
    int count = 0;
    
    // 找入度为0的顶点
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            queue[rear++] = i;
        }
    }
    
    while (front < rear) {
        int v = queue[front++];
        order[count++] = v;
        
        for (int i = 0; i < n; i++) {
            if (edge[v][i] > 0) {
                // 更新最早时间
                if (earliest[v] + edge[v][i] > earliest[i]) {
                    earliest[i] = earliest[v] + edge[v][i];
                }
                if (--inDegree[i] == 0) {
                    queue[rear++] = i;
                }
            }
        }
    }
    return count == n;
}

int main() {
    // 初始化
    n = 8;
    memset(edge, 0, sizeof(edge));
    memset(earliest, 0, sizeof(earliest));
    memset(inDegree, 0, sizeof(inDegree));
    
    // 添加边和权重
    edge[0][1] = 2; inDegree[1]++;
    edge[0][3] = 1; inDegree[3]++;
    edge[1][2] = 3; inDegree[2]++;
    edge[2][4] = 2; inDegree[4]++;
    edge[2][5] = 1; inDegree[5]++;
    edge[3][6] = 1; inDegree[6]++;
    edge[4][7] = 1; inDegree[7]++;
    edge[5][7] = 1; inDegree[7]++;
    edge[6][7] = 1; inDegree[7]++;
    
    int order[MAX];
    if (!topologicalSort(order)) {
        printf("图中有环！\n");
        return 1;
    }
    
    // 初始化最晚时间
    for (int i = 0; i < n; i++) {
        latest[i] = earliest[order[n-1]];
    }
    
    // 逆向计算最晚时间
    for (int i = n - 1; i >= 0; i--) {
        int v = order[i];
        for (int j = 0; j < n; j++) {
            if (edge[v][j] > 0) {
                if (latest[j] - edge[v][j] < latest[v]) {
                    latest[v] = latest[j] - edge[v][j];
                }
            }
        }
    }
    
    // 输出关键路径
    printf("小型家庭宴会关键路径分析:\n");
    printf("\n关键路径: ");
    int v = 0;
    while (v != 7) {
        printf("%c", 'A' + v);
        for (int i = 0; i < n; i++) {
            if (edge[v][i] > 0 && earliest[v] == latest[v] && 
                earliest[i] == latest[i] && 
                earliest[v] + edge[v][i] == earliest[i]) {
                printf(" -> ");
                v = i;
                break;
            }
        }
    }
    printf("%c\n", 'A' + v);
    printf("总筹备时间: %d 天\n", earliest[7]);
    
    return 0;
}