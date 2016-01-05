#ifndef JIAN_NUC2D_GETSS
#define JIAN_NUC2D_GETSS

#include "../util/std.h"
#include "../pdb/IFModel.h"
#include "util.h"

namespace jian {
namespace nuc2d {

class GetSS {
public:
    typedef std::pair<int, int> BP;
    typedef std::list<BP> BPs;

    template<typename ModelType> std::string operator ()(ModelType &&model) {
        return get_ss(model);
    }

    template<typename ModelType> std::string get_ss(ModelType &&model) {
        auto bps = get_bps(model);
//        print_bps(bps);
        int len = model.res_nums();
        return bps_to_ss(bps, len);
    }

    void print_bps(const BPs &bps) const {
        for (auto && bp : bps) {
            std::cout << bp.first << '-' << bp.second << std::endl;
        }
    }

    template<typename MolType> BPs get_bps(MolType &&model) {
        BPs bps;
        int num_res1 = 0;
        for (auto &&chain : model.chains) for (auto &&residue : chain.residues) {
            int num_res2 = 0;
            for (auto &&chain2 : model.chains) for (auto &&residue2 : chain2.residues) {
                if (num_res2 > num_res1) {
                    if (is_paired(residue, residue2)) append(bps, std::make_pair(num_res1, num_res2));
                }
                num_res2++;
            }
            num_res1++;
        }
        return bps;
    }

    template<typename ResType> bool is_paired(ResType &&res1, ResType &&res2) {
        return res1.is_paired(res2);
    }

    std::string bps_to_ss(const BPs &bps, int len) {
        std::string ss(len, '.');
        for (auto && bp : bps) {
            auto level = bp_level(ss, bp);
            if (level >= 0) {
                ss[bp.first] = paired_keys[level].first;
                ss[bp.second] = paired_keys[level].second;
            }
        }
        return ss;
    }

    int bp_level(const std::string &ss, const BP &bp) {
        int left = bp.first, right = bp.second;
        if (ss[left] != '.' or ss[right] != '.') return -1;
        for (int i = 0; i < paired_keys.size(); i++) {
            int score = 0, flag = 1;
            for (int j = left + 1; j < right; j++) {
                if (ss[j] == paired_keys[i].first) score++;
                else if (ss[j] == paired_keys[i].second) score--;
                if (score < 0) break;
            }
            if (score != 0) flag = 0;
            if (flag == 1) return i;
        }
        return -1;
    }

};

} // namespace nuc2d
} // namespace jian

#endif
