#include "application.h"
#include "exceptions.cpp"
#include "pipeline.h"

Pipeline::Pipeline(const DescriptorColl& dc) {
    for (const FilterDescriptor& fd : dc) {
        if (Application::NAMES.find(fd.filter_name) != Application::NAMES.end()) {
            filters.push_back(Application::PRODUCER.at(fd.filter_name)(fd));
        } else {
            throw WrongFilterDescriptionException("Incorrect filter name");
        }
    }
}

Pipeline::~Pipeline() {
    for (Filter* filter : filters) {
        delete filter;
    }
    filters.clear();
}

void Pipeline::Apply(Image& image) {
    for (Filter* filter : filters) {
        filter->ApplyFilter(image);
    }
}
