//
// Created by piotr on 15/10/2021.
//

#include "../maze_generator/maze_generator.h"
#include "../path_search/path_search.h"
#include "pm_include.h"
#include "pr_file_format.h"
#include <array>
#include <fstream>
#include <utility>

void PlaneSizeCorrelation(MazeType maze_type);
void MazeComplexityCorrelation(MazeType maze_type);
#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()
#define INT(x) (int) Algorithm::x

void PlaneSizeCorrPerformTestsForGivenPlane(std::vector<unsigned> &path_length,
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
std::vector<double> RelativeToDLS(std::vector<double> path_lengths) {

  std::vector<double> relative = {0};

  for (int i = 1; i < path_lengths.size(); i++)
    relative.push_back(path_lengths[i] - path_lengths[0]);

  return relative;
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

  //  MazeComplexityCorrelation(MazeType::SQUARE_MAZE);
  //  PlaneSizeCorrelation(MazeType::SNAIL_MAZE);
  PlaneSizeCorrelation(MazeType::EMPTY_PLANE);
  //  PlaneSizeCorrelation(MazeType::CIRCUlAR_MAZE);
  return 1;
}
void MazeComplexityCorrelation(MazeType maze_type) {
  if (maze_type != MazeType::SQUARE_MAZE) throw "bad Maze";

  std::vector<std::string> dataset_labels;
  dataset_labels.reserve((int) Algorithm::SIZE);
  for (int i = 0; i < (int) Algorithm::SIZE; i++)
    dataset_labels.push_back(ToString((Algorithm(i))));

  PRFileFormat timings_file("Time complexity in MazeComplexityCorrelation for " + ToString(maze_type), "Maze area [j^2]", "Time [ #mus ]", dataset_labels);
  PRFileFormat path_lengths_file("Found path in MazeComplexityCorrelation length for " + ToString(maze_type), "Maze area [j^2]", "Average path length", dataset_labels);
  PRFileFormat path_misses_file("Path misses in MazeComplexityCorrelation for " + ToString(maze_type), "Maze area [j^2]", "Total sum of missed path", dataset_labels);
  PRFileFormat relative_path_lengths_file("Found path length relative to DLS in MazeComplexityCorrelation for " + ToString(maze_type), "Maze area [j^2]", "Difference in path lengths", dataset_labels);

  const int kMazeSize = 100;
  const int kMazeMinCavitySize = 5;
  const int kMazeMaxCavitySize = 50;
  const int kMazeCavitySizeJump = 5;
  const int kNoTests = 100;

  std::vector<std::vector<unsigned>> path_lengths;

  for (int cavity_size = kMazeMaxCavitySize; cavity_size > kMazeMinCavitySize; cavity_size -= kMazeCavitySizeJump) {

    std::vector<unsigned> path_length = {};
    std::vector<unsigned> path_misses = {};
    std::vector<unsigned> relative_path_length = {};

    std::vector<double> times{};
    for (int i = 0; i < (int) Algorithm::SIZE; i++) {
      path_length.push_back(0);
      path_misses.push_back(0);
      times.push_back(0.);
    }

    for (int i = 0; i < kNoTests; ++i) {

      MazeGenerator maze_generator(kMazeSize, kMazeSize, {1, {cavity_size, cavity_size}, cavity_size * cavity_size});

      PlaneSizeCorrPerformTestsForGivenPlane(path_length, path_misses, times, maze_generator.GetPlane());

      path_lengths.push_back(path_length);
    }

    printf("current cavity size:%d\n", cavity_size);

    timings_file.PushData(cavity_size * cavity_size, Average(times, kNoTests));

    path_lengths_file.PushData(cavity_size * cavity_size, Average(path_lengths, kNoTests));

    path_misses_file.PushData(cavity_size * cavity_size, Convert(path_misses));

    relative_path_lengths_file.PushData(cavity_size * cavity_size, RelativeToDLS(Average(path_lengths, kNoTests)));
  }
}
void PlaneSizeCorrelation(MazeType maze_type) {

  std::vector<std::string> dataset_labels;
  dataset_labels.reserve((int) Algorithm::SIZE);
  for (int i = 0; i < (int) Algorithm::SIZE; i++)
    dataset_labels.push_back(ToString((Algorithm(i))));

  PRFileFormat timings_file("Time complexity for " + ToString(maze_type), "Maze area [j^2]", "Time [ #mus ]", dataset_labels);
  PRFileFormat path_lengths_file("Found path length for " + ToString(maze_type), "Maze area [j^2]", "Average path length", dataset_labels);
  PRFileFormat path_misses_file("Path misses for " + ToString(maze_type), "Maze area [j^2]", "Total sum of missed path", dataset_labels);
  PRFileFormat relative_path_lengths_file("Found path length relative to DLS for " + ToString(maze_type), "Maze area [j^2]", "Difference in path lengths", dataset_labels);

  const int kMinMazeSize = 10;
  const int kMaxMazeSize = 50;
  const int kMazeSizeJump = 5;
  const int kNoTests = 100;

  std::vector<std::vector<unsigned>> path_lengths;

  for (int maze_size = kMinMazeSize; maze_size < kMaxMazeSize; maze_size += kMazeSizeJump) {

    std::vector<unsigned> path_length = {};
    std::vector<unsigned> path_misses = {};
    std::vector<unsigned> relative_path_length = {};

    std::vector<double> times{};

    for (int i = 0; i < (int) Algorithm::SIZE; i++) {
      path_length.push_back(0);
      path_misses.push_back(0);
      times.push_back(0.);
    }

    for (int i = 0; i < kNoTests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size, maze_type);

      PlaneSizeCorrPerformTestsForGivenPlane(path_length, path_misses, times, maze_generator.GetPlane());

      path_lengths.push_back(path_length);
    }

    printf("\rcurrent maze size:%d", maze_size);

    timings_file.PushData(maze_size * maze_size, Average(times, kNoTests));

    path_lengths_file.PushData(maze_size * maze_size, Average(path_lengths, kNoTests));

    path_misses_file.PushData(maze_size * maze_size, Convert(path_misses));

    relative_path_lengths_file.PushData(maze_size * maze_size, RelativeToDLS(Average(path_lengths, kNoTests)));
  }
}

