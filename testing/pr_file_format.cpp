//
// Created by piotr on 22/10/2021.
//

#include "pr_file_format.h"
#include <fstream>

int PRFileFormat::file_id = 0;
std::string PRFileFormat::directory_path = "../testing/pr_directory/";
PRFileFormat::PRFileFormat(std::string graph_label, std::string x_axis_name, std::string y_axis_name, std::vector<std::string> datasets_labels)
    : datasets_labels(std::move(datasets_labels)), x_axis_name(std::move(x_axis_name)), y_axis_name(std::move(y_axis_name)), graph_label(std::move(graph_label)) {
}
PRFileFormat::~PRFileFormat() {
  Save();
}



void PRFileFormat::Save() {

  ClearFile();

  std::fstream f(directory_path + "pr_id" + std::to_string(file_id) + ".txt", std::ios::app);
  file_id++;

  f << (graph_label) << "\n";
  f << (x_axis_name) << "\n";
  f << (y_axis_name) << "\n";
  f << datasets_labels.size() << "\n";
  f << values.size() << "\n";

  for (auto &l : datasets_labels)
    f << l << "\n";

  for (int x = 0; x < arguments.size(); x++) {
    f << arguments[x] << "\t";
    for (double v : values[x]) {
      f << v << "\t";
    }
    f << "\n";
  }

  f.close();
}

void PRFileFormat::ClearFile() {
  std::fstream f(directory_path + "pr_id" + std::to_string(file_id) + ".txt", std::ios::out | std::ios::trunc);
  f.close();
}

/// add another slice of data
/// \param x  axis point
/// \param y  axis points
void PRFileFormat::PushData(int x, const std::vector<double> &y) {
  if (y.size() != datasets_labels.size()) throw "inconsistent value count";
  arguments.push_back(x);
  values.push_back(y);
}

void PRFileFormat::Load(const std::string &path) {
  std::ifstream f(path, std::ios::in);

  if (f.bad()) {
    f.close();
    throw "bad file path";
  }
  std::getline(f, graph_label);
  // AddSpace(graph_label);

  std::getline(f, x_axis_name);
  // AddSpace(x_axis_name);

  std::getline(f, y_axis_name);
  // AddSpace(y_axis_name);

  int datasets_count;
  f >> datasets_count;

  int values_count;
  f >> values_count;

  std::string label;
  std::getline(f, label);

  for (int i = 0; i < datasets_count; i++) {
    std::getline(f, label);
    datasets_labels.push_back(label);
  }

  for (int i = 0; i < values_count; ++i) {
    int arg;
    f >> arg;
    arguments.push_back(arg);
    std::vector<double> buffer;

    for (int v = 0; v < datasets_count; v++) {
      double val;
      f >> val;
      buffer.push_back(val);
    }
    values.push_back(buffer);
  }

  f.close();
}
