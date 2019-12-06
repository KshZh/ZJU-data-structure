## 02-线性结构1 两个有序链表序列的合并 (15 分)

```cpp
// L1和L2是给定的带头结点的单链表，其结点存储的数据是递增有序的；
// 函数Merge要将L1和L2合并为一个非递减的整数序列。
// 应直接使用原序列中的结点，返回归并后的带头结点的链表头指针。
List Merge(List L1, List L2) {
    List L3 = (PtrToNode)malloc(sizeof(struct Node));
    PtrToNode p1 = L1->Next;
    PtrToNode p2 = L2->Next;
    PtrToNode p3 = L3;
    L1->Next = L2->Next = L3->Next = NULL; // 注意将L1->Next和L2->Next置为NULL，使得它们抛弃它们所有的结点。
    while (p1 && p2) {
        if (p1->Data <= p2->Data) {
            p3->Next = p1;
            p1 = p1->Next;
        } else {
            p3->Next = p2;
            p2 = p2->Next;
        }
        p3 = p3->Next;
    }
	// 直接把剩下的串起来。
    if (p1) p3->Next = p1;
    if (p2) p3->Next = p2;
    return L3;
}
```


## 02-线性结构2 一元多项式的乘法与加法运算 (20 分)

```cpp
#include <iostream>
using namespace std;

struct Node {
    int coef;
    int exponent;
    Node* next;
    Node(int c, int e): coef(c), exponent(e), next(nullptr) {}
    Node(): next(nullptr) {}
};

// XXX 在multiply()和add()之间有内存泄露，因为是算法题，就不考虑了。

Node *poly1, *poly2, *poly3, *p;

int i, n1, n2, n3, c, e;

void output(Node* p) {
    if (n3 == 0) {
        printf("0 0\n");
        return;
    }
    while (p->next) {
        printf("%d %d ", p->coef, p->exponent);
        p = p->next;
    }
    printf("%d %d\n", p->coef, p->exponent);
}

// 参数是没有头结点的链表。
void add(Node* a, Node* b) {
    n3 = 0;
    p = poly3; // 由于要插入和删除，所以p指向前驱结点，这样方便操作。
    while (a && b) {
        // if (a->exponent==b->exponent && a->coef+b->coef!=0) { // 系数为0的项，不存储。
        if (a->exponent==b->exponent) {
            int c = a->coef+b->coef;
            if (c != 0) {
                p->next = new Node(c, a->exponent);
                p = p->next;
                n3++;
            }
            a = a->next; // XXX 这个更新操作，不管c为不为0，都要执行。
            b = b->next;
        } else if (a->exponent > b->exponent) {
			// XXX 总是先将指数大的并入结果链表中，另一个链表先不动。
            p->next = new Node(a->coef, a->exponent);
            a = a->next;
            p = p->next;
            n3++;
        } else {
            p->next = new Node(b->coef, b->exponent);
            b = b->next;
            p = p->next; // p的更新和n3的更新是一样的，都是在有新的项插入/删除时才更新。
            n3++;
        }
    }
    while (a) {
        p->next = new Node(a->coef, a->exponent);
        a = a->next;
        p = p->next;
        n3++;
    }
    while (b) {
        p->next = new Node(b->coef, b->exponent);
        b = b->next;
        p = p->next;
        n3++;
    }
}

void multiply(Node* a, Node* b) {
    n3 = 0;
    p = poly3;
    // 先将a与b的第一项相乘，产出第一个多项式。
    Node* temp = a;
    while (temp) {
        p->next = new Node(temp->coef*b->coef, temp->exponent+b->exponent); // 系数相乘，指数相加。
		// 这里还可以判断一下，相乘后的系数是否为0，为零则不加入多项式，不过测试用例并没有在这里坑我们。
        p = p->next;
        temp = temp->next;
        n3++;
    }
    Node* newNode;
    bool flag;
    b = b->next; // XXX 跳过多项式b的第一项。
    while (b) {
        temp = a;
        while (temp) {
            newNode = new Node(temp->coef*b->coef, temp->exponent+b->exponent);
            flag = false;
            // 插入结果多项式中。
            for (p=poly3; p->next; p=p->next) { // p是遍历过程中，当前结点的前驱指针，这样删除和添加比较方便。
                if (newNode->exponent == p->next->exponent) { // 指数相同，系数相加。
                    int c = newNode->coef + p->next->coef;
                    if (c == 0) {
                        // 删除结果多项式中的项p->next。
                        Node* t = p->next;
                        p->next = t->next;
                        delete t;
                        n3--; // 项数减一。
                    } else {
                        p->next->coef += newNode->coef;
                    }
                    delete newNode;
                    flag = true;
                    break;
                } else if (newNode->exponent > p->next->exponent) {
                    newNode->next = p->next;
                    p->next = newNode;
                    n3++;
                    flag = true;
                    break;
                }
            }
            // XXX 注意，这里`p->next==nullptr`这个条件不能**唯一确定**
            // newNode的指数比结果链表中的所有项的指数都小，
            // 因为上面循环中可能会删除结点可能使得`p->next==nullptr`这个条件成立。
            // 导致出现bug。
            // 最简单的解法是设置一个标志。
            // if (p->next == nullptr) {
            if (!flag) {
                // 插入表尾。
                p->next = newNode;
                // newNode的ctor使得next成员默认初始化为nullptr。
                n3++;
            }
            temp = temp->next;
        }
        b = b->next;
    }
}

int main() {
    scanf("%d", &n1);
    poly1 = new Node; // 带一个头结点，这样方便接受输入。
    p = poly1;
    for (i=0; i<n1; i++) {
        scanf("%d %d", &c, &e);
        p->next = new Node(c, e);
        p = p->next;
    }
    scanf("%d", &n2);
    poly2 = new Node;
    p = poly2;
    for (i=0; i<n2; i++) {
        scanf("%d %d", &c, &e);
        p->next = new Node(c, e);
        p = p->next;
    }
    poly3 = new Node;
    multiply(poly1->next, poly2->next);
    output(poly3->next);
    add(poly1->next, poly2->next);
    output(poly3->next);
}
```


