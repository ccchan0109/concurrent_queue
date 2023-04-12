#include <thread>
#include <queue>

template<typename T>
class ConcurrentQueue : public std::queue<T> {
public:
    template<typename... Args>
    ConcurrentQueue(Args &&... args) : std::queue<T>(std::forward<Args>(args)...) {}

    using std::queue<T>::emplace;
    using std::queue<T>::push;
    using std::queue<T>::pop;
    using std::queue<T>::front;
    using std::queue<T>::back;
    using std::queue<T>::size;
    using std::queue<T>::empty;

    template<typename... Args>
    void emplace(Args&&... args) {
        std::lock_guard lock(mtx);
        std::queue<T>::emplace(std::forward<Args>(args)...);
    }

    void push(const T &val) {
        std::lock_guard lock(mtx);
        std::queue<T>::push(val);
    }

    void pop() {
        // here designing the behavior of popping empty queue to nothing happen
        std::lock_guard lock(mtx);
        if (!std::queue<T>::empty()) {
            std::queue<T>::pop();
        }
    }

    const T& front() const {
        std::lock_guard lock(mtx);
        return std::queue<T>::front();
    }

    T& front() {
        std::lock_guard lock(mtx);
        return std::queue<T>::front();
    }

    const T& back() const {
        std::lock_guard lock(mtx);
        return std::queue<T>::back();
    }

    T& back() {
        std::lock_guard lock(mtx);
        return std::queue<T>::back();
    }

private:
    // made the mtx mutable, since the front() method needs to acquire a lock
    // on the mutex even though it is a const method
    mutable std::mutex mtx;
};
