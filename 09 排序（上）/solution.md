## 09-排序2 Insert or Merge (25 分)

1. `for (i=0; i+1<N && partialSorted[i]<=partialSorted[i+1]; i++) // 注意这里是<=，等于是可以的。`

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;

void mergePass(vector<int>& nums, int length) {
    int i, j;
    for (i=0; i+2*length<=N; i+=2*length) // XXX 左闭+区间长度=右开。
        // merge(nums, temp, i, i+length, i+2*length);
        // 这里偷懒，直接用std::sort了。
        sort(nums.begin()+i, nums.begin()+i+length*2);
    // 最后可能剩一个子序列，不需要合并。
    // XXX 还可能剩一个半子序列，要合并。
    if (i+length < N) {
        // merge(nums, temp, i, i+length, N);
        sort(nums.begin()+i, nums.end());
    }
}

int main() {
    int i, j;
    scanf("%d", &N);
    vector<int> partialSorted(N);
    vector<int> init(N);
    for (i=0; i<N; i++)
        scanf("%d", &init[i]);
    for (i=0; i<N; i++)
        scanf("%d", &partialSorted[i]);
    // 挑一个容易判断的来判断，如果不是其中一个，那就是另一个了。
    for (i=0; i+1<N && partialSorted[i]<=partialSorted[i+1]; i++) // 注意这里是<=，等于是可以的。
        ;
    for (j=++i; i<N && partialSorted[i]==init[i]; i++);
        ;
    if (i == N) {
        printf("Insertion Sort\n");
        int temp = partialSorted[j];
        for (; j>0 && partialSorted[j-1]>temp; j--)
            partialSorted[j] = partialSorted[j-1];
        partialSorted[j] = temp; // 插入。
    } else {
        printf("Merge Sort\n");
        vector<int> temp(N);
        bool oneMore = false;
        for (i=1; i<N; i*=2) {
            if (equal(init.begin(), init.end(), partialSorted.begin())) {
                oneMore = true;
            }
            mergePass(init, i);
            if (oneMore)
                break;
        }
        partialSorted = move(init);
    }
    printf("%d", partialSorted[0]);
    for (i=1; i<N; i++)
        printf(" %d", partialSorted[i]);
    printf("\n");
}
```


## 09-排序3 Insertion or Heap Sort (25 分)

1. 注意“删掉”刚才被提出的最大元，不将其纳入下滤的考察范围，这是堆操作中容易疏忽，容易出错的地方。
2. `for (i=0; i+1<N && partialSorted[i]<=partialSorted[i+1]; i++) // 注意这里是<=，等于是可以的。`

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;

int main() {
    int i, j;
    scanf("%d", &N);
    vector<int> partialSorted(N);
    vector<int> init(N);
    for (i=0; i<N; i++)
        scanf("%d", &init[i]);
    for (i=0; i<N; i++)
        scanf("%d", &partialSorted[i]);
    // 挑一个容易判断的来判断，如果不是其中一个，那就是另一个了。
    for (i=0; i+1<N && partialSorted[i]<=partialSorted[i+1]; i++) // 注意这里是<=，等于是可以的。
        ;
    for (j=++i; i<N && partialSorted[i]==init[i]; i++);
        ;
    if (i == N) {
        printf("Insertion Sort\n");
        int temp = partialSorted[j];
        for (; j>0 && partialSorted[j-1]>temp; j--)
            partialSorted[j] = partialSorted[j-1];
        partialSorted[j] = temp; // 插入。
    } else {
        printf("Heap Sort\n");
        int n = 0;
        for (i=N-1; i>0 && partialSorted[i]>partialSorted[i-1]; i--)
            n++;
        int heapSize = N-n;
        swap(partialSorted[0], partialSorted[heapSize-1]);
        // 将partialSorted[0]下滤到合适的位置。
        // 注意这是一个最大堆，用最大堆来做升序排序。
        // XXX 还要注意，这里完全二叉树不是从1开始存储的。
        int child;
        int temp = partialSorted[0];
        heapSize--; // XXX 注意“删掉”刚才被提出的最大元，不将其纳入下滤的考察范围，这里容易疏忽，容易出错。
        for (i=0; i*2+1<heapSize; i=child) { 
            child = i*2+1;
            if (child+1<heapSize && partialSorted[child+1]>partialSorted[child])
                child++;
            if (temp>=partialSorted[child]) break; // 大于孩子中最大的孩子，i就是一个合适的位置了。
            else partialSorted[i] = partialSorted[child];
        }
        partialSorted[i] = temp;
    }
    printf("%d", partialSorted[0]);
    for (i=1; i<N; i++)
        printf(" %d", partialSorted[i]);
    printf("\n");
}
```