#include "application.h"
#include "exceptions.cpp"
#include "pipeline.h"

Pipeline::Pipeline(const DescriptorColl& dc) {
    for (const FilterDescriptor& fd : dc) {
        try {
            if (Application::NAMES.find(fd.filter_name) != Application::NAMES.end()) {
                filters.push_back(Application::PRODUCER.at(fd.filter_name)(fd));
            } else {
                throw WrongFilterDescriptionException("Incorrect filter name");
            }
        } catch (...) {
            Reset();
            throw;
        }
    }
}

void Pipeline::Reset() {
    for (Filter* filter : filters) {
        delete filter;
    }
    filters.clear();
}

Pipeline::~Pipeline() {
    Reset();
}

void Pipeline::Apply(Image& image) {
    for (Filter* filter : filters) {
        filter->ApplyFilter(image);
    }
}


