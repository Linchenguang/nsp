#include "DG.hpp"
#include "DGImpl.hpp"

namespace jian {

DG::DG() : _impl(new DGImpl) {}

DG::~DG() {
    if (_impl != NULL) delete _impl;
}

DG::DG(const Mat &b) : _impl(new DGImpl(b)) {
}

DG::DG(const DistBoundType &b, const DihBound &d) : DG(b) {
}

Mat DG::operator ()() {
    return _impl->operator ()();
}

Mat DG::operator ()(const Mat &b) {
    return _impl->operator ()(b);
}

Mat DG::operator ()(const Mat &b, const DihBound &d) {
    return _impl->operator ()(b, d);
}

} // namespace jian

