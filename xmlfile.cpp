#include <QFile>
#include <QTextStream>

#include "xmlfile.hpp"

using namespace Headway;

XmlFile::XmlFile(const QString& filePath)
{
    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text))
        throw FileException(QStringLiteral("Failed to open file: ") + fileHandle.errorString());

    QDomDocument::ParseResult parseResult = document.setContent(&fileHandle);
    if (!parseResult)
        throw FileException(QStringLiteral("Failed to parse file.\n\n") + parseResult.errorMessage);

    const QDomElement root = document.documentElement();

    if (root.tagName() != QStringLiteral("world"))
        throw FileException(QStringLiteral("Unknown root element."));

    if (!root.hasAttribute(QStringLiteral("width")))
        throw FileException(QStringLiteral("Width attribute missing."));

    if (!root.hasAttribute(QStringLiteral("height")))
        throw FileException(QStringLiteral("Height attribute missing."));

    bool ok = true;

    width = root.attribute(QStringLiteral("width"), QStringLiteral("invalid")).toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("Width attribute invalid."));

    height = root.attribute(QStringLiteral("height"), QStringLiteral("invalid")).toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("Height attribute invalid."));

    generations = root.attribute(QStringLiteral("generations"), 0).toULongLong(&ok);
    if (!ok) throw FileException(QStringLiteral("Generations attribute invalid."));

    cells = root.elementsByTagName(QStringLiteral("cell"));
}

void XmlFile::readCoordinate(quint32& x, quint32& y)
{
    const QDomNode node = cells.item(index);
    ++index;

    if (node.isNull())
        throw FileException(QStringLiteral("No more coordinates to read."));

    const QDomElement elem = node.toElement();

    if (elem.isNull())
        throw FileException(QStringLiteral("Cell node is not an element type."));

    if (!elem.hasAttribute(QStringLiteral("x")))
        throw FileException(QStringLiteral("Missing x coordinate."));

    if (!elem.hasAttribute(QStringLiteral("y")))
        throw FileException(QStringLiteral("Missing y coordinate."));

    bool ok = true;

    x = elem.attribute(QStringLiteral("x"), QStringLiteral("invalid")).toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("x coordinate has invalid format."));

    y = elem.attribute(QStringLiteral("y"), QStringLiteral("invalid")).toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("y coordinate has invalid format."));
}

bool XmlFile::hasNext() const noexcept
{
    return index < cells.size();
}

void XmlFile::rewind() noexcept
{
    index = 0;
}

void XmlFile::write(const QString& filePath, const Headway::World& biotope)
{
    QDomDocument document;
    QDomElement root = document.createElement(QStringLiteral("world"));

    root.setAttribute(QStringLiteral("width"), QString::number(biotope.width()));
    root.setAttribute(QStringLiteral("height"), QString::number(biotope.height()));
    root.setAttribute(QStringLiteral("generations"), QString::number(biotope.generations()));

    document.appendChild(root);

    for (quint32 y = 0; y < biotope.height(); ++y)
    {
        for (quint32 x = 0; x < biotope.width(); ++x)
        {
            if (biotope.isAlive(x, y)) // save alive cells only
            {
                QDomElement elem = document.createElement(QStringLiteral("cell"));
                elem.setAttribute(QStringLiteral("x"), QString::number(x));
                elem.setAttribute(QStringLiteral("y"), QString::number(y));

                root.appendChild(elem);
            }
        }
    }

    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw FileException(QStringLiteral("Failed to open file: ") + fileHandle.errorString());
    }

    QTextStream stream(&fileHandle);
    stream << document.toString(2);
}
