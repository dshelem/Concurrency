#include <iostream>
#include <thread>
#include <utility>

class ScopedThread {
private:
    std::thread thread;
public:
    explicit ScopedThread(std::thread t): thread {std::move(t)} {
        if(!thread.joinable()) {
            throw std::logic_error("No joinable thread");
        }
    }
    ~ScopedThread() noexcept {
        thread.join();
    }
    ScopedThread(const std::thread&) = delete;
    ScopedThread& operator=(const std::thread&) = delete;
};