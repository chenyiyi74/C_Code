#include <stdio.h>
#include <string.h>

#define START_YEAR 1900
#define END_YEAR 2001

typedef struct
{
    int y,m,d;
}Date;

int dp[2002][13][32];

int isLeap(int y)
{
    return (y%400==0)||(y%4==0&&y%100!=0);
}

int daysInMonth(int y,int m)
{
    int day[13]={
        0,
        31,28,31,30,31,30,
        31,31,30,31,30,31
    };

    if(m==2&&isLeap(y))
        return 29;

    return day[m];
}

int cmp(Date a,Date b)
{
    if(a.y!=b.y) return a.y-b.y;
    if(a.m!=b.m) return a.m-b.m;
    return a.d-b.d;
}

Date nextDay(Date cur)
{
    cur.d++;

    if(cur.d>daysInMonth(cur.y,cur.m))
    {
        cur.d=1;
        cur.m++;

        if(cur.m>12)
        {
            cur.m=1;
            cur.y++;
        }
    }

    return cur;
}

Date nextMonth(Date cur)
{
    int ny=cur.y;
    int nm=cur.m+1;

    if(nm>12)
    {
        nm=1;
        ny++;
    }

    if(cur.d<=daysInMonth(ny,nm))
    {
        Date t={ny,nm,cur.d};
        return t;
    }

    return nextDay(cur);
}

void build()
{
    Date end={2001,11,4};

    memset(dp,0,sizeof(dp));

    dp[2001][11][4]=0;

    for(int y=2001;y>=1900;y--)
    {
        for(int m=12;m>=1;m--)
        {
            for(int d=31;d>=1;d--)
            {
                if(d>daysInMonth(y,m))
                    continue;

                if(y==2001&&m==11&&d==4)
                    continue;

                Date cur={y,m,d};

                if(cmp(cur,end)>0)
                    continue;

                Date n1=nextDay(cur);
                Date n2=nextMonth(cur);

                int win=0;

                if(cmp(n1,end)<=0)
                {
                    if(dp[n1.y][n1.m][n1.d]==0)
                        win=1;
                }

                if(!win&&cmp(n2,end)<=0)
                {
                    if(dp[n2.y][n2.m][n2.d]==0)
                        win=1;
                }

                dp[y][m][d]=win;
            }
        }
    }
}

int main()
{
    build();

    printf("请输入日期（格式：年 月 日）:\n");
    int y,m,d;

    while(scanf("%d%d%d",&y,&m,&d)==3)
    {
        if(dp[y][m][d])
            printf("YES\n");
        else
            printf("NO\n");
    }

    return 0;
}