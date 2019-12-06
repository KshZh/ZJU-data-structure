## 07-图4 哈利·波特的考试 (25 分)

哈利·波特要考试了，他需要你的帮助。这门课学的是用魔咒将一种动物变成另一种动物的本事。例如将猫变成老鼠的魔咒是haha，将老鼠变成鱼的魔咒是hehe等等。反方向变化的魔咒就是简单地将原来的魔咒倒过来念，例如ahah可以将老鼠变成猫。另外，如果想把猫变成鱼，可以通过念一个直接魔咒lalala，也可以将猫变老鼠、老鼠变鱼的魔咒连起来念：hahahehe。

现在哈利·波特的手里有一本教材，里面列出了所有的变形魔咒和能变的动物。老师允许他自己带一只动物去考场，要考察他把这只动物变成任意一只指定动物的本事。于是他来问你：带什么动物去可以让最难变的那种动物（即该动物变为哈利·波特自己带去的动物所需要的魔咒最长）需要的魔咒最短？例如：如果只有猫、鼠、鱼，则显然哈利·波特应该带鼠去，因为鼠变成另外两种动物都只需要念4个字符；而如果带猫去，则至少需要念6个字符才能把猫变成鱼；同理，带鱼去也不是最好的选择。

输入格式:
输入说明：输入第1行给出两个正整数N (≤100)和M，其中N是考试涉及的动物总数，M是用于直接变形的魔咒条数。为简单起见，我们将动物按1~N编号。随后M行，每行给出了3个正整数，分别是两种动物的编号、以及它们之间变形需要的魔咒的长度(≤100)，数字之间用空格分隔。

输出格式:
输出哈利·波特应该带去考场的动物的编号、以及最长的变形魔咒的长度，中间以空格分隔。如果只带1只动物是不可能完成所有变形要求的，则输出0。如果有若干只动物都可以备选，则输出编号最小的那只。

1. 求解多源最短路径用folyd算法。
2. 求从每个顶点出发的到其它顶点的最短路径中最长的那条，这样就会每个顶点对应一条，取其中最短的那条。
3. 这道题有点意思，题意没有明说这是一个图，需要我们自己转换问题。

```cpp
#include <iostream>
#include <climits>
using namespace std;

// 编号从1开始。
int graph[101][101]; // 默认初始化为0。
int D[101][101];
int N, M;

void folyd() {
    int i, j, k;
    for (i=1; i<=N; i++) {
        for (j=1; j<=N; j++) {
            if (graph[i][j] != 0)
                D[i][j] = graph[i][j];
            else
                D[i][j] = INT_MAX/2; // XXX 不能用INT_MAX，否则下面相加会溢出，从而错误地满足条件。
        }
    }
    // 从D^1到D^N，遍历每一个中继点k。
    // 状态转移方程：D[i][j] = min(D[i][k]+D[k][j], D[i][j])。
    for (k=1; k<=N; k++) {
        for (i=1; i<=N; i++) {
            for (j=1; j<=N; j++) {
                if (D[i][k]+D[k][j] < D[i][j]) {
                    D[i][j] = D[i][k]+D[k][j];
                }
            }
        }
    }
}

int main() {
    int i, x, y, z, j, tempMax, minn=INT_MAX, mini;
    scanf("%d %d", &N, &M);
    for (i=0; i<M; i++) {
        scanf("%d %d %d", &x, &y, &z);
        // 无向图。
        graph[x][y] = z;
        graph[y][x] = z;
    }
    folyd();
    for (i=1; i<=N; i++) {
		// 取i到其它顶点的最短路径中最长的那条。
        tempMax = -1;
        for (j=1; j<=N; j++) {
            if (j!=i && D[i][j]>tempMax)
                tempMax = D[i][j];
        }
        if (tempMax == INT_MAX/2) { // 顶点i到某个顶点不存在最短路径，其实是不存在路径。
            printf("0\n");
            return 0;
        }
        if (tempMax < minn) { // 这个小于号，而不是<=，使得当有多个同样长的路径时，我们只选择编号最小的那条。
            minn = tempMax;
            mini = i;
        }
    }
    printf("%d %d\n", mini, minn);
}
```


