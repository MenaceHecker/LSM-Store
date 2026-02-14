#pragma once

#if __cplusplus < 201703L
#error "LSM-Store requires C++17 or later (std::optional)."
#else
#include <optional>
namespace lsm {
template <typename T>
using optional = std::optional<T>;
constexpr auto nullopt = std::nullopt;
}  // namespace lsm
#endif
