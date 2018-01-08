#include "thread_pool.hpp"
#include <utility>

using namespace std;

void ThreadPool::setWorkerThreadCount(unsigned workerThreadCount) {
  workerThreads.clear();
  for (unsigned i = 0; i < workerThreadCount; i++)
          workerThreads.emplace_back(new Worker);
}

void ThreadPool::pushWorkToThread(unsigned threadIndex, std::function<void()> func) {
  workerThreads[threadIndex]->pushWork(move(func));
}

void ThreadPool::waitIdle() {
  for (auto &worker : workerThreads)
          worker->waitIdle();
}

ThreadPool::Worker::Worker() {
  workerThread = thread(&ThreadPool::Worker::threadEntry, this);
}

ThreadPool::Worker::~Worker() {
  if (workerThread.joinable()) {
    waitIdle();

    lock.lock();
    threadIsAlive = false;
    cond.notify_one();
    lock.unlock();

    workerThread.join();
  }
}

void ThreadPool::Worker::pushWork(std::function<void()> func) {
  lock_guard<mutex> holder{ lock };
  workQueue.push(move(func));
  cond.notify_one();
}

void ThreadPool::Worker::waitIdle() {
  unique_lock<mutex> holder{ lock };
  cond.wait(holder, [this] { return workQueue.empty(); });
}

void ThreadPool::Worker::threadEntry() {
  for (;;) {
    function<void()> *pWork = nullptr;
    {
      unique_lock<mutex> holder{ lock };
      cond.wait(holder, [this] { return !workQueue.empty() || !threadIsAlive; });
      if (!threadIsAlive) {
        break;
      }

      pWork = &workQueue.front();
    }

    (*pWork)();

    {
      lock_guard<mutex> holder{ lock };
      workQueue.pop();
      cond.notify_one();
    }
  }
}
