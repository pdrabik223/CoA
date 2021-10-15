//
// Created by piotr on 15/10/2021.
//
#include "clock.h"
#include <iostream>
int Fun() {
  START_CLOCK("fun1");
  double k = 0;
  for (int i = 0; i < 10000; i++)
    k += sqrt(i * k);
  return 7;
}

int Fun2() {
  START_CLOCK("fun2");
  double k = 0;
  for (int i = 0; i < 1000; i++)
    k += sqrt(i * k);
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
  Fun2();
  Fun3();
  Fun3();
  Fun3();

  std::cout << "clock";
  SAVE_TIMINGS("test.txt");
  return 9;
}