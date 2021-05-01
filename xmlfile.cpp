#include <stdexcept>

using std::runtime_error;

#include "xmlfile.hpp"

using namespace Headway;

XmlFile::XmlFile(const QString& filePath)
{
    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text))
        throw runtime_error((QStringLiteral("Failed to open file: ") + fileHandle.errorString()).toStdString());

    QString error;

    if (!document.setContent(&fileHandle, &error))
        throw runtime_error((QStringLiteral("Failed to parse file.\n") + "\n" + error).toStdString());

    QDomElement root = document.documentElement();

    if (root.tagName() != "world")
        throw runtime_error("Unknown root element.");

    if (!root.hasAttribute("width"))
        throw runtime_error("Width attribute missing.");

    if (!root.hasAttribute("height"))
        throw runtime_error("Height attribute missing.");

    bool ok = true;

    width = root.attribute("width", "invalid").toUInt(&ok);
    if (!ok) throw runtime_error("Width attribute invalid.");

    height = root.attribute("height", "invalid").toUInt(&ok);
    if (!ok) throw runtime_error("Height attribute invalid.");

    generations = root.attribute("generations", "0").toULongLong(&ok);
    if (!ok) throw runtime_error("Generations attribute invalid.");

    cells = root.elementsByTagName("cell");
    rewind();
}

void XmlFile::readCoordinate(quint32& x, quint32& y)
{
    QDomNode node = cells.item(index);
    ++index;

    if (node.isNull())
        throw runtime_error("No more coordinates to read.");

    QDomElement elem = node.toElement();

    if (elem.isNull())
        throw runtime_error("Cell node is not an element type.");

    if (!elem.hasAttribute("x"))
        throw runtime_error("Missing x coordinate.");

    if (!elem.hasAttribute("y"))
        throw runtime_error("Missing y coordinate.");

    bool ok = true;

    x = elem.attribute("x", "invalid").toUInt(&ok);
    if (!ok) throw runtime_error("x coordinate has invalid format.");

    y = elem.attribute("y", "invalid").toUInt(&ok);
    if (!ok) throw runtime_error("y coordinate has invalid format.");
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
        throw runtime_error((QStringLiteral("Failed to open file: ") + fileHandle.errorString()).toStdString());
    }

    QTextStream stream(&fileHandle);
    stream << document.toString(2);
}
