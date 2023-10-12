#include "filter.h"

void MatrixFilter::ApplyMatrix(Image& image) {
    std::vector<int> div{-1, 0, 1};
    int height = static_cast<int>(image.GetHeight());
    int width = static_cast<int>(image.GetWidth());

    Image::RGBMatrix temp(image.rgb_matrix);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {

            int nr = 0;
            int ng = 0;
            int nb = 0;

            for (int y : div) {
                for (int x : div) {

                    int new_i = 0;
                    int new_j = 0;

                    if ((y + i) >= 0) {
                        new_i = y + i;
                    } else {
                        new_i = 0;
                    }
                    if ((x + j) >= 0) {
                        new_j = x + j;
                    } else {
                        new_j = 0;
                    }

                    new_i = new_i < height ? new_i : height - 1;
                    new_j = new_j < width ? new_j : width - 1;

                    size_t cx = 1 + x;
                    size_t cy = 1 + y;
                    size_t c = cy * 3 + cx;

                    nr += temp(new_i, new_j).r * GetMFilter()[c];
                    ng += temp(new_i, new_j).g * GetMFilter()[c];
                    nb += temp(new_i, new_j).b * GetMFilter()[c];
                }
            }

            image.rgb_matrix(i, j).r =
                (nr >= 0 && nr < Image::RGB::MAX + 1) ? nr : (nr < Image::RGB::MAX + 1 ? 0 : Image::RGB::MAX);
            image.rgb_matrix(i, j).g =
                (ng >= 0 && ng < Image::RGB::MAX + 1) ? ng : (ng < Image::RGB::MAX + 1 ? 0 : Image::RGB::MAX);
            image.rgb_matrix(i, j).b =
                (nb >= 0 && nb < Image::RGB::MAX + 1) ? nb : (nb < Image::RGB::MAX + 1 ? 0 : Image::RGB::MAX);
        }
    }
}

void CropFilter::ApplyFilter(Image& image) {
    size_t new_height = std::min(height, image.GetHeight());
    size_t new_width = std::min(width, image.GetWidth());
    image.rgb_matrix.Resize(new_height, new_width);
    image.iheader_.width = new_width;
    image.iheader_.height = new_height;
}

void NegativeFilter::ApplyFilter(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); i++) {
        for (size_t j = 0; j < image.GetWidth(); j++) {
            image.rgb_matrix(i, j).r = Image::RGB::MAX - image.rgb_matrix(i, j).r;
            image.rgb_matrix(i, j).g = Image::RGB::MAX - image.rgb_matrix(i, j).g;
            image.rgb_matrix(i, j).b = Image::RGB::MAX - image.rgb_matrix(i, j).b;
        }
    }
}

void GrayscaleFilter::ApplyFilter(Image& image) {
    for (size_t i = 0; i < image.GetHeight(); i++) {
        for (size_t j = 0; j < image.GetWidth(); j++) {
            auto nrgb = static_cast<uint8_t>(static_cast<double>(image.rgb_matrix(i, j).r) * CR +
                                             static_cast<double>(image.rgb_matrix(i, j).g) * CG +
                                             static_cast<double>(image.rgb_matrix(i, j).b) * CB);
            image.rgb_matrix(i, j).r = nrgb;
            image.rgb_matrix(i, j).g = nrgb;
            image.rgb_matrix(i, j).b = nrgb;
        }
    }
}

EdgeDetectionFilter::EdgeDetectionFilter(double t) {
    double temp = Image::RGB::MAX * t;
    temp = temp > Image::RGB::MAX ? Image::RGB::MAX : temp;
    threshold = static_cast<uint8_t>(temp);
}

void EdgeDetectionFilter::ApplyFilter(Image& image) {
    GrayscaleFilter temp;
    temp.ApplyFilter(image);
    ApplyMatrix(image);
    for (size_t i = 0; i < image.GetHeight(); i++) {
        for (size_t j = 0; j < image.GetWidth(); j++) {
            uint8_t nrgb = (image.rgb_matrix(i, j).r > threshold || image.rgb_matrix(i, j).g > threshold ||
                            image.rgb_matrix(i, j).b > threshold)
                               ? Image::RGB::MAX
                               : 0;
            image.rgb_matrix(i, j).r = nrgb;
            image.rgb_matrix(i, j).g = nrgb;
            image.rgb_matrix(i, j).b = nrgb;
        }
    }
}
