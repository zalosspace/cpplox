#include "LoxClass.h"
#include "LoxInstance.h"

Value LoxClass::call(
    Interpreter*,
    const std::vector<Value>&
) {
    auto instance = std::make_shared<LoxInstance>(
        std::make_shared<LoxClass>(*this)
    );
    return instance;
}

std::string LoxInstance::toString() const {
    return klass->name + " instance";
}
