#include <stdexcept>

using std::runtime_error;

#include "jsonfile.hpp"

using namespace Headway;

JsonFile::JsonFile(const QString& filePath)
{
    QFile fileHandle(filePath);

    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw runtime_error((QStringLiteral("Failed to open file: ") + fileHandle.errorString()).toStdString());
    }

    QTextStream stream(&fileHandle);
    QJsonParseError error;
    document = QJsonDocument::fromJson(stream.readAll().toUtf8(), &error);

    if (document.isNull())
        throw runtime_error((QStringLiteral("Failed to parse file.\n") + "\n" + error.errorString()).toStdString());

    if (!document.isObject())
        throw runtime_error("Document is malformed.");

    QJsonObject object = document.object();

    if (!object.contains("world"))
        throw runtime_error("Missing world element.");

    QJsonValue rootValue = object.value("world");

    if (!rootValue.isObject())
        throw runtime_error("World element is malformed.");

    QJsonObject root = rootValue.toObject();

    if (!root.contains("width"))
        throw runtime_error("Width attribute missing.");

    if (!root.contains("height"))
        throw runtime_error("Height attribute missing.");

    bool ok = true;

    width = root.value("width").toString().toUInt(&ok);
    if (!ok) throw runtime_error("Width attribute invalid.");

    height = root.value("height").toString().toUInt(&ok);
    if (!ok) throw runtime_error("Height attribute invalid.");

    if (!root.contains("generations"))
        generations = 0;
    else
    {
        generations = root.value("generations").toString().toULongLong(&ok);
        if (!ok) throw runtime_error("Generations attribute invalid.");
    }

    QJsonValue array = root.value("cells");

    if (!array.isArray())
        throw runtime_error("Cells array missing.");

    cells = array.toArray();
    rewind();
}

void JsonFile::readCoordinate(quint32& x, quint32& y)
{
    QJsonValue value = *iterator;
    ++iterator;

    if (!value.isObject())
        throw runtime_error("Cell value is not an object.");

    QJsonObject cell = value.toObject();

    if (!cell.contains("x"))
        throw runtime_error("Missing x coordinate.");

    if (!cell.contains("y"))
        throw runtime_error("Missing y coordinate.");

    bool ok = true;

    x = cell.value("x").toString().toUInt(&ok);
    if (!ok) throw runtime_error("x coordinate has invalid format.");

    y = cell.value("y").toString().toUInt(&ok);
    if (!ok) throw runtime_error("y coordinate has invalid format.");

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
        throw runtime_error((QStringLiteral("Failed to open file: ") + fileHandle.errorString()).toStdString());
    }

    QString text = QString::fromUtf8(document.toJson(QJsonDocument::Indented));
    QTextStream stream(&fileHandle);
    stream << text;
}
