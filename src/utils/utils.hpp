#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

namespace utils {

#define UNREFERENCED(var) (void)(var);

#ifdef KRAIT_TESTING
std::string join(const std::vector<std::string>& vec, const std::string& delimiter);

template <typename S, typename T>
std::vector<S> map(const std::vector<T>& vec, S (*func)(const T&)) {
    std::vector<S> result;
    result.reserve(vec.size());
    for (const auto& item : vec) {
        result.push_back(func(item));
    }
    return result;
}
#endif // KRAIT_TESTING

} // namespace utils

#endif // UTILS_HPP
