#include "jsonfile.hpp"
#include "xmlfile.hpp"
#include "world.hpp"

using namespace Headway;

World::World(QObject* parent) : QObject(parent)
{
    createWorld();
}

World::World(const World& other) : QObject(other.parent()), generations_(other.generations()), count_(other.count()), matrix(new Matrix<bool>(*(other.matrix)))
{
}

World::~World() noexcept
{
    delete matrix;
}

bool World::createWorld(quint32 width, quint32 height, quint64 generations) noexcept
{
    if (width == 0)
    {
        emit error("Width must not be zero.");
        return false;
    }

    if (height == 0)
    {
        emit error("Height must not be zero.");
        return false;
    }

    if (matrix != nullptr)
        delete matrix;

    matrix = new Matrix<bool>(width, height);
    matrix->fill(false);

    generations_ = generations; emit generationsChanged(generations);
    count_ = 0; emit countChanged(0);

    return true;
}

bool World::loadFile(const QUrl& fileUrl) noexcept
{
    QSharedPointer<File> file;
    const QString filePath = fileUrl.toLocalFile();

    try
    {
        if (filePath.endsWith(QStringLiteral(".xml"), Qt::CaseInsensitive))
        {
            file = QSharedPointer<File>(new XmlFile(filePath));
        }

        else if (filePath.endsWith(QStringLiteral(".json"), Qt::CaseInsensitive))
        {
            file = QSharedPointer<File>(new JsonFile(filePath));
        }

        else
        {
            emit error("Unknown file type.");
            return false;
        }

        if (!createWorld(file->readWidth(), file->readHeight(), file->readGenerations()))
            return false;

        while (file->hasNext())
        {
            quint32 x, y;

            file->readCoordinate(x, y);
            createCell(x, y);
        }
    }

    catch (FileException& e)
    {
        emit error(QStringLiteral("File is invalid: ") + e.error());
        return false;
    }

    return true;
}

bool World::saveFile(const QUrl& fileUrl) noexcept
{
    const QString filePath = fileUrl.toLocalFile();

    try
    {
        if (filePath.endsWith(QStringLiteral(".xml"), Qt::CaseInsensitive))
        {
            XmlFile::write(filePath, *this);
        }

        else if (filePath.endsWith(QStringLiteral(".json"), Qt::CaseInsensitive))
        {
            JsonFile::write(filePath, *this);
        }

        else
        {
            emit error("Unknown file type.");
            return false;
        }
    }

    catch (FileException& e)
    {
        emit error(e.error());
        return false;
    }

    return true;
}

bool World::isValid(quint32 x, quint32 y) const noexcept
{
    return x < width() && y < height();
}

bool World::isAlive(quint32 x, quint32 y) const noexcept
{
    if (!isValid(x ,y))
        return false;

    return matrix->get(x, y);
}

bool World::createCell(quint32 x, quint32 y) noexcept
{
    if (isValid(x, y) && !isAlive(x, y))
    {
        matrix->set(x, y, true);
        emit countChanged(++count_);

        return true;
    }

    return false;
}

bool World::killCell(quint32 x, quint32 y) noexcept
{
    if (isValid(x, y) && isAlive(x, y))
    {
        matrix->set(x, y, false);
        emit countChanged(--count_);

        return true;
    }

    return false;
}

quint8 World::neighbors(quint32 x, quint32 y) const noexcept
{
    quint8 amount = 0;

    for (quint32 b = y - 1; b <= y + 1; ++b)
    {
        for (quint32 a = x - 1; a <= x + 1; ++a)
        {
            if (y == b && x == a) continue; // ignore self
            if (isValid(a, b) && isAlive(a, b)) ++amount;
        }
    }

    return amount;
}

bool World::random(quint64 number) noexcept
{
    if (number > matrix->size() - count())
    {
        emit error("Number of cells exceeds available space.");
        return false;
    }

    QRandomGenerator* chaos = QRandomGenerator::global();

    while (number > 0)
    {
        const quint32 x = chaos->bounded(width());
        const quint32 y = chaos->bounded(height());

        if (createCell(x, y)) // if cell did not exist
            --number;
    }

    return true;
}

void World::next() noexcept
{
    Headway::Matrix<quint8> cache(width(), height()); // store neighbor information

    for (quint32 y = 0; y < height(); ++y)
    {
        for (quint32 x = 0; x < width(); ++x)
        {
            const quint8 n = neighbors(x, y);
            cache.set(x, y, n);
        }
    }

    blockSignals(true); // emit countChanged only once

    for (quint32 y = 0; y < height(); ++y)
    {
        for (quint32 x = 0; x < width(); ++x)
        {
            const quint8 n = cache.get(x, y);

            if (n <= 1 || n >= 4) killCell(x, y);
            else if (n == 3) createCell(x, y);
        }
    }

    blockSignals(false);

    emit countChanged(count_);
    emit generationsChanged(++generations_);
}

bool World::operator==(const World& other) const
{
    return *matrix == *(other.matrix) &&
           generations_ == other.generations_ &&
           count_ == other.count_;
}

bool World::operator!=(const World& other) const
{
    return !(*this == other);
}
