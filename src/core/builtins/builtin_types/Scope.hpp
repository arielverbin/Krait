#ifndef CORE_SCOPE_HPP
#define CORE_SCOPE_HPP

#include "core/Object.hpp"

namespace core {
class Scope : public Object {
public:
    Scope();
    Scope* getScope() override;

    // Operations supported (optimization)
    String* toString() override;
    Boolean* toBool() override;
    Object* equal(Object* another) override;
    Object* notEqual(Object* another) override;

    Object* getMember(const std::string& varName);
    bool hasMember(const std::string& varName);
    void setMember(const std::string& varName, MemberEntry obj);

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* toBoolOp(const CallArgs& args);
    static Object* equalOp(const CallArgs& args);
    static Object* notEqualOp(const CallArgs& args);

    static Object* createNewOp(const CallArgs& args);

    #ifdef KRAIT_TESTING
    std::unordered_map<std::string, MemberEntry> getMembers();
    #endif // KRAIT_TESTING

private:
    std::unordered_map<std::string, MemberEntry> members_;
};

}

#endif // CORE_SCOPE_HPP