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
void TimePlane(std::vector<unsigned> &path_length,
               std::vector<unsigned> &path_misses,
               std::vector<double> &times,
               const Plane &plane);

std::vector<double> Average(const std::vector<double> &values, int total_no_tests) {
  std::vector<double> output;
  output.reserve(values.size());
  for (auto &t : values)
    output.emplace_back(t / (double) total_no_tests);
  return output;
}

std::vector<double> Convert(const std::vector<unsigned> &values) {
  std::vector<double> output;
  output.reserve(values.size());
  for (int value : values)
    output.emplace_back((double) value);
  return output;
}
std::vector<double> Average(const std::vector<unsigned> &values, int total_no_tests) {
  std::vector<double> output;
  output.reserve(values.size());

  for (int value : values)
    output.emplace_back((double) value / (double) total_no_tests);

  return output;
}
std::vector<double> Average(const std::vector<int> &values, int total_no_tests) {
  std::vector<double> output;

  output.reserve(values.size());
  for (int value : values)
    output.emplace_back((double) value / (double) total_no_tests);

  return output;
}
std::vector<double> Average(const std::vector<std::vector<unsigned>> &path_lengths, int total_no_tests) {

  std::vector<double> averages;
  for (int i = 0; i < path_lengths.begin()->size(); i++) averages.push_back(0);

  for (auto p : path_lengths) {
    for (int i = 0; i < p.size(); i++)
      averages[i] += p[i];
  }

  return Average(averages, total_no_tests);
}
void Save(int plane_size, const std::vector<double> &average_values, const std::string &file_path, bool add_new_line) {

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

  PRFileFormat timings_file("Time comparison for Square Maze", "Maze area [j^2]", "Time [ #mus ]", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule", "Deep First", "Greedy Deep First"});
  PRFileFormat path_lengths_file("Found path length comparison for Square Maze", "Maze area [j^2]", "Average path length", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule", "Deep First", "Greedy Deep First"});
  PRFileFormat path_misses_file("Path misses comparison for Square Maze", "Maze area [j^2]", "Total sum of missed path", {"Dijkstra", "A*", "Random Walk", "Right Hand Rule", "Deep First", "Greedy Deep First"});

  int min_maze_size = 10;
  int max_maze_size = 50;
  int maze_size_jump = 5;
  int no_tests = 100;

  std::vector<std::vector<unsigned>> path_lengths;

  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {

    std::vector<unsigned> path_length = {};
    std::vector<unsigned> path_misses = {};
    std::vector<double> times{};

    for (int i = 0; i < (int) Algorithm::SIZE; i++) {
      path_length.push_back(0);
      path_misses.push_back(0);
      times.push_back(0.);
    }

    for (int i = 0; i < no_tests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size, MazeType::SQUARE_MAZE);

      TimePlane(path_length, path_misses, times, maze_generator.GetPlane());

      path_lengths.push_back(path_length);
    }

    printf("\rcurrent maze size:%d", maze_size);

    timings_file.PushData(maze_size * maze_size, Average(times, no_tests));

    path_lengths_file.PushData(maze_size * maze_size, Average(path_lengths, no_tests));

    path_misses_file.PushData(maze_size * maze_size, Convert(path_misses));
  }
}

/// generates time-to-solve data for given plane
/// \param path_length
/// \param path_misses
/// \param times timings output
/// \param plane tested maze
void TimePlane(std::vector<unsigned> &path_length,
               std::vector<unsigned> &path_misses,
               std::vector<double> &times,
               const Plane &plane) {

  Dijkstra dijkstra(plane);
  AStar a_star(plane);
  RandomWalk random(plane);
  RHR right_hand_rule(plane);
  DepthFirst depth_first(plane);
  GreedyBestFirst greedy_best_first(plane);
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
{
  auto time = T_START;
  path_length[INT(DEPTH_FIRST)] = depth_first.FindPath().size();
  times[INT(DEPTH_FIRST)] += T_RECORD(time);

  if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(DEPTH_FIRST)] == 0)
    path_misses[INT(DEPTH_FIRST)]++;
}
{
  auto time = T_START;
  path_length[INT(GREEDY_BEST_FIRST)] = greedy_best_first.FindPath().size();
  times[INT(GREEDY_BEST_FIRST)] += T_RECORD(time);

  if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(GREEDY_BEST_FIRST)] == 0)
    path_misses[INT(GREEDY_BEST_FIRST)]++;
}
}