#include <atomic>
#include <thread>
#include <iostream>
#include <string>
#include <exception>

std::atomic<std::string*> ptr {nullptr};
int data = -1;
std::atomic<int> atomicData;

void producer() {
    std::string* p;
    try {
        p = new std::string("C++11");
    }
    catch(const std::exception& e) {
        std::cout << e.what() << "\n";
        throw e;
    }
    data = 2011;
    atomicData.store(2014, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    ptr.store(p, std::memory_order_relaxed);
}

void consumer() {
    std::string* p2;
    while(!(p2 = ptr.load(std::memory_order_relaxed)))
        ;
    std::atomic_thread_fence(std::memory_order_acquire);
    std::cout << "*p2 = " << *p2 << "\n";
    std::cout << "data = " << data << "\n";
    std::cout << "atomicData = " << atomicData.load(std::memory_order_relaxed) << "\n";    
}


int main() {
    std::cout << "\n";
    
    std::thread t1(producer);
    std::thread t2(consumer);
    
    t1.join(); t2.join();

    if(ptr) {
        delete ptr;
    }

    std::cout << "\n";

    return 0;
}