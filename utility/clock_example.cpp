//
// Created by piotr on 15/10/2021.
//
#include "clock.h"
#include <iostream>
#include <thread>
int Fun3();
int Fun2();
int Fun() {
  START_CLOCK("fun1");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  Fun2();
  return 7;
}

int Fun2() {
  START_CLOCK("fun2");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return 7;
}

int Fun3() {
  START_CLOCK("fun3");
  double k = 0;
  for (int i = 0; i < 10000; i++)
    k += sqrt(i * k);
  return 7;
}

int main() {

  Fun();

  std::cout << "clock";
  SAVE_TIMINGS("test.txt");
  return 9;
}