## 07-图5 Saving James Bond - Hard Version (30 分)

1. diameter，直径。
2. 单源无权最短路径，可以用DFS或BFS，首选BFS。
3. BFS可以记录顶点的前驱，然后用DFS回溯得到完整最短路径。
4. 一步到达岸边这个测试点真没想到。
5. 可能有多个起点，扫描一遍，把这些起点都入队。
6. If there are many shortest paths, just output the one with the minimum first jump, which is guaranteed to be unique. 所以先对输入序列排序，在BFS/DFS。

```cpp
#include <iostream>
#include <climits>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

vector<pair<int, int>> vertexes;
vector<bool> visited;
vector<int> shortestPath;
vector<int> path;

int N, D, n, minN=INT_MAX;

double distanceOfTwoPoint(int u, int v) {
    return sqrt(pow(vertexes[u].first-vertexes[v].first, 2)+pow(vertexes[u].second-vertexes[v].second, 2));
}

bool firstReachable(int v) {
    return distanceOfTwoPoint(0, v) <= D+7.5;
}

bool reachable(int u, int v) {
    return distanceOfTwoPoint(u, v) <= D;
}

void dfs(int v) {
    visited[v] = true;
    path.push_back(v);
    if (abs(vertexes[v].first)+D>=50 || abs(vertexes[v].second)+D>=50) {
        if (path.size() < minN) {
            shortestPath = path;
            minN = path.size();
        }
        path.pop_back();
        visited[v] = false;
        return;
    }
    for (int i=1; i<=N; i++) {
        if (!visited[i] && reachable(v, i))
            dfs(i);
    }
    path.pop_back();
    visited[v] = false;
}

bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
    return sqrt(pow(a.first, 2)+pow(a.second, 2)) < sqrt(pow(b.first, 2)+pow(b.second, 2));
}

int main() {
    int i;
    scanf("%d %d", &N, &D);
    vertexes.resize(N+1);
    vertexes[0].first = 0;
    vertexes[0].second = 0;
    visited.resize(N);
    for (i=1; i<=N; i++)
        scanf("%d %d", &vertexes[i].first, &vertexes[i].second);
    // 查看是否一步到达岸边。
    if (D+7.5 >= 50) {
        printf("1\n");
        return 0;
    }
    sort(vertexes.begin()+1, vertexes.end(), cmp);
    fill(visited.begin(), visited.end(), false);
    for (i=1; i<=N; i++) {
        if (firstReachable(i))
            dfs(i);
    }
    if (shortestPath.empty())
        printf("0\n");
    else {
        printf("%lu\n", shortestPath.size()+1);
        for (int x: shortestPath)
            printf("%d %d\n", vertexes[x].first, vertexes[x].second);
    }
}
```

