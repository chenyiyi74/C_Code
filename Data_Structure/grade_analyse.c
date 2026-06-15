#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "input.dat"

// ===== 学生结构体 =====
typedef struct Student {
    int id;
    char name[20];
    float c, math, english;
    float avg;
    struct Student *next;
} Student;

// ===== 创建节点 =====
Student* create(int id, char name[], float c, float m, float e) {
    Student *p = (Student*)malloc(sizeof(Student));
    p->id = id;
    strcpy(p->name, name);
    p->c = c;
    p->math = m;
    p->english = e;
    p->avg = (c + m + e) / 3.0;
    p->next = NULL;
    return p;
}

// ===== 插入 =====
Student* insert(Student *head, Student *node) {
    if (!head) return node;
    Student *p = head;
    while (p->next) p = p->next;
    p->next = node;
    return head;
}

// ===== 复制链表 =====
Student* copyList(Student *head) {
    Student *newHead = NULL;
    for (Student *p = head; p; p = p->next) {
        newHead = insert(newHead,
                         create(p->id, p->name, p->c, p->math, p->english));
    }
    return newHead;
}

// ===== 读取文件 =====
Student* load() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) {
        printf("文件打开失败\n");
        return NULL;
    }

    Student *head = NULL;
    char line[200];

    fgets(line, sizeof(line), fp); // 跳过表头

    int id;
    char name[20];
    float c, m, e;

    while (fscanf(fp, "%d %s %f %f %f", &id, name, &c, &m, &e) == 5) {
        head = insert(head, create(id, name, c, m, e));
    }

    fclose(fp);
    return head;
}

// ===== 排序 =====
Student* sortBy(Student *head, int type) {
    for (Student *i = head; i; i = i->next) {
        for (Student *j = i->next; j; j = j->next) {

            int flag = 0;

            if (type == 1 && i->c < j->c) flag = 1;
            if (type == 2 && i->math < j->math) flag = 1;
            if (type == 3 && i->english < j->english) flag = 1;
            if (type == 4 && i->avg < j->avg) flag = 1;

            if (flag) {
                Student tmp = *i;
                *i = *j;
                *j = tmp;

                Student *t = i->next;
                i->next = j->next;
                j->next = t;
            }
        }
    }
    return head;
}

// ===== 排序 =====
void sortAndSave(Student *head, int type, char *filename) {

    Student *temp = copyList(head);
    temp = sortBy(temp, type);

    FILE *fp = fopen(filename, "w");

    fprintf(fp, "%-6s %-10s %-6s %-6s %-6s %-6s\n",
            "ID","Name","C","Math","Eng","Avg");

    for (Student *p = temp; p; p = p->next) {
        fprintf(fp, "%-6d %-10s %-6.2f %-6.2f %-6.2f %-6.2f\n",
                p->id, p->name, p->c, p->math, p->english, p->avg);
    }

    fclose(fp);

    printf("%s 已生成\n", filename);
}

