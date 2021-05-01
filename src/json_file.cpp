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

#include <fstream>
#include <iostream>

#include "world.hpp"
#include "json_file.hpp"

using namespace headway;

json_file::json_file(const std::string& path)
{
    Json::CharReaderBuilder builder;
    std::ifstream json_file(path);
    std::string errs;

    if (!Json::parseFromStream(builder, json_file, &json_data, &errs))
    {
        throw std::invalid_argument("Malformed JSON string.");
    }

    else json_file.close();

    if (!json_data.isMember("world"))
    {
        throw std::invalid_argument("Missing world element.");
    }

    auto world = json_data["world"];

    if (not world.isMember("width") or not world.isMember("height"))
    {
        throw std::invalid_argument("Width and/or height attribute missing.");
    }

    else if (not world["width"].isUInt() or not world["height"].isUInt())
    {
        throw std::invalid_argument("Width and/or height value invalid type.");
    }

    else
    {
        width  = world["width"].asUInt();
        height = world["height"].asUInt();
    }

    if (world.isMember("generations"))
    {
        if (world["generations"].isUInt64())
            generations = world["generations"].asUInt64();
        else throw std::invalid_argument("Generations value invalid type.");
    }

    if (world.isMember("cells") and world["cells"].isArray())
    {
        cells = world["cells"];
        rewind();
    }
}

void json_file::read_coordinate(unsigned int& x, unsigned int& y)
{
    auto elem = cells[index]; ++index;

    if (not elem.isMember("x") or not elem.isMember("y"))
        throw std::runtime_error("Missing x or y element.");

    else if (not elem["x"].isUInt() or not elem["y"].isUInt())
        throw std::runtime_error("x or y value invalid type.");

    else
    {
        x = elem["x"].asUInt();
        y = elem["y"].asUInt();

    }
}

bool json_file::has_next() const noexcept
{
    return index < cells.size();
}

void json_file::rewind() noexcept
{
    index = 0;
}

void json_file::write(const headway::world& biotope, const std::string& path)
{
    Json::Value json_data;
    Json::Value world;
    Json::Value cells;

    world["width"] = biotope.width();
    world["height"] = biotope.height();
    world["generations"] = biotope.generations();

    for (unsigned int y = 0; y < biotope.height(); ++y)
    {
        for (unsigned int x = 0; x < biotope.width(); ++x)
        {
            if (biotope.is_alive(x, y)) // save alive cells only
            {
                Json::Value cell;
                cell["x"] = x; cell["y"] = y;
                cells.append(cell);
            }
        }
    }

    world["cells"] = cells;
    json_data["world"] = world;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

    std::ofstream json_file(path);
    writer->write(json_data, &json_file);
    json_file.close();
}
