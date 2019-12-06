## 08-图7 公路村村通 (30 分)

1. 最小生成树，可用prim算法或kruskal算法。
2. prim算法和dijkstra算法很像，但要注意区别，前者的`dist[i]`表示未在MST中的顶点i到MST（中的顶点）的最小距离，后者的`dist[i]`表示从原点v到vi的最短路径。所以更新这些数据结构时也有区别。
3. 如果有多个连通分量，则不存在最小生成树。如何判断，对于prim，如果MST中的结点数不足N则不存在，对于kruskal，如果MST中的边数不足N-1则不存在。

```cpp
#include <iostream>
#include <climits>
using namespace std;

int N, M, minWeight;
// 编号从1开始。
int dist[1001]; // dist[i]表示未在MST中的顶点i到MST（中的顶点）的最小距离。
int graph[1001][1001];

bool prim() {
    int i, min_, v, cnt=0;
    dist[1] = 0; // 默认初始点为1。
    while (true) {
        // 找出未收录到MST的顶点中到MST中的顶点有最小距离的顶点。
        min_ = INT_MAX;
        for (i=1; i<=N; i++) {
            if (dist[i]!=-1 && dist[i]<min_) {
                min_ = dist[i];
                v = i;
            }
        }
        if (min_ == INT_MAX)
            break;
        minWeight += dist[v];
        dist[v] = -1; // 将顶点v收录到MST中，当然也可以用一个visited数组代替。
        cnt++;
        // 考察以顶点v为中继，其它未收录到MST的顶点到MST的距离是否更小。
		// XXX 遍历顶点v的邻接点。
        for (i=1; i<=N; i++) {
			// 注意和dijkstra算法一样，都要保证顶点i与顶点v邻接，XXX 即这个循环其实是遍历顶点v的邻接点。
            if (dist[i]!=-1 && graph[v][i]!=0 && graph[v][i]<dist[i]) {
                dist[i] = graph[v][i];
            }
        }
    }
    return cnt==N; // 可能有多个连通分量，生成树不存在。
}

int main() {
    int i, u, v, w;
    scanf("%d %d", &N, &M);
    for (i=0; i<M; i++) {
        scanf("%d %d %d", &u, &v, &w);
        graph[u][v] = w;
        graph[v][u] = w;
    }
    // 类似dijkstra算法，这里是应用prim算法的必要的初始化。
    for (i=1; i<=N; i++)
        dist[i] = INT_MAX;
    if (prim()) {
        printf("%d\n", minWeight);
    } else {
        printf("-1\n");
    }
}
```


## 08-图8 How Long Does It Take (25 分)

1. 拓扑排序（应用于AOV图）的变种，计算AOE图的最早完成时间，如果AOE图存在环，则不存在最早完成时间，否则，一个顶点的最早完成时间仅当其入度为0时才被确定。
2. 对拓扑排序，只有不存在环路（“子任务A依赖于子任务B，子任务B依赖于子任务C，子任务C又依赖于子任务A”）的AOV图才有合法的拓扑序列，对于AOV图和AOE图，**有多个连通分量是没关系的**。
4. XXX **使用拓扑排序可以判断一个有向图是否是DAG（有向无环图）**。

