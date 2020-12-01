/*
    Packaged Tasks executing heavy computations in parallel.

    © Denis Shelemekh, 2020
*/

#include <utility>
#include <future>
#include <iostream>
#include <thread>
#include <deque>

using ReturnType = long long;
using InputType = const long;
using PackagedTaskType = std::packaged_task<ReturnType(InputType, InputType)>;

class SumUp {
public:
    ReturnType operator()(InputType beg, InputType end) noexcept {
        ReturnType sum {0};
        for(auto i = beg; i < end; ++i) {
            sum += i;
        }
        return sum;
    }
};

int main() {
    std::cout << "\n";

    SumUp sumUp1;
    SumUp sumUp2;
    SumUp sumUp3;
    SumUp sumUp4;

    PackagedTaskType sumTask1(sumUp1);
    PackagedTaskType sumTask2(sumUp2);
    PackagedTaskType sumTask3(sumUp3);
    PackagedTaskType sumTask4(sumUp4);

    auto sumResult1 = sumTask1.get_future();
    auto sumResult2 = sumTask2.get_future();
    auto sumResult3 = sumTask3.get_future();
    auto sumResult4 = sumTask4.get_future();

    std::deque<PackagedTaskType> allTasks;
    allTasks.push_back(std::move(sumTask1));
    allTasks.push_back(std::move(sumTask2));
    allTasks.push_back(std::move(sumTask3));
    allTasks.push_back(std::move(sumTask4));

    long begin = 1, increment = 100'000'000, end = begin + increment;

    while(!allTasks.empty()) {
        PackagedTaskType task = std::move(allTasks.front());
        allTasks.pop_front();
        std::thread sumThread(std::move(task), begin, end);
        begin = end;
        end += increment;
        sumThread.detach();
    }

    auto sum = sumResult1.get() + sumResult2.get() + sumResult3.get() + sumResult4.get();

    std::cout << "sum = " << sum << "\n\n";

    return 0;
}