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
        throw FileException("Document is malformed.");

    const QJsonObject object = document.object();

    if (!object.contains("world"))
        throw FileException("Missing world element.");

    const QJsonValue rootValue = object.value("world");

    if (!rootValue.isObject())
        throw FileException("World element is malformed.");

    const QJsonObject root = rootValue.toObject();

    if (!root.contains("width"))
        throw FileException("Width attribute missing.");

    if (!root.contains("height"))
        throw FileException("Height attribute missing.");

    bool ok = true;

    width = root.value("width").toString().toUInt(&ok);
    if (!ok) throw FileException("Width attribute invalid.");

    height = root.value("height").toString().toUInt(&ok);
    if (!ok) throw FileException("Height attribute invalid.");

    if (!root.contains("generations"))
        generations = 0;
    else
    {
        generations = root.value("generations").toString().toULongLong(&ok);
        if (!ok) throw FileException("Generations attribute invalid.");
    }

    const QJsonValue array = root.value("cells");

    if (!array.isArray())
        throw FileException("Cells array missing.");

    cells = array.toArray();
    iterator = cells.begin();
}

void JsonFile::readCoordinate(quint32& x, quint32& y)
{
    const QJsonValue value = *iterator;
    ++iterator;

    if (!value.isObject())
        throw FileException("Cell value is not an object.");

    const QJsonObject cell = value.toObject();

    if (!cell.contains("x"))
        throw FileException("Missing x coordinate.");

    if (!cell.contains("y"))
        throw FileException("Missing y coordinate.");

    bool ok = true;

    x = cell.value("x").toString().toUInt(&ok);
    if (!ok) throw FileException("x coordinate has invalid format.");

    y = cell.value("y").toString().toUInt(&ok);
    if (!ok) throw FileException("y coordinate has invalid format.");

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
    root.insert("width", QString::number(biotope.width()));
    root.insert("height", QString::number(biotope.height()));
    root.insert("generations", QString::number(biotope.generations()));

    QJsonArray cells;

    for (quint32 y = 0; y < biotope.height(); ++y)
    {
        for (quint32 x = 0; x < biotope.width(); ++x)
        {
            if (biotope.isAlive(x, y)) // save alive cells only
            {
                QJsonObject item;
                item.insert("x", QString::number(x));
                item.insert("y", QString::number(y));

                cells.append(item);
            }
        }
    }

    root.insert("cells", cells);
    object.insert("world", root);
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