```cpp
#include <iostream>
#include <climits>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

vector<pair<int, int>> vertexes;
vector<bool> visited;
vector<int> shortestPath;

int N, D, n;

double distanceOfTwoPoint(int u, int v) {
    return sqrt(pow(vertexes[u].first-vertexes[v].first, 2)+pow(vertexes[u].second-vertexes[v].second, 2));
}

bool firstReachable(int v) {
    return distanceOfTwoPoint(0, v) <= D+7.5;
}

bool reachable(int u, int v) {
    return distanceOfTwoPoint(u, v) <= D;
}

vector<int> pre;

int bestDst;

void bfs() {
    int i, v;
    queue<int> q;
	// 可能有多个起点，扫描一遍，把这些起点都入队。
    for (i=1; i<=N; i++) {
        if (firstReachable(i)) {
            q.push(i);
            visited[i] = true; // 一定要在push()时更新visited，不能在pop()时更新，否则会推入已推入过的顶点。
            pre[i] = 0;
        }
    }
    while (!q.empty()) {
        v = q.front();
        q.pop();
        if (abs(vertexes[v].first)+D>=50 || abs(vertexes[v].second)+D>=50) {
            bestDst = v;
            return;
        }
        for (i=1; i<=N; i++) {
            if (!visited[i] && reachable(v, i)) {
                q.push(i);
                visited[i] = true;
                pre[i] = v; // 记录前驱顶点，以便回溯出完整路径。
            }
        }
    }
}

void dfs(int v) {
    n++;
    if (v != 0) {
        dfs(pre[v]);
        shortestPath.push_back(v);
    }
}

bool cmp(const pair<int, int>& a, const pair<int, int>& b) {
    return sqrt(pow(a.first, 2)+pow(a.second, 2)) < sqrt(pow(b.first, 2)+pow(b.second, 2));
}

int main() {
    int i;
    scanf("%d %d", &N, &D);
    vertexes.resize(N+1);
    vertexes[0].first = 0;
    vertexes[0].second = 0;
    visited.resize(N);
    pre.resize(N+1);
    for (i=1; i<=N; i++)
        scanf("%d %d", &vertexes[i].first, &vertexes[i].second);
    // 查看是否一步到达岸边。
    if (D+7.5 >= 50) {
        printf("1\n");
        return 0;
    }
    // XXX If there are many shortest paths, just output the one with the minimum first jump, which is guaranteed to be unique. 所以先对输入序列排序，在BFS/DFS。
    sort(vertexes.begin()+1, vertexes.end(), cmp);
    fill(visited.begin(), visited.end(), false);
    bfs();
    if (bestDst == 0)
        printf("0\n");
    else {
        dfs(bestDst);
        printf("%d\n", n);
        for (int x: shortestPath)
            printf("%d %d\n", vertexes[x].first, vertexes[x].second);
    }
}
```


## 07-图6 旅游规划 (25 分)

1. 单源最短路径用dijkstra算法。
2. 主要的权相等，次要的权更小，才更小次要的权，否则不需要做什么。千万不要看到主要的权相等，就不管三七二十一更新次要的权。

```
其他类似问题
 要求数最短路径有多少条
 count[s] = 1;
 如果找到更短路：count[W]=count[V];
 如果找到等长路：count[W]+=count[V];
 要求边数最少的最短路
 count[s] = 0;
 如果找到更短路：count[W]=count[V]+1;
 如果找到等长路：count[W]=count[V]+1;
```

```cpp
#include <iostream>
#include <climits>
using namespace std;

int N, M, S, D;

// 编号从0开始。
pair<int, int> graph[500][500];
bool visited[500]; // 默认初始化为false。
int shortestDis[500];
int smallestValue[500];

void dijkstra() {
    int i, v, minDis;
    shortestDis[S] = 0;
    smallestValue[S] = 0;
    while (!visited[D]) {
        minDis = INT_MAX;
        for (i=0; i<N; i++) {
            if (!visited[i] && shortestDis[i]<minDis) {
                minDis = shortestDis[i];
                v = i;
            }
        }
        visited[v] = true;
        // 考察通过v中继，未确定最短路径的顶点是否有更短的路径。
        for (i=0; i<N; i++) {
            if (!visited[i] && graph[v][i].first!=0) {
                if (shortestDis[v]+graph[v][i].first < shortestDis[i]) {
                    shortestDis[i] = shortestDis[v]+graph[v][i].first;
                    smallestValue[i] = smallestValue[v]+graph[v][i].second;
                } else if (shortestDis[v]+graph[v][i].first==shortestDis[i] && smallestValue[v]+graph[v][i].second<smallestValue[i]) {
					// 主要的权相等，次要的权更小，才更小次要的权，否则不需要做什么。
                    smallestValue[i] = smallestValue[v]+graph[v][i].second;
                }
            }
        }
    }
}

int main() {
    int i, city1, city2, dis, value;
    scanf("%d %d %d %d", &N, &M, &S, &D);
    for (i=0; i<M; i++) {
        scanf("%d %d %d %d", &city1, &city2, &dis, &value);
        // 无向图。
        graph[city1][city2].first = dis;
        graph[city1][city2].second = value;
        graph[city2][city1].first = dis;
        graph[city2][city1].second = value;
    }
    // 使用dijkstra算法前必要的初始化。
    for (i=0; i<N; i++) {
        shortestDis[i] = INT_MAX;
        smallestValue[i] = INT_MAX;
    }
    dijkstra();
    printf("%d %d\n", shortestDis[D], smallestValue[D]);
}
```