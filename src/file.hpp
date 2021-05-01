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

#include <string>

namespace headway
{
    /*! \brief Interface for reading data from a file that stores a headway::world object.
     */

    class file
    {
    public:
        virtual unsigned int read_width() const = 0; /*!< Reads the width from file. */
        virtual unsigned int read_height() const = 0; /*!< Reads the height from file. */
        virtual unsigned long read_generations() const = 0; /*!< Reads the generations passed from file. */

        virtual void read_coordinate(unsigned int& x, unsigned int& y) = 0; /*!< Reads one pair of coordinates from file. */
        virtual bool has_next() const noexcept = 0; /*!< Checks if end of file was reached */
        virtual void rewind() noexcept = 0; /*!< Sets iterator position to beginning of file. */

        /*! \brief Checks if a filename ends with a specific extension.
         *
         * \param filename The filename that is checked.
         * \param extension The extension to check for.
         *
         * \return true if filename ends with extension, false if not.
         */

        static inline bool check_extension(const std::string& filename, const std::string& extension) noexcept
        {
            return extension.size() <= filename.size() &&
                   std::equal(extension.rbegin(), extension.rend(), filename.rbegin());
        }
    };
}
