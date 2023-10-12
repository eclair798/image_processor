#pragma once

#include "filter_creators.h"

#include <string>

class Parser {
    friend class Application;

public:
    static const int PN_POS = 0;
    static const int IF_POS = 1;
    static const int OF_POS = 2;
    static const int START_FILTERS_POS = 3;

public:
    Parser(int argc, char** argv);

protected:
    std::string program_name;
    std::string input_file_name_;
    std::string output_file_name_;
    DescriptorColl filter_descriptors_;
};