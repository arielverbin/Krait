#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

namespace utils {

#define UNREFERENCED(var) (void)(var);

#define RESET       "\033[0m"
#define BLUE        "\033[0;34m"
#define GREY        "\033[38;5;240m"
#define CYAN        "\033[0;36m"
#define BOLDMAGENTA "\033[1;35m"

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
