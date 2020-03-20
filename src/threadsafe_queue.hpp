#pragma once
#include <mutex>
#include <queue>


// https://codereview.stackexchange.com/questions/149676/writing-a-thread-safe-queue-in-c

// Note: pointers passed into the SafeQueue are not owned by the queue, they
// must be managed and deleted externally. For this use-case, the pointers will
// not be deleted while the queue is in use.
template<class T>
class SafeQueue {

  private:

    std::queue<T*> q;
    std::mutex m;

  public:

    SafeQueue() {}

    void push(T* elem)
    {
      if(elem == nullptr)
        return;

      std::lock_guard<std::mutex> l(this->m);
      q.push(elem);
    }

    T* pop()
    {

      T* elem = nullptr;

      {
        std::lock_guard<std::mutex> l(this->m);
        if(!q.empty()) {
          elem = q.front();
          q.pop();
        }
      }

      return elem;

    }

    void clear()
    {
      this->q = std::queue<T*>();
    }

};
