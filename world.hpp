#pragma once

#include <QtCore>

#include "file.hpp"
#include "matrix.hpp"

namespace Headway
{
    /*! \brief A finite, two-dimensional world for the cells to live in.
     */

    class World : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(quint32 width READ width)
        Q_PROPERTY(quint32 height READ height)
        Q_PROPERTY(quint64 generations READ generations NOTIFY generationsChanged)
        Q_PROPERTY(quint64 count READ count NOTIFY countChanged)

    public:
        quint32 width() const { return matrix->width(); } /*!< Width of the world. */
        quint32 height() const { return matrix->height(); } /*!< Height of the world. */
        quint64 generations() const { return generations_; } /*!< Generations that passed. */
        quint64 count() const { return count_; } /*!< Number of alive cells. */

        /*! \brief Default constructor for World class.
         */

        explicit World(QObject* parent = nullptr);

        /*! \brief Copy constructor
        */

        World(const World& other);

        /*!
         * \brief Destructor for World class.
         */

        virtual ~World() noexcept;

        /*! \brief Creates the world with new parameters.
         *
         * This method changes the properties of
         * the world and kills all cells.
         *
         * If creation fails, the error signal is emitted.
         *
         * \param width Width of the world, defaults to 30.
         * \param height Height of the world, defaults to 30.
         * \param generations Generations that already passed, defaults to zero.
         *
         * \return true if creation was successful, false if not.
         */

        Q_INVOKABLE bool createWorld(quint32 width = 30, quint32 height = 30, quint64 generations = 0) noexcept;

        /*! \brief Loads World object from file.
         *
         * The type of the file is automatically
         * recognized by its file extension.
         *
         * If loading fails, the error signal is
         * emitted and old world state is restored.
         *
         * \param fileUrl Location of source file.
         * \return true if loading was successful, false if not.
        */

        Q_INVOKABLE bool loadFile(const QUrl& fileUrl) noexcept;

        /*! \brief Saves World object to file.
         *
         * The type of the file is automatically
         * recognized by its file extension.
         *
         * If saving fails, the error signal is emitted.
         *
         * \param fileUrl Location of destination file.
         * \return true if saving was successful, false if not.
         */

        Q_INVOKABLE bool saveFile(const QUrl& fileUrl) noexcept;

        /*! \brief Checks if a cell is valid.
         *
         * Valid in this context means that the cell's coordinates are
         * within the world's boundaries (x < width and y < height).
         *
         * \return true if valid, false otherwise.
         */

        Q_INVOKABLE bool isValid(quint32 x, quint32 y) const noexcept;

        /*! \brief Checks if a cell is alive.
         *
         * \return true if alive and valid, false otherwise.
         */

        Q_INVOKABLE bool isAlive(quint32 x, quint32 y) const noexcept;

        /*! \brief Creates a cell and increases count.
         *
         * \return true if cell is valid and was dead before, false otherwise.
         */

        Q_INVOKABLE bool createCell(quint32 x, quint32 y) noexcept;

        /*! \brief Kills a cell and decreases count.
         *
         * \return true if cell is valid and was alive before, false otherwise.
         */

        Q_INVOKABLE bool killCell(quint32 x, quint32 y) noexcept;

        /*! \brief Counts the amount of neighbors a cell has.
         *
         * \return The amount of neighbors [0-8].
         */

        Q_INVOKABLE quint8 neighbors(quint32 x, quint32 y) const noexcept;

        /*! \brief Creates cells at random locations.
         *
         * \param number How many cells to generate.
         * \return false if number of cells exceeded available space, true otherwise.
         */

        Q_INVOKABLE bool random(quint64 number) noexcept;

        /*! \brief Calculates and applies the next generation.
         *
         * Counts the neighbors of each cell, and then
         * applies the usual *Game Of Life* rules in-place.
         */

        Q_INVOKABLE void next() noexcept;

        bool operator==(const World& other) const;
        bool operator!=(const World& other) const;

    signals:
        void generationsChanged(quint64 generations);
        void countChanged(quint64 count);

        void error(const QString& message);

    private:
        quint64 generations_ = 0;
        std::atomic<quint64> count_ = ATOMIC_VAR_INIT(0);

        Headway::Matrix<bool>* matrix = nullptr;
    };
}
