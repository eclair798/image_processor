#include "exceptions.cpp"
#include "image.h"

bool Image::RGB::operator==(const Image::RGB& other) const {
    return (this->r == other.r && this->g == other.g && this->b == other.b);
}

Image::RGBMatrix::RGBMatrix(size_t rows_n, size_t cols_n, Image::RGB def) {
    if (rows_n == 0 && cols_n == 0) {
        rows_num_ = 0;
        cols_num_ = 0;
        table_ = nullptr;
        return;
    }

    if (rows_n == 0 || cols_n == 0) {
        throw std::invalid_argument("Not exist matrix size");
    }

    rows_num_ = rows_n;
    cols_num_ = cols_n;
    table_ = AllocateMatrix(rows_n, cols_n);

    for (size_t i = 0; i < rows_n; ++i) {
        for (size_t j = 0; j < cols_n; ++j) {
            this->operator()(i, j) = def;
        }
    }
}

Image::RGBMatrix::RGBMatrix(const Image::RGBMatrix& other) {
    rows_num_ = other.rows_num_;
    cols_num_ = other.cols_num_;

    table_ = AllocateMatrix(rows_num_, cols_num_);
    for (size_t i = 0; i < rows_num_; ++i) {
        for (size_t j = 0; j < cols_num_; ++j) {
            this->operator()(i, j) = other(i, j);
        }
    }
}

Image::RGBMatrix& Image::RGBMatrix::operator=(const Image::RGBMatrix& other) {
    this->Resize(other.rows_num_, other.rows_num_);
    for (size_t i = 0; i < rows_num_; ++i) {
        for (size_t j = 0; j < cols_num_; ++j) {
            this->operator()(i, j) = other(i, j);
        }
    }
    return *this;
}

void Image::RGBMatrix::Resize(size_t new_row_num, size_t new_col_num, Image::RGB def) {
    if (new_row_num == 0 && new_col_num == 0) {
        Clear();
        return;
    }
    if (new_row_num == 0 || new_col_num == 0) {
        throw std::invalid_argument("Zero rows or cols");
    }
    RGB* new_table = AllocateMatrix(new_row_num, new_col_num);
    for (size_t i = 0; i < new_row_num; ++i) {
        for (size_t j = 0; j < new_col_num; ++j) {
            if (i >= rows_num_ || j >= cols_num_) {
                GetIJEL(new_table, i, j, new_col_num) = def;
            } else {
                GetIJEL(new_table, i, j, new_col_num) = GetIJEL(table_, i, j, cols_num_);
            }
        }
    }
    rows_num_ = new_row_num;
    cols_num_ = new_col_num;
    delete[] table_;
    table_ = new_table;
}

bool Image::RGBMatrix::operator==(const Image::RGBMatrix& other) const {
    if (GetColsNum() != other.GetColsNum() || GetRowsNum() != other.GetRowsNum()) {
        return false;
    }
    for (size_t i = 0; i < GetRowsNum(); ++i) {
        for (size_t j = 0; j < GetColsNum(); ++j) {
            if (this->operator()(i, j) != other(i, j)) {
                return false;
            }
        }
    }
    return true;
}

Image::Image(const std::string& path) {
    ReadFile(path);
}

Image::~Image() {
    rgb_matrix.Clear();
    fheader_.Clear();
    iheader_.Clear();
}

void Image::ReadFile(const std::string& path) {
    Clear();

    std::fstream stream;

    stream.open(path, std::ios_base::in | std::ios_base::binary);

    if (!stream.is_open()) {
        throw WrongFileFormatException("Can't open file");
    }

    stream.read(reinterpret_cast<char*>(&fheader_), sizeof(FileHeader));
    stream.read(reinterpret_cast<char*>(&iheader_), sizeof(InfoHeader));

    if (fheader_.name[0] != BM[0] || fheader_.name[1] != BM[1]) {
        throw WrongFileFormatException("File must be in BMP format");
    }

    if (iheader_.bitsPerPixel != BMP_FORMAT) {
        throw WrongFileFormatException("File must be in 24 bit per pixel format");
    }

    rgb_matrix.Resize(iheader_.height, iheader_.width);

    const std::streamsize garb_between_lines = iheader_.width % 4;

    for (size_t i = 0; i < iheader_.height; ++i) {
        for (size_t j = 0; j < iheader_.width; ++j) {
            RGB pixel(0, 0, 0);
            stream.read(reinterpret_cast<char*>(&pixel), sizeof(RGB));
            rgb_matrix.At(iheader_.height - 1 - i, j) = pixel;
        }
        stream.ignore(garb_between_lines);
    }
}

void Image::WriteFile(const std::string& path) {

    if (!path.ends_with(".bmp")) {
        throw WrongFileFormatException("File for write must be in BMP format");
    }

    std::fstream stream;

    stream.open(path, std::ios_base::out | std::ios_base::binary);

    stream.write(reinterpret_cast<char*>(&fheader_), sizeof(FileHeader));
    stream.write(reinterpret_cast<char*>(&iheader_), sizeof(InfoHeader));

    rgb_matrix.Resize(iheader_.height, iheader_.width);

    const std::streamsize garb_between_lines = iheader_.width % 4;

    for (size_t i = 0; i < iheader_.height; ++i) {
        for (size_t j = 0; j < iheader_.width; ++j) {
            RGB pixel = rgb_matrix.At(iheader_.height - 1 - i, j);
            stream.write(reinterpret_cast<char*>(&pixel), sizeof(RGB));
        }
        char garbage[garb_between_lines + 1];
        stream.write(reinterpret_cast<char*>(&garbage), garb_between_lines);
    }
}
