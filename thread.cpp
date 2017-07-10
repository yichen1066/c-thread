#include <thread>
#include <mutex>
#include <iostream>
#include <windows.h>

std::mutex mu;
class Sales {
public:
  Sales(int tickets);
  virtual ~Sales();

  virtual void Init(int tickets);

  void ThreadA();
  void ThreadB();

protected:
  int tickets_;
};

Sales::Sales(int tickets) {
  Init(tickets);
}

Sales::~Sales() {
}

void Sales::Init(int tickets) {
  tickets_ = tickets;

  std::thread a(&Sales::ThreadA, this);
  std::thread b(&Sales::ThreadB, this);

  a.detach();
  b.detach();

  std::cout << "In Main Thread" << std::endl;
}

void Sales::ThreadA() {
  while (true) {
//     std::mutex mu;
//     mu.lock();

    std::lock_guard<std::mutex> guard(mu);

    if (tickets_ > 0) {
      Sleep(1);
      std::cout << "A Sales:" << tickets_-- << std::endl;
//      mu.unlock();
    } else {
//      mu.unlock();
      break;
    }
  }
}

void Sales::ThreadB() {
  while (true) {
//     std::mutex mu;
//     mu.lock();
    std::lock_guard<std::mutex> guard(mu);

    if (tickets_ > 0) {
      Sleep(10);
      std::cout << "B Sales:" << tickets_-- << std::endl;

//      mu.unlock();
    } else {
//      mu.unlock();
      break;
    }
  }
}

int main() {
  Sales a(100);
  getchar();
}
