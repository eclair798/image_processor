#pragma once

#include <fstream>

class Image {
    friend class Filter;
    friend class CropFilter;

public:
    struct __attribute__((packed)) FileHeader {
        char name[2];
        unsigned int size;
        int garbage;
        unsigned int offset;

        void Clear() {
            *this = FileHeader({});
        }
    };

    static const int GARB_SIZE = 24;
    struct __attribute__((packed)) InfoHeader {
        unsigned int header_size;
        unsigned int width;
        unsigned int height;
        uint16_t color_planes;
        uint16_t bitsPerPixel;
        char garbage[GARB_SIZE];

        void Clear() {
            *this = InfoHeader({});
        }
    };

    struct __attribute__((packed)) RGB {

        explicit RGB(uint8_t b = 0, uint8_t g = 0, uint8_t r = 0) : b(b), g(g), r(r) {
        }

        uint8_t b;
        uint8_t g;
        uint8_t r;

        RGB& operator=(const RGB& other) = default;

        bool operator==(const RGB& other) const;
        bool operator!=(const RGB& other) const;

    public:
        static const uint8_t MAX = 255;
    };

    class RGBMatrix {
    public:
        explicit RGBMatrix(size_t rows_n = 0, size_t cols_n = 0, RGB def = RGB(0, 0, 0));

        RGBMatrix(const RGBMatrix& other);

        ~RGBMatrix() {
            Clear();
        }

        RGBMatrix& operator=(const RGBMatrix& other);

        // Возвращает i, j элемент, без возможности изменения
        const RGB& operator()(size_t i, size_t j) const {
            return table_[cols_num_ * i + j];
        }

        // Возвращает i, j элемент, с возможностью изменения
        RGB& operator()(size_t i, size_t j) {
            return table_[cols_num_ * i + j];
        }

        bool operator==(const RGBMatrix& other) const;

        RGB& At(size_t i, size_t j) {
            if (i < rows_num_ && j < cols_num_) {
                return this->operator()(i, j);
            }
            throw std::out_of_range("IOJ (matrix)");
        }

        const RGB& At(size_t i, size_t j) const {
            if (i < rows_num_ && j < cols_num_) {
                return this->operator()(i, j);
            }
            throw std::out_of_range("IOJ (matrix)");
        }

    public:
        size_t GetRowsNum() const {
            return rows_num_;
        }

        size_t GetColsNum() const {
            return cols_num_;
        }

        void Resize(size_t new_row_num, size_t new_col_num, RGB def = RGB(0, 0, 0));

        void Clear() {
            delete[] table_;
            table_ = nullptr;
            rows_num_ = 0;
            cols_num_ = 0;
        }

        static RGB& GetIJEL(RGB* arr, size_t i, size_t j, size_t cols_num) {
            return arr[i * cols_num + j];
        }

    private:
        static RGB* AllocateMatrix(size_t rows_n, size_t cols_n) {
            RGB* table = new RGB[rows_n * cols_n];
            return table;
        }

    private:
        RGB* table_;
        size_t rows_num_;
        size_t cols_num_;
    };

    explicit Image(const std::string& path);

    ~Image();

    size_t GetWidth() const {
        return rgb_matrix.GetColsNum();
    }

    size_t GetHeight() const {
        return rgb_matrix.GetRowsNum();
    }

    void ReadFile(const std::string& path);

    void WriteFile(const std::string& path);

    void Clear() {
        rgb_matrix.Clear();
        fheader_.Clear();
        iheader_.Clear();
    }

public:
    RGBMatrix rgb_matrix;
    static const uint16_t BMP_FORMAT = 24;
    constexpr static const char BM[2] = {'B', 'M'};

protected:
    FileHeader fheader_{};
    InfoHeader iheader_{};
};
