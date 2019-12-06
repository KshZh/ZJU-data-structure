## 05-树7 堆中的路径 (25 分)

```cpp
#include <iostream>
using namespace std;

int N, M;

int cbt[1001]; // 完全二叉树用数组存储从1开始，这会比较方便。

int heapSize;

// 最小堆的插入。
void insert(int key) {
    heapSize++; // 先放在最底下，然后上滤到合适的位置。
    int i;
    for (i=heapSize; i/2>=1 && cbt[i/2]>key; i/=2)
        cbt[i] = cbt[i/2]; // 父结点下来。
    cbt[i] = key;
}

int main() {
    int i, key;
    scanf("%d %d", &N, &M);
    for (i=0; i<N; i++) {
        scanf("%d", &key);
        insert(key);
    }
    while (M--) {
        scanf("%d", &i);
        printf("%d", cbt[i]);
        while (i/2 >= 1) {
            i /= 2;
            printf(" %d", cbt[i]);
        }
        printf("\n");
    }
}
```


## 05-树8 File Transfer (25 分)

```cpp
#include <iostream>
#include <vector>
using namespace std;

int N;

vector<int> disjoint;
vector<int> rank_;

void make_set() {
    disjoint.resize(N+1);
    rank_.resize(N+1);
    for (int i=1; i<=N; i++) {
        disjoint[i] = i;
        rank_[i] = 0; // 初始的秩都相等。
    }
}

int find(int x) {
    if (disjoint[x] != x)
        disjoint[x] = find(disjoint[x]);
    return disjoint[x];
}

void union_(int x, int y) {
    int a = find(x); // 先找到两个集合的头目。
    int b = find(y);
    if (rank_[a] > rank_[b]) {
        disjoint[b] = a;
    } else {
        disjoint[a] = b;
        if (rank_[a] == rank_[b])
            rank_[b]++;
    }
}

int main() {
    char cmd;
    int x, y, i, n;
    scanf("%d\n", &N); // 因为后面要接受字符输入，所以这里要把换行符吃掉。
    make_set();
    scanf("%c", &cmd); // 记得吃到无关字符。
    while (cmd != 'S') {
        scanf("%d %d\n", &x, &y);
        if (cmd == 'I') {
            union_(x, y);
        } else {
            if (find(x) == find(y))
                printf("yes\n");
            else
                printf("no\n");
        }
        scanf("%c", &cmd);
    }
	// 判断集合个数。
    for (i=1, n=0; i<=N; i++) {
        if (disjoint[i] == i)
            n++;
    }
    if (n == 1)
        printf("The network is connected.\n");
    else
        printf("There are %d components.\n", n);
}
```


## 05-树9 Huffman Codes (30 分)

1. 第二份代码用std::priority_queue代替了手写的最小堆，这使得我们不必担心自己的堆是否实现正确。另外要注意std::priority_queue的排序函数的作用有点不一样，Note that the Compare parameter is defined such that it returns true if its first argument comes before its second argument in a weak ordering. But because the priority queue outputs largest elements first, the elements that "come before" are actually output last. 
2. 也可以通过建树来判断输入序列是否是前缀码，前缀码要求仅在哈夫曼树的叶子上存放被编码的字符。
3. 堆插入的步骤为将元素插入最后，然后上滤到合适位置。
4. 堆建立可以调用插入操作，时间复杂度O(NlogN)。
5. 或者先将输入序列放好，然后从最后一个父结点开始，下滤到合适的位置。这样逐步使每一个小的子堆满足最大/最小堆的性质。这个方法是线性时间复杂度。
6. 一般采用后者，下面的实现采用了前者。

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

struct TreeNode {
    char data;
    int weight;
    TreeNode* left;
    TreeNode* right;
    TreeNode(): left(nullptr), right(nullptr) {}
    TreeNode(char c, int w): data(c), weight(w), left(nullptr), right(nullptr) {}
};

vector<TreeNode*> heap;

unordered_map<char, int> weights;

int N, heapSize;

// 最小堆的插入。
void insert(TreeNode* t) {
    heapSize++; // 先在数组最后/堆最底下分配一个slot，然后再上滤找到合适的插入位置i。
    int i;
    for (i=heapSize; i/2>=1 && heap[i/2]->weight>t->weight; i/=2)
        heap[i] = heap[i/2]; // 父结点下来。
    heap[i] = t;
}

