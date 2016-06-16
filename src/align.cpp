#include <iostream>
#include <map>
#include <string>
#include <functional>
#include <deque>
#include <Eigen/Dense>
#include "nsp.hpp"
#include <jian/pdb/Model.hpp>
#include <jian/cluster/Cluster.hpp>
#include <jian/geom/suppos.hpp>
#include <jian/utils/file.hpp>

namespace jian {

static Eigen::MatrixXd * model_to_mat_aa(const Model &model) {
    int len = 0;
    for (auto && chain : model) {
        for (auto && res : chain) {
            for (auto && atom : res) {
                if (std::find(atom.name.begin(), atom.name.end(), 'P') == atom.name.end()) {
                    len++;
                }
            }
        }
    }
    Eigen::MatrixXd *mat = new Eigen::MatrixXd(len, 3);
    int i = 0;
    for (auto && chain : model) {
        for (auto && res : chain) {
            for (auto && atom : res) {
                if (std::find(atom.name.begin(), atom.name.end(), 'P') == atom.name.end()) {
                    for (int k = 0; k < 3; k++) {
                        (*mat)(i, k) = atom[k];
                    }
                    i++;
                }
            }
        }
    }
    return mat;
}

REGISTER_NSP_COMPONENT(align) {
    Model m1(par["pdb1"][0]);
    Model m2(par["pdb2"][0]);
    Eigen::MatrixXd *mat1 = model_to_mat_aa(m1);
    Eigen::MatrixXd *mat2 = model_to_mat_aa(m2);
    auto sp = geom::suppos(*mat1, *mat2);
    INIT_SUPPOS(sp);
    for (auto && chain : m1) {
        for (auto && res : chain) {
            for (auto && atom : res) {
                APPLY_SUPPOS(atom, sp);
            }
        }
    }
    std::cout << m1 << std::endl;
    delete mat1;
    delete mat2;
}

} // namespace jian

