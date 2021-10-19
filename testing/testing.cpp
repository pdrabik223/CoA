//
// Created by piotr on 15/10/2021.
//

#include "../maze/maze_generator.h"
#include "../path_search/path_search.h"
#include "../plane/plane.h"
#include "pm_include.h"
#include <array>
#include <fstream>
void PerformTest(const std::string &output_file_path);
#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()

void Save(int plane_size, int total_no_tests, std::array<double, 3> &timings, const std::string &file_path, bool add_new_line) {

  for (auto &t : timings)
    t = t / total_no_tests;

  std::fstream f(file_path, std::ios::app);
  f << plane_size << "\t";
  for (auto t : timings)
    f << t << "\t";
  if (add_new_line) f << "\n";
  f.close();

  timings.fill(0);
}

int main() {
  srand(time(NULL));

  PerformTest("../testing/test.txt");

  return 1;
}

void PerformTest(const std::string &output_file_path) {

  std::fstream f(output_file_path, std::ofstream::out | std::ofstream::trunc);
  f.close();

  int min_maze_size = 10;
  int max_maze_size = 50;
  int maze_size_jump = 5;
  int no_tests = 100;

  std::array<double, 3> times;
  times.fill(0);
  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    for (int i = 0; i < no_tests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size);
      maze_generator.GenRandomMaze(10);

      AStar a_star(maze_generator.GetPlane());
      RandomWalk random(maze_generator.GetPlane());
      Dijkstra dijkstra(maze_generator.GetPlane());
      {
        auto t_0 = T_START;
        a_star.FindPath();
        times[0] += T_RECORD(t_0);
      }
      {
        auto t_1 = T_START;
        dijkstra.FindPath();
        times[1] += T_RECORD(t_1);
      }
      {
        auto t_2 = T_START;
        random.FindPath();
        times[2] += T_RECORD(t_2);
      }
    }

    printf("\rcurrent maze size:%d", maze_size);
    Save(maze_size * max_maze_size, no_tests, times, output_file_path, maze_size + maze_size_jump < max_maze_size);
  }
}
