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
#include "json_file.hpp"
#include "xml_file.hpp"

using namespace headway;

world::world(unsigned int width, unsigned int height, unsigned long generations) : generations_(generations), count_(0), matrix(width, height)
{
    matrix.fill(false);
}

world::world(const world& other) : generations_(other.generations()), count_(other.count()), matrix(other.matrix)
{
}

world::world(std::shared_ptr<headway::file> input) : generations_(input->read_generations()), count_(0), matrix(input->read_width(), input->read_height())
{
    while (input->has_next())
    {
        unsigned int x, y;

        input->read_coordinate(x, y);
        create(x, y);
    }
}

std::shared_ptr<headway::file> world::load(const std::string& path)
{
    if (headway::file::check_extension(path, ".xml"))
    {
        return std::make_shared<headway::xml_file>(path);
    }

    else if (headway::file::check_extension(path, ".json"))
    {
        return std::make_shared<headway::json_file>(path);
    }

    throw std::invalid_argument("Unknown file type.");
}

void world::save(const std::string& path) const
{
    if (headway::file::check_extension(path, ".xml"))
    {
        headway::xml_file::write(*this, path);
    }

    else if (headway::file::check_extension(path, ".json"))
    {
        headway::json_file::write(*this, path);
    }

    else throw std::invalid_argument("Unknown file type.");
}

bool world::is_valid(unsigned int x, unsigned int y) const noexcept
{
    return x < width() and y < height();
}

bool world::is_alive(unsigned int x, unsigned int y) const
{
    throw_if_invalid(x, y);
    return matrix.get(x, y);
}

bool world::create(unsigned int x, unsigned int y)
{
    if (!is_alive(x, y))
    {
        matrix.set(x, y, true);
        ++count_;
        return true;
    }

    return false;
}

bool world::kill(unsigned int x, unsigned int y)
{
    if (is_alive(x, y))
    {
        matrix.set(x, y, false);
        --count_;
        return true;
    }

    return false;
}

void world::clear() noexcept
{
    count_ = 0;
    matrix.fill(false);
}

unsigned short world::neighbors(unsigned int x, unsigned int y) const
{
    throw_if_invalid(x, y);
    unsigned short amount = 0;

    for (unsigned int b = y - 1; b <= y + 1; ++b)
    {
        for (unsigned int a = x - 1; a <= x + 1; ++a)
        {
            if (y == b and x == a) continue; // ignore self
            if (is_valid(a, b) and is_alive(a, b)) ++amount;
        }
    }

    return amount;
}

void world::random(unsigned long number)
{
    if (number > matrix.size() - count_)
        throw std::logic_error("Number of cells exceeds available space.");

    std::default_random_engine random;
    std::uniform_int_distribution<unsigned int> x(0, width() - 1);
    std::uniform_int_distribution<unsigned int> y(0, height() - 1);

    while (number > 0)
    {
        if (create(x(random), y(random))) // if cell did not exist
            --number;
    }
}

void world::next() noexcept
{
    headway::matrix<unsigned short> cache(width(), height()); // store neighbor information

    for (unsigned int y = 0; y < height(); ++y)
    {
        for (unsigned int x = 0; x < width(); ++x)
        {
            unsigned short n = neighbors(x, y);
            cache.set(x, y, n);
        }
    }

    for (unsigned int y = 0; y < height(); ++y)
    {
        for (unsigned int x = 0; x < width(); ++x)
        {
            unsigned short n = cache.get(x, y);

            if (n <= 1 or n >= 4) kill(x, y);
            else if (n == 3) create(x, y);
        }
    }

    ++generations_;
}

bool world::operator==(const world& other) const
{
    return matrix == other.matrix and
           generations_ == other.generations_ and
           count_ == other.count_;
}

bool world::operator!=(const world& other) const
{
    return !(*this == other);
}

void world::throw_if_invalid(unsigned int x, unsigned int y) const
{
    if (is_valid(x, y))
        return;

    std::ostringstream error;
    error << "Cell invalid: x = " << x << "; y = " << y;
    throw std::out_of_range(error.str());
}
