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

#include <list>
#include <string>

#include <libxml++/parsers/domparser.h>

#include "file.hpp"
#include "world.hpp"

namespace headway
{
    /*! \brief Reads and writes headway::world objects from/to xml files.
     */

    class xml_file : public file
    {
    public:

        /*! \brief Reads data from xml file.
         *
         * \param path Location of xml file.
         */

        explicit xml_file(const std::string& path);

        unsigned int read_width() const override { return width; }
        unsigned int read_height() const override { return height; }
        unsigned long read_generations() const override { return generations; }

        void read_coordinate(unsigned int& x, unsigned int& y) override;
        bool has_next() const noexcept override;
        void rewind() noexcept override;

        /*! \brief Writes data to xml file.
         *
         * \param biotope World object to save.
         * \param path Location of xml file.
         */

        static void write(const headway::world& biotope, const std::string& path);

    private:
        unsigned int width;
        unsigned int height;
        unsigned long generations = 0;

        xmlpp::DomParser parser;
        std::list<const xmlpp::Node*> cells;
        std::list<const xmlpp::Node*>::const_iterator iterator;
    };
}
