#ifndef CORE_OBJECT_HPP
#define CORE_OBJECT_HPP

#include <string>
#include <map>

namespace core {

class Object {
private:
    std::map<std::string, std::shared_ptr<Object>> members_;

public:
    void* _loc_();

    virtual std::string _str_();
    virtual std::shared_ptr<Object> _add_(Object& another);
    virtual std::shared_ptr<Object> _sub_(Object& another);

    virtual std::shared_ptr<Object> _att_(std::string varName);
};

} // namespace core

#endif // CORE_OBJECT_HPP