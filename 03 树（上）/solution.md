## 03-树1 树的同构 (25 分)

1. 注意前面的判断，这使得不会出现越界异常。
2. 没有边指向结点i，则结点i是根。
3. scanf()，当想要接受%c时，记得吃掉换行符！即`scanf("%c\n", &c);`。
4. 没有接受掉换行符，之后接受字符输入时，就会获取到这个换行符！

```cpp
#include <iostream>
#include <vector>
using namespace std;

struct Node {
    char data;
    // Node *left, *right; // 指向结点ID，这里，结点的ID是结点在数组中的下标，而不是在内存中的地址。
    int left, right;
    Node(): left(-1), right(-1) {}
};

int N, i, root1, root2;

// 用数组顺序存储结点，每个结点有一个ID，也就是下标。
vector<Node> t1;
vector<Node> t2;
vector<bool> checked;

int createTree(vector<Node>& t) {
    if (!N) // 为了使得算法对空树可以正常工作。
        return -1;
    int i;
    char lchild, rchild;
    for (i=0; i<N; i++) // 因为该函数会被多次调用，所以这一个重新初始化是必要的。
        checked[i] = false;
    for (i=0; i<N; i++) {
        scanf("%c %c %c\n", &t[i].data, &lchild, &rchild);
        // printf("%c %c %c\n", t[i].data, lchild, rchild);
        if (lchild != '-') {
            // t[i].left = lchild-'-'; // 错误！lchild是一个一位的字符数字。
            t[i].left = lchild-'0';
            checked[t[i].left] = true; // 有边指向结点t[i].left。
        }
        if (rchild != '-') {
            t[i].right = rchild-'0';
            checked[t[i].right] = true; // 有边指向结点t[i].right。
        }
    }
    for (i=0; i<N && checked[i]; i++)
        ;
    return i; // 没有边指向结点i，则结点i是根。
}

bool isomorphic(int root1, int root2) {
	// base case:
    if (root1==-1 && root2==-1) // 都为NULL。
        return true;
    if (root1==-1 || root2==-1) // 只有其中一个为NULL。
        return false;
    Node& r1 = t1[root1]; // 引用，避免拷贝。
    Node& r2 = t2[root2];
    if (r1.data != r2.data) // 根值不同。
        return false;
    // Node& r1l = t1[r1.left]; // 没经过判断，可能数组访问越界。
    // Node& r2l = t2[r2.left];
    if (r1.left!=-1 && r2.left!=-1 && t1[r1.left].data==t2[r2.left].data) // 注意前面的判断，这使得不会出现越界异常。
        return isomorphic(r1.left, r2.left) && isomorphic(r1.right, r2.right); // 不需要交换左右孩子。
    else
        return isomorphic(r1.left, r2.right) && isomorphic(r1.right, r2.left);
}

int main() {
	// scanf("%d", &N); // XXX 没有接受掉换行符，之后接受字符输入时，就会获取到这个换行符！
    scanf("%d\n", &N);
    t1.resize(N);
    checked.resize(N);
    root1 = createTree(t1);
    int N1 = N;
    scanf("%d\n", &N);
    if (N1 != N) {
        printf("No\n");
        return 0;
    }
    t2.resize(N);
    root2 = createTree(t2);

    if (isomorphic(root1, root2))
        printf("Yes\n");
    else
        printf("No\n");
}
```


## 03-树2 List Leaves (25 分)

```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Node {
    int left, right;
    Node(): left(-1), right(-1) {}
};

vector<Node> tree;
vector<bool> checked;
vector<int> leaves;

int N, i, root;

char lchild, rchild;

// 树的层序遍历。
void bfs() {
    int x, i;
    queue<int> q;
    q.push(root);
    while (!q.empty()) {
        x = q.front();
        q.pop();
        if (tree[x].left==-1 && tree[x].right==-1) {
            leaves.push_back(x);
            continue;
        }
        if (tree[x].left != -1)
            q.push(tree[x].left);
        if (tree[x].right != -1)
            q.push(tree[x].right);
    }
}

int main() {
    scanf("%d\n", &N); // 注意要把换行符吃掉。
    tree.resize(N);
    checked.resize(N);
    for (i=0; i<N; i++) {
        scanf("%c %c\n", &lchild, &rchild);
        if (lchild != '-') {
            tree[i].left = lchild-'0';
            checked[tree[i].left] = true;
        }
        if (rchild != '-') {
            tree[i].right = rchild-'0';
            checked[tree[i].right] = true;
        }
    }
    for (i=0; i<N && checked[i]; i++)
        ;
    root = i;
    bfs();
    printf("%d", leaves[0]);
    for (i=1; i<leaves.size(); i++)
        printf(" %d", leaves[i]);
}
```


## 03-树3 Tree Traversals Again (25 分)

1. 根据给出的输入序列，得到先序序列和中序序列。
2. 如果不会直接根据先序和中序序列进行后序遍历的话，也可以先建树，然后再后序遍历。
3. resize()后用`[]`而不用push_back()！

```cpp
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

stack<int> s;
vector<int> pre;
vector<int> in;
vector<int> post;

int N, i, x;

char cmd[5]; // 多一个字节存放'\0'。

// 后序遍历。
void postorder(int pStart, int iStart, int len) {
    if (len <= 0)
        return;
    int root = pre[pStart];
    for (i=iStart; i<iStart+len && in[i]!=root; i++)
        ;
    int lChildLen = i-iStart;
    int rChildLen = len-1-lChildLen;
    // 左、右、根。
    postorder(pStart+1, iStart, lChildLen);
    postorder(pStart+1+lChildLen, iStart+lChildLen+1, rChildLen);
    post.push_back(root);
}

int main() {
    scanf("%d", &N);
    for (i=0; i<N; ) {
        scanf("%s", cmd);
        if (cmd[1] == 'u') {
            scanf("%d", &x);
            pre.push_back(x);
            s.push(x);
        } else {
            in.push_back(s.top());
            s.pop();
            i++;
        }
    }
    postorder(0, 0, N);
    printf("%d", post[0]);
    for (i=1; i<N; i++)
        printf(" %d", post[i]);
}
```