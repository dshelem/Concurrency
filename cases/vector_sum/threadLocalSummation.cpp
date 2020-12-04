#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>

constexpr long SIZE = 100'000'000;
constexpr long FIRST_QUARTER = 25'000'000;
constexpr long SECOND_QUARTER = 50'000'000;
constexpr long THIRD_QUARTER = 75'000'000;
constexpr long FOURTH_QUARTER = 100'000'000;

thread_local long long tmpSum = 0;

short nextRandomNumber() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<short> distrib(1, 10);

    return distrib(gen);
}

void initVectorMultiThreaded(std::vector<short>& vec, short (*func)(), const long long beg, const long long end) noexcept {
    for(auto i = beg; i < end; ++i) {
        vec[i] = func();
    }
}

void sumVectorMultiThreaded(std::atomic<long long>& sum, const std::vector<short>& vec, const long long beg, const long long end) noexcept {
    for(auto i = beg; i < end; ++i) {
        tmpSum += vec[i];
    }

    sum.fetch_add(tmpSum, std::memory_order_relaxed);
}

int main() {
    std::vector<short> vec(SIZE);

    auto start = std::chrono::steady_clock::now();

    std::thread it1(initVectorMultiThreaded, std::ref(vec), nextRandomNumber, 0, FOURTH_QUARTER);

    it1.join();

    auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = finish - start;
    std::cout << "vector size = " << vec.size() << " elements\n";
    std::cout << "init duration = " << duration.count() << " secs\n";

    std::atomic<long long> sum = 0;

    start = std::chrono::steady_clock::now();
    
    std::thread t1(sumVectorMultiThreaded, std::ref(sum), std::ref(vec), 0, FIRST_QUARTER);
    std::thread t2(sumVectorMultiThreaded, std::ref(sum), std::ref(vec), FIRST_QUARTER, SECOND_QUARTER);
    std::thread t3(sumVectorMultiThreaded, std::ref(sum), std::ref(vec), SECOND_QUARTER, THIRD_QUARTER);
    std::thread t4(sumVectorMultiThreaded, std::ref(sum), std::ref(vec), THIRD_QUARTER, FOURTH_QUARTER);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    finish = std::chrono::steady_clock::now();
    duration = finish - start;
    std::cout << "sum vector with atomic variable relaxed memory order duration = " << duration.count() << " secs\n";

    std::cout << "sum(vec) = " << sum << "\n";

    return 0;
}