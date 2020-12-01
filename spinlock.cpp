#include <atomic>
#include <thread>
#include <iostream>

class Spinlock {
private:
    std::atomic_flag flag;
public:
    Spinlock(): flag(ATOMIC_FLAG_INIT) {}

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire))
            ;
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

void workOnResource(Spinlock& spin) {
    spin.lock();
    std::cout << "Working on critical section... ";
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Work done!\n";
    spin.unlock();
}

int main() {
    Spinlock spin;

    std::thread t1(workOnResource, std::ref(spin));
    std::thread t2(workOnResource, std::ref(spin));

    t1.join();
    t2.join();

    return 0;
}