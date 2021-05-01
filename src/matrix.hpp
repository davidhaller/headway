/*
 * Copyright (C) 2017 David Haller <davidhaller@mailbox.org>
 *
 * headway is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * headway is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>

namespace headway
{
    /*! \brief Utility class implementing a generic 2D array.
     */

    template<typename T>
    class matrix
    {
    public:

        /*! \brief Constructor for matrix class.
         *
         * \param width Width of the matrix, must be greater than zero.
         * \param height Height of the matrix, must be greater than zero.
         */

        matrix(unsigned int width, unsigned int height) :
                width_(width), height_(height), data(width * height)
        {
            if (width == 0 or height == 0)
                throw std::invalid_argument("Width and height must not be zero.");
        }

        matrix(const matrix& other) : width_(other.width()), height_(other.height()), data(other.data) {}

        unsigned int width() const { return width_; } /*!< Width of the matrix. */
        unsigned int height() const { return height_; } /*!< Height of the matrix. */
        unsigned long size() const { return data.size(); } /*!< Size of the matrix. */

        T get(unsigned int x, unsigned int y) const
        {
            return data[offset(x, y)];
        }

        void set(unsigned int x, unsigned int y, const T& value)
        {
            data[offset(x, y)] = value;
        }

        void fill(const T& value) noexcept
        {
            std::fill(data.begin(), data.end(), value);
        }

        bool operator==(const matrix& other) const
        {
            return width() == other.width() and
                   height() == other.height() and
                   data == other.data;
        }

        bool operator!=(const matrix& other) const
        {
            return !(*this == other);
        }

    private:
        const unsigned int width_;
        const unsigned int height_;

        std::vector<T> data;

        /*! \brief Calculates the array position of an item.
         *
         * Internally, the items are stored as one big array
         * with width x height as its size. This function is used
         * to transform the two coordinates into an array index.
         *
         * \returns An array index that can be directly used with data.
         */

        unsigned long offset(unsigned int x, unsigned int y) const noexcept
        {
            return y * width() + x;
        }
    };
}
