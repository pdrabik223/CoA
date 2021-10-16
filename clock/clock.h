//
// Created by piotr on 15/10/2021.
//

#ifndef COA_CLOCK_CLOCK_H
#define COA_CLOCK_CLOCK_H

#include <chrono>
#include <map>
#include <stack>
#include <string>
#include <utility>

#define DEBUG_MODE true
#define AUTO_FUNCTION_NAMES true

#if DEBUG_MODE
#if AUTO_FUNCTION_NAMES
#define START_CLOCK Clock timer(__FILE__, __func__)
#else
#define START_CLOCK(x) Clock timer(x)
#endif
#define SAVE_TIMINGS(x) Clock::SaveToFile(x)
#else

#define START_CLOCK(x)
#define SAVE_TIMINGS(x)
#endif

class Clock {
 public:
  Clock() = delete;
  Clock(std::string clock_name);
  Clock(const std::string &file_name, const std::string &function_name);
  Clock(const Clock &other) = delete;
  Clock &operator=(const Clock &other) = delete;
  ~Clock();
  void Stop();
  void Start();
  static void SaveToFile(const std::string &filename);

 protected:
  ///for no timings will simply store value in microseconds
  static std::map<std::string, long long unsigned int> timings_;
  static std::stack<Clock *> clocks_;
  std::string clock_name_;
  std::chrono::time_point<std::chrono::high_resolution_clock> time_point_;
};

#endif//COA_CLOCK_CLOCK_H_
