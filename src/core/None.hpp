#ifndef CORE_NONE_HPP
#define CORE_NONE_HPP

#include "Object.hpp"

namespace core {

class None : public Object {
private:
    static std::shared_ptr<None> none_;
public:
    None() = default;

    virtual std::string _str_() override {
        return "None";
    }

    std::string _type_() override {
        return "None";
    }

    static std::shared_ptr<None> none() {
        if (none_ == nullptr) {
            none_ = std::make_shared<None>(None());
        }
        return none_;
    }

    virtual ~None() = default;
};

} // namespace core

#endif // CORE_NONE_HPP