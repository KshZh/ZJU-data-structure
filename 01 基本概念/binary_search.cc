#include <iostream>
using namespace std;

// 查找区间是闭开区间[l, r)，和stl中的upper_bound(), lower_bound(), binary_search()及其它等等保持一致。
// 返回第一个大于upper_bound的元素的下标。
int upper_bound(int* a, int upper_bound, int l, int r) {
    int mid;
    while (l < r) { // 不能`l<=r`，因为l==r时，mid==r，可能越界。
        mid = (l+r)/2;
        if (a[mid] > upper_bound) // 还要继续缩小范围，找到**第一个**大于upper_bound的元素。
            r = mid; // 缩小范围的同时，r隐含地暂存了大于upper_bound的元素的下标。
        else // a[mid] <= upper_bound
            l = mid+1;
    }
    return r;
}

int binary_search(int* a, int key, int l, int r) {
    int mid;
    while (l < r) {
        mid = (l+r)/2;
        if (a[mid] > key)
            r = mid; // r是开，所以不能`r=mid-1`。
        else if (a[mid] < key)
            l = mid+1;
        else
            return mid;
    }
    return -1;
}

// 返回**第一个**大于等于lower_bound的元素的下标。
int lower_bound(int* a, int lower_bound, int l, int r) {
    int mid;
    while (l < r) {
        mid = (l+r)/2;
        if (a[mid] >= lower_bound)
            r = mid; // 缩小范围继续找，找到**第一个**大于等于lower_bound的，缩小范围的同时，r隐含地暂存了大于等于lower_bound的元素的下标。
        else // a[mid] < lower_bound
            l = mid+1; 
    }
    return r;
}

int main() {
    int a[9] = {1, 2, 3, 4, 6, 7, 9, 15, 20};
    cout << binary_search(a, 1, 0, 9) << endl;  // 0, a[0]=1
    cout << binary_search(a, 8, 0, 9) << endl;  // -1
    cout << binary_search(a, 15, 0, 9) << endl; // 7, a[7]=15
    cout << binary_search(a, 20, 0, 9) << endl; // 8, a[8]=20

    cout << upper_bound(a, 1, 0, 9) << endl;  // 1, a[1]=2
    cout << upper_bound(a, 8, 0, 9) << endl;  // 6, a[6]=9
    cout << upper_bound(a, 15, 0, 9) << endl; // 8, a[8]=20
    cout << upper_bound(a, 20, 0, 9) << endl; // 9，也就是[begin, end)中的end，行为和std::upper_bound()一样。

    cout << lower_bound(a, 1, 0, 9) << endl; // 0, a[0]=1
    cout << lower_bound(a, 8, 0, 9) << endl; // 6, a[6]=9
    cout << lower_bound(a, 15, 0, 9) << endl; // 7
    cout << lower_bound(a, 20, 0, 9) << endl; // 8
}
