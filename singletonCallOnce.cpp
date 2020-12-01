/*
    Singleton design pattern in multithreaded environment
    Author Denis Shelemekh © 2020
*/
#include <iostream>
#include <mutex>
#include <memory>

class Singleton {
public:
    using SingletonPtrType = Singleton*;

private:
    static std::once_flag initInstanceFlag;
    static SingletonPtrType instance;
    Singleton() = default;
    ~Singleton() {
        if(instance) delete instance;
    }

protected:
    static void initSingleton() {
        instance = new Singleton;
    }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static SingletonPtrType getInstance() {
        std::call_once(initInstanceFlag, initSingleton);
        return instance;
    }
};

Singleton::SingletonPtrType Singleton::instance;
std::once_flag Singleton::initInstanceFlag;

int main() {

    std::cout << "Singletone::getInstance() " << Singleton::getInstance() << "\n";
    std::cout << "Singletone::getInstance() " << Singleton::getInstance() << "\n";

    return 0;
}