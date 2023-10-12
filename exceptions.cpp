#include <cstring>
#include <exception>

class Exception : public std::exception {
protected:
    char *message;

public:
    explicit Exception(const char *s) {
        message = new char[strlen(s) + 1];
        for (size_t i = 0; i < strlen(s) + 1; ++i) {
            message[i] = s[i];
        }
    }
    ~Exception() override {
        delete[] message;
    }
    const char *what() const noexcept override {
        return message;
    }
};

class ParametersException : public Exception {
public:
    explicit ParametersException(const char *s) : Exception(s) {
    }
    const char *what() const noexcept override {
        return message;
    }
};

class WrongFileFormatException : public ParametersException {
public:
    explicit WrongFileFormatException(const char *s) : ParametersException(s) {
    }
};

class WrongFilterDescriptionException : public ParametersException {
public:
    explicit WrongFilterDescriptionException(const char *s) : ParametersException(s) {
    }
};