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

    if (root.tagName() != "world")
        throw FileException("Unknown root element.");

    if (!root.hasAttribute("width"))
        throw FileException("Width attribute missing.");

    if (!root.hasAttribute("height"))
        throw FileException("Height attribute missing.");

    bool ok = true;

    width = root.attribute("width", "invalid").toUInt(&ok);
    if (!ok) throw FileException("Width attribute invalid.");

    height = root.attribute("height", "invalid").toUInt(&ok);
    if (!ok) throw FileException("Height attribute invalid.");

    generations = root.attribute("generations", "0").toULongLong(&ok);
    if (!ok) throw FileException("Generations attribute invalid.");

    cells = root.elementsByTagName("cell");
}

void XmlFile::readCoordinate(quint32& x, quint32& y)
{
    const QDomNode node = cells.item(index);
    ++index;

    if (node.isNull())
        throw FileException("No more coordinates to read.");

    const QDomElement elem = node.toElement();

    if (elem.isNull())
        throw FileException("Cell node is not an element type.");

    if (!elem.hasAttribute("x"))
        throw FileException("Missing x coordinate.");

    if (!elem.hasAttribute("y"))
        throw FileException("Missing y coordinate.");

    bool ok = true;

    x = elem.attribute("x", "invalid").toUInt(&ok);
    if (!ok) throw FileException("x coordinate has invalid format.");

    y = elem.attribute("y", "invalid").toUInt(&ok);
    if (!ok) throw FileException("y coordinate has invalid format.");
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
    QDomElement root = document.createElement("world");

    root.setAttribute("width", QString::number(biotope.width()));
    root.setAttribute("height", QString::number(biotope.height()));
    root.setAttribute("generations", QString::number(biotope.generations()));

    document.appendChild(root);

    for (quint32 y = 0; y < biotope.height(); ++y)
    {
        for (quint32 x = 0; x < biotope.width(); ++x)
        {
            if (biotope.isAlive(x, y)) // save alive cells only
            {
                QDomElement elem = document.createElement("cell");
                elem.setAttribute("x", QString::number(x));
                elem.setAttribute("y", QString::number(y));

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
