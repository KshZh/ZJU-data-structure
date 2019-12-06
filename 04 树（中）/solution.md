## 04-树7 二叉搜索树的操作集 (30 分)

1. 因为可能改变BST->Left指向另一个结点，所以要对BST->Left重新赋值，同时对应的操作都要返回当前结点的地址。
2. 对于这些插入、删除的操作，对于初学者，一个很重要的点是区分，树的结点，和指向结点的指针，这两者不能混为一谈。

```c
BinTree Insert(BinTree BST, ElementType X) {
    if (BST == NULL) {
        BST = malloc(sizeof(struct TNode));
        BST->Data = X;
        BST->Left = BST->Right = NULL;
    } else if (X <= BST->Data) {
        BST->Left = Insert(BST->Left, X);
    } else {
        BST->Right = Insert(BST->Right, X);
    }
    return BST;
}


Position Find(BinTree BST, ElementType X) {
    if (BST == NULL)
        return NULL;
    if (X == BST->Data)
        return BST;
    else if (X < BST->Data)
        return Find(BST->Left, X);
    else
        return Find(BST->Right, X);
}

// 二叉搜索树最左边的结点是最小的。
Position FindMin(BinTree BST) {
    if (BST == NULL)
        return NULL;
    while (BST->Left)
        BST = BST->Left;
    return BST;
}

// 二叉搜索树最右边的结点是最大的。
Position FindMax(BinTree BST) {
    if (BST == NULL)
        return NULL;
    while (BST->Right)
        BST = BST->Right;
    return BST;
}

BinTree Delete(BinTree BST, ElementType X) {
    Position Tmp;
    if (!BST)
        printf("Not Found\n");
    else if (X < BST->Data)
        BST->Left = Delete(BST->Left, X); // XXX 因为可能改变BST->Left指向另一个结点，所以要对BST->Left重新赋值。
    else if (X > BST->Data)
        BST->Right = Delete(BST->Right, X);
    else {
        if (BST->Left && BST->Right) { // 被删除结点有左右子树。
            Tmp = FindMin(BST->Right); // 用右子树最小的结点代替当前要删除的结点。
            BST->Data = Tmp->Data;
            BST->Right = Delete(BST->Right, BST->Data); // 然后从右子树中删去那个最小的结点。
        } else { // 无子树，或只有一个子树。
            Tmp = BST;
            if (!BST->Left) // 无子树或只有右子树。
                BST = BST->Right; // BST指针指向其右子结点。
            else
                BST = BST->Left;
            free(Tmp);
        }
    }
    return BST;
}
```


## 04-树4 是否同一棵二叉搜索树 (25 分)

1. 插入的结点就没有和root结点连接起来，相当于没插入。
2. 并不能通过检查中序序列是否一样来判断是否是同一棵二叉搜索树，因为输入的都是二叉搜索树，中序序列很可能是一样的上升序列。

```cpp
#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    bool flag;
    Node(): left(nullptr), right(nullptr), flag(false) {}
    Node(int d): data(d), left(nullptr), right(nullptr), flag(false) {}
};

int N, L, v, i;

Node* t;

Node* insert(Node* root, int v) {
    if (root == nullptr)
        root = new Node(v);
    else if (v < root->data)
        root->left = insert(root->left, v); // 记得要给root->left重新赋值，没准root->left原本是个nullptr，现在插入了一个结点，返回了该结点的地址，如果没有给root->left赋值，那么**插入的结点就没有和root结点连接起来，相当于没插入**。
    else
        root->right = insert(root->right, v);
    return root;
}

Node* make_tree() {
    scanf("%d", &v);
    Node* root = new Node(v);
    for (i=1; i<N; i++) {
        scanf("%d", &v);
        root = insert(root, v); // 建树，不断插入。
    }
    return root;
}

void free_tree(Node* root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

bool check(Node* root, int v) {
    if (root->flag) { // 该结点前面遇到过，
        if (root->data == v) // 这个判断其实算是一个提前剪支，没有这个判断，算法也是能工作的。
            return false;
        else if (v < root->data)
            return check(root->left, v);
        else
            return check(root->right, v);
    } else { // 该结点前面没遇到过，那么该结点必然要与v相等，否则就不是同一棵二叉搜索树。
        if (root->data == v) {
            root->flag = true;
            return true;
        }
        return false;
    }
}

void reset(Node* root) {
    if (root) {
        root->flag = false;
        reset(root->left);
        reset(root->right);
    }
}

// 如何判别序列3 2 4 1是否 与树T一致？
// 方法：在树T中按顺序搜索序列3 2 4 1中的每个数，
// 如果每次搜索所经过的结点在前面均出现过，则一致；
// 否则（某次搜索中遇到前面未出现的结点），则不一致。
bool judge() {
    // 当发现序列中的某个数与T不一致时，必须把序列后面的数都读完。
    int i = 1;
    bool flag = true;
    scanf("%d", &v);
    if (v != t->data) flag=false; // XXX 首先树的根必须一样，否则肯定不是同一棵树。
    else t->flag = true; // XXX 记得若根一样，将根标记为已访问。
    for (; flag && i<N; i++) {
        scanf("%d", &v);
        if (!check(t, v))
            flag = false;
    }
    // 多轮查询，即使提前知道了答案，也要把当前的输入读完。
    for (; i<N; i++)
        scanf("%d", &v);
    return flag;
}

int main() {
    scanf("%d", &N);
    while (N) {
        scanf("%d", &L);
        t = make_tree();
        while (L--) {
            if (judge()) printf("Yes\n");
            else printf("No\n");
            reset(t); // 重置flag。
        }
        free_tree(t); // 因为有多轮，所以要释放动态资源，避免循环次数过多，内存不够。
        scanf("%d", &N);
    }
}
```


