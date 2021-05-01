#pragma once

#include <QtCore>

namespace Headway
{
    /*! \brief Interface for reading data from a file that stores a Headway::World object.
     */

    class File : public QObject
    {
        Q_OBJECT

    public:
        virtual quint32 readWidth() const = 0; /*!< Reads the width from file. */
        virtual quint32 readHeight() const = 0; /*!< Reads the height from file. */
        virtual quint64 readGenerations() const = 0; /*!< Reads the generations passed from file. */

        virtual void readCoordinate(quint32& x, quint32& y) = 0; /*!< Reads one pair of coordinates from file. */
        virtual bool hasNext() const noexcept = 0; /*!< Checks if end of file was reached */
        virtual void rewind() noexcept = 0; /*!< Sets iterator position to beginning of file. */
    };
}
