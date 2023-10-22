#pragma once
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>

template <class T>
class LockQueue
{
public:
    void Push(const T &value)
    {
        std::lock_guard<std::mutex> lock(mtx);
        que.push(value);
        m_condvariable.notify_one();
    }
    T Pop()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while (que.empty())
        {
            m_condvariable.wait(lock);
        }
        T r= que.front();
        que.pop();
        return r;
    }
private:
    std::queue<T> que;
    std::mutex mtx;
    std::condition_variable m_condvariable;
};


