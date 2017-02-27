#include "DHMC.hpp"
#include "../mcsm/McsmUpdate.hpp"
#include "SetMvEl.hpp"

BEGIN_JN

template<typename _SSE>
auto loop_nums(const _SSE &sse) {
    Deque<Int> dq;
    Int a = 0, b = 0;
    Int i = 0;
    for (auto && res : sse.loop) {
        if (res.type == '(') {
            a++;
            if (a % 2 != 0) {
                dq.push_back(res.num - 1);
            }
        } else if (res.type == ')') {
            b++;
            if (b % 2 != 1) {
                dq.push_back(res.num - 1);
            }
        } else {
            dq.push_back(res.num - 1);
        }
        i++;
    }
    if (sse.has_helix()) {
        dq.pop_front();
        dq.pop_back();
    }
    return dq;
}

void set_mvels_helices(DHMC &m) {
    // set mvels of helices
    auto end = (m.m_pk_ahead ? m.m_trees.end() : std::next(m.m_trees.begin()));
    for (auto it = m.m_trees.begin(); it != end; it++) {
        for (auto &&sse : **it) {
            if (sse.has_helix()) {
                auto &front = sse.helix.front();
                auto &back = sse.helix.back();
                m.m_mvels.push_back(new MvEl(front.res1.num-1, back.res1.num-1, back.res2.num-1, front.res2.num-1, MvEl::MVEL_HL));
            }
        }
    }
}

void set_mvels_frag3(DHMC &m) {
    Int i, j, l = size(m._seq);
    for (i = 0; i + 2 < l; i++) {
        if (!is_fixed(m, i) || !is_fixed(m, i+1) || !is_fixed(m, i+2)) {
            m.m_mvels.push_back(new MvEl(i, i + 2, MvEl::MVEL_FG));
        }
    }
}

void set_mvels_tree(DHMC &m) {
    for (auto && sse : *(m.m_trees.front())) {
        if (sse.has_loop()) {
            auto nums = loop_nums(sse);
            Int l = size(nums);
            for (Int i = 0; i < l; i++) {
                for (Int j = i + 1; j < l; j++) {
                    if (nums[j] - nums[i] > 2 && m._ss[nums[i]] != ')' && m._ss[nums[j]] != '(') {
                        m.m_mvels.push_back(new MvEl(nums[i], nums[j], MvEl::MVEL_FG));
                    }
                }
            }
        }
    }
}

void set_mvels_all(DHMC &m) {
    Int l = size(m._seq);
    for (Int i = 0; i < l; i++) {
        for (Int j = i; j < l; j++) {
            //if (not_in_fixed_areas(i, j) && (m_all_free || not_in_helix_ranges(i, j))) {
            m.m_mvels.push_back(new MvEl(i, j, MvEl::MVEL_FG));
        }
    }
}

void set_mvels(DHMC &m) {
//    const auto &keys = NASS::instance().paired_keys;
//    const auto &bks = NASS::instance().break_keys;
//    Deque<Char> ss;
//    for (auto && c : _ss) if (std::find(bks.begin(), bks.end(), c) == bks.end()) ss.push_back(c);

    if (m.m_save_ss) {
        if (m.m_sample_tree) {
            set_mvels_tree(m);
        }
        else {
            set_mvels_helices(m);
        }
    }
    set_mvels_frag3(m);


}

//auto area_min = [](const auto &area) {
//    Int i = 0;
//    for (; i < size(area); i++) if (area[i]) break;
//    return i;
//};
//
//auto area_max = [](const auto &area) {
//    Int i = size(area) - 1;
//    for (; i >= 0; i--) if (area[i]) break;
//    return i;
//};
//
//auto not_in_fixed_areas = [this, &area_min, &area_max](Int i, Int j) {
//    for (auto && area : m_fixed_areas) {
//        Int min = area_min(area);
//        Int max = area_max(area);
//        if (i <= min && j >= max) return true;
//        for (Int n = i; n <= j; n++) if (area[n]) return false;
//    }
//    return true;
//};

//template<typename T, typename K>
//Bool is_paired(DHMC &dhmc, int i, int j) {
//    return m_bps[i] == j && (ss[i] == '(' || m_pk_ahead);
//};

//auto not_in_helix_ranges = [this, &ss, &is_paired, &keys](int i, int j) {
//    if (is_paired(i, j) && i > 0 && j < size(_seq) - 1 && is_paired(i-1, j+1)) return false;
//    auto end = std::next(keys.begin(), 1);
//    std::vector<int> v(std::distance(keys.begin(), end), 0);
//    int m;
//    for (int n = i; n <= j; n++) {
//        char c = ss[n];
//        auto it1 = std::find_if(keys.begin(), end, [&c](const NASS::PairedKey &pair) {
//                return pair.first == c;
//                });
//        auto it2 = std::find_if(keys.begin(), end, [&c](const NASS::PairedKey &pair) {
//                return pair.second == c;
//                });
//        if (it1 != end) {
//            m = std::distance(keys.begin(), it1);
//            v[m]++;
//        }
//        else if (it2 != end) {
//            m = std::distance(keys.begin(), it2);
//            v[m]--;
//            if (v[m] < 0) return false;
//        }
//    }
//    return std::all_of(v.begin(), v.end(), [](int s) {return s == 0; });
//};

END_JN

