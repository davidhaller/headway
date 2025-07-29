#include <QFile>
#include <QJsonObject>

#include "jsonfile.hpp"

using namespace Headway;

JsonFile::JsonFile(const QString& filePath)
{
    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw FileException(QStringLiteral("Failed to open file: ") + fileHandle.errorString());
    }

    QTextStream stream(&fileHandle);
    QJsonParseError error;
    document = QJsonDocument::fromJson(stream.readAll().toUtf8(), &error);

    if (document.isNull())
        throw FileException(QStringLiteral("Failed to parse file.\n\n") + error.errorString());

    if (!document.isObject())
        throw FileException(QStringLiteral("Document is malformed."));

    const QJsonObject object = document.object();

    if (!object.contains(QStringLiteral("world")))
        throw FileException(QStringLiteral("Missing world element."));

    const QJsonValue rootValue = object.value(QStringLiteral("world"));

    if (!rootValue.isObject())
        throw FileException(QStringLiteral("World element is malformed."));

    const QJsonObject root = rootValue.toObject();

    if (!root.contains(QStringLiteral("width")))
        throw FileException(QStringLiteral("Width attribute missing."));

    if (!root.contains(QStringLiteral("height")))
        throw FileException(QStringLiteral("Height attribute missing."));

    bool ok = true;

    width = root.value(QStringLiteral("width")).toString().toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("Width attribute invalid."));

    height = root.value(QStringLiteral("height")).toString().toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("Height attribute invalid."));

    if (!root.contains(QStringLiteral("generations")))
        generations = 0;
    else
    {
        generations = root.value(QStringLiteral("generations")).toString().toULongLong(&ok);
        if (!ok) throw FileException(QStringLiteral("Generations attribute invalid."));
    }

    const QJsonValue array = root.value(QStringLiteral("cells"));

    if (!array.isArray())
        throw FileException(QStringLiteral("Cells array missing."));

    cells = array.toArray();
    iterator = cells.begin();
}

void JsonFile::readCoordinate(quint32& x, quint32& y)
{
    const QJsonValue value = *iterator;
    ++iterator;

    if (!value.isObject())
        throw FileException(QStringLiteral("Cell value is not an object."));

    const QJsonObject cell = value.toObject();

    if (!cell.contains(QStringLiteral("x")))
        throw FileException(QStringLiteral("Missing x coordinate."));

    if (!cell.contains(QStringLiteral("y")))
        throw FileException(QStringLiteral("Missing y coordinate."));

    bool ok = true;

    x = cell.value(QStringLiteral("x")).toString().toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("x coordinate has invalid format."));

    y = cell.value(QStringLiteral("y")).toString().toUInt(&ok);
    if (!ok) throw FileException(QStringLiteral("y coordinate has invalid format."));

}

bool JsonFile::hasNext() const noexcept
{
    return iterator != cells.end();
}

void JsonFile::rewind() noexcept
{
    iterator = cells.begin();
}

void JsonFile::write(const QString& filePath, const Headway::World& biotope)
{
    QJsonDocument document;
    QJsonObject object;

    QJsonObject root;
    root.insert(QStringLiteral("width"), QString::number(biotope.width()));
    root.insert(QStringLiteral("height"), QString::number(biotope.height()));
    root.insert(QStringLiteral("generations"), QString::number(biotope.generations()));

    QJsonArray cells;

    for (quint32 y = 0; y < biotope.height(); ++y)
    {
        for (quint32 x = 0; x < biotope.width(); ++x)
        {
            if (biotope.isAlive(x, y)) // save alive cells only
            {
                QJsonObject item;
                item.insert(QStringLiteral("x"), QString::number(x));
                item.insert(QStringLiteral("y"), QString::number(y));

                cells.append(item);
            }
        }
    }

    root.insert(QStringLiteral("cells"), cells);
    object.insert(QStringLiteral("world"), root);
    document.setObject(object);

    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw FileException(QStringLiteral("Failed to open file: ") + fileHandle.errorString());
    }

    const QString text = QString::fromUtf8(document.toJson(QJsonDocument::Indented));
    QTextStream stream(&fileHandle);
    stream << text;
}
