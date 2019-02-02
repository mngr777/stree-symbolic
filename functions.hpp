#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <stree/stree.hpp>

stree::Value plus(const stree::Arguments& args, stree::DataPtr);
stree::Value minus(const stree::Arguments& args, stree::DataPtr);
stree::Value multiply(const stree::Arguments& args, stree::DataPtr);
stree::Value divide(const stree::Arguments& args, stree::DataPtr);

#endif
