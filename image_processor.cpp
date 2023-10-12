#include <iostream>

#include "application.h"
#include "exceptions.cpp"

int main(int argc, char** argv) {
    try {
        Application::Run(argc, argv);
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return -1;
    } catch (...) {
        return -2;
    }
}