#include <cmath>
#include <iostream>
#include <vector>

// std::vector<bool> is possibly specialized to be space-efficient.
void eratosthenes(std::vector<bool>& sieve)
{
    if (sieve.size() > 0) {
        sieve[0] = false;
    }
    if (sieve.size() > 1) {
        sieve[1] = false;
    }
    // Filter even numbers
    for (int n = 4; n < sieve.size(); n += 2) {
        sieve[n] = false;
    }
    // Filter odd numbers
    for (int n = 3; n <= std::sqrt(sieve.size() - 1); n += 2) {
        if (!sieve[n]) {
            continue;
        }
        for (int m = n * n; m < sieve.size(); m += n) {
            sieve[m] = false;
        }
    }
}

int main()
{
    int cnt{100000000};
    std::vector<bool> sieve(cnt, true);
    eratosthenes(sieve);
}
