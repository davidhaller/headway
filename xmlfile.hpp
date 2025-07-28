#pragma once

#include <QString>
#include <QDomDocument>
#include <QDomNodeList>

#include "file.hpp"
#include "world.hpp"

namespace Headway
{
    /*! \brief Reads and writes Headway::World objects from/to xml files.
     */

    class XmlFile : public File
    {
    public:

        /*! \brief Reads data from xml file.
         *
         * \param filePath Location of xml file.
         * \throw FileException if file cannot be loaded.
         */

        explicit XmlFile(const QString& filePath);

        quint32 readWidth() const override { return width; }
        quint32 readHeight() const override { return height; }
        quint64 readGenerations() const override { return generations; }

        void readCoordinate(quint32& x, quint32& y) override;
        bool hasNext() const noexcept override;
        void rewind() noexcept override;

        /*! \brief Writes data to xml file.
         *
         * \param filePath Location of xml file.
         * \param biotope World object to save.
         *
         * \throw FileException if file cannot be saved.
         */

        static void write(const QString& filePath, const Headway::World& biotope);

    private:
        quint32 width;
        quint32 height;
        quint64 generations = 0;

        QDomDocument document;
        QDomNodeList cells;
        int index = 0;
    };
}
