#ifndef __ROUND_QUEUE_H_
#define __ROUND_QUEUE_H_

#include <opencv4/opencv2/opencv.hpp>
#include <mutex>
#include <condition_variable>


template<typename type, int length>
class RoundQueue {
private:
    type data[length];
    int head;
    int tail;
    std::condition_variable cond_not_empty;
    std::mutex lock;

public:
    RoundQueue<type, length>() : head(0), tail(0) {};

    constexpr int size() const {
        return length;
    };

    bool empty() const {
        return this->head == this->tail;
    };

    void push(const type &obj) {
        // std::cout << "Push function start to get a lock..." << std::endl;
        this->lock.lock();
        data[head] = obj;
        head = (head + 1) % length;
        if (head == tail) {
            tail = (tail + 1) % length;
        }
        this->lock.unlock();
        // std::cout << "Push function unlock successfully..." << std::endl;
        this->cond_not_empty.notify_one();
        // std::cout << "Push function notify other threads..." << std::endl;
    };

    bool pop(type &obj) {
        // std::cout << "Pop funtion try to get a lock..." << std::endl;
        std::unique_lock<std::mutex> locker(this->lock);
        // std::cout << "Pop funtion start to wait..." << std::endl;
        this->cond_not_empty.wait(
            locker, [&]() {return !this->empty(); }
        );
        obj = data[tail];
        tail = (tail + 1) % length;
        locker.unlock();
        // std::cout << "Pop funtion unlock..." << std::endl;
        return true;
    };

    bool clear(){
        this->head = this->tail;
    }
};

#endif