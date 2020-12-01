#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

std::map<std::string, int> teleBook {{"Dijkstra", 1972}, {"Scott", 1976},
                                    {"Ritchie", 1983}};

std::shared_mutex teleBookMutex;

void printNumber(const std::string& name) {
    std::shared_lock readerLock(teleBookMutex);
    auto searchResult = teleBook.find(name);
    if(searchResult != teleBook.end()) {
        std::cout << searchResult->first << ": " << searchResult->second << "\n";
    }
    else {
        std::cout << name << " not found in the book\n";
    }
}

void addToTeleBook(const std::string& name, int tele) {
    std::lock_guard writerLock(teleBookMutex);
    std::cout << "\nStarting update " << name;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    teleBook[name] = tele;
    std::cout << "... update finished\n";
}

int main() {
    std::cout << "\n";

    std::thread reader8([]{ printNumber("Bjarne");});
    std::thread reader1([]{ printNumber("Scott");});
    std::thread reader2([]{ printNumber("Ritchie");});
    std::thread w1([]{ addToTeleBook("Scott", 1968);});
    std::thread reader3([]{ printNumber("Dijkstra");});
    std::thread reader4([]{ printNumber("Scott");});
    std::thread w2([]{ addToTeleBook("Bjarne", 1965);});
    std::thread reader5([]{ printNumber("Scott");});
    std::thread reader6([]{ printNumber("Ritchie");});
    std::thread reader7([]{ printNumber("Scott");});

    reader1.join();
    reader2.join();
    reader3.join();
    reader4.join();
    reader5.join();
    reader6.join();
    reader7.join();
    reader8.join();
    w1.join();
    w2.join();

    std::cout << "\n\nThe new telephone book is:\n";
    for(auto elem: teleBook) {
        std::cout << elem.first << ": " << elem.second << "\n";
    }
    std::cout << "\n";

    return 0;
}