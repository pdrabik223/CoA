//
// Created by piotr on 15/10/2021.
//

#include "../maze/maze_generator.h"
#include "../path_search/path_search.h"
#include "pm_include.h"
#include "pr_file_format.h"
#include <array>
#include <fstream>
#include <utility>

void PerformTest();
#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()
#define INT(x) (int) Algorithm::x
void TestMe(std::array<unsigned, 4> &path_length,
            std::array<unsigned, 4> &path_misses,
            std::array<double, 4> &times,
            const Plane &plane);
std::vector<double> Average(std::array<double, 4> &values, int total_no_tests) {
  std::vector<double> output;
  output.reserve(values.size());
  for (auto &t : values)
    output.emplace_back(t / (double) total_no_tests);
  return output;
}

std::vector<double> Convert(const std::array<unsigned, 4> &values) {
  std::vector<double> output;
  output.reserve(values.size());
  for (int value : values)
    output.emplace_back((double) value);
  return output;
}
std::vector<double> Average(const std::array<unsigned, 4> &values, int total_no_tests) {
  std::vector<double> output;
  output.reserve(values.size());

  for (int value : values)
    output.emplace_back((double) value / (double) total_no_tests);

  return output;
}
std::vector<double> Average(const std::array<int, 4> &values, int total_no_tests) {
  std::vector<double> output;

  output.reserve(values.size());
  for (int value : values)
    output.emplace_back((double) value / (double) total_no_tests);

  return output;
}
std::vector<double> Average(const std::vector<std::array<unsigned, 4>> &path_lengths, int total_no_tests) {

  std::array<int, 4> averages{};
  for (auto p : path_lengths) {
    averages[0] += p[0];
    averages[1] += p[1];
    averages[2] += p[2];
    averages[3] += p[3];
  }

  return Average(averages, total_no_tests);
}
void Save(int plane_size, const std::array<double, 4> &average_values, const std::string &file_path, bool add_new_line) {

  std::fstream f(file_path, std::ios::app);
  f << plane_size << "\t";
  for (auto t : average_values)
    f << t << "\t";
  if (add_new_line) f << "\n";
  f.close();
}

int main() {
  // todo one executable to perform every test needed
  // todo time left timer simple in console timer that count's in seconds time left, of course every maze size switch timer updates

  // one executable creates whole directory of output files, correctly named
  // than at the end of test faze all that data is moved to

  srand(time(NULL));

  PerformTest();

  return 1;
}

void PerformTest() {

  PRFileFormat timings_file("Time comparison for Square Maze", "Maze area [j^2]", "Time [ #mus ]", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule"});
  PRFileFormat path_lengths_file("Found path length comparison for Square Maze", "Maze area [j^2]", "Average path length", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule"});
  PRFileFormat path_misses_file("Path misses comparison for Square Maze", "Maze area [j^2]", "Total sum of missed path", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule"});

  int min_maze_size = 10;
  int max_maze_size = 50;
  int maze_size_jump = 5;
  int no_tests = 100;

  std::vector<std::array<unsigned, 4>> path_lengths;
  std::array<unsigned, 4> path_length = {};
  std::array<unsigned, 4> path_misses = {};
  std::array<double, 4> times{};

  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    times.fill(0);
    path_misses.fill(0);

    for (int i = 0; i < no_tests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size);
      maze_generator.GenSquareMaze();

      TestMe(path_length, path_misses, times, maze_generator.GetPlane());

      path_lengths.push_back(path_length);
    }

    printf("\rcurrent maze size:%d", maze_size);

    timings_file.PushData(maze_size * maze_size, Average(times, no_tests));

    path_lengths_file.PushData(maze_size * maze_size, Average(path_lengths, no_tests));

    path_misses_file.PushData(maze_size * maze_size, Convert(path_misses));
  }
}
void TestMe(std::array<unsigned, 4> &path_length,
            std::array<unsigned, 4> &path_misses,
            std::array<double, 4> &times,
            const Plane &plane) {

  Dijkstra dijkstra(plane);
  AStar a_star(plane);
  RandomWalk random(plane);
  RHR right_hand_rule(plane);
  {
    auto time = T_START;
    path_length[INT(DIJKSTRA)] = dijkstra.FindPath().size();
    times[INT(DIJKSTRA)] += T_RECORD(time);
  }
  {
    auto time = T_START;
    path_length[INT(A_STAR)] = a_star.FindPath().size();
    times[INT(A_STAR)] += T_RECORD(time);

    if (path_length[(int) Algorithm::DIJKSTRA] != 0 and path_length[INT(A_STAR)] == 0)
      path_misses[INT(A_STAR)]++;
  }
  {
    auto time = T_START;
    path_length[INT(RANDOM_WALK)] = random.FindPath().size();
    times[INT(RANDOM_WALK)] += T_RECORD(time);

    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(RANDOM_WALK)] == 0)
      path_misses[INT(RANDOM_WALK)]++;
  }
  {
    auto time = T_START;
    path_length[INT(RIGHT_HAND_RULE)] = right_hand_rule.FindPath().size();
    times[INT(RIGHT_HAND_RULE)] += T_RECORD(time);

    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(RIGHT_HAND_RULE)] == 0)
      path_misses[INT(RIGHT_HAND_RULE)]++;
  }
}