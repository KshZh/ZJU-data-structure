```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int N;

ostream& operator<<(ostream& o, const vector<int>& v) {
    for (int x: v)
        o << x << ' ';
    return o;
}

// ======= 冒泡排序 =======

void bubbleSort(vector<int>& nums) {
    int i, j;
    bool flag;
    for (i=0; i<N-1; i++) { // N-1趟。
        flag = false;
        for (j=0; j+1<N; j++) { // 冒泡一次。
            if (nums[j]>nums[j+1]) {
                swap(nums[j], nums[j+1]);
                flag = true;
            }
        }
        if (flag == false) // 没交换过，说明已经提前有序了，排序完成。
            return;
    }
}


// ======= 插入排序 =======

void insertionSort(vector<int>& nums) {
    int i, j, temp;
    for (i=1; i<N; i++) { // 从第二个元素开始，因为第一个元素已经是有序序列了。逐个元素插入到有序序列合适的位置中去。
        temp = nums[i];
        // 注意以下两种j初始值的区别，前者在一次也没有进入循环时执行`nums[j]=temp`会出错，并且当`j>=0`不成立，即`j==-1`时，执行`nums[j]=temp`更是会使程序奔溃。
        // for (j=i-1; j>=0 && nums[j]>temp; j--)
        //    nums[j+1] = nums[j]; 
        for (j=i; j-1>=0 && nums[j-1]>temp; j--) // j指针从i开始往前走。
            nums[j] = nums[j-1];
        nums[j] = temp; // 插入。
    }
}


// ======= 希尔排序 =======

void shellSort(vector<int>& nums) {
    int i, j, temp;
    // 只需要插入排序的基础上，嵌入一个希尔增量系列即可。
    for (int D=N/2; D>=1; D/=2) {
        for (i=D; i<N; i++) { // 从第二个元素开始，注意这里不是`i+=D`。
            temp = nums[i];
            for (j=i; j-D>=0 && nums[j-D]>temp; j-=D) // j指针从i开始往前走。
                nums[j] = nums[j-D];
            nums[j] = temp; // 插入。
        }
    }
}


// ======= 选择排序 =======

void selectionSort(vector<int>& nums) {
    int i, j, min_, minI_;
    for (i=0; i<N-1; i++) { // 遍历slot，从slot及后面选择一个最小元放入slot中。
        min_ = INT_MAX; // min初始化为max。
        for (j=i; j<N; j++) {
            if (nums[j] < min_) {
                min_ = nums[j];
                minI_ = j;
            }
        }
        swap(nums[i], nums[minI_]);
    }
}


// ======= 堆排序 =======

// 堆排序优化了选择排序中寻找最小元的步骤，从O(N)到O(logN)。

void percDown(int i, vector<int>& nums, int len) {
    int temp = nums[i], child;
    for (; i*2<=len; i=child) {
        child = i*2;
        if (child+1<=len && nums[child+1]>nums[child])
            child++;
        if (temp >= nums[child]) break; // 最大堆，如果当前结点大于孩子中最大的结点，那么这就是一个合适的插入位置。
        else nums[i] = nums[child];
    }
    nums[i] = temp;
}

// XXX 用最大堆来做升序排序。
void heapSort(vector<int>& nums) {
    // 完全二叉树用数组存储，从1开始存储，不然2*i会失效，虽然也可以用2*i+1, 2*i+2，但不直观，且求父结点时，i/2更符合二进制的特点。
    // 线性时间复杂度建立堆，从最后一个父结点开始，逐步将每个子堆调整为最小堆。
    int i;
    for (i=N/2; i>0; i--)
        percDown(i, nums, N);

    for (i=N; i>1;) {
        // 注意用循环变量i表示堆的大小的变化。
        swap(nums[1], nums[i--]); // 每次将最大的元素/堆顶交换到后面去。
        percDown(1, nums, i); // XXX 注意这里的i已经递减过了，把刚刚交换出去的最大的元素从堆中删除。
    }
}

void useHeapSort() {
    int i;
    vector<int> nums(N+1);
    for (i=1; i<=N; i++)
        scanf("%d", &nums[i]);
    heapSort(nums);
    printf("%d", nums[1]);
    for (i=2; i<=N; i++)
        printf(" %d", nums[i]);
    printf("\n");
}


// ======= 归并排序 =======

// 合并两个有序序列。
void merge(vector<int>& nums, vector<int>& temp, int f, int mid, int r) {
    int pa=f, pb=mid, pc=f;
    for (; pa<mid && pb<r; pc++) {
        if (nums[pa] < nums[pb])
            temp[pc] = nums[pa++];
        else
            temp[pc] = nums[pb++];
    }
    while (pa < mid)
        temp[pc++] = nums[pa++];
    while (pb < r)
        temp[pc++] = nums[pb++];
    for (pc=f; pc<r; pc++) // 将有序的部分复制回nums。
        nums[pc] = temp[pc];
}

// 与标准库保持一致，左闭右开区间，[f, r)。
void mSort(vector<int>& nums, vector<int>& temp, int f, int r) { // front, rear
    if (f+1 == r) // 若只有一个元素，有序，排序完成。
        return;

    int mid = (f+r)/2;
    mSort(nums, temp, f, mid);
    mSort(nums, temp, mid, r);

    merge(nums, temp, f, mid, r);
}

void mergeSort(vector<int>& nums) {
    vector<int> temp(nums.size());
    mSort(nums, temp, 0, nums.size());
}

// 下面是迭代版本的归并排序，其中length为有序的子序列长度。
void mergePass(vector<int>& nums, vector<int>& temp, int length) {
    int i, j;
    for (i=0; i+2*length<=N; i+=2*length) // XXX 左闭+区间长度=右开。
        merge(nums, temp, i, i+length, i+2*length);
    // 最后可能剩一个子序列，不需要合并。
    // XXX 还可能剩一个半子序列，要合并。
    if (i+length < N) {
        merge(nums, temp, i, i+length, N);
    }
}

void mergeSortIter(vector<int>& nums) {
    vector<int> temp(nums.size());
    int length;
    for (length=1; length<N; length*=2)
        mergePass(nums, temp, length);
}


// ======= 快速排序 =======

// [left, right]
int median3(vector<int>& nums, int left, int right) {
    int mid = (left+right)/2;
    if (nums[left] > nums[mid])
        swap(nums[left], nums[mid]);
    if (nums[left] > nums[right])
        swap(nums[left], nums[right]);
    if (nums[mid] > nums[right])
        swap(nums[mid], nums[right]);
    // nums[left] <= nums[mid] <= nums[right]
    swap(nums[mid], nums[right-1]);
    return nums[right-1]; // 返回pivot。
}

void doQSort(vector<int>& nums, int left, int right) {
    if (left+2 >= right) {// 少于三个元素，用简单排序。
        sort(nums.begin()+left, nums.begin()+right+1); // 这里偷懒了。
        return;
    }
    int pivot = median3(nums, left, right);
    int i=left, j=left+1; // nums[left]比pivot小。
	for (; j<right-1; j++) {
        if (nums[j] <= pivot) {
			// i+1指向>pivot的元素，j指向<=pivot的元素。
            swap(nums[++i], nums[j]);
        }
    }
    swap(nums[++i], nums[right-1]); // 把主元换到中间。
    doQSort(nums, left, i-1); // XXX 注意不包括主元，主元已经在正确的位置上了。
    doQSort(nums, i+1, right);
}

void qSort(vector<int>& nums) {
    doQSort(nums, 0, nums.size()-1);
}

// ======= 快速排序2 =======

// [left, right]
// from CLRS
// 下面这个双指针的用法值得学习。
int partition(vector<int>& nums, int left, int right) {
    int pivot = nums[right];
    int i=left-1, j=left;
    for (; j<right; j++) {
        if (nums[j] <= pivot) {
			// i+1指向>pivot的元素，j指向<=pivot的元素。
            swap(nums[++i], nums[j]);
        }
    }
    swap(nums[right], nums[++i]); // 将主元交换到中间。
    return i;
}

void doQSort2(vector<int>& nums, int left, int right) {
    if (left < right) {
        int pivotIdx = partition(nums, left, right);
        doQSort2(nums, left, pivotIdx-1); // XXX 注意pivot已经在正确的位置上了。
        doQSort2(nums, pivotIdx+1, right);
    }
}

void qSort2(vector<int>& nums) {
    doQSort2(nums, 0, nums.size()-1);
}


void useOtherSort() {
    int i;
    vector<int> nums(N);
    for (i=0; i<N; i++)
        scanf("%d", &nums[i]);
    
    // bubbleSort(nums); // O(N^2) 超时！
    // insertionSort(nums); // O(N^2) 4k毫秒！
    // shellSort(nums); // O(N^2)，相比其它O(N^2)，能对逆序序列处理得较好。
    // selectionSort(nums); // O(N^2) 3k多毫秒！
    // mergeSort(nums); // O(NlogN)
    // mergeSortIter(nums);
	qSort(nums);
	// qSort2(nums); // 比上面的快排慢很多。

    printf("%d", nums[0]);
    for (i=1; i<N; i++)
        printf(" %d", nums[i]);
    printf("\n");
}

int main() {
    scanf("%d", &N);
    // useHeapSort();
    useOtherSort();
}
```

