#pragma once

#include <chrono>
#include <iostream>

using std::cout;
using std::endl;

static unsigned long const REPEAT_COUNT = 1'000;

template <typename Function>
inline auto time_run(Function && function) {
  auto start = std::chrono::high_resolution_clock::now();

  for(auto i = REPEAT_COUNT; i > 0; --i) {
    function();
  }

  auto end = std::chrono::high_resolution_clock::now();

  return (end - start) / REPEAT_COUNT;
}


inline auto print_duration(std::string name, std::chrono::nanoseconds duration) {

  cout << name << " duration: " << duration.count() << " ns" << endl;

}

inline auto print_duration_diff(std::string first_name, std::chrono::nanoseconds first_duration,
                                std::string second_name, std::chrono::nanoseconds second_duration) {

  cout << second_name << " / " << first_name << " : " << second_duration / first_duration << endl;
}