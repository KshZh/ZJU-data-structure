## 11-散列1 电话聊天狂人 (25 分)

```cpp
#include <map>
#include <iostream>
#include <string>
#include <climits>
using namespace std;

int main() {
    int N;
    cin >> N;
    map<string, int> m;
    string s1, s2;
    while (N--) {
        cin >> s1 >> s2;
        m[s1]++;
        m[s2]++;
    }
    int max_ = INT_MIN;
    int n;
    for (auto& p: m) {
        if (p.second > max_) {
            s1 = p.first;
            n = 1;
            max_ = p.second;
        } else if (p.second == max_) {
            n++;
        }
    }
    cout << s1 << ' ' << max_;
    if (n != 1) cout << ' ' << n;
    cout << endl;
}
```


## 11-散列2 Hashing (25 分)

```cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

bool isPrime(int x) {
    if (x < 2)
        return false;
    int sq = sqrt(x);
    for (int i=2; i<=sq; i++)
        if (x%i == 0)
            return false;
    return true;
}

int main() {
    int N, M, i, h, k, pos;
    scanf("%d %d", &M, &N);
    while (!isPrime(M))
        M++;
    vector<int> table(M, -1);
    vector<int> v;
    while (N--) {
        scanf("%d", &k);
        h = k%M;
        for (i=0; i<=M; i++) {
            pos = (h+i*i)%M;
            if (table[pos] == -1) {
                table[pos] = k;
                v.push_back(pos);
                break;
            }
        }
        if (i == M+1)
            v.push_back(-1);
    }
    if (v[0] == -1) printf("-");
    else printf("%d", v[0]);
    for (i=1; i<v.size(); i++) {
        if (v[i] == -1) printf(" -");
        else printf(" %d", v[i]);
    }
    printf("\n");
}
```


## 11-散列3 QQ帐户的申请与登陆 (25 分)

```cpp
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

int main() {
    int N, i;
    string account, pwd;
    char cmd;
    unordered_map<string, string> m;
    cin >> N;
    while (N--) {
        cin >> cmd >> account >> pwd;
        if (cmd == 'L') {
            // 注意不能用m[account]，因为不存在会创建！
            if (m.find(account) == m.end()) printf("ERROR: Not Exist\n");
            else if (m[account] != pwd) printf("ERROR: Wrong PW\n");
            else printf("Login: OK\n");
        } else {
            if (m.find(account) != m.end()) printf("ERROR: Exist\n");
            else {
                m[account] = pwd;
                printf("New: OK\n");
            }
        }
    }
}
```


## 11-散列4 Hashing - Hard Version (30 分)

1. 第一段代码是通过观察测试样例写出来的，无法通过最后一个测试。
2. 其实如果多个Key通过hash以及线性退避后映射到同一个slot，那么已经在slot上的key一定是先于要找位置插入的key插入的。通过这个现象，我们可以画出一个DAG（有向无环图）。然后使用拓扑排序来解决问题。
3. 当元素之间有明确的先后关系的时候（也许可以构成DAG），可以考虑是否跟拓扑排序有关，用拓扑排序来生成序列。
4. std::priority_queue在头文件<queue>中。

```cpp
// A negative integer represents an empty cell in the hash table.
// 所以不要以为测试样例给出-1，就认定-1表示空位，其实是负数表示空位。
// if (table[i] != -1)
if (table[i] >= 0)
```

```cpp
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

int main() {
    int N, i, h, pos;
    scanf("%d", &N);
    vector<int> table(N);
    vector<int> v;
    vector<int> res;
    unordered_set<int> book;
    for (i=0; i<N; i++) {
        scanf("%d", &table[i]);
        // A negative integer represents an empty cell in the hash table.
        // 所以不要以为测试样例给出-1，就认定-1表示空位，其实是负数表示空位。
        // if (table[i] != -1)
        if (table[i] >= 0)
            v.push_back(table[i]);
    }
    // Whenever there are multiple choices, the smallest number is always taken.
    // 提示我们要先排序一下。
    sort(v.begin(), v.end());
    for (int x: v) {
        if (book.find(x) != book.end())
            continue;
        h = x%N;
        for (i=0; ; i++) {
            pos = (h+i)%N; // linear probing，看测试样例只需要加正数。要加括号，不然变成i%N先执行，然后才是h+。
            if (table[pos] == x) {
                res.push_back(x);
                book.insert(x);
                break;
            } else {
                if (book.find(table[pos]) == book.end()) {
                    res.push_back(table[pos]);
                    book.insert(table[pos]);
                }
            }
        }
    }
    printf("%d", res[0]);
    for (i=1; i<res.size(); i++)
        printf(" %d", res[i]);
    printf("\n");
}
```

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
using namespace std;

int N;

unordered_map<int, int> idx;
unordered_map<int, vector<int>> edge;
vector<pair<int, int>> seq; // p.first是输入的非负数，p.second是该顶点的入度。
vector<int> res;

int hash_(int x) {
    return x%N;
}

void topSort() {
    // queue<int> q;
	// 这里要用一个优先队列，总是先取最小的元素。greater表示大的元素放前面，但对优先队列来说，作用相反，小的在前面，先被取出来。
    priority_queue<int, vector<int>, greater<int>> q;
    int x;
    // 将所有入度为0的顶点入队。
    for (auto& p: seq) {
        if (p.second == 0) {
            q.push(p.first);
        }
    }
    while (!q.empty()) {
        // x = q.front();
        x = q.top();
        q.pop();
        res.push_back(x);
        for (int y: edge[x]) {
            seq[idx[y]].second--;
            if (seq[idx[y]].second == 0)
                q.push(y);
        }
    }
}

int main() {
    int i, h, pos;
    scanf("%d", &N);
    vector<int> table(N);
    for (i=0; i<N; i++) {
        scanf("%d", &table[i]);
        if (table[i] >= 0) {
            seq.push_back(pair<int, int>(table[i], 0));
            idx[table[i]] = seq.size()-1;
        }
    }
    for (auto& p: seq) {
        h = hash_(p.first);
        for (i=0; ; i++) {
            // pos = h+i; // 错误。
            // pos = h+i%N; // 错误。
            pos = (h+i)%N;
            if (table[pos] != p.first) {
                edge[table[pos]].push_back(p.first); // table[pos]比x先插入，故table[pos]有一条指向x的边。
                seq[idx[p.first]].second++;
            } else {
                break;
            }
        }
    }
    topSort();
    printf("%d", res[0]);
    for (i=1; i<res.size(); i++)
        printf(" %d", res[i]);
    printf("\n");
}
```
