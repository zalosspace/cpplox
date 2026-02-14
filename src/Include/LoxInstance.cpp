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

Value LoxInstance::get(const Token& name) {
    if (fields.count(name.lexeme)) {
        return fields[name.lexeme];
    }

    throw RuntimeError(name, 
                       "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(const Token& name, const Value& value) {
    fields[name.lexeme] = value;
}

std::string LoxInstance::toString() const {
    return klass->name + " instance";
}
