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

#include <cstdio>
#include <stdexcept>

#include <gtest/gtest.h>
#include <glibmm.h>

#include "file.hpp"
#include "world.hpp"

using headway::file;
using headway::world;

TEST(world, create_zero)
{
    bool ex_thrown = false;

    try
    {
        const world biotope(0, 0, 0);
    }

    catch (std::invalid_argument& ex)
    {
        ex_thrown = true;
    }

    ASSERT_TRUE(ex_thrown);
}

TEST(world, random_too_high)
{
    bool ex_thrown = false;

    try
    {
        world biotope(40, 40, 0);
        biotope.random(50000);
    }

    catch (std::logic_error& ex)
    {
        ex_thrown = true;
    }

    ASSERT_TRUE(ex_thrown);
}

TEST(world, cell_write_invalid)
{
    bool ex_thrown = false;

    try
    {
        world biotope(10, 11, 0);
        biotope.create(10, 10);
    }

    catch (std::out_of_range& ex)
    {
        ex_thrown = true;
    }

    ASSERT_TRUE(ex_thrown);
}

TEST(world, cell_write_twice)
{
    world biotope(10, 10, 0);
    biotope.create(1, 2);

    ASSERT_FALSE(biotope.create(1, 2));
}

TEST(world, clear_all_cells)
{
    world w1(10, 15, 13);
    world w2(w1);

    w1.random(50);
    w1.clear();

    ASSERT_EQ(w1, w2);
}

TEST(world, next_eq)
{
    world w1(40, 30, 2);
    w1.random(100);

    world w2(w1);
    w1.next();

    ASSERT_NE(w1, w2);

    w2.next();

    ASSERT_EQ(w1, w2);
}

TEST(world, save_load_eq)
{
    world genesis(8, 20, 4);
    genesis.random(10);

    const std::string xml_path = Glib::build_filename(Glib::get_tmp_dir(), "headway_test_file.xml");
    const std::string json_path = Glib::build_filename(Glib::get_tmp_dir(), "headway_test_file.json");
    
    genesis.save(xml_path);
    genesis.save(json_path);

    auto xml_file = headway::world::load(xml_path);
    auto json_file = headway::world::load(json_path);
   
    const world xml_world(xml_file);
    const world json_world(json_file);

    ASSERT_EQ(genesis, xml_world);
    ASSERT_EQ(genesis, json_world);

    std::remove(xml_path.c_str());
    std::remove(json_path.c_str());
}
