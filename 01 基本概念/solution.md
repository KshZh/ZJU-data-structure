## 01-复杂度3 二分查找 (20 分)

O(logn)

```cpp
// 二分查找只能应用在有序的序列上。
Position BinarySearch(List L, ElementType X) {
    Position left=1, right=L->Last, mid;
    while (left <= right) { // 因为right是闭的，所以`left==right`时不会越界，所以可以继续循环。
        mid = (left+right)/2;
        if (L->Data[mid] == X) {
            return mid;
        } else if (L->Data[mid] > X) {
            right = mid-1;
        } else {
            left = mid+1;
        }
    }
    return NotFound;
}
```

## 01-复杂度1 最大子列和问题 (20 分)

O(n^2)

```cpp
#include <iostream>
using namespace std;

int N, i, j, k, thisSum, maxSum, a[100000];

int main() {
    scanf("%d", &N);
    for (i=0; i<N; i++)
        scanf("%d", a+i);
    for (i=0; i<N; i++) { // 子序列的左闭
        thisSum = 0;
        for (j=i; j<N; j++) { // 子序列的右闭
            thisSum += a[j];
            if (thisSum > maxSum)
                maxSum = thisSum;
        }
    }
    printf("%d\n", maxSum);
}
```

O(nlogn)

```cpp
#include <iostream>
using namespace std;

int N, a[100000];

int max3(int A, int B, int C){
    /* 返回3个整数中的最大值 */
    return A > B ? A > C ? A : C : B > C ? B : C;
}

// [left, right]
int divideAndConquer(int left, int right) {

    // 递归终点。
    if (left == right) {
        if (a[left] < 0) return 0;
        return a[left];
    }

    int mid = (left+right)/2;

    // 分，递归。
    // 左、右、跨中点。
    int maxLeftSum = divideAndConquer(left, mid);
    int maxRightSum = divideAndConquer(mid+1, right);

    int maxLeftBorderSum=0, maxRightBorderSum=0;
    int leftBorderSum=0, rightBorderSum=0;
    for (int i=mid; i>=left; i--) {
        leftBorderSum += a[i];
        if (leftBorderSum > maxLeftBorderSum)
            maxLeftBorderSum = leftBorderSum;
    }
    for (int i=mid+1; i<=right; i++) {
        rightBorderSum += a[i];
        if (rightBorderSum > maxRightBorderSum)
            maxRightBorderSum = rightBorderSum;
    }

    // 治。
    return max3(maxLeftSum, maxRightSum, maxLeftBorderSum+maxRightBorderSum);
}

int main() {
    scanf("%d", &N);
    for (int i=0; i<N; i++)
        scanf("%d", a+i);
    printf("%d\n", divideAndConquer(0, N-1));
}
```

O(n)，下面这个其实是贪心。

```cpp
#include <iostream>
using namespace std;

int N, i, j, k, thisSum, maxSum, a[100000];

int main() {
    scanf("%d", &N);
    for (i=0; i<N; i++)
        scanf("%d", a+i);
    for (i=0; i<N; i++) {
        thisSum += a[i];
        if (thisSum > maxSum)
            maxSum = thisSum;
        else if (thisSum < 0) // 如果当前子序列和为负，则不可能对后面的部分和有贡献（即使得后面的部分和更大），抛弃之。
            thisSum = 0;
    }
    printf("%d\n", maxSum);
}
```

## 01-复杂度2 Maximum Subsequence Sum (25 分)

PAT a1007

DP。

```cpp
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int K;

int main() {
    int i;
    scanf("%d", &K);
    vector<int> v(K);
	// 定义子问题。
    vector<int> dp(K); // dp[i]表示以v[i]结尾的最大连续子序列。
    vector<int> start(K); // start[i]表示以v[i]结尾的最大连续子序列的起点。
    int nNegative = 0;
    for (i=0; i<K; i++) {
        scanf("%d", &v[i]);
        if (v[i] < 0)
            nNegative++;
    }
    if (nNegative == K) {
        printf("0 %d %d\n", v.front(), v.back());
        return 0;
    }
    // 边界：
    dp[0] = v[0];
    start[0] = 0;
    // int maxDP = INT_MIN;
    int maxDP = dp[0];
    int maxI = 0;
    for (i=1; i<K; i++) {
        if (v[i]+dp[i-1] > v[i]) {
            dp[i] = v[i]+dp[i-1];
            start[i] = start[i-1];
        } else {
            dp[i] = v[i];
            start[i] = i;
        }
        if (dp[i] > maxDP) {
            maxDP = dp[i];
            maxI = i;
        }
    }
    printf("%d %d %d\n", maxDP, v[start[maxI]], v[maxI]);
}
```
