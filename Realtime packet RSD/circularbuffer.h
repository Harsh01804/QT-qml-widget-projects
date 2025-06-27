#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>

template<typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(size_t capacity)
        : buf(capacity), head(0), tail(0), full(false), stopping(false) {}

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mtx);
        buf[head] = item;
        if (full) {
            tail = (tail + 1) % buf.size();
        }
        head = (head + 1) % buf.size();
        full = (head == tail);
        cv.notify_one();
    }

   
    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{ return stopping || full || (head != tail); });
        if (stopping && !full && (head == tail)) {
            return T{};
        }
        T val = buf[tail];
        full = false;
        tail = (tail + 1) % buf.size();
        return val;
    }

    bool empty() const {
        std::unique_lock<std::mutex> lock(mtx);
        return (!full && head == tail);
    }

    void stop() {
        std::unique_lock<std::mutex> lock(mtx);
        stopping = true;
        cv.notify_all();
    }

private:
    std::vector<T> buf;
    size_t head, tail;
    bool full;
    bool stopping;
    mutable std::mutex mtx;
    std::condition_variable cv;
};
