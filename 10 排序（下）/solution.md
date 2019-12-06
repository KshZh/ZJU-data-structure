## 10-排序4 统计工龄 (20 分)

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main() {
    int N, i, j;
    scanf("%d", &N);
    vector<int> v(N);
    for (i=0; i<N; i++)
        scanf("%d", &v[i]);
    sort(v.begin(), v.end());
    for (i=0; i<N; i++) {
        for (j=i; j<N && v[j]==v[i]; j++)
            ;
        printf("%d:%d\n", v[i], j-i); // 右开减左闭等于区间长度。
        i = j-1;
    }
}
```


## 10-排序5 PAT Judge (25 分)

1. "id's are 5-digit numbers from 00001 to N" -> `printf("%05d")`。
2. 编号从1开始 ->
	1. `for (i=1; i<=K; i++)`；
	2. `int a[5+1]`, `vector<User> users(N+1)`；
	3. `sort(v.begin()+1, v.end())`；
	4. 从`users[1]`开始访问。
3. 因为最终的输出是每一个用户一条记录，所以可以以用户为中心构建数据结构，而不是以提交或问题为中心。
4. 注意，0是一个有效的成绩，所以不能用0表示未通过编译。而且提交了未通过编译和从未提交是两个不同的事件，不能都用-1表示，后者可以用-2表示，而且只有后者在输出时才需要用'-'表示。那对于前者，输出时怎么表示呢？通过观察测试用例，得到用0表示。另外注意还有一种情况是提交且编译过了但得0分。

```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>
using namespace std;

struct User {
    int id; // 虽然可以以id作为下标从而不存储，但排序要用到。
    int scores[6]; // scores[i]表示该用户在问题i上获得的成绩，初始值为-2，表示未提交过。
    int nPerfectlySolved;
    int totalScore;
    // XXX 究其原因，我们不应该把没有任何提交和没有任何编译通过判断标准转换为totalScore为0，这增加了复杂度，若考虑不全面，很容易出错。
    int display; // 故添加这个字段。
    User() {
        display = 0;
        totalScore = 0;
        nPerfectlySolved = 0;
        for (int i=1; i<6; i++)
            scores[i] = -2;
    }
};

int fullMark[6]; // 问题从1开始编号。

bool cmp(const User& u1, const User& u2) {
    // XXX 究其原因，我们不应该把没有任何提交和没有任何编译通过判断标准转换为totalScore为0，这增加了复杂度，若考虑不全面，很容易出错。
    if (u1.display != u2.display) return u1.display>u2.display;
    if (u1.totalScore != u2.totalScore) return u1.totalScore>u2.totalScore; // 自然把totalScore为0的全部排到后面去了。
    if (u1.nPerfectlySolved != u2.nPerfectlySolved) return u1.nPerfectlySolved>u2.nPerfectlySolved;
    return u1.id<u2.id;
}

unordered_map<int, bool> display;

int main() {
    int N, K, M, i, j, uid, pid, ps, n;
    scanf("%d %d %d", &N, &K, &M);
    vector<User> users(N+1); // 编号从1开始。
    for (i=1; i<=K; i++)
        scanf("%d", &fullMark[i]);
    for (i=0; i<M; i++) {
        scanf("%d %d %d", &uid, &pid, &ps);
        users[uid].id = uid; // 就不判断后赋值了，直接赋值。
        if (users[uid].scores[pid] < ps) {
            users[uid].scores[pid] = ps;
            if (ps == fullMark[pid])
                users[uid].nPerfectlySolved++;
        }
    }
    for (i=1; i<=N; i++) {
        auto& user = users[i];
        n = 0;
        for (j=1; j<=K; j++) {
            /*
            if (user.scores[j] == 0) { // 提交了编译过了但是得0分，这个用户就需要输出，即使它的totalScore为0。
                display[i] = true;
                continue;
            }
            */
            if (user.scores[j] == -1) {
                user.scores[j] = 0;
                continue;
            }
            // if (user.scores[j] > 0) {
            if (user.scores[j] >= 0) {
                user.totalScore += user.scores[j];
                n++;
            }
        }
        if (n > 0)
            user.display = 1;
    }
    sort(users.begin()+1, users.end(), cmp);
    int rank = 1;
    printf("%d %05d %d", rank, users[1].id, users[1].totalScore);
    for (j=1; j<=K; j++) {
        if (users[1].scores[j] == -2)
            printf(" -");
        else
            printf(" %d", users[1].scores[j]);
    }
    printf("\n");
    for (i=2; i<=N; i++) {
        // 由于totalScore为0的都在后面，且要display的可能在这部分的后面，第一个遇到的是不display的，于是算法失效了。
        // XXX 究其原因，我们不应该把没有任何提交和没有任何编译通过判断标准转换为totalScore为0，这增加了复杂度，若考虑不全面，很容易出错。
        /*
        if (users[i].totalScore == 0 && !display[i])
            break;
        */
        if (!users[i].display)
            break;
        if (users[i].totalScore != users[i-1].totalScore)
            rank = i;
        printf("%d %05d %d", rank, users[i].id, users[i].totalScore);
        for (j=1; j<=K; j++) {
            if (users[i].scores[j] == -2)
                printf(" -");
            else
                printf(" %d", users[i].scores[j]);
        }
        printf("\n");
    }
}
```


## 10-排序6 Sort with Swap(0, i) (25 分)

1. 贪心问题。
2. 考试如果想不出来，没有思路，那就考虑暴力破解吧，起码能拿几分是几分。
3. 不需要存储原始序列，交换了下标相当于交换了原序列中两个元素的位置。

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N, i, n, x;
    scanf("%d", &N);
    vector<int> /*v(N),*/ idx(N);
    for (i=0; i<N; i++) {
        // scanf("%d", &v[i]); // 不需要存储原始序列，交换了下标相当于交换了原序列中两个元素的位置。
        scanf("%d", &x);
        idx[x] = i;
    }
    n = 0;
    for (i=1; i<N; i++) { // 遍历元素/数字[1, N]。
        // 如果等于，那么数字i就已经在正确的位置上了，不需要做什么。
        if (idx[i] != i) {
            while (idx[0] != 0) {
                swap(idx[0], idx[idx[0]]);
                n++;
            }
            if (idx[i] != i) { // 这里还要判断一下，如果上面的循环已经把i交换到了正确的位置，则这里就不需要再做什么了。
                swap(idx[0], idx[i]);
                n++;
            }
        }
    }
    printf("%d\n", n);
}
```