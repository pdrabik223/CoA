//
// Created by piotr on 22/10/2021.
//

#ifndef COA_TESTING_PR_FILE_FORMAT_H_
#define COA_TESTING_PR_FILE_FORMAT_H_

#include <string>
#include <vector>
struct PRFileFormat {
  PRFileFormat() = delete;
  PRFileFormat(std::string graph_label, std::string x_axis_name, std::string y_axis_name, std::vector<std::string> datasets_labels);
  PRFileFormat(const PRFileFormat &other) = delete;
  PRFileFormat &operator=(const PRFileFormat &other) = delete;
  void PushData(int x, const std::vector<double> &y);
  void Save();
  void Load(std::string path);
  static void ClearFile();

  ~PRFileFormat();
  static int file_id;
  static std::string directory_path;
  std::vector<int> arguments;
  std::vector<std::vector<double>> values;
  std::vector<std::string> datasets_labels;
  std::string x_axis_name;
  std::string y_axis_name;
  std::string graph_label;
};
#endif//COA_TESTING_PR_FILE_FORMAT_H_