// ===== 统计 =====
void stats(Student *head) {
    if (!head) {
        printf("暂无数据\n");
        return;
    }

    // ===== 每门课统计变量 =====
    float sc=0, sm=0, se=0;
    float maxc=-1, maxm=-1, maxe=-1;
    float minc=101, minm=101, mine=101;

    int n=0;

    int c_fail=0,c_60=0,c_70=0,c_80=0,c_90=0;
    int m_fail=0,m_60=0,m_70=0,m_80=0,m_90=0;
    int e_fail=0,e_60=0,e_70=0,e_80=0,e_90=0;

    for (Student *p=head; p; p=p->next) {
        float c = p->c, m = p->math, e = p->english;

        // ===== 累加平均 =====
        sc += c;
        sm += m;
        se += e;

        // ===== 最大最小 =====
        if (c > maxc) maxc = c;
        if (m > maxm) maxm = m;
        if (e > maxe) maxe = e;

        if (c < minc) minc = c;
        if (m < minm) minm = m;
        if (e < mine) mine = e;

        // ===== C统计 =====
        if (c < 60) c_fail++;
        else if (c < 70) c_60++;
        else if (c < 80) c_70++;
        else if (c < 90) c_80++;
        else c_90++;

        // ===== Math统计 =====
        if (m < 60) m_fail++;
        else if (m < 70) m_60++;
        else if (m < 80) m_70++;
        else if (m < 90) m_80++;
        else m_90++;

        // ===== English统计 =====
        if (e < 60) e_fail++;
        else if (e < 70) e_60++;
        else if (e < 80) e_70++;
        else if (e < 90) e_80++;
        else e_90++;

        n++;
    }

    printf("\n========== 课程统计 ==========\n");

    printf("\n----- C语言 -----\n");
    printf("平均分: %.2f 最高分: %.2f 最低分: %.2f\n", sc/n, maxc, minc);
    printf("不及格:%d 60-69:%d 70-79:%d 80-89:%d 90+:%d\n",
           c_fail, c_60, c_70, c_80, c_90);

    printf("\n----- Math -----\n");
    printf("平均分: %.2f 最高分: %.2f 最低分: %.2f\n", sm/n, maxm, minm);
    printf("不及格:%d 60-69:%d 70-79:%d 80-89:%d 90+:%d\n",
           m_fail, m_60, m_70, m_80, m_90);

    printf("\n----- English -----\n");
    printf("平均分: %.2f 最高分: %.2f 最低分: %.2f\n", se/n, maxe, mine);
    printf("不及格:%d 60-69:%d 70-79:%d 80-89:%d 90+:%d\n",
           e_fail, e_60, e_70, e_80, e_90);
}

// ===== 显示 =====
void show(Student *head) {
    printf("\n%-6s %-10s %-6s %-6s %-6s %-6s\n",
           "ID","Name","C","Math","Eng","Avg");

    for (Student *p=head; p; p=p->next) {
        printf("%-6d %-10s %-6.2f %-6.2f %-6.2f %-6.2f\n",
               p->id, p->name, p->c, p->math, p->english, p->avg);
    }
}

// ===== 查找 =====
void search(Student *head, char name[], int id) {
    int found = 0;

    for (Student *p=head; p; p=p->next) {
        if ((id!=-1 && p->id==id) ||
            (strlen(name)>0 && strcmp(p->name,name)==0)) {

            printf("%d %s C:%.1f M:%.1f E:%.1f Avg:%.1f\n",
                   p->id,p->name,p->c,p->math,p->english,p->avg);
            found = 1;
        }
    }

    if (!found) printf("未找到\n");
}

// ===== 菜单 =====
void menu() {
    printf("\n========= 成绩系统 =========\n");
    printf("1 读取文件\n");
    printf("2 生成以C语言成绩的排序文件\n");
    printf("3 生成以Math成绩的排序文件\n");
    printf("4 生成以English成绩的排序文件\n");
    printf("5 生成以平均分的排序文件\n");
    printf("6 统计总体数据\n");
    printf("7 显示全部成绩\n");
    printf("8 查找学生成绩\n");
    printf("0 退出\n");
}

int main() {
    Student *head = NULL;
    int op;

    while (1) {
        menu();
        printf("选择:");
        scanf("%d",&op);

        if (op == 0) break;

        else if (op == 1) head = load();

        else if (op == 2) sortAndSave(head, 1, "c_sort.dat");

        else if (op == 3) sortAndSave(head, 2, "math_sort.dat");

        else if (op == 4) sortAndSave(head, 3, "eng_sort.dat");

        else if (op == 5) sortAndSave(head, 4, "avg_sort.dat");

        else if (op == 6) stats(head);

        else if (op == 7) show(head);

        else if (op == 8) {
            char name[20]="";
            int id;
            printf("id(-1忽略):");
            scanf("%d",&id);
            printf("name(无输入0):");
            scanf("%s",name);
            if (strcmp(name,"0")==0) name[0]='\0';
            search(head,name,id);
        }
    }

    return 0;
}