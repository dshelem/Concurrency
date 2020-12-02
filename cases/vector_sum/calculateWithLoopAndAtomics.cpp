#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <atomic>

constexpr long SIZE = 100'000'000;

std::atomic<long long> sum;

/*struct StaticStruct {
    StaticStruct() {
        std::cout << "StaticStruct()\n";
    }
    ~StaticStruct() {
        std::cout << "~StaticStruct()\n";
    }
};*/

short nextRandomNumber() noexcept {
    //static StaticStruct sc;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<short> distrib(1, 10);

    return distrib(gen);
}

void initVector(std::vector<short>& vec, short (*func)()) {
    if(vec.capacity() < SIZE) {
        vec.reserve(SIZE);
    }
    for(auto i = 0L; i < SIZE; ++i) {
        vec.push_back((*func)());
    }
}

long long sumVectorOperator(const std::vector<short>& vec) noexcept {
    sum = {0};
    for(auto elem: vec) {
        sum += elem;
    }
    return sum;
}

long long sumVectorFetchAdd(const std::vector<short>& vec) noexcept {
    sum = {0};
    for(auto elem: vec) {
        sum.fetch_add(elem);
    }
    return sum;
}

int main() {
    std::vector<short> vec;

    std::cout << std::boolalpha << "sum.is_lock_free() = " << sum.is_lock_free() << "\n";

    auto start = std::chrono::steady_clock::now();
    initVector(vec, &nextRandomNumber);
    auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = finish - start;
    std::cout << "init duration = " << duration.count() << " secs\n";

    start = std::chrono::steady_clock::now();
    sumVectorOperator(vec);
    finish = std::chrono::steady_clock::now();
    duration = finish - start;
    std::cout << "sum vector with operator+= duration = " << duration.count() << " secs\n";

    start = std::chrono::steady_clock::now();
    sumVectorFetchAdd(vec);
    finish = std::chrono::steady_clock::now();
    duration = finish - start;
    std::cout << "sum vector with fetch_add duration = " << duration.count() << " secs\n";

    std::cout << "sum(vec) = " << sum << "\n";

    return 0;
}