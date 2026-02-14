#pragma once

#include <memory>
#include <variant>
#include <string>

class LoxCallable;
class LoxInstance;

using Value = std::variant<
std::shared_ptr<LoxCallable>,
std::shared_ptr<LoxInstance>,
std::monostate,
std::string,
double, 
bool
>;
