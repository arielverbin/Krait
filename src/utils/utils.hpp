#ifndef UTILS_HPP
#define UTILS_HPP

#include <sstream>

namespace core { class Object; }

namespace utils {

#define UNREFERENCED(var) (void)(var);

struct LazyValue {
    std::function<std::shared_ptr<core::Object>()> creator;
    std::vector<std::shared_ptr<core::Object>> args;
    
    LazyValue(std::function<std::shared_ptr<core::Object>()> fn): creator(std::move(fn)) {}
};

using MemberEntry = std::variant<std::shared_ptr<core::Object>, utils::LazyValue>;
using Scope = std::unordered_map<std::string, MemberEntry>;

// Helper template that implements _shared_from_this_base automatically
template<typename Base, typename Derived>
class EnableSharedFromThis : public Base, public std::enable_shared_from_this<Derived> {
protected:
    std::shared_ptr<Base> _shared_from_this() override {
        return std::static_pointer_cast<Base>(this->shared_from_this());
    }
public:
    template<typename... Args>
    EnableSharedFromThis(Args&&... args) : Base(std::forward<Args>(args)...) {}
};

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
