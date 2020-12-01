#include <vector>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> count {0};

void add(int i) {
    std::cout << "Thread # " << i << "\n";
    for(int n = 0; n < 1000; ++n) {
        count.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    //system("clear");

    std::vector<std::thread> v;
    for(int i = 0; i < 10; ++i) {
        v.emplace_back(add, i);
    }
    for(auto& t: v) {
        t.join();
    }
    std::cout << "Final counter value = " << count << "\n";

    return 0;
}