TreeNode* deleteMin() {
    TreeNode* ret = heap[1];
    int tempWeight = heap[heapSize]->weight;
    heapSize--; // 删除堆最底下的那个结点A。
    int i, child; // i指向结点A的合适的插入位置。
    for (i=1; i*2<=heapSize; i=child) { // 如果没有左孩子，那肯定没有右孩子。
        child = i*2; // 选择孩子中最小的那个。
        if (child+1<=heapSize && heap[child+1]->weight<heap[child]->weight)
            child++;
        if (heap[child]->weight < tempWeight)
            heap[i] = heap[child]; // 结点i下滤相当于结点child上滤。
        else
            break;
    }
    heap[i] = heap[heapSize+1]; // 插入。
    return ret;
}

TreeNode* createHuffmanTree() {
    TreeNode* t;
    // 哈夫曼树没有度为1的结点，叶子一定是成对出现的。
    while (heapSize > 1) {
        // 哈夫曼树的建立过程就是每次选择最小的两个子树合并。
        t = new TreeNode(); // 下面这部分可以放在构造函数中。
        t->left = deleteMin();
        t->right = deleteMin();
        t->weight = t->left->weight + t->right->weight;
        insert(t);
    }
    return deleteMin();
}

map<char, vector<char>> paths;

vector<char> path;

// 走左边标0，走右边标1。在边上标，而不是在结点上标。
void dfs(TreeNode* root) {
    if (!root->left && !root->right) { // 所有字符都挂在哈夫曼树的叶子上。
        paths[root->data] = path;
        return;
    }
    if (root->left) {
        path.push_back('0');
        dfs(root->left);
        path.pop_back();
    }
    if (root->right) {
        path.push_back('1');
        dfs(root->right);
        path.pop_back();
    }
}

int WPL; // 如果不作为全局变量，那就要作为dfs2()的引用参数进行传递，注意是引用参数，而不是值参数，或者接受dfs2()的返回值。

void dfs2(TreeNode* root, int len) {
    if (!root->left && !root->right) { // 所有字符都挂在哈夫曼树的叶子上。
        WPL += len * root->weight;
        return;
    }
	// 这里可以不检查左右子树是否为空，因为哈夫曼树只有度为0和度为2的结点，所以如果不是上面的没有左右子树的情况，那么这里一定有左右子树。
    if (root->left)
        dfs2(root->left, len+1);
    if (root->right)
        dfs2(root->right, len+1);
}

bool isPrefix(const string& a, string& b) {
    int i;
    for (i=0; i<a.size() && i<b.size() && a[i]==b[i]; i++)
        ;
    return i==a.size() || i==b.size();
}

// 也可以通过建树来判断输入序列是否是前缀码，前缀码要求仅在哈夫曼树的叶子上存放被编码的字符。
bool allPrefixCode(unordered_map<char, string>& codes) {
    // XXX 要求两两不是对方的前缀。
    for (auto& p1: codes) { // 这里的这个遍历方式其实多做了一点重复的检查。
        for (auto& p2: codes) {
            if (p1 == p2)
                continue;
            if (isPrefix(p1.second, p2.second))
                return false;
        }
    }
    return true;
}

int calculateWPL(const unordered_map<char, string>& codes) {
    int WPL = 0;
    for (auto& p: codes)
        WPL += weights[p.first] * p.second.size();
    return WPL;
}


