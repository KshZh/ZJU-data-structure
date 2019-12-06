#include <iostream>
#include <cmath>
using namespace std;

// O(sqrt(N))
bool isPrime(int x) {
    if (x <= 1)
        return false;
    int sqr = sqrt(x);
    for (int i=2; i<=sqr; i++) {
        if (x % i == 0)
            return false;
    }
    return true;
}

// 获取素数表，直接使用isPrime()，时间复杂度为O(Nsqrt(N))。
// 下面使用筛法，筛这个动作使用一个数组，将一个被筛掉的元素标记为true即可。
int primes[1024];
bool filter[1024];
int getPrimes(int N) {
    int i, j, num=0;
    for (i=2; i<N; i++) {
        if (!filter[i]) {
            primes[num++] = i;
            for (j=i+i; j<N; j+=i) {
                filter[j] = true;
            }
        }
    }
    return num;
}

int main() {
    cout << isPrime(3) << '\n';
    cout << isPrime(150) << '\n';
    int n = getPrimes(100);
    for (int i=0; i<n; i++)
        cout << primes[i] << ' ';
    cout << '\n';
}
