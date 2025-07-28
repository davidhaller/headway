#pragma once

#include <QObject>
#include <QException>
#include <QString>
#include <QByteArray>

namespace Headway
{
    /*! \brief Interface for reading data from a file that stores a Headway::World object.
     */

    class File : public QObject
    {
    public:
        virtual quint32 readWidth() const = 0; /*!< Reads the width from file. */
        virtual quint32 readHeight() const = 0; /*!< Reads the height from file. */
        virtual quint64 readGenerations() const = 0; /*!< Reads the generations passed from file. */

        virtual void readCoordinate(quint32& x, quint32& y) = 0; /*!< Reads one pair of coordinates from file. */
        virtual bool hasNext() const noexcept = 0; /*!< Checks if end of file was reached */
        virtual void rewind() noexcept = 0; /*!< Sets iterator position to beginning of file. */
    };

    /*!
     * \brief Exception class thrown if reading or writing fails.
     */

    class FileException : public QException
    {
    public:
        /*!
         * \brief Creates a new FileException.
         *
         * \param message A human-readable description of the error which can be shown to the user.
         */

        explicit FileException(const QString& message): message_(message), buffer_(message.toLocal8Bit()) {}

        void raise() const override { throw *this; }
        FileException* clone() const override { return new FileException(*this); }

        QString error() const noexcept { return message_; }

        const char* what() const noexcept override { return buffer_.data();}

    private:
        const QString message_;
        const QByteArray buffer_;
    };
}
