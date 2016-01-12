#ifndef JIAN_FPL_LIST
#define JIAN_FPL_LIST

#include "../util/std.h"

namespace jian {
namespace fpl {

template<template<typename...> class LS = std::list, typename Fn>
auto list(int len, Fn &&f) {
    using T = std::result_of_t<Fn(int)>;
    LS<T> ls;
    if (len == 0) return ls;
    if (std::is_same<LS<T>, std::vector<T>>::value) {
        ls.reserve(len);
    }
    int n = 0;
    while (n < len) {
        ls.push_back(f(n));
        n++;
    }
    return ls;
}

template<typename Fn, typename ListType, typename... ListsType>
bool exists(Fn &&f, const ListType &list, const ListsType & ...lists) {
    for (int i = 0; i < list.size(); i++) {
        if (f(*(std::next(std::begin(list), i)), (*(std::next(std::begin(lists), i)))...)) return true;
    }
    return false;
}

template<typename Fn, typename ListType>
auto find(Fn &&f, const ListType &list) {
    for (int i = 0; i < list.size(); i++) {
        const auto &val = *std::next(std::begin(list), i);
        if (f(val)) return val;
    }
    throw "not found!";
}

template<template<typename...> class ResultType = std::list, class Fn, class DataType, 
         template<typename...> class ListType = std::vector>
ResultType<DataType> filter(Fn &&f, const ListType<DataType> &list) {
    ResultType<DataType> result;
    for (auto && data : list) if (f(data)) result.push_back(data);
    return result;
}

} // namespace fpl
} // namespace jian

#endif





