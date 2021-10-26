//
// Created by piotr on 15/10/2021.
//

#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
int root_data_loader() {

  std::vector<Float_t> timings;
  std::vector<Float_t> x_axis_values;

  ifstream d;
  d.open("C:\\Users\\piotr\\Documents\\CoA\\testing\\DijkstraRandom.txt");

  while (!d.eof()) {
    int x;
    double t;
    d >> x;
    x_axis_values.push_back(x);
    d >> y;
    timings.push_back(y);
  }
  d.close();
  TGraph *hi1 = new TGraph(x_axis_values.size(), x_axis_values.data(), timings.data());

  TCanvas *test = new TCanvas("A* test", "A*", 200, 10, 600, 400);
  // test->SetFillColor(0);

  // hi1->SetFillColor(2);
  // hi1->SetFillStyle(3002);

  hi1->Draw("AC*");
  return;
}
