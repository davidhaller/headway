#pragma once

#include <QtGlobal>

namespace Headway
{
    /*! \brief Utility class implementing a generic 2D array.
     */

    template<typename T>
    class Matrix
    {
    public:

        /*! \brief Constructor for Matrix class.
         *
         * \param width Width of the Matrix
         * \param height Height of the Matrix.
         */

        Matrix(quint32 width, quint32 height) : width_(width), height_(height), data(width * height) {}

        Matrix(const Matrix& other) : width_(other.width()), height_(other.height()), data(other.data) {}

        quint32 width() const { return width_; } /*!< Width of the matrix. */
        quint32 height() const { return height_; } /*!< Height of the matrix. */
        size_t size() const { return data.size(); } /*!< Size of the matrix. */

        T get(quint32 x, quint32 y) const
        {
            return data[offset(x, y)];
        }

        void set(quint32 x, quint32 y, const T& value)
        {
            data[offset(x, y)] = value;
        }

        void fill(const T& value) noexcept
        {
            data.fill(value);
        }

        bool operator==(const Matrix& other) const
        {
            return width() == other.width() &&
                   height() == other.height() &&
                   data == other.data;
        }

        bool operator!=(const Matrix& other) const
        {
            return !(*this == other);
        }

    private:
        const quint32 width_;
        const quint32 height_;

        QVector<T> data;

        /*! \brief Calculates the array position of an item.
         *
         * Internally, the items are stored as one big array
         * with width x height as its size. This function is used
         * to transform the two coordinates into an array index.
         *
         * \return An array index that can be directly used with the data vector.
         */

        quint64 offset(quint32 x, quint32 y) const noexcept
        {
            return y * width() + x;
        }
    };
}
