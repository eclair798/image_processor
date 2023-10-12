#pragma once

#include "filter.h"

class FilterCreator {
public:
    static Filter* CreateCropFilter(const FilterDescriptor& fd);

    static Filter* CreateNegFilter(const FilterDescriptor& fd);

    static Filter* CreateSharpFilter(const FilterDescriptor& fd);

    static Filter* CreateGSFilter(const FilterDescriptor& fd);

    static Filter* CreateEDFilter(const FilterDescriptor& fd);

    static Filter* CreateBlurFilter(const FilterDescriptor& fd);
};