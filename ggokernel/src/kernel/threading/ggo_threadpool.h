#ifndef __GGO_THREAD_POOL__
#define __GGO_THREAD_POOL__

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace ggo
{
  class threadpool final
  {
  public:
    threadpool(size_t threads_count);
    ~threadpool();

    template<class func_t, class... args_t>
    auto enqueue(func_t && f, args_t &&... args) -> std::future<typename std::invoke_result<func_t(args_t...)>::type>;

    auto threads_count() const { return workers.size(); }

    static void init_global(int threads_count = std::thread::hardware_concurrency());
    static threadpool * global();
    
  private:
    static std::unique_ptr<threadpool> _global;

    // need to keep track of threads so we can join them
    std::vector<std::thread> workers;
    // the task queue
    std::queue<std::function<void()>> tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;
  };

  // add new work item to the pool
  template<class func_t, class... args_t>
  auto threadpool::enqueue(func_t && f, args_t &&... args) -> std::future<typename std::invoke_result<func_t(args_t...)>::type>
  {
    using return_type = typename std::invoke_result<func_t(args_t...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<func_t>(f), std::forward<args_t>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(queue_mutex);

      tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
  }
}

#endif