int id = 0;
/// generates time-to-solve data for given plane
/// \param path_length
/// \param path_misses
/// \param times timings output
/// \param plane tested maze
void PlaneSizeCorrPerformTestsForGivenPlane(std::vector<unsigned> &path_length,
                                            std::vector<unsigned> &path_misses,
                                            std::vector<double> &times,
                                            const Plane &plane) {

  for (int i = 0; i < (int) Algorithm::SIZE; i++) {

    PathSearch engine(plane, (Algorithm) i);
    auto time = T_START;
    path_length[i] = engine.FindPath().size();
    times[i] += T_RECORD(time);

    if (path_length[(int) Algorithm::DIJKSTRA] != 0 and path_length[i] == 0)
      path_misses[i]++;
  }

  //
  //  Dijkstra dijkstra(plane);
  //  AStar a_star(plane);
  //  RandomWalk random(plane);
  //  DepthFirst depth_first(plane);
  //  GreedyBestFirst greedy_best_first(plane);
  //  GreedyPDistance greedy_p_distance(plane);
  //
  //  {
  //    auto time = T_START;
  //    path_length[INT(DIJKSTRA)] = dijkstra.FindPath().size();
  //    times[INT(DIJKSTRA)] += T_RECORD(time);
  //  }
  //  {
  //    auto time = T_START;
  //    path_length[INT(A_STAR)] = a_star.FindPath().size();
  //    times[INT(A_STAR)] += T_RECORD(time);
  //
  //    if (path_length[(int) Algorithm::DIJKSTRA] != 0 and path_length[INT(A_STAR)] == 0)
  //      path_misses[INT(A_STAR)]++;
  //  }
  //  {
  //    auto time = T_START;
  //    path_length[INT(RANDOM_WALK)] = random.FindPath().size();
  //    times[INT(RANDOM_WALK)] += T_RECORD(time);
  //
  //    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(RANDOM_WALK)] == 0)
  //      path_misses[INT(RANDOM_WALK)]++;
  //  }
  //  {
  //    auto time = T_START;
  //    path_length[INT(DEPTH_FIRST)] = depth_first.FindPath().size();
  //    times[INT(DEPTH_FIRST)] += T_RECORD(time);
  //
  //    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(DEPTH_FIRST)] == 0)
  //      path_misses[INT(DEPTH_FIRST)]++;
  //  }
  //  {
  //    auto time = T_START;
  //    path_length[INT(GREEDY_BEST_FIRST)] = greedy_best_first.FindPath().size();
  //    times[INT(GREEDY_BEST_FIRST)] += T_RECORD(time);
  //
  //    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(GREEDY_BEST_FIRST)] == 0)
  //      path_misses[INT(GREEDY_BEST_FIRST)]++;
  //  }
  //  {
  //    auto time = T_START;
  //    path_length[INT(GREEDY_P_DISTANCE)] = greedy_p_distance.FindPath().size();
  //    times[INT(GREEDY_P_DISTANCE)] += T_RECORD(time);
  //
  //    if (path_length[INT(DIJKSTRA)] != 0 and path_length[INT(GREEDY_P_DISTANCE)] == 0)
  //      path_misses[INT(GREEDY_P_DISTANCE)]++;
  //  }
}
