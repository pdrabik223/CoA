//
// Created by piotr on 04/02/2022.
//

#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "../maze_generator/maze_generator.h"
#include "../path_search/path_search.h"

#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()
#define INT(x) (int) Algorithm::x

void MazeComplexityCorrelation(MazeType maze_type, Neighbourhood neighbourhood);
void PlaneSizeCorrelation(MazeType maze_type, Neighbourhood neighbourhood);
void PlaneSizeCorrPerformTestsForGivenPlane(std::vector<unsigned> &path_length, std::vector<double> &times, const Plane &plane, Neighbourhood neighbourhood);

double Average(const std::vector<double> &values) {
  double output = 0.0;
  for (auto &t : values)
    output += t;
  return output / values.size();
}

//std::vector<double> Convert(unsigned &values) {
//  std::vector<double> output;
//  output.reserve(values.size());
//  for (int value : values)
//    output.emplace_back((double) value);
//  return output;
//}
//std::vector<double> Average(const std::vector<unsigned> &values, unsigned no_tests) {
//  std::vector<double> output(values.size(), 0.0);
//
//  for (int i = 0; i < output.size(); i++)
//    output[i] = values[i] / no_tests;
//
//  return output;
//}
#define DRAWALL(i) \
  i.Draw();        \
  c->Print(std::string((i).GetName() + ".png").c_str());

int main() {

  TApplication app("app", nullptr, nullptr);
  PlaneSizeCorrelation(MazeType::SNAIL_MAZE, Neighbourhood::MOORE);
  PlaneSizeCorrelation(MazeType::SNAIL_MAZE, Neighbourhood::VON_NEUMAN);
  PlaneSizeCorrelation(MazeType::EMPTY_PLANE, Neighbourhood::MOORE);
  PlaneSizeCorrelation(MazeType::EMPTY_PLANE, Neighbourhood::VON_NEUMAN);
  //  app.Run();

  return 0;
}
void PlaneSizeCorrelation(MazeType maze_type, Neighbourhood neighbourhood) {

  TMultiGraph timings;
  timings.SetName(std::string("Execution time vs maze size for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood").c_str());
  timings.SetTitle(std::string("Execution time vs maze size for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood;Maze area [j^2];Time [ #mus ]").c_str());

  TMultiGraph path_lengths;
  path_lengths.SetName(std::string("Path length dependency on maze complexity for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood").c_str());
  path_lengths.SetTitle(std::string("Path length dependency on maze complexity for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood;Maze area [j^2];Average path length [arb. units]").c_str());

  std::vector<TGraph> time_g((int) Algorithm::SIZE);
  std::vector<TGraph> path_length_g((int) Algorithm::SIZE);

  int color = 4;
  for (int i = 0; i < (int) Algorithm::SIZE; i++) {
    path_length_g[i].SetName(std::string(ToString(Algorithm(i))).c_str());
    time_g[i].SetName(ToString(Algorithm(i)).c_str());
    path_length_g[i].SetLineColor(color);
    path_length_g[i].SetLineWidth(3);
    time_g[i].SetLineWidth(3);
    time_g[i].SetLineColor(color);
    color++;
  }

  const int kMinMazeSize = 15;
  const int kMaxMazeSize = 100;
  const int kMazeSizeJump = 10;
  const int kNoTests = 10;

  for (int maze_size = kMinMazeSize; maze_size < kMaxMazeSize; maze_size += kMazeSizeJump) {

    std::vector<unsigned> path_length((int) Algorithm::SIZE, 0);
    std::vector<double> times((int) Algorithm::SIZE, .0);

    for (int i = 0; i < kNoTests; ++i) {
      MazeGenerator maze_generator(maze_size, maze_size, {1, {10, 10}, 10 * 10});
      PlaneSizeCorrPerformTestsForGivenPlane(path_length, times, maze_generator.GetPlane(), neighbourhood);
    }

    for (int i = 0; i < (int) Algorithm::SIZE; i++) {
      path_length_g[i].AddPoint(maze_size, (double) path_length[i] / kNoTests);
      time_g[i].AddPoint(maze_size, times[i] / kNoTests);
    }
    printf("current maze size:%d\n", maze_size);
  }
  for (int i = 0; i < (int) Algorithm::SIZE; i++) {
    path_lengths.Add(&path_length_g[i]);
    timings.Add(&time_g[i]);
  }

  //  TApplication app("app", nullptr, nullptr);
  TCanvas *c = new TCanvas("canvas", "NeuralNets", 10, 10, 800, 600);
  path_lengths.Draw("AC");
  //  c->Draw();
  c->BuildLegend();
  c->Print(std::string("../root_enabled/Execution time vs maze size for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood.png").c_str());
  delete c;

  TCanvas *c1 = new TCanvas("canvas", "NeuralNets", 10, 10, 800, 600);
  timings.Draw("AC");
  c1->BuildLegend();
  c1->Print(std::string("../root_enabled/Path length dependency on maze complexity for " + ToString(maze_type) + " in " + ToString(neighbourhood) + " neighbourhood.png").c_str());
  delete c1;
  //  app.Run();
}

/// generates time-to-solve data for given plane
/// \param path_length
/// \param path_misses
/// \param times timings output
/// \param plane tested maze
void PlaneSizeCorrPerformTestsForGivenPlane(std::vector<unsigned> &path_length, std::vector<double> &times, const Plane &plane, Neighbourhood neighbourhood) {

  for (int i = 0; i < (int) Algorithm::SIZE; i++) {
    PathSearch engine(plane, (Algorithm) i, neighbourhood);
    auto time = T_START;
    path_length[i] = engine.FindPath().size();
    times[i] += T_RECORD(time);
  }
}
