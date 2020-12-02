#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

constexpr long SIZE = 100'000'000;
constexpr long FIRST_QUARTER = 25'000'000;
constexpr long SECOND_QUARTER = 50'000'000;
constexpr long THIRD_QUARTER = 75'000'000;
constexpr long FOURTH_QUARTER = 100'000'000;

std::mutex mutex;

short nextRandomNumber() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<short> distrib(1, 10);

    return distrib(gen);
}

void initVectorMultiThreaded(std::vector<short>& vec, short (*func)(), const long size) {
    std::lock_guard lck(mutex);
    for(auto i = 0L; i < size; ++i) {
        vec.push_back((*func)());
    }
}

void sumVectorMultiThreaded(std::atomic<long long>& sum, const std::vector<short>& vec, const long long beg, const long long end) noexcept {
    long long tmpSum = 0;
    for(auto i = beg; i < end; ++i) {
        tmpSum += vec[i];
    }

    sum += tmpSum;
}

int main() {
    std::vector<short> vec;
    vec.reserve(SIZE);

    auto start = std::chrono::steady_clock::now();

    std::thread it1(initVectorMultiThreaded, std::ref(vec), &nextRandomNumber, SIZE/4);
    std::thread it2(initVectorMultiThreaded, std::ref(vec), &nextRandomNumber, SIZE/4);
    std::thread it3(initVectorMultiThreaded, std::ref(vec), &nextRandomNumber, SIZE/4);
    std::thread it4(initVectorMultiThreaded, std::ref(vec), &nextRandomNumber, SIZE/4);

    it1.join();
    it2.join();
    it3.join();
    it4.join();

    auto finish = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = finish - start;
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
    std::cout << "sum vector with atomic variable duration = " << duration.count() << " secs\n";

    std::cout << "sum(vec) = " << sum << "\n";

    return 0;
}