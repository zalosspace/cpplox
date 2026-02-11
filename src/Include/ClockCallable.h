#pragma once

#include "LoxCallable.h"
#include <chrono>
#include <vector>

class ClockCallable : public LoxCallable {
public:
    int arity() const override {
        return 0;
    }

    Value call(Interpreter*,
               const std::vector<Value>&) override 
    {
        using namespace std::chrono;
        auto now = system_clock::now().time_since_epoch();
        double seconds = 
            duration_cast<milliseconds>(now).count() / 1000.0;
        
        return Value(seconds);
    }

    std::string toString() const override {
        return "<native fn>";
    }
};
