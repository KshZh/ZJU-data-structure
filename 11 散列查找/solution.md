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
2. TODO

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