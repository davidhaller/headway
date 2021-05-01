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

#include "world.hpp"
#include "xml_file.hpp"

using namespace headway;

xml_file::xml_file(const std::string& path) : parser(path)
{
    const xmlpp::Document* doc = parser.get_document();
    const xmlpp::Element* root = doc->get_root_node();

    if (root->get_name() != "world")
        throw std::invalid_argument("Unknown root element.");

    try
    {
        width  = static_cast<unsigned int>(std::stoi(root->get_attribute_value("width")));
        height = static_cast<unsigned int>(std::stoi(root->get_attribute_value("height")));
    }

    catch (std::invalid_argument& ex)
    {
        throw std::invalid_argument("Width and/or height attribute missing.");
    }

    catch (std::out_of_range& ex)
    {
        throw std::invalid_argument("Width and/or height value too large.");
    }

    const std::string generations_attr = root->get_attribute_value("generations");

    if (not generations_attr.empty())
    {
        try
        {
            generations = std::stoul(generations_attr);
        }

        catch (std::out_of_range& ex)
        {
            throw std::invalid_argument("Generations value too large.");
        }
    }

    cells = root->get_children("cell");
    rewind();
}

void xml_file::read_coordinate(unsigned int& x, unsigned int& y)
{
    auto elem = dynamic_cast<const xmlpp::Element*>(*iterator); ++iterator;

    if (elem == nullptr)
        throw std::runtime_error("Cell node is not an element type.");

    try
    {
        x = static_cast<unsigned int>(std::stoi(elem->get_attribute_value("x")));
        y = static_cast<unsigned int>(std::stoi(elem->get_attribute_value("y")));
    }

    catch (std::exception& ex)
    {
        throw std::runtime_error("Cell element has invalid attributes.");
    }
}

bool xml_file::has_next() const noexcept
{
    return iterator != cells.end();
}

void xml_file::rewind() noexcept
{
    iterator = cells.begin();
}

void xml_file::write(const headway::world& biotope, const std::string& path)
{
    xmlpp::Document doc;
    xmlpp::Element* root = doc.create_root_node("world");

    root->set_attribute("width", std::to_string(biotope.width()));
    root->set_attribute("height", std::to_string(biotope.height()));
    root->set_attribute("generations", std::to_string(biotope.generations()));

    for (unsigned int y = 0; y < biotope.height(); ++y)
    {
        for (unsigned int x = 0; x < biotope.width(); ++x)
        {
            if (biotope.is_alive(x, y)) // save alive cells only
            {
                xmlpp::Element* elem = root->add_child_element("cell");
                elem->set_attribute("x", std::to_string(x));
                elem->set_attribute("y", std::to_string(y));
            }
        }
    }

    doc.write_to_file_formatted(path);
}
