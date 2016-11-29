#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace ggo
{
  class signal
  {
  public:

    void wait()
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _wait_notification.wait(lock, [&] { return _notified; });
      _notified = false;
    }

    void notify()
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _notified = true;
      _wait_notification.notify_one();
    }

  private:

    bool                    _notified = false;
    std::mutex              _mutex;
    std::condition_variable _wait_notification;
  };
}
