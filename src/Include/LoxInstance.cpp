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
    auto field = fields.find(name.lexeme);
    if (field != fields.end()) {
        return field->second;
    }

    std::shared_ptr<LoxFunction> method =
        klass->findMethod(name.lexeme);

    if (method != nullptr) {
        return std::static_pointer_cast<LoxCallable>(method);
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void LoxInstance::set(const Token& name, const Value& value) {
    fields[name.lexeme] = value;
}

std::string LoxInstance::toString() const {
    return klass->name + " instance";
}
