#include <map>
#include <deque>
#include <memory>
#include "../utils/file.hpp"
#include "../utils/Env.hpp"
#include "RNA.hpp"

namespace jian {

struct res_t {
    std::string name;
    std::deque<std::string> atoms;
};

using all_res_t = std::array<res_t, 4>;

using rank_t = std::map<std::string, std::map<std::string, int>>;

static all_res_t *load_rna_all_res() {
    all_res_t *all_res = new all_res_t;
    std::string file_name = Env::lib() + "/RNA/pars/pdb/RNA/atoms";
    std::string name;
    EACH_SPLIT_LINE(file_name, " ",
        if (N < 8) {
            if (N % 2 == 0) {
                all_res->at(N / 2).name = F[0];
            } else {
                for (auto && s : F) {
                    all_res->at(N / 2).atoms.push_back(s);
                }
            }
        }
    );
    return all_res;
}

static std::unique_ptr<all_res_t> l_all_res {load_rna_all_res()};

static rank_t *get_rna_rank() {
    rank_t *rank = new rank_t;
    int i = 0;
    for (auto && res : *l_all_res) {
        for (auto && atom : res.atoms) {
            (*rank)[res.name][atom] = i;
            i++;
        }
    }
    return rank;
}

static std::unique_ptr<rank_t> l_rank {get_rna_rank()};

int RNA::rank(const std::string &res_name, const std::string &atom_name) {
    return l_rank->at(res_name).at(atom_name);
}

using check_atom_t = struct {std::string chain_name, res_name, atom_name; int res_num;};
using check_res_t = struct {std::deque<check_atom_t> repeat, lack, useless;};
using check_t = std::array<check_res_t, 4>;

static check_t check_res(const Residue &res, const std::string &chain_name) {
    std::array<std::map<std::string, int>, 4> arr;
    check_t check;
    for (int i = 0; i < 4; i++) {
        std::deque<std::string> &r = l_all_res->at(i).atoms;
        for (auto && atom : res) {
            if (std::find(r.begin(), r.end(), atom.name) == r.end()) {
                check[i].useless.push_back({chain_name, res.name, atom.name, res.num});
            } else {
                if (arr[i].find(atom.name) == arr[i].end()) {
                    arr[i][atom.name] = 1;
                } else {
                    check[i].repeat.push_back({chain_name, res.name, atom.name, res.num});
                }
            }
        }
        for (auto && name : r) {
            if (arr[i].find(name) == arr[i].end()) {
                check[i].lack.push_back({chain_name, res.name, name, res.num});
            }
        }
    }
    return check;
}

void RNA::check(const Model &m) {
    for (auto && chain : m) {
        for (auto && res : chain) {
            if (res.name != "HOH") {
                check_t c = check_res(res, chain.name);
                check_res_t &r = *std::min_element(c.begin(), c.end(), [](auto && r1, auto && r2){return r1.lack.size() < r2.lack.size();});
                std::cout << "Check Chain " << chain.name << " Residue " << res.num << ' ' << res.name << std::endl;
                for (auto && a : r.repeat) {
                    std::cout << "Repeat: " << a.atom_name << std::endl;
                }
                for (auto && a : r.lack) {
                    std::cout << "Lack: " << a.atom_name << std::endl;
                }
                for (auto && a : r.useless) {
                    std::cout << "Useless: " << a.atom_name << std::endl;
                }
            }
        }
    }
}

} // namespace jian

