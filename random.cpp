#include "random.hpp"

stree::Value RandomValue::get() {
    return distr_(rd_);
}
