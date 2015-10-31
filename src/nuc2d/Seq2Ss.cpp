#include "Seq2Ss.h"

namespace jian {
namespace nuc2d {

std::vector<std::pair<int, int>> Seq2Ss::operator ()(std::string seq) {
    _len = seq.size();
    _seq = seq;
    std::map<char, int> temp_map{{'A', 0}, {'U', 1}, {'G', 2}, {'C', 3}};
    std::transform(_seq.begin(), _seq.end(), std::back_inserter(_types), [&](char c){return temp_map[c];});
    _mms = MatrixXf::Zero(_len, _len);
    for (int i = 0; i < _len - 1 - _min_hairpin_size; i++) {
        for (int j = 0; j < _len - 1 - _min_hairpin_size - i; j++) {
            _mms(j, i + j + 1 + _min_hairpin_size) = mms(j, i + j + 1 + _min_hairpin_size);
        }
    }
    auto all_pairs = backtrack(0, _len - 1);
    for (auto &&pairs: all_pairs) {
        std::string s(_seq.size(), '.');
        for (auto &&pair: pairs) {
            s[pair.first] = '(';
            s[pair.second] = ')';
        }
        std::cout << s << std::endl;
    }
    return _pairs;
}

int Seq2Ss::mms(int m, int n) {
    if (n <= m + _min_hairpin_size) {
        return 0;
    }
    int max_size = get_mms(m, n - 1);
    for (int i = m; i < n - _min_hairpin_size; i++) {
        int temp = get_mms(m, i - 1) + get_mms(i + 1, n - 1) + score(i, n);
        if (temp > max_size) {
            max_size = temp;
        }
    }
    return max_size;
}

int Seq2Ss::score(int m, int n) {
    int a = _types[m] + _types[n];
    int b = _types[m] * _types[n];
    if (a == 1 || a == 5 || b == 2) {
        return 1;
    } else {
        return 0;
    }
}

std::vector<std::vector<std::pair<int, int>>> Seq2Ss::backtrack(int m, int n) {
    std::vector<std::vector<std::pair<int, int>>> temp_vec;
    if (n <= m + _min_hairpin_size) {
        return temp_vec;;
    }
    if (get_mms(m, n - 1) == get_mms(m, n)) {
        temp_vec = backtrack(m, n - 1);
    }
    for (int i = m; i < n - _min_hairpin_size; i++) {
        int size = get_mms(m, i - 1) + get_mms(i + 1, n - 1) + score(i, n);
        if (size == get_mms(m, n) && score(i, n) == 1) {
            auto pairs1 = backtrack(m, i - 1);
            auto pairs2 = backtrack(i + 1, n - 1);
            std::vector<std::pair<int, int>> pairs;
            pairs.push_back(make_pair(i, n));
            if (pairs1.size() != 0 && pairs2.size() != 0) {
                for (int i = 0; i < pairs1.size(); i++) {
                    for (int j = 0; j < pairs2.size(); j++) {
                        auto temp_pairs = pairs;
                        std::copy(pairs1[i].begin(), pairs1[i].end(), std::back_inserter(temp_pairs));
                        std::copy(pairs2[j].begin(), pairs2[j].end(), std::back_inserter(temp_pairs));
                        temp_vec.push_back(temp_pairs);
                    }
                }
            } else if (pairs1.size() != 0) {
                for (int i = 0; i < pairs1.size(); i++) {
                    auto temp_pairs = pairs;
                    std::copy(pairs1[i].begin(), pairs1[i].end(), std::back_inserter(temp_pairs));
                    temp_vec.push_back(temp_pairs);
                }
            } else if (pairs2.size() != 0) {
                for (int i = 0; i < pairs2.size(); i++) {
                    auto temp_pairs = pairs;
                    std::copy(pairs2[i].begin(), pairs2[i].end(), std::back_inserter(temp_pairs));
                    temp_vec.push_back(temp_pairs);
                }
            } else {
                temp_vec.push_back(pairs);
            }
        }
    }
    return temp_vec;
}

int Seq2Ss::get_mms(int m, int n) {
    if (n <= m + _min_hairpin_size) {
        return 0;
    } else {
        return _mms(m, n);
    }
    
}
        
} /// namespace nuc2d
} /// namespace jian