```cpp
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int N, M;
int earliest[100]; // 从0开始编号。
vector<pair<int, int>> graph[100];
// bool visited[100]; // 因为可能多个顶点指向同一个顶点，即一个顶点有多个入度，所以要用一个visited数组，避免重复push()同一个顶点。
// 但这里不需要，因为我们在indegree为0时才push()，可确保不会重复push()。
int indegree[100];

bool topSort() {
    int i, v, cnt=0;
    queue<int> q;
    // XXX 把所有入度为0的顶点入队。不要下意识地以为入度为0的顶点只有一个。
    for (i=0; i<N; i++) {
        if (indegree[i] == 0) {
            q.push(i);
            earliest[i] = 0;
        }
    }
    while (!q.empty()) {
        v = q.front();
        q.pop();
        cnt++;
		// 处理邻接的边/活动。
        for (const auto& p: graph[v]) {
            earliest[p.first] = max(earliest[p.first], earliest[v]+p.second);
            indegree[p.first]--; // 删去这条边。
            if (indegree[p.first] == 0) { // 没有边指向顶点p.first时，该顶点的最早完成时间才确定，才将该顶点入队。
                q.push(p.first);
            }
        }
    }
    return cnt==N; // 通过这一个判断可以判断该图是否是有向无环图。
}


int main() {
    int i, u, v, t;
    scanf("%d %d", &N, &M);
    for (i=0; i<M; i++) {
        scanf("%d %d %d", &u, &v, &t);
        graph[u].push_back(pair<int, int>(v, t));
        indegree[v]++;
    }
    // 有多个起点，并不意味着有多个连通分量。
    if (!topSort()) {
        printf("Impossible\n");
    } else {
        int max_ = -1;
        for (i=0; i<N; i++) {
            if (earliest[i] > max_)
                max_ = earliest[i];
        }
        printf("%d\n", max_);
    }
}
```


## 08-图9 关键活动 (30 分)

1. 有环的AOE图是无效的，不可调度，无法计算出完成时间。

2. 机动时间为0的边/任务是关键任务。

3. 任务调度问题中，如果还给出了完成每个子任务需要的时间，则我们可以算出完成整个工程需要的最短时间。在这些子任务中，有些任务即使推迟几天完成，也不会影响全局的工期；但是有些任务必须准时完成，否则整个项目的工期就要因此延误，这种任务就叫“关键活动”。

4. 从右往左遍历时，注意初始化问题：

   ```cpp
   // 记得做下面的初始化，否则算法会失效。
   // 注意，下面的第一个初始化是错误的，第二个初始化才是正确的。
   // vertexes[i].latestFinish = vertexes[i].earliestFinish; // 在多终点情况下算法失效。
   vertexes[i].latestFinish = maxEarliestFinish;
   ```

5. 在写算法题时，尽量分开来写，容易排错。比如不要把topSortL2R()和topSortR2L()写成一个函数，然后在函数中加条件分支，这样在写算法题时不太好。算法题首先要求正确性，代码复用是最不重要的。

