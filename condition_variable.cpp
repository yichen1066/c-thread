#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>
#include <stdlib.h>

std::mutex mu;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;

void Worker() {
  std::unique_lock<std::mutex> lock(mu);

  cv.wait(lock, [] {return ready; });

  std::cout << "main thread is processing" << std::endl;

  data += "已处理";

  processed = true;
  std::cout << "工作线程通知数据已处理完毕" << std::endl;

  lock.unlock();
  cv.notify_one();
}

int main() {
  std::thread worker(Worker);

  // 把数据发给工作线程
  {
    std::lock_guard<std::mutex> lock(mu);

    std::cout << "主线程正在准备数据..." << std::endl;


    data = "样本数据";
    ready = true;

    std::cout << "主线程通知数据已经准备完毕" << std::endl;
  }

  cv.notify_one();

  // 等待工作线程处理数据
  {
    std::unique_lock<std::mutex> lock(mu);
    cv.wait(lock, [] {return processed; });
  }

  std::cout << "回到主线程， 数据=" << data << std::endl;
  worker.join();
  getchar();

  return 0;
}