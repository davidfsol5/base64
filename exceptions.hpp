#include <stdexcept>
#include <exception>

class not_implemented_yet : public std::logic_error {
public:
    not_implemented_yet(const char* whatString) : logic_error(whatString){}
};

class invalid_email : public std::runtime_error {
public:
    invalid_email(const char* whatString) : runtime_error(whatString){}
};