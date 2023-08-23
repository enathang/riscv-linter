#include <exception>
#include <stdexcept>
#include "exception.h"

LinterException::LinterException(char const* const message) throw() : std::runtime_error(message) {

}

/*
char const * LinterException::what() const throw() {
    return std::exception::what();
}*/
