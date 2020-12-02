#include <iostream>
#include <vector>
#include <random>
#include <chrono>

constexpr long SIZE = 100'000'000;

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
    vec.reserve(SIZE);
    for(auto i = 0L; i < SIZE; ++i) {
        vec.push_back((*func)());
    }
}

long long sumVector(std::vector<short>& vec) noexcept {
    long long res = 0;
    for(auto elem: vec) {
        res += elem;
    }
    return res;
}

int main() {
    std::vector<short> vec;

    auto start = std::chrono::steady_clock::now();
    initVector(vec, &nextRandomNumber);
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