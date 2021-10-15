//
// Created by piotr on 15/10/2021.
//

#ifndef COA_CLOCK_CLOCK_H_
#define COA_CLOCK_CLOCK_H_

#include <chrono>
#include <map>
#include <string>
#include <utility>

#define DEBUG_MODE false

#if DEBUG_MODE
#define START_CLOCK(x) Clock timer(x)
#define SAVE_TIMINGS(x) Clock::SaveToFile(x)
#else
#define START_CLOCK(x)
#define SAVE_TIMINGS(x)
#endif

class Clock {
 public:
  Clock() = delete;
  Clock(std::string clock_name);
  Clock(const Clock &other) = delete;
  Clock &operator=(const Clock &other) = delete;
  ~Clock();

  static void SaveToFile(const std::string &filename);

 protected:
  ///for no timings will simply store value in microseconds
  static std::map<std::string, long long unsigned int> timings_;
  std::string clock_name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> time_point_;
};

#endif//COA_CLOCK_CLOCK_H_
