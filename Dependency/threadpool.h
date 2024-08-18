#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>
#include"header.h"
using namespace std;
using namespace chrono;
////////////////////////
struct Task {
    uint priority;
    function<void()> fun;
    Task(uint priority_, const function<void()> &fun_);
    Task(const function<void()> &fun_);
    struct TaskPriority {
        bool operator()(Task &t1, Task &t2);
    };
};
class ThreadPool {
private:
    vector<thread*> WorkThread;
    priority_queue<Task, vector<Task>,Task::TaskPriority> TaskQueue;
    mutex lock;
    atomic<bool> run;
    condition_variable cv;
    void Process();

public:
    ThreadPool(uint thread_nums);
    void append(const Task&task);
    void append(const function<void()> &fun);
    bool empty();
    ~ThreadPool();
};
#endif // THREADPOOL_H
