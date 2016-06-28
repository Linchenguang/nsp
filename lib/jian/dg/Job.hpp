#pragma once

#include "../utils/rand.hpp"
#include "../utils/Log.hpp"
#include "../matrix.hpp"
#include "DihBound.hpp"

namespace jian {
namespace dg {

struct Job {
//    using Mat = Eigen::MatrixXd;
    using DihEntry = std::vector<int>;
    using DistBoundType = Mat;
    using DihBoundType = DihBound;

    int len = 0;

    Mat bound;
    double _min_dist = 5;
    DihBound _dih_bound;
    Mat d, m, c, g;
    double g2 = 0;
    double _dist_en = 0;
    double _dih_en = 0;
    int _num_mc_steps = 2000;

    Log log;

};

} // namespace dg
} // namespace jian