## 02-线性结构3 Reversing Linked List (25 分)

1. 注意有多余结点不在链表上这种情况，故注意要使用链表真正的长度。
2. 注意，For example, given L being 1→2→3→4→5→6, if K=3, then you must output 3→2→1→6→5→4; if K=4, you must output 4→3→2→1→5→6. 并不是K=3，然后结果是 3→2→1→4→5→6。
3. 一开始审题不清，总以为是后面的情况。
4. 参考http://lulalap.com/2018/11/28/PAT-A1074-cpp/，**链表不一定要用链式存储，也可以线性存储，特别是对于题目给出每个结点的地址的情况**。
5. std::reverse。
6. 因为我们连续存储结点，而**next指针的值就是下一个结点的地址**嘛。
7. 转换成带头结点的链表，方便处理。

```cpp
#include <iostream>
using namespace std;

struct Node {
    int data;
    int next_;
    Node(): next_(-1) {};
};

Node list[100002];

int N, i, head, K, addr, data, next_, cnt;

// 三指针。
int reverse1(int head, int k) {
    int p = list[head].next_; // p指向已逆序好的结点，第一个结点是已逆序好的（哨兵结点并不是第一个结点，哨兵结点指向的结点才是第一个结点）
    int q = list[p].next_; // q指向将要逆序的结点。
    int temp;
    for (int i=1; i<k && q!=-1; i++) {
        temp = list[q].next_;
        list[q].next_ = p;
        p = q;
        q = temp;
    }
    int nextHead = list[head].next_;
    list[list[head].next_].next_ = q;
    list[head].next_ = p;
    return nextHead;
}

int main() {
    scanf("%d %d %d", &head, &N, &K);
    for (i=0; i<N; i++) {
        scanf("%d %d %d", &addr, &data, &next_);
        list[addr].data = data;
        list[addr].next_ = next_;
    }
	// 先遍历一遍，计算出链表真正的长度cnt。
	int hs = head;
    while (hs != -1) {
        cnt++;
        hs = list[hs].next_;
    }
    list[100001].next_ = head; // 转换成带头结点的链表，方便处理。
    head = 100001;
    int nextHead = head; // 下一个头结点。
    // for (i=0; i+K<=N; i+=K) { // 注意条件是`i+K<=N`，即如果剩下的序列长度不足K，那就不逆序了。
	for (i=0; i+K<=cnt; i+=K) { // XXX 注意要使用链表真正的长度。
        nextHead = reverse1(nextHead, K);
    }
    int h = list[head].next_; // 去掉头结点。
    while (list[h].next_ != -1) {
        printf("%05d %d %05d\n", h, list[h].data, list[h].next_);
        h = list[h].next_;
    }
    printf("%05d %d -1\n", h, list[h].data);
}
```

