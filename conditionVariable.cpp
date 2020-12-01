#include <condition_variable>
#include <mutex>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
bool dataReady(false);
std::condition_variable cv;
std::mutex mutex;

void waitingForWork() noexcept {
    std::cout << "Waiting...\n";
    std::unique_lock lck(mutex);
    cv.wait(lck, []() {return dataReady; });
    mySharedWork[1] = 2;
    std::cout << "Work done\n";
}

void setDataReady() noexcept {
    mySharedWork = {1, 0, 3};
    {
        std::lock_guard lock(mutex);
        dataReady = true;
    }
    std::cout << "Data prepared\n";
    cv.notify_one();
}

int main() {
    std::cout << "Condition Variable Example\n==========================\n";

    std::thread t1(waitingForWork);
    std::thread t2(setDataReady);

    t1.join();
    t2.join();

    for(auto v: mySharedWork) {
        std::cout << v << " ";
    }

    std::cout << "\n\n";

    return 0;
}