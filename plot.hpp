#ifndef PLOT_HPP_
#define PLOT_HPP_

#include <fstream>
#include <string>
#include "fitness.hpp"

namespace plot {

struct Data {
    Data()
        : generation(0),
          min_fitness(0.0) {}
    unsigned generation;
    Fitness min_fitness;
};

class Plotter {
public:
    Plotter(const std::string& filename);
  
    void dump(const Data& data);

    void plot() const;

private:
    void header();

    std::string filename_;
    std::ofstream output_;
};


} // namespace plot

#endif
