#pragma once

#include <iostream>
#include <iomanip>

namespace cpp_utils{

class ProgressBar {
public:
    ProgressBar(size_t total) : total_(total), current_(0) {}

    void update(size_t processed) {
        current_ = processed;
        float percent = 100.0f * current_ / total_;
        std::cout << "\r" << current_ << "/" << total_ << " (" 
                    << std::fixed << std::setprecision(1) << percent << "%)"
                    << std::flush;
    }

    void increment() {
        update(current_++);
    }

    void finish() {
        update(total_);
        std::cout << std::endl;
    }

private:
    size_t total_;
    size_t current_;
};

} // namespace cpp_utils