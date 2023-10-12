#pragma once

#include "filter_creators.h"

class Pipeline {
public:
    using FilterColl = std::vector<Filter*>;

    explicit Pipeline(const DescriptorColl& dc);

    ~Pipeline();

    void Apply(Image& image);

protected:
    FilterColl filters;
};