## 04-树5 Root of AVL Tree (25 分)

```cpp
#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    // 这里可以选择缓存结点的高度（从叶子到该结点的路径长度），
    // 但是插入时，也要沿途更新路径上的所有结点的高度。
    Node(int k): key(k), left(nullptr), right(nullptr) {}
};

int N;

// 插入发现者的左子树的左边导致发现者不平衡，形象点，就是要把发现者的左子树吊起来，发现者落下去。
Node* LL(Node* root) {
    // 这个函数体看起来还挺像swap()函数的。
    Node* newRoot = root->left; // LL
    root->left = newRoot->right;
    newRoot->right = root;
    return newRoot;
}

// 插入发现者的右子树的右边导致发现者不平衡，那么要把右子树吊起来，发现者落下去。
Node* RR(Node* root) {
    Node* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    return newRoot;
}

// 插入发现者的左子树的右边导致发现者不平衡，先RR，再LL，如LR其名（误）。
Node* LR(Node* root) {
    root->left = RR(root->left);
    return LL(root);
}

Node* RL(Node* root) {
    root->right = LL(root->right);
    return RR(root);
}

int height(Node* root) {
    if (root == nullptr)
        return 0;
    return 1+max(height(root->left), height(root->right));
}

// 记住，AVL树首先是一棵二叉搜索树。
Node* insertNode(Node* root, int key) {
    if (root == nullptr) {
        return new Node(key);
    }
    if (key < root->key) {
        root->left = insertNode(root->left, key); // 注意这里别忘了接收返回值更新root->left，说不定插入后调整平衡后，左子树的根变了呢。
        // 若当前结点是发现者，注意，由于key是往root的左子树中插入的，所以如果出现不平衡，那么必定是左子树的高度大于右子树。
        if (height(root->left) > height(root->right)+1) {
            root = key<root->left->key? LL(root): LR(root);
        }
        // else 没有不平衡，不需要做什么。
    } else {
        root->right = insertNode(root->right, key);
        if (height(root->right) > height(root->left)+1) {
            root = key>root->right->key? RR(root): RL(root);
        }
        // else 没有不平衡，不需要做什么。
    }
    return root;
}

int main() {
    int i, x;
    scanf("%d", &N);
    Node* root = nullptr;
    while (N--) {
        scanf("%d", &x);
        root = insertNode(root, x);
    }
    printf("%d\n", root->key);
}
```

## 04-树6 Complete Binary Search Tree (30 分)

1. 完全二叉树可用数组存储。
2. 中序遍历二叉搜索树可以得到一个有序的升序序列。
3. 对一个升序序列，通过中序遍历，填充用数组存储的CBT。

```cpp
#include <iostream>
#include <algorithm>
using namespace std;

int N, i, p;

int a[1000];

int cbt[1001]; // 从1开始。

void inorder(int i) {
    if (i <= N) {
        inorder(2*i);
        cbt[i] = a[p++];
        inorder(2*i+1);
    }
}

int main() {
    scanf("%d", &N);
    for (i=0; i<N; i++)
        scanf("%d", a+i);
    sort(a, a+N);
    inorder(1);
    printf("%d", cbt[1]);
    for (i=2; i<=N; i++)
        printf(" %d", cbt[i]);
}
```
