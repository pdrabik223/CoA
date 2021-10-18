//
// Created by piotr on 15/10/2021.
//

#include "../a_star/a_star.h"
#include "../dijkstra/dijkstra.h"
#include "../maze/maze_generator.h"
#include "../plane/plane.h"
#include "../random_walk/random_walk_algorithm.h"
#include "pm_include.h"
#include <fstream>
void AStarRandom();

int main() {
  srand(time(NULL));

  AStarRandom();
  //  SAVE_TIMINGS("i_v0.txt");
  //  system("root");
  //  std::this_thread::sleep_for(std::chrono::seconds(3));
  //  system(".x root_incrementer.cpp ");
  //  system(".q");
  return 1;
}

void AStarRandom() {
  std::fstream output_file;
  output_file.open("../testing/Data.txt", std::ios::out);

  int min_maze_size = 10;
  int max_maze_size = 210;
  int maze_size_jump = 5;
  int no_tests = 100;

  for (int maze_size = min_maze_size; maze_size < max_maze_size; maze_size += maze_size_jump) {
    output_file << maze_size << '\t';
    double a_star_time_sum = 0;
    double dijkstra_time_sum = 0;
    double random_time_sum = 0;

    for (int i = 0; i < no_tests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size);
      maze_generator.GenCircularMaze();
      Plane test = maze_generator.GetPlane();
      //      test.SetCell({0, 0}, CellState::FINISH);
      //      test.SetCell({maze_size / 2, maze_size / 2}, CellState::START);

      AStar a_star(test);
      auto t_2 = std::chrono::steady_clock::now();
      auto path_2 = a_star.FindPath();
      a_star_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_2).count();

      Dijkstra dijkstra(test);
      auto t_1 = std::chrono::steady_clock::now();
      auto path = dijkstra.FindPath();
      dijkstra_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_1).count();

      RandomWalk random(test);
      auto t_3 = std::chrono::steady_clock::now();
      auto path_3 = random.FindPath();
      random_time_sum += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t_3).count();
    }

    output_file << (dijkstra_time_sum / no_tests) << '\t' << (a_star_time_sum / no_tests) << '\t' << (random_time_sum / no_tests) << '\n';
    printf("current maze size:%d\n", maze_size);
  }

  output_file.close();
}
