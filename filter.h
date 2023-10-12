#pragma once

#include "image.h"

#include <vector>

struct FilterDescriptor {
    std::string filter_name;
    std::vector<std::string> filter_params;
    void Clear() {
        filter_params.clear();
        filter_name = "";
    }
};

using DescriptorColl = std::vector<FilterDescriptor>;

class Filter {
public:
    Filter() {
    }

    virtual void ApplyFilter(Image& image) = 0;
    virtual ~Filter() {
    }
};

class MatrixFilter : public Filter {
public:
    using MFilter = std::vector<int>;

    virtual void ApplyMatrix(Image& image);

    virtual const MFilter& GetMFilter() = 0;

    // protected:
    //     const MFilter matrix_filter;
};

class CropFilter : public Filter {
public:
    CropFilter(size_t width, size_t height) : width(width), height(height) {
    }

    void ApplyFilter(Image& image) override;

protected:
    size_t width;
    size_t height;
};

class NegativeFilter : public Filter {
public:
    NegativeFilter() {
    }

    void ApplyFilter(Image& image) override;
};

class SharpFilter : public MatrixFilter {
public:
    SharpFilter() {
    }

    void ApplyFilter(Image& image) override {
        ApplyMatrix(image);
    }

    const MFilter& GetMFilter() override {
        return matrix_filter;
    }

protected:
    const MFilter matrix_filter = {0, -1, 0, -1, 5, -1, 0, -1, 0};
};

class GrayscaleFilter : public Filter {
public:
    GrayscaleFilter() {
    }

    void ApplyFilter(Image& image) override;

protected:
    const double CR = 0.299;
    const double CG = 0.587;
    const double CB = 0.114;
};

class EdgeDetectionFilter : public MatrixFilter {
public:
    explicit EdgeDetectionFilter(double t);

    void ApplyFilter(Image& image) override;

    const MFilter& GetMFilter() override {
        return matrix_filter;
    }

protected:
    const MFilter matrix_filter = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    uint8_t threshold;
};

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma) : sigma(sigma) {
    }

    void ApplyFilter(Image& image) override {
        // TODO: Написать Блюр((
    }

protected:
    double sigma;
};
