#pragma once

#include "Module.hpp"

namespace jian {
namespace qhmc {

class TailHairpin : public Module {
public:
    TailHairpin(const Tuple &, const Tuple &);
    virtual std::string type() const;
};

} // namespace quadruple
}


