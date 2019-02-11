#include "functions.hpp"
#include <cassert>
#include <cmath>
#include <functional>
#include <numeric>

namespace fun {

stree::Value plus(const stree::Arguments& args, stree::DataPtr) {
    return std::accumulate(args.begin(), args.end(), stree::Value{});
}

stree::Value minus(const stree::Arguments& args, stree::DataPtr) {
    stree::Value init{};
    auto begin = args.begin();
    if (args.size() > 0) {
        init = args[0];
        ++begin;
    }
    return std::accumulate(
        begin, args.end(),
        init,
        std::minus<stree::Value>());
}

stree::Value multiply(const stree::Arguments& args, stree::DataPtr) {
    return std::accumulate(
        args.begin(), args.end(),
        1,
        std::multiplies<stree::Value>());
}

// Save division: return 0 if divisor is 0
stree::Value divide(const stree::Arguments& args, stree::DataPtr) {
    stree::Value result = (args.size() > 0) ? args[0] : 0.0;
    for (auto& arg : args) {
        if (arg == stree::Value{}) {
            return stree::Value{};
        } else {
            result /= arg;
        }
    }
    return result;
}

stree::Value square(const stree::Arguments& args, stree::DataPtr) {
    assert(args.size() > 0);
    const stree::Value& arg = args[0];
    return arg * arg;
}

stree::Value cube(const stree::Arguments& args, stree::DataPtr) {
    assert(args.size() > 0);
    const stree::Value& arg = args[0];
    return arg * arg * arg;
}

stree::Value power(const stree::Arguments& args, stree::DataPtr) {
    assert(args.size() > 1);
    return static_cast<stree::Value>(std::pow(args[0], args[1]));
}

} // namespace fun
