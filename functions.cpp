#include "functions.hpp"
#include <functional>
#include <numeric>

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
        stree::Value{},
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
