#pragma once

#include "parser.h"

#include <map>
#include <set>

class Application {
public:
    using Ptr2FC = Filter* (*)(const FilterDescriptor&);

    Application(int argc, char** argv) : parser(argc, argv) {
    }

    static void Run(int argc, char** argv);

    inline static const std::set<std::string> NAMES = {"-crop", "-neg", "-sharp", "-gs", "-edge", "-blur"};

    inline static const std::map<std::string, Ptr2FC> PRODUCER = {
        {"-crop", &FilterCreator::CreateCropFilter},   {"-neg", &FilterCreator::CreateNegFilter},
        {"-sharp", &FilterCreator::CreateSharpFilter}, {"-gs", &FilterCreator::CreateGSFilter},
        {"-edge", &FilterCreator::CreateEDFilter},     {"-blur", &FilterCreator::CreateBlurFilter}};

protected:
    Parser parser;
};