int main() {
    int i, w, M;
    char c;
    scanf("%d\n", &N); // 下面要接受字符输入，所以要吃掉换行符。
    heap.resize(N+1); // 完全二叉树用数组存储从下标1开始会方便许多。
    for (i=0; i<N; i++) {
        if (i == N-1)
            scanf("%c %d", &c, &w);
        else
            scanf("%c %d ", &c, &w);
        weights[c] = w;
        insert(new TreeNode(c, w));
    }
    TreeNode* t = createHuffmanTree();
    /*
    // 哈夫曼树生成的编码是最优的，但不唯一。
    dfs(t);
    for (auto& p: paths)
        cout << string(p.second.begin(), p.second.end()) << '\t' << p.first << '\n';
    */

    // 题目说了，只要输入是的编码都是前缀码，且每个字符的编码长度和哈夫曼树生成的编码一致即说明该输入的编码是最优的。
    // 但又不对，因为可能有多个选项/叶子可以存放字符，这就导致了上面的判断方法也不唯一。
    // 其实是要对输入序列计算WPL，即带权路径长度，如果与我们通过哈夫曼树构造的编码相等，则认为输入的编码也是最优的。

    dfs2(t, 0);
    unordered_map<char, string> codes;
    string code;
    scanf("%d", &M);
    while (M--) {
        codes.clear();
        for (i=0; i<N; i++) {
            cin >> c >> code;
            codes[c] = code;
        }
        if (allPrefixCode(codes) && calculateWPL(codes)==WPL) // 短路操作，可减少不必要的调用。
            printf("Yes\n");
        else
            printf("No\n");
    }
}
```

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
using namespace std;

struct TreeNode {
    char data;
    int weight;
    TreeNode* left;
    TreeNode* right;
    TreeNode(): left(nullptr), right(nullptr) {}
    TreeNode(char c, int w): data(c), weight(w), left(nullptr), right(nullptr) {}
};

unordered_map<char, int> weights;

// XXX the elements that "come before" are actually output last.
auto cmp = [](const TreeNode* a, const TreeNode* b) { return a->weight>b->weight; };

priority_queue<TreeNode*, vector<TreeNode*>, decltype(cmp)> q(cmp);

int N;

TreeNode* createHuffmanTree() {
    TreeNode* t;
    // 哈夫曼树没有度为1的结点，叶子一定是成对出现的。
    while (q.size() > 1) {
        // 哈夫曼树的建立过程就是每次选择最小的两个子树合并。
        t = new TreeNode(); // 下面这部分可以放在构造函数中。
        t->left = q.top();
        q.pop();
        t->right = q.top();
        q.pop();
        t->weight = t->left->weight + t->right->weight;
        q.push(t);
    }
    return q.top(); // 这里优先队列中还剩最后一个元素没有弹出，不过因为队列之后不会再被使用了，所以没关系。
}

int WPL; // 如果不作为全局变量，那就要作为dfs2()的引用参数进行传递，注意是引用参数，而不是值参数，或者接受dfs2()的返回值。

void dfs2(TreeNode* root, int len) {
    if (!root->left && !root->right) { // 所有字符都挂在哈夫曼树的叶子上。
        WPL += len * root->weight;
        return;
    }
    if (root->left)
        dfs2(root->left, len+1);
    if (root->right)
        dfs2(root->right, len+1);
}

bool isPrefix(const string& a, string& b) {
    int i;
    for (i=0; i<a.size() && i<b.size() && a[i]==b[i]; i++)
        ;
    return i==a.size() || i==b.size();
}

bool allPrefixCode(unordered_map<char, string>& codes) {
    // 要求两两不是对方的前缀。
    for (auto& p1: codes) { // 这里的这个遍历方式其实多做了一点重复的检查。
        for (auto& p2: codes) {
            if (p1 == p2)
                continue;
            if (isPrefix(p1.second, p2.second))
                return false;
        }
    }
    return true;
}

int calculateWPL(const unordered_map<char, string>& codes) {
    int WPL = 0;
    for (auto& p: codes)
        WPL += weights[p.first] * p.second.size();
    return WPL;
}

int main() {
    int i, w, M;
    char c;
    scanf("%d\n", &N); // 下面要接受字符输入，所以要吃掉换行符。
    for (i=0; i<N; i++) {
        if (i == N-1)
            scanf("%c %d", &c, &w);
        else
            scanf("%c %d ", &c, &w);
        weights[c] = w;
        q.push(new TreeNode(c, w));
    }
    TreeNode* t = createHuffmanTree();

    dfs2(t, 0);
    unordered_map<char, string> codes;
    string code;
    scanf("%d", &M);
    while (M--) {
        codes.clear();
        for (i=0; i<N; i++) {
            cin >> c >> code;
            codes[c] = code;
        }
        if (allPrefixCode(codes) && calculateWPL(codes)==WPL) // 短路操作，可减少不必要的调用。
            printf("Yes\n");
        else
            printf("No\n");
    }
}
```