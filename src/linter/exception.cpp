#include <exception>
#include <stdexcept>
#include "exception.h"

LinterException::LinterException(char const* const message) throw() : std::runtime_error(message) {

}