```cpp
#include <iostream>
#include <map>
#include <algorithm>
using namespace std;

int N, K, i, n, head, addr, key, next_;

// XXX 因为我们连续存储结点，而next指针的值就是下一个结点的地址嘛。

map<int, pair<int, int>> nodes;
int list[100001];

int main() {
    scanf("%d %d %d", &head, &N, &K);
    for (i=0; i<N; i++) {
        scanf("%d %d %d", &addr, &key, &next_);
        nodes[addr] = pair<int, int>(key, next_);
    }
	// 把链表中各个结点的地址顺序存储在一个数组中，然后对数组每K段进行反转，而next指针的值就是下一个结点的地址嘛。
	// 这样我们在反转的时候就不必修改next指针了！
    for (n=0; head!=-1; n++) {
        list[n] = head;
        head = nodes[head].second;
    }
    for (i=0; i+K<=n; i+=K) { // 反转n/k段。注意i+K<=n，如果剩下的部分长度不足K，那就不反转了。
        reverse(list+i, list+i+K);
    }
    for (i=0; i<n-1; i++) {
        printf("%05d %d %05d\n", list[i], nodes[list[i]].first, list[i+1]);
    }
    printf("%05d %d -1\n", list[i], nodes[list[i]].first);
}
```


## 02-线性结构4 Pop Sequence (25 分)

1. 给定一个入栈序列（序列中的数字从1开始），出栈是随意的，问能够构造出给出的出栈序列。
2. 一个事实是，要pop()出数字x，必须先push()序列中未入栈的所有小于x的数。
3. 记得把栈清空，避免影响下一轮测试。清空栈只能一个一个pop()。
4. 记得continue，不要无条件地执行了下面的更新语句`lastPushed = a[i];`。

5 7 5
1 2 3 4 5 6 7 YES
3 2 1 7 5 6 4 NO
7 6 5 4 3 2 1 NO
5 6 4 3 7 2 1 YES
1 7 6 5 4 3 2 NO

```cpp
#include <stack>
#include <iostream>
using namespace std;

stack<int> s;

int N, M, K, i, j, lastPushed, a[1000];

bool flag;

int main() {
    scanf("%d %d %d", &M, &N, &K);
    while (K--) {
		lastPushed = 0;
		flag = true;
        for (i=0; i<N; i++)
            scanf("%d", a+i);
		for (i=0; i<N; i++) {
			if (a[i] < lastPushed) {
				if (s.empty() || s.top()!=a[i]) {
					flag = false;
					break;
				}
				s.pop();
                continue; // XXX 记得continue，不要无条件地执行了下面的更新语句`lastPushed = a[i];`。
			}
			for (j=lastPushed+1; j<a[i]; j++)
				s.push(j);
			// 数字a[i]并不入栈，我们假装它入栈又马上出栈了。
			if (s.size()+1 > M) { // 记得加1，因为a[i]也是要入栈的，只是因为a[i]要入栈然后马上弹出，所以我们省略了将a[i]入栈的步骤。
				flag = false;
				break;
			}
			lastPushed = a[i];
		}
		if (flag)
			printf("YES\n");
		else
			printf("NO\n");
        j = s.size();
        while (j--) s.pop(); // 记得把栈清空，避免影响下一轮测试。清空栈只能一个一个pop()。
    }
}
```
