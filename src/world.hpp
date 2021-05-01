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

#include <atomic>
#include <memory>
#include <string>
#include <sstream>
#include <random>

#include "file.hpp"
#include "matrix.hpp"

namespace headway
{
    /*! \brief A finite, two-dimensional world for the cells to live in.
     */

    class world
    {
    public:

        /*! \brief Constructor for world class.
         *
         * \param width Width of the world, must be greater than zero.
         * \param height Height of the world, must be greater than zero.
         * \param generations Generations that already passed, defaults to zero.
         */

        world(unsigned int width, unsigned int height, unsigned long generations = 0);

        /*! \brief copy constructor
         */

        world(const world& other);

        /*! \brief Constructs the world from a file.
         *
         * \param input The file to use.
         */

        explicit world(std::shared_ptr<headway::file> input);

        /*! \brief Loads file contents from disk.
         *
         * The type of the file is automatically recognized by its file extension.
         *
         * \param path Location of source file.
         *
         * \throw std::exception Various I/O errors can occur.
         * \return File pointer.
         */

        static std::shared_ptr<headway::file> load(const std::string& path);

        /*! \brief Saves the current generation to a file.
         *
         * \param path Relative or absolute filepath.
         * \throw std::exception Various I/O errors can occur.
         */

        void save(const std::string& path) const;

        unsigned int width() const { return matrix.width(); } /*!< Width of the world. */
        unsigned int height() const { return matrix.height(); } /*!< Height of the world. */
        unsigned long generations() const { return generations_; } /*!< Generations that passed. */
        unsigned long count() const { return count_; } /*!< Number of alive cells. */

        /*! \brief Checks if a cell is valid.
         *
         * *Valid* in this context means that the cell's coordinates are
         * within the world's boundaries (x < width and y < height).
         *
         * \return true if valid, false if invalid.
         */

        bool is_valid(unsigned int x, unsigned int y) const noexcept;

        /*! \brief Checks if a cell is alive.
         * \return true if alive, false if dead.
         */

        bool is_alive(unsigned int x, unsigned int y) const;

        /*! \brief Creates a cell and increases count.
         *
         * \throws std::out_of_range If cell is invalid.
         * \returns true if cell was dead, false if already alive.
         */

        bool create(unsigned int x, unsigned int y);

        /*! \brief Kills a cell and decreases count.
         *
         * \throws std::out_of_range If cell is invalid.
         * \returns true if cell was alive, false if already dead.
         */

        bool kill(unsigned int x, unsigned int y);

        /*! \brief Deletes all cells and sets count to zero.
         */

        void clear() noexcept;

        /*! \brief Counts the amount of neighbors a cell has.
         *
         * \throws std::out_of_range If cell is invalid.
         * \returns The amount of neighbors [0-8]
         */

        unsigned short neighbors(unsigned int x, unsigned int y) const;

        /*! \brief Creates cells at random locations.
         *
         * \param number How many cells to generate.
         * \throws std::logic_error If there is not enough space available.
         */

        void random(unsigned long number);

        /*! \brief Calculates and applies the next generation.
         *
         * Counts the neighbors of each cell, and then
         * applies the usual *Game Of Life* rules in-place.
         */

        void next() noexcept;

        bool operator==(const world& other) const;
        bool operator!=(const world& other) const;

    private:
        unsigned long generations_;
        std::atomic<unsigned long> count_;

        headway::matrix<bool> matrix;

        /*! \brief Checks if a cell is valid, throws exception if not.
         *
         * \throws std::out_of_range If cell is invalid.
         */

        void throw_if_invalid(unsigned int x, unsigned int y) const;
    };
}
