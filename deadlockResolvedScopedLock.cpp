#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

struct CriticalData {
    std::mutex mut;
};

void deadLock(CriticalData& a, CriticalData& b) {
    std::scoped_lock(a.mut, b.mut);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::cout << "Thread: " << std::this_thread::get_id() << " got both mutexes\n";
}

int main() {

    std::cout << "\n";

    CriticalData c1;
    CriticalData c2;

    std::thread t1([&]{deadLock(c1,c2);});
    std::thread t2([&]{deadLock(c2,c1);});

    t1.join();
    t2.join();

    std::cout << "\n";

    return 0;
}