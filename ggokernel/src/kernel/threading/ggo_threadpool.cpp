#include "ggo_threadpool.h"

namespace ggo
{
  std::unique_ptr<threadpool> threadpool::_global;

  // the constructor just launches some amount of workers
  threadpool::threadpool(size_t threads_count)
  {
    for (size_t i = 0; i < threads_count; ++i)
    {
      workers.emplace_back([this]
      {
        for (;;)
        {
          std::function<void()> task;

          {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
            if (this->stop && this->tasks.empty())
              return;
            task = std::move(this->tasks.front());
            this->tasks.pop();
          }

          task();
        }
      });
    }
  }

  // the destructor joins all threads
  threadpool::~threadpool()
  {
    {
      std::unique_lock<std::mutex> lock(queue_mutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
    {
      worker.join();
    }
  }

  void threadpool::init_global(int threads_count)
  {
    _global = std::make_unique<threadpool>(threads_count);
  }

  threadpool * threadpool::global()
  {
    return _global.get();
  }
}
