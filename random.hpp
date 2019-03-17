#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <random>
#include <type_traits>
#include <stree/stree.hpp>

// TODO:
// typedef std::conditional<sizeof(int) <= 4, std::mt19937, std::mt19937_64>::type Engine;
// https://stackoverflow.com/questions/17854407/how-to-make-a-conditional-typedef-in-c

class Random {
public:
    typedef std::uint_fast32_t result_type;

    virtual ~Random() {};

    virtual result_type operator()() = 0;

    virtual result_type min() = 0;
    virtual result_type max() = 0;
};


// TODO: selectable distribution type
class RandomValue {
public:
    typedef std::uniform_real_distribution<stree::Value> DistrType;

    RandomValue(Random& rd, stree::Value min, stree::Value max)
        : rd_(rd), distr_(min, max) {}

    stree::Value get();

private:
    Random& rd_;
    DistrType distr_;
};


class RandomMt19937 : public Random {
public:
    RandomMt19937(std::mt19937::result_type seed)
        : generator_(seed) {}

    RandomMt19937(std::seed_seq& seed_seq)
        : generator_(seed_seq) {}

    virtual result_type operator()() {
        return generator_();
    }

    virtual result_type min() {
        return generator_.min();
    }

    virtual result_type max() {
        return generator_.max();
    }

private:
    std::mt19937 generator_;
};

#endif
