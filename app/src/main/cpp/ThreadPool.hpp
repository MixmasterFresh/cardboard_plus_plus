#ifndef FRAMEWORK_THREAD_POOL_HPP
#define FRAMEWORK_THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
public:
  void setWorkerThreadCount(unsigned workerThreadCount);

  unsigned getWorkerThreadCount() const
  {
          return workerThreads.size();
  }

  void pushWorkToThread(unsigned threadIndex, std::function<void()> func);

  void waitIdle();

private:
  class Worker {
  public:
          Worker();
          ~Worker();

          void pushWork(std::function<void()> func);
          void waitIdle();

  private:
          std::thread workerThread;
          std::mutex lock;
          std::condition_variable cond;

          std::queue<std::function<void()>> workQueue;
          bool threadIsAlive = true;

          void threadEntry();
  };

  std::vector<std::unique_ptr<Worker>> workerThreads;
};

#endif
