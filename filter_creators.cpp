#include "exceptions.cpp"
#include "filter_creators.h"

Filter* FilterCreator::CreateCropFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-crop") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (fd.filter_params.size() != 2) {
        throw WrongFilterDescriptionException("Incorrect filter params. Crop filter needs 2 parameters");
    }

    size_t w = 0;
    size_t h = 0;
    try {
        w = stoi(fd.filter_params[0]);
        h = stoi(fd.filter_params[1]);
    } catch (const std::exception& exception) {
        throw WrongFilterDescriptionException("Incorrect filter params. Crop filter needs 2 int parameters");
    }
    if (w == 0 || h == 0) {
        throw WrongFilterDescriptionException("Incorrect filter params. You can't crop BMP to zero size");
    }

    return new CropFilter(w, h);
}

Filter* FilterCreator::CreateNegFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-neg") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (!fd.filter_params.empty()) {
        throw WrongFilterDescriptionException("Incorrect filter params. Negative filter doesn't need parameters");
    }
    return new NegativeFilter();
}

Filter* FilterCreator::CreateSharpFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-sharp") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (!fd.filter_params.empty()) {
        throw WrongFilterDescriptionException("Incorrect filter params. Sharp filter doesn't need parameters");
    }
    return new SharpFilter();
}

Filter* FilterCreator::CreateGSFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-gs") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (!fd.filter_params.empty()) {
        throw WrongFilterDescriptionException("Incorrect filter params. Grayscale filter doesn't need parameters");
    }
    return new GrayscaleFilter();
}

Filter* FilterCreator::CreateEDFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-edge") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (fd.filter_params.size() != 1) {
        throw WrongFilterDescriptionException("Incorrect filter params. Edge Detection filter needs 1 parameter");
    }

    double t = 0;
    try {
        t = stod(fd.filter_params[0]);
    } catch (const std::exception& exception) {
        throw WrongFilterDescriptionException(
            "Incorrect filter params. Edge Detection filter needs 1 double parameter");
    }
    return new EdgeDetectionFilter(t);
}

Filter* FilterCreator::CreateBlurFilter(const FilterDescriptor& fd) {
    if (fd.filter_name != "-blur") {
        throw WrongFilterDescriptionException("Incorrect filter name");
    }
    if (fd.filter_params.size() != 1) {
        throw WrongFilterDescriptionException("Incorrect filter params. Blur filter needs 1 parameter");
    }

    double s = 0;
    try {
        s = stod(fd.filter_params[0]);
    } catch (const std::exception& exception) {
        throw WrongFilterDescriptionException("Incorrect filter params. Blur filter needs 1 double parameter");
    }

    return new GaussianBlurFilter(s);
}
