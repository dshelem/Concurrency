#include <mutex>
#include <thread>
#include <iostream>

std::mutex mutex;

void workOnResource() noexcept {
    mutex.lock();
    std::cout << "Working on critical section... ";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Work done!\n";
    mutex.unlock();
}

int main() {

    std::thread t1(workOnResource);
    std::thread t2(workOnResource);

    t1.join();
    t2.join();

    return 0;
}
