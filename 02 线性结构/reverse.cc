#include <iostream>
#include <string>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(): next(nullptr) {}
    Node(int d): data(d), next(nullptr) {}
};

void reverse1(Node* head) {
    Node* p = head->next;
    Node* q;
    head->next = nullptr;
    while (p) {
        // 头插法。
        q = p->next;
        p->next = head->next;
        head->next = p;
        p = q;
    }
}
   
// 带头结点的链表反转。
// 只逆序前k个。
// 如果还是按照上面的思路就不太好办，因为需要先遍历得到第k个结点的地址，设置head->next，才能使用上面的算法。
// 三指针。
void reverse2(Node* head, int k) {
    Node* p = head->next; // p指向已逆序好的结点。
    Node* q = p->next; // q指向要逆序的结点。
    Node* temp;
    for (int i=1; i<k && q; i++) {
        temp = q->next;
        q->next = p;
        p = q;
        q = temp;
    }
    // 现在，head->next指向反转后的序列的最后一个结点，p指向反转后序列的第一个结点，q指向下一个待反转序列的第一个结点或下一个不反转序列的第一个结点。
    head->next->next = q;
    head->next = p;
}

// 三指针。
Node* reverse3(Node* head, int k) {
    Node* p = head->next; // p指向已逆序好的结点。
    Node* q = p->next; // q指向要逆序的结点。
    Node* temp;
    for (int i=1; i<k && q; i++) { // 注意判断q，防止空指针访问异常。
        temp = q->next;
        q->next = p;
        p = q;
        q = temp;
    }
    // 现在，head->next指向反转后的序列的最后一个结点，p指向反转后序列的第一个结点，q指向下一个待反转序列的第一个结点或下一个不反转序列的第一个结点。
    // 如果是要求按照每k段每k段来逆序的话，就要返回head->next作为下一个待逆序序列的头结点。
    temp = head->next;
    head->next->next = q;
    head->next = p; // 头结点连到反转后的序列的第一个结点。
    return temp;
}

void print(Node* head) {
    while (head->next) {
        printf("%d ", head->next->data);
        head = head->next;
    }
    printf("\n");
}

int main() {
    Node* head = new Node();
    Node* p=head;
    int N = 6;
    for (int i=1; i<N+1; i++) {
        p->next = new Node(i);
        p = p->next;
    }
    print(head);
    // reverse1(head);
    // reverse2(head, 3);
    // reverse2(head, 10); // 如果k大于链表长度，那就全反转。
    Node* nextHead = head;
    int k = 2;
    for (int i=0; i+k<=N; i+=k) { // 如果剩下的结点不足K，则不反转。注意要等于，因为N是右开，左闭加区间长度可以等于右开。
        nextHead = reverse3(nextHead, k);
    }
    print(head);
}
