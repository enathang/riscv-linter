#import <stdexcept>

class LinterException : public std::runtime_error {
    public:
        LinterException(char const* const message) throw();
};

