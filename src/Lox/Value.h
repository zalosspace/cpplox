#pragma once
#include <variant>
#include <string>

using Value = std::variant<std::monostate, double, std::string, bool>;
