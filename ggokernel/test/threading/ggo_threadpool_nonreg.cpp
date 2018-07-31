#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/threading/ggo_threadpool.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(threadpool, base)
{
  std::mutex m;
  ggo::threadpool pool(2);

  std::vector<std::future<void>> results;

  auto job = [&]()
  {
    m.lock();
    std::cout << "begin" << std::endl;
    m.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    m.lock();
    std::cout << "end" << std::endl;
    m.unlock();
  };

  std::cout << "start" << std::endl;

  results.emplace_back(pool.enqueue(job));
  results.emplace_back(pool.enqueue(job));

  for (auto & r : results)
  {
    r.wait();
  }

  std::cout << "join" << std::endl;
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(threadpool, many_jobs)
{
  std::mutex m;
  ggo::threadpool pool(2);

  std::vector<std::future<int>> results;

  auto job = [&](int i) -> int
  {
    return i;
  };

  for (int i = 0; i < 100; ++i)
  {
    results.emplace_back(pool.enqueue(job, i));
  }

  int sum = 0;
  for (auto & r : results)
  {
    sum += r.get();
  }

  GGO_CHECK_EQ(sum, 4950);
}
