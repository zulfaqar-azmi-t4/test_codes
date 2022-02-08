#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <fmt/core.h>
#include <Eigen/Core>
#include <fmt/ostream.h>
#include <numeric>
#include <cmath>
#include <algorithm>
// https://www.youtube.com/watch?v=GqOIzUEZgsA

struct ShiftPoint {
  int start_idx {0};
  int end_idx {0};
};
using ShiftPoints = std::vector<ShiftPoint>;

enum class Stat : int{
  match,
  insert,
  deletion
};

struct DTWInfo {
  int row = -1;
  int col = -1;
  Stat status = Stat::match;
};

int main(){


  const auto lambda = [](std::ifstream file, ShiftPoints & shift_points_){
    char comma;
    std::string parsed {};
    while(getline(file, parsed)) {
      std::stringstream ss(parsed);
      ShiftPoint sp;
      ss >> sp.start_idx >> comma >> sp.end_idx;
      shift_points_.push_back(sp);
    }
  };

  const auto printer = [](const ShiftPoints & shifts) {
    for(const auto & sp:shifts){
      fmt::print("{}, {}\n", sp.start_idx, sp.end_idx);
    }
  };

  ShiftPoints shift_points1;
  lambda(std::ifstream{"../input1.txt"}, shift_points1);
  // printer(shift_points1);

  ShiftPoints shift_points2;
  lambda(std::ifstream{"../input2.txt"}, shift_points2);
  // printer(shift_points2);

  Eigen::MatrixXd DTWMatrix(shift_points1.size() + 1, shift_points2.size() + 1);
  DTWMatrix.setZero();
  Eigen::MatrixXd trace_back = DTWMatrix;

  DTWMatrix.topRows(1).setOnes();
  DTWMatrix.leftCols(1).setOnes();
  DTWMatrix(0, 0) = 0;
  DTWMatrix = std::numeric_limits<double>::max() * DTWMatrix;
  std::size_t idx_row = 1;
  for(const auto & sp1:shift_points1){
    std::size_t idx_col = 1;

    for(const auto & sp2:shift_points2) {
      // const auto distance = std::hypot(sp1.start_idx - sp2.start_idx, sp1.end_idx - sp2.end_idx);
      const auto match = DTWMatrix(idx_row - 1, idx_col - 1);
      const auto insertion = DTWMatrix(idx_row - 1, idx_col);
      const auto deletion = DTWMatrix(idx_row, idx_col - 1);
      const std::vector<double> penalties {match, insertion, deletion};
      const auto min_elem = std::min_element(penalties.cbegin(), penalties.cend());
      DTWMatrix(idx_row, idx_col) = distance + *(min_elem);
      trace_back(idx_row, idx_col) = std::distance(penalties.cbegin(), min_elem);
      ++idx_col;
    }
    ++idx_row;
  }
  DTWInfo init;
  init.row = trace_back.rows() - 1;
  init.col = trace_back.cols() - 1;
  const auto val = trace_back(init.row, init.col);
  init.status = (val == 0) ? Stat::match : ((val == 1) ? Stat::insert : Stat::deletion) ;
  std::vector<DTWInfo> paths {};
  paths.push_back(init);
  int i = trace_back.rows() - 1;
  int j = trace_back.cols() - 1;
  while(i > 0 || j > 0){
    {
      DTWInfo path;
      if(trace_back(i, j) == 0){
        --i;
        --j;
        path.status = Stat::match;
      }
      else if(trace_back(i, j) == 1){
        --i;
        path.status = Stat::insert;
      }
      else if(trace_back(i, j) == 2){
        --j;
        path.status = Stat::deletion;
      }
      path.row = i;
      path.col = j;
      paths.push_back(path);
    }
  }
 

  // fmt::print("Dynamic Time warping \n {}\n", DTWMatrix);
  std::reverse(paths.begin(), paths.end());
 
  paths.erase(paths.begin());
  for (const auto p : paths) {
    fmt::print("Alignment sp1 : {} < - sp2 : {} by {} \n", p.row, p.col, p.status);
  }

  fmt::print("\n");
}
