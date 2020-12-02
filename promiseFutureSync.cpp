/*
    Using promise and future for communication btw threads

    Author Denis Shelemekh © 2020
*/

#include <future>
#include <iostream>
#include <utility>

void doTheWork() {
    std::cout << "Processing the shared data...\n";
}

void setDataReady(std::promise<void>&& promise) {
    std::cout << "Sender: the data is ready.\n";
    promise.set_value();
}

void waitingForWork(std::future<void>&& future) {
    std::cout << "Waiting for work...\n"; 
    future.wait();
    doTheWork();
    std::cout << "The data is processed.\n";
}

int main() {
    std::promise<void> promise;
    auto future = promise.get_future();

    std::thread t1(waitingForWork, std::move(future));
    std::thread t2(setDataReady, std::move(promise));

    t1.join();
    t2.join();

    return 0;
}