```cpp
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>
#include <iostream>
using namespace std;

struct Edge {
    int id; // 只是为了方便排序。
    int start;
    int end;
    int weight;
    int flexibleTime;
    Edge(): flexibleTime(-1) {}
    Edge(int u, int v, int w): start(u), end(v), weight(w), flexibleTime(-1) {}
};

struct Vertex {
    // 顶点ID隐含地用数组下标表示/存储。
    int earliestFinish; // 指向该顶点（活动结束点）的那些活动/边的最早完成时间。
    int latestFinish;
    Vertex(): earliestFinish(INT_MIN), latestFinish(INT_MAX) {}
};

int N, M, maxEarliestFinish=-1;

vector<Vertex> vertexes;
vector<Edge> edges;
// 因为要“从左往右”处理一遍，然后“从右往左”处理一遍，所以要维护两份数据结构。
vector<int> indegreeL2R; // 顶点编号从1开始。
vector<int> indegreeR2L;
vector<vector<int>> graphL2R; // graphL2R[i]是以顶点i开始的边的ID的集合。
vector<vector<int>> graphR2L;

bool topSortL2R() {
    int i, cnt=0, v;
    queue<int> q;
    // 入度为0的顶点先入队。不要下意识地以为入度为0的顶点只有一个。
    for (i=1; i<=N; i++) {
        if (indegreeL2R[i] == 0) {
            q.push(i);
            // 记得做下面的初始化，否则算法会失效。
            vertexes[i].earliestFinish = 0;
        }
    }
    while (!q.empty()) {
        v = q.front();
        q.pop();
        cnt++;
        for (int e: graphL2R[v]) {
            vertexes[edges[e].end].earliestFinish = max(vertexes[edges[e].end].earliestFinish, vertexes[v].earliestFinish+edges[e].weight); // max+加法，取最大的最早完成时间。
            indegreeL2R[edges[e].end]--; // 删去这条边。
            if (indegreeL2R[edges[e].end] == 0) { // 仅当以顶点end为终点的任务/边都结束了，该顶点的最早/最晚完成时间才能确定，这时入度为0，才可以入队。
                q.push(edges[e].end);
            }
        }
    }
    return cnt==N; // 否则图存在回路，不是一个AOE图。
}

bool topSortR2L() {
    int i, cnt=0, v;
    queue<int> q;
    // 入度为0的顶点先入队。不要下意识地以为入度为0的顶点只有一个。
    for (i=1; i<=N; i++) {
        if (indegreeR2L[i] == 0) {
            q.push(i);
            // 记得做下面的初始化，否则算法会失效。
			// 注意，下面的第一个初始化是错误的，第二个初始化才是正确的。
            // vertexes[i].latestFinish = vertexes[i].earliestFinish;
            vertexes[i].latestFinish = maxEarliestFinish;
        }
    }
    while (!q.empty()) {
        v = q.front();
        q.pop();
        cnt++;
        for (int e: graphR2L[v]) {
            vertexes[edges[e].start].latestFinish = min(vertexes[edges[e].start].latestFinish, vertexes[v].latestFinish-edges[e].weight); // min+减法，取最小的最迟完成时间。
            edges[e].flexibleTime = vertexes[v].latestFinish-vertexes[edges[e].start].earliestFinish-edges[e].weight; // 机动时间是活动/边的最晚完成时间（存储在终点中）减去最早完成时间（该起点就绪的最晚完成时间）/该活动最早开始时间（存储在起点中），再减去活动执行所需要的时间。
            indegreeR2L[edges[e].start]--; // 删去这条边。
            if (indegreeR2L[edges[e].start] == 0) { // 仅当以顶点start为终点的任务/边都结束了，该顶点的最早/最晚完成时间才能确定，这时入度为0，才可以入队。
                q.push(edges[e].start);
            }
        }
    }
    return cnt==N; // 否则图存在回路，不是一个AOE图。
}

bool cmp(const Edge& e1, const Edge& e2) {
    if (e1.start != e2.start) return e1.start<e2.start;
    return e1.id>e2.id; // 若起点相同，则按输入顺序的逆序输出。
}

int main() {
    int i, j, u, v;
    scanf("%d %d", &N, &M);
    indegreeL2R.resize(N+1); // 顶点编号从1开始。
    indegreeR2L.resize(N+1);
    graphL2R.resize(N+1);
    graphR2L.resize(N+1);
    edges.resize(M+1); // 边从1开始编号。
    vertexes.resize(N+1);
    for (i=1; i<=M; i++) {
        scanf("%d %d %d", &u, &v, &edges[i].weight);
        edges[i].id = i;
        edges[i].start = u;
        edges[i].end = v;
        indegreeL2R[v]++;
        indegreeR2L[u]++;
        graphL2R[u].push_back(i);
        graphR2L[v].push_back(i);
    }
    if (!topSortL2R()) {
        printf("0\n");
        return 0;
    }
    // 可能有多个终点，输出其中最大的那个的最早完成时间。
    // printf("%d\n", vertexes[N].earliestFinish); // 而且这里还犯了一个错误，即认为终点就是顶点N。
    for (i=1; i<=N; i++) {
        if (vertexes[i].earliestFinish > maxEarliestFinish)
            maxEarliestFinish = vertexes[i].earliestFinish;
    }
    printf("%d\n", maxEarliestFinish);
    if (!topSortR2L()) {
        printf("0\n");
        return 0;
    }
    sort(edges.begin()+1, edges.end(), cmp); // 编号从1开始。
    for (auto& e: edges) {
        if (e.flexibleTime == 0)
            printf("%d->%d\n", e.start, e.end);
    }
}
```
