#pragma once

#include "LoxObject.h"
#include <memory>
#include <string>

class LoxClass;  

class LoxInstance : public LoxObject {
public:
    explicit LoxInstance(std::shared_ptr<LoxClass> klass)
        : klass(std::move(klass)) {}

    std::string toString() const override;

private:
    std::shared_ptr<LoxClass> klass;
};
