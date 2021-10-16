//
// Created by piotr on 15/10/2021.
//

#include "../a_star/a_star.h"
#include "../clock/tracer.h"
#include "../dijkstra/dijkstra.h"
#include "../maze/maze_generator.h"
#include "../plane/plane.h"
#include <fstream>
void AStarRandom();

int main() {
  srand(time(NULL));

  AStarRandom();
  SAVE_TIMINGS("release.txt");
  return 1;
}

void AStarRandom() {
  std::fstream output_file;
  output_file.open("../testing/RandomMaze.txt", std::ios::out);

  int min_maze_size = 10;
  int max_maze_size = 440;
  int maze_size_jump = 40;
  int no_tests = 50;

  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    output_file << maze_size << '\t';
    double a_star_time_sum = 0;
    double dijkstra_time_sum = 0;
    for (int i = 0; i < no_tests; ++i) {
      Plane maze(maze_size, maze_size, 10);

      maze.SetCell({0, 0}, CellState::FINISH);
      maze.SetCell({maze_size / 2, maze_size / 2}, CellState::START);

      AStar a_star(maze);
      auto t_2 = std::chrono::steady_clock::now();
      auto path_2 = a_star.FindPath();
      a_star_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_2).count();

      Dijkstra dijkstra(maze);
      auto t_1 = std::chrono::steady_clock::now();
      auto path = dijkstra.FindPath();
      dijkstra_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_1).count();
    }

    output_file << (dijkstra_time_sum / no_tests) << '\t' << (a_star_time_sum / no_tests) << '\n';
    printf("current maze size:%d\n", maze_size);
  }

  output_file.close();
}
