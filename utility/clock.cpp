//
// Created by piotr on 15/10/2021.
//

#include "clock.h"
#include <fstream>
std::map<std::string, long long unsigned int> Clock::timings_;
std::stack<Clock *> Clock::clocks_;

Clock::Clock(std::string clock_name) : clock_name_(std::move(clock_name)) {
  if (!clocks_.empty()) clocks_.top()->Stop();
  clocks_.push(this);
  Start();
}
Clock::~Clock() {
  Stop();
  clocks_.pop();
  if (!clocks_.empty()) clocks_.top()->Start();
}
void Clock::SaveToFile(const std::string &filename) {
  std::fstream output_file;
  output_file.open("../testing/" + filename, std::ios::out);

  for (auto &t : timings_)
    output_file << t.first << "\t" << t.second << "\n";
  output_file.close();
}
void Clock::Stop() {
  auto time_point_2 = std::chrono::high_resolution_clock::now();
  if (timings_.find(clock_name_) != timings_.end())
    timings_[clock_name_] += std::chrono::duration_cast<std::chrono::microseconds>(time_point_2 - time_point_).count();
  else
    timings_[clock_name_] = std::chrono::duration_cast<std::chrono::microseconds>(time_point_2 - time_point_).count();
}
void Clock::Start() {
  time_point_ = std::chrono::high_resolution_clock::now();
}
