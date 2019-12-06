## 06-图1 列出连通集 (25 分)

1. 注意别写错，不然就无限递归同一个顶点了。最后会爆栈，segmentation fault。
2. 注意BFS时，push()时更新相关数据和pop()时更新相关数据的区别。比如visited数组要在push()时更新，否则就可能重复访问某个顶点。
3. vector resize()后不要push_back()，除非这是你期望的。
4. 题目要求总是从编号最小的顶点开始遍历，所以用set自动排序一下，也可以用vector，然后sort。

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

// 无边权，且输入给出了一个顶点邻接的所有顶点，故用邻接表存储，
// 这样能使BFS和DFS的时间复杂度为O(N+E)，若用邻接矩阵，则时间复杂度为O(N^2)。
vector<set<int>> edge;
// 题目要求总是从编号最小的顶点开始遍历，所以用set自动排序一下，也可以用vector，然后sort。

vector<bool> visited;

vector<int> path;

void print() {
    printf("{ %d", path[0]);
    for (int i=1; i<path.size(); i++) {
        printf(" %d", path[i]);
    }
    printf(" }\n");
}

void dfs(int v) {
    visited[v] = true;
    path.push_back(v);

    // 这里的递归终点是`edge[v].empty()`，不过这里没有显式地写出来。
    for (int x: edge[v]) {
        if (!visited[x])
            // dfs(v); // XXX 注意别写错，不然就无限递归同一个顶点了。最后会爆栈，segmentation fault。
            dfs(x);
    }
}

void bfs(int v) {
    queue<int> q;
    q.push(v);
    visited[v] = true; // XXX 注意BFS时，push()时更新相关数据和pop()时更新相关数据的区别。
    while (!q.empty()) {
        v = q.front();
        q.pop();
        path.push_back(v);
        for (int x: edge[v]) {
            if (!visited[x]) {
                q.push(x);
                visited[x] = true;
            }
        }
    }
}

int main() {
    int N, E, i, x, y;
    scanf("%d %d", &N, &E);
    edge.resize(N); // 从0开始编号。
    visited.resize(N);
    for (i=0; i<E; i++) {
        scanf("%d %d", &x, &y);
        edge[x].insert(y);
        edge[y].insert(x);
    }
    // 输入的图可能有多个连通分量，所以要多次调用DFS和BFS。
    fill(visited.begin(), visited.end(), false); // 注意不要在循环中写这一条语句，不然就错了。
    for (i=0; i<N; i++) {
        if (!visited[i]) {
            dfs(i);
            print();
            path.clear();
        }
    }
    fill(visited.begin(), visited.end(), false);
    for (i=0; i<N; i++) {
        if (!visited[i]) {
            bfs(i);
            print();
            path.clear();
        }
    }
}
```


## 06-图2 Saving James Bond - Easy Version (25 分)

1. 注意这道题和课程视频讲解的略微不同，这道题表示(0, 0)是一个半径为7.5的圆地，这使得007从圆地上跳到第一只鳄鱼上时，他的跳跃的最大距离不是D而是D+7.5。
2. 没过测试时，多仔细读题，看看有没有漏了什么。
3. 这里没有存储图，如果查询次数多，或者图比较稠密的话，可能先建立图比较好，可以预先计算出两点之间的距离作为两点的边权。
4. diameter，直径，radius，半径。

```cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<pair<int, int>> points; // 结点从0开始编号，points[i]存储顶点i的坐标。

vector<bool> visited;

int N, D;

bool reachable(int start, int end) {
    int x = sqrt(pow(points[end].first-points[start].first, 2) + pow(points[end].second-points[start].second, 2));
    return x <= D;
}

bool firstReachable(int start, int end) {
    double x = sqrt(pow(points[end].first-points[start].first, 2) + pow(points[end].second-points[start].second, 2));
    return x <= D+7.5;
}

bool dfs(int v) {
    visited[v] = true;

    if (abs(points[v].first)+D>=50 || abs(points[v].second)+D>=50)
        return true;

    bool ans = false;
    for (int i=1; i<=N; i++) {
        if (!visited[i] && reachable(v, i)) { // `reachable(v, i)`相当于看是否有边相连。
            ans = dfs(i);
            if (ans) // 只要有一个可行路径就行了。
                break;
        }
    }
    return ans;
}

int main() {
    int i, x, y;
    scanf("%d %d", &N, &D);
    points.resize(N+1); // (0, 0)是起点。
    visited.resize(N+1);
    for (i=1; i<=N; i++) {
        scanf("%d %d", &x, &y);
        points[i] = pair<int, int>(x, y);
    }
    // 记得初始化。
    points[0] = pair<int, int>(0, 0);
    fill(visited.begin(), visited.end(), false);
    visited[0] = true;
    // 图中可能有多个连通分量，所以需要多次DFS。
    // 顶点0是比较特殊的，放在DFS外处理。
    for (i=1; i<=N; i++) {
        if (!visited[i] && firstReachable(0, i)) {
            // 从第一跳开始DFS，而不是从0开始。
            if (dfs(i)) {
                printf("Yes\n");
                return 0;
            }
        }
    }
    printf("No\n");
}
```


## 06-图3 六度空间 (30 分)

1. 假如给你一个社交网络图，请你对每个节点计算符合“六度空间”理论的结点占结点总数的百分比。**也就是从每个顶点开始，往外遍历6层，求这6层的结点数占图中总结点数的百分比。**
2. 记住层数的BFS。
3. 一般来说，DFS,BFS要用visited数组记录哪些顶点已访问过，避免重复访问某个顶点。
4. 注意BFS时，push()时更新相关数据和pop()时更新相关数据的区别。比如visited数组要在push()时更新，否则就可能重复访问某个顶点。
5. 必要的变量才作为全局变量。少用全局变量，这样才不会无意中使用到全局变量，影响其它函数的执行。

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, M; // 必要的变量才作为全局变量。少用全局变量，这样才不会无意中使用到全局变量，影响其它函数的执行。

vector<vector<int>> edge;

vector<bool> visited;

// 记录层数的BFS。
int bfs(int v) {
    queue<int> q;
    int level=1, n=1, last=v;
    q.push(v);
    visited[v] = true;
    while (!q.empty()) {
        v = q.front();
        q.pop();
        for (int x: edge[v]) {
            if (!visited[x]) {
                q.push(x);
                visited[x] = true;
                n++;
            }
        }
        if (last == v) {
            level++;
            if (level > 6)
                return n;
            last = q.back();
        }
    }
    // 不足6层的话，会执行到这里。
    return n;
}

int main() {
    int i, x, y;
    scanf("%d %d", &N, &M);
    edge.resize(N+1); // 顶点从1开始编号，0号位不使用。
    visited.resize(N+1);
    for (i=0; i<M; i++) {
        scanf("%d %d", &x, &y);
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    for (i=1; i<=N; i++) {
        fill(visited.begin(), visited.end(), false);
        x = bfs(i);
        printf("%d: %.2f%%\n", i, x*100.0/N);
    }
}
```

