#ifndef CORE_SCOPE_HPP
#define CORE_SCOPE_HPP

#include "core/Object.hpp"

namespace core {
class Scope : public Object {
private:
    struct MemberEntry {
        AttributeEntry value;
    };
public:
    Scope();
    // Scope* getScope() override; - prevent Scope from having its own scope (weird behaviour, deprecated)

    // Operations supported (optimization)
    String* toString() override;
    Boolean* toBool() override;
    Object* equal(Object* another) override;
    Object* notEqual(Object* another) override;

    Object* getMember(const std::string& varName);
    bool hasMember(const std::string& varName);
    void setMember(const std::string& varName, AttributeEntry obj);

    // Operations supported
    static Object* toStringOp(const CallArgs& args);
    static Object* toBoolOp(const CallArgs& args);
    static Object* equalOp(const CallArgs& args);
    static Object* notEqualOp(const CallArgs& args);

    std::vector<gc::GCTrackable*> referencees() override;

    #ifdef KRAIT_TESTING
    size_t length();
    #endif // KRAIT_TESTING

    virtual size_t size() override { return sizeof(Scope); }
    virtual ~Scope() = default;

private: 
    std::unordered_map<std::string, MemberEntry> scopeMembers_;
};

}

#endif // CORE_SCOPE_HPP