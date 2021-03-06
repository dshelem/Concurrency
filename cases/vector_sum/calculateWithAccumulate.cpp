#include <iostream>
#include <vector>
#include <random>
#include <chrono>

constexpr long SIZE = 100'000'000;

short nextRandomNumber() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<short> distrib(1, 10);

    return distrib(gen);
}

void initVector(std::vector<short>& vec, short (*func)()) {
    for(auto i = 0L; i < SIZE; ++i) {
        vec[i] = func();
    }
}

long long sumVector(std::vector<short>& vec) noexcept {
    return std::accumulate(vec.begin(), vec.end(), 0);
}

int main() {
    std::vector<short> vec(SIZE);

    auto start = std::chrono::steady_clock::now();
    initVector(vec, nextRandomNumber);
    auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = finish - start;
    std::cout << "init duration = " << duration.count() << " secs\n";

    start = std::chrono::steady_clock::now();
    long long res = sumVector(vec);
    finish = std::chrono::steady_clock::now();
    duration = finish - start;
    
    std::cout << "sum duration = " << duration.count() << " secs\n";
    std::cout << "sum(vec) = " << res << "\n";

    return 0;
}