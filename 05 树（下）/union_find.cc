#include <iostream>
#include <algorithm>
#include <unordered_set>
using namespace std;

// ## 1114 Family Property (25 分)
// dfs判断子连通图会超时。

int disjoint[10000];
// int rank[10000]; // 这里不使用按秩合并。而是按id的大小来的合并，因为题目要求输出每一个集合中最小的id。

void make_set() {
    for (int i=0; i<10000; i++) {
        disjoint[i] = i; // 初始时，每个结点是一个帮派/集合。
        // rank[i] = 0;
    }
}

// 查，返回i所属集合/帮派的头目。
int find(int i) {
    if (disjoint[i] != i)
        disjoint[i] = find(disjoint[i]); // 递归，通过i的上级找头目。对disjoint[i]重新赋值，实现路径压缩。
    return disjoint[i];
}

// 并，将x和y所属的两个集合并为一个集合，即将一个集合的头目作为另一个集合的头目的手下。
void union_(int x, int y) {
    int a = find(x);
    int b = find(y);
    if (a < b) // id更小的做老大。
        disjoint[b] = a;
    else
        disjoint[a] = b;
}

/*
void union_(int x, int y) {
    int a = find(x);
    int b = find(y);
    if (rank[a] > rank[b]) // 按秩合并。
        disjoint[b] = a;
    else {
        disjoint[a] = b;
		if (rank[a] == rank[b])
			rank[b]++;
	}
}
*/

struct Node {
    int id, M, A;
};

Node a[1000]; // 存储输入的N个有权值的结点。无权值的结点只需记录在disjoint中即可。

struct Result {
    int id, N, totalM, totalA;
    double avgM, avgA;
    bool flag; // 标记Result数组中的某个slot是否被使用。
};

Result r[10000]; // 默认初始化，以id作为下标。

int N, id, x, y, i, nChilds, M, A, nSet;

// 自动去重。
unordered_set<int> ids; // 记录输入数据的所有id，包括没有权值的结点的id。

bool cmp(const Result& a, const Result& b) {
    if (a.avgA != b.avgA) return a.avgA>b.avgA;
    return a.id<b.id;
}

int main () {
    make_set(); // XXX 记得在main中调用初始化函数。
    scanf("%d", &N);
    for (i=0; i<N; i++) {
        scanf("%d %d %d %d", &id, &x, &y, &nChilds);
        ids.insert(id);
        if (x != -1) {
            union_(x, id);
            ids.insert(x);
        }
        if (y != -1) {
            union_(y, id);
            ids.insert(y);
        }
        while (nChilds--) {
            scanf("%d", &x);
            union_(x, id);
            ids.insert(x);
        }
        scanf("%d %d", &a[i].M, &a[i].A);
        a[i].id = id;
    }
    // 累加每个集合的权值。
    for (i=0; i<N; i++) {
        id = find(a[i].id);
        r[id].id = id;
        r[id].totalM += a[i].M;
        r[id].totalA += a[i].A;
        r[id].flag = true;
        // r[id].N++; // 这里累计的只是有权值的结点。
    }
    // 累计每个集合的结点数。
    for (auto it=ids.begin(); it!=ids.end(); it++)
        r[find(*it)].N++;
    // 累计集合数，并且每个集合的大小已求出，可以求平均值了。
    for (i=0; i<10000; i++) {
        if (r[i].flag) {
            r[i].avgM = r[i].totalM*1.0/r[i].N;
            r[i].avgA = r[i].totalA*1.0/r[i].N;
            nSet++;
        }
    }
    sort(r, r+10000, cmp);
    printf("%d\n", nSet);
    for (i=0; i<nSet; i++)
        printf("%04d %d %.3lf %.3lf\n", r[i].id, r[i].N, r[i].avgM, r[i].avgA);
}