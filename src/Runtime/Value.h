#pragma once

#include <memory>
#include <variant>
#include <string>

class LoxCallable;

using Value = std::variant<
std::monostate,
std::string,
std::shared_ptr<LoxCallable>,
double, 
bool
>;
