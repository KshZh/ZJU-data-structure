## KMP 串的模式匹配 (25分)

```
if (p0 != pj)
	match[j] = -1;
else
	match[j] = 满足p0……pi==p(j-i)……pj的最大的i(<j); // 等号两边是子串p0……pj的两端的子串。因为对于每一个子串，总有i==j，使得i最大，但这对KMP算法并没有意义。
```

```cpp
#include <string>
#include <iostream>
#include <vector>
using namespace std;

vector<int> buildMatch(const string& pattern) {
    // 注意到match数组和源文本是没有任何关系的，只和pattern有关，这样我们就不需要去分析往往很长的源文本。
    // string::size_type i, j; // 错误，因为match数组是int，有负值，赋值给无符号数，得到一个很大的无符号数。
    int i, j;
    auto n = pattern.size();
    vector<int> match(n);
    match[0] = -1; // 按照match的定义。

    for (j=1; j<n; j++) {
        i = match[j-1];
        while (i>=0 && pattern[i+1]!=pattern[j])
            i = match[i];
        if (pattern[i+1] == pattern[j])
            match[j] = i+1;
        else
            match[j] = -1;
    }
    return match;
}

// O(N+M)
int kmp(const string& src, const string& pattern) {
    string::size_type s, p;
    auto n=src.size(), m=pattern.size();
    vector<int> match = buildMatch(pattern);
    // for (s=p=0; s<src.size() && p<pattern.size(); ) {
    for (s=p=0; s<n && p<m; ) {
        if (src[s] == pattern[p])
            s++, p++;
        else if (p > 0)
            p = match[p-1]+1; // 看PPT中的图帮助理解。
        else
            s++;
    }
    if (p == m)
        return s-p; // 右开减区间长度等于左闭。
    return -1;
}

// O(N*M)
int strstr(const string& src, const string& pattern) {
    string::size_type s, p;
    auto n=src.size(), m=pattern.size();
    for (s=p=0; s<n && p<m; ) {
        if (src[s] == pattern[p]) {
            p++;
            s++;
        } else {
            s = s-p+1;
            p = 0;
        }
    }
    if (p == m)
        return s-p;
    return -1;
}

int main() {
    int n, pos;
    string src, pattern;
    getline(cin, src);
    // cin >> n;
    // cin >> n >> '\n'; // error
    scanf("%d\n", &n);
    while (n--) {
        getline(cin, pattern);
        // if ((pos=kmp(src, pattern)) == -1)
        if ((pos=strstr(src, pattern)) == -1)
            cout << "Not Found" << '\n';
        else
            cout << src.substr(pos) << '\n';
    }
}
```
