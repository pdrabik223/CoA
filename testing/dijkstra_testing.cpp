//
// Created by piotr on 15/10/2021.
//

#include "../brute_force/brute_force.h"
#include "../dijkstra/dijkstra.h"
#include "../maze/maze_generator.h"
#include "../plane/plane.h"
#include <fstream>

void DijkstraCMaze();
void DijkstraRandom();
void DijkstraMaze();
void MessageMe(int maze_nr, const std::string &maze_type, const std::string &algorithm, size_t time, int path_length) {
  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}
int main() {
  srand(time(NULL));
  DijkstraRandom();
  return 1;
}

void DijkstraRandom() {
  std::fstream output_file;
  output_file.open("../testing/DijkstraRandom.txt", std::ios::out);

  int min_maze_size = 10;
  int max_maze_size = 200;
  int maze_size_jump = 10;
  int no_tests = 50;

  //  output_file << "no different maze sizes: " << (max_maze_size - min_maze_size) / 5 << "\n average time" << no_tests << "\n";
  //  output_file << "maze size \\ dijkstra time [ms] \t A* time \n";
  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    output_file << maze_size << '\t';
    double a_star_time_sum = 0;
    double dijkstra_time_sum = 0;
    for (int i = 0; i < no_tests; ++i) {
      Plane sic(maze_size, maze_size, 0);

      sic.SetCell({0, 0}, CellState::FINISH);
      sic.SetCell({maze_size / 2, maze_size / 2}, CellState::START);

      BruteForce cos(sic);
      Dijkstra cos_2(sic);
      auto t_1 = std::chrono::steady_clock::now();
      auto path = cos.FindPath();
      if (!path.empty())
        dijkstra_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_1).count();

      auto t_2 = std::chrono::steady_clock::now();
      auto path_2 = cos_2.FindPath();
      if (!path_2.empty())
        a_star_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_2).count();
    }
    output_file << (dijkstra_time_sum / no_tests) << '\t' << (a_star_time_sum / no_tests) << '\n';
    printf("current maze size:%d\n", maze_size);
  }

  output_file.close();
}
void DijkstraMaze() {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    Dijkstra cos(sic.GetPlane());

    auto t1 = std::chrono::steady_clock::now();
    auto path = cos.FindPath();
    MessageMe(i, "square maze", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
}

void DijkstraCMaze() {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    Dijkstra cos(sic.GetPlane());

    auto t1 = std::chrono::steady_clock::now();
    auto path = cos.FindPath();
    MessageMe(i, "circular maze", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
}
