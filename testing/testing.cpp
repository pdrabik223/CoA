//
// Created by piotr on 15/10/2021.
//

#include "../a_star/a_star.h"
#include "../dijkstra/dijkstra.h"
#include "../maze/maze_generator.h"
#include "../plane/plane.h"
#include <fstream>

void AStarCMaze();
void AStarRandom();
void AStarMaze();
void MessageMe(int maze_nr, const std::string &maze_type, const std::string &algorithm, size_t time, int path_length) {
  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}
int main() {
  srand(time(NULL));
  AStarRandom();
  return 1;
}

void AStarRandom() {
  std::fstream output_file;
  output_file.open("../testing/RandomMaze.txt", std::ios::out);

  int min_maze_size = 10;
  int max_maze_size = 210;
  int maze_size_jump = 10;
  int no_tests = 50;

  //  output_file << "no different maze sizes: " << (max_maze_size - min_maze_size) / 5 << "\n average time" << no_tests << "\n";
  //  output_file << "maze size \\ dijkstra time [ms] \t A* time \n";
  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    output_file << maze_size << '\t';
    double a_star_time_sum = 0;
    double dijkstra_time_sum = 0;
    for (int i = 0; i < no_tests; ++i) {
      Plane maze(maze_size, maze_size, 0);

      maze.SetCell({0, 0}, CellState::FINISH);
      maze.SetCell({maze_size / 2, maze_size / 2}, CellState::START);

      Dijkstra dijkstra(maze);
      auto t_1 = std::chrono::steady_clock::now();
      auto path = dijkstra.FindPath();
      dijkstra_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_1).count();

      AStar a_star(maze);
      auto t_2 = std::chrono::steady_clock::now();
      auto path_2 = a_star.FindPath();
      a_star_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_2).count();
    }
    output_file << (dijkstra_time_sum / no_tests) << '\t' << (a_star_time_sum / no_tests) << '\n';
    printf("current maze size:%d\n", maze_size);
  }

  output_file.close();
}
void AStarMaze() {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    AStar cos(sic.GetPlane());

    auto t1 = std::chrono::steady_clock::now();
    auto path = cos.FindPath();
    MessageMe(i, "square maze", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
}

void AStarCMaze() {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    AStar cos(sic.GetPlane());

    auto t1 = std::chrono::steady_clock::now();
    auto path = cos.FindPath();
    MessageMe(i, "circular maze", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
}
