#include <iostream>
#include <cmath>
using namespace std;

int gcd(int a, int b) {
    return b==0? a: gcd(b, a%b);
}

int lcm(int a, int b) {
    int g = gcd(a, b);
    // return a*b/g; // 容易溢出，调整一下运算顺序，
    return a/g*b;
}

int main() {
    cout << gcd(4, 6) << '\n'; // 2
    cout << lcm(4, 6) << '\n'; // 12
}
