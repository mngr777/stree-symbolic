#include "plot.hpp"

namespace plot {

Plotter::Plotter(const std::string& filename)
    : filename_(filename)
{
    output_.open(filename, std::ofstream::out);
    header();
}

void Plotter::dump(const Data& data) {
    output_ << data.generation << " "
            << data.min_fitness << " "
            << std::endl;
}

void Plotter::plot() const {
}

void Plotter::header() {
    output_ << "# "
            << "Generation "
            << "Min. Fitness"
            << std::endl;

}

} // namespace plot
