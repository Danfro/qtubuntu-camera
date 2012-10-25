/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Guenter Schwann <guenter.schwann@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "storagemanager.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>

const QLatin1String photoBase = QLatin1String("image");
const QLatin1String photoExtension = QLatin1String("jpg");
const QLatin1String videoExtension = QLatin1String("mpg");
const QLatin1String dateFormat = QLatin1String("yyyyMMdd");

StorageManager::StorageManager()
{
}

QString StorageManager::nextPhotoFileName(const QString &directoy)
{
    m_directory = directoy;
    if (m_directory.isEmpty())
        m_directory = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

    return nextMediaFileName(photoExtension);
}

QString StorageManager::nextVideoFileName(const QString &directoy)
{
    m_directory = directoy;
    if (m_directory.isEmpty())
        m_directory = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);

    return nextMediaFileName(videoExtension);
}

void StorageManager::checkDirectory(const QString &path) const
{
    QFileInfo fi(path);
    QDir dir;
    if (fi.isDir())
        dir.setPath(path);
    else
        dir.setPath(fi.absoluteDir().absolutePath());

    if (!dir.exists())
        dir.mkpath(path);
}

QString StorageManager::nextMediaFileName(const QString &extension)
{
    int idx = 1;
    QString fileName = fileNameGenerator(idx, extension);
    while (QFile::exists(fileName)) {
        ++idx;
        fileName = fileNameGenerator(idx, extension);
    }
    return fileName;
}

QString StorageManager::fileNameGenerator(int idx, const QString& extension)
{
    QString date = QDate::currentDate().toString(dateFormat);
    return QString("%1/%2%3_%4.%5")
            .arg(m_directory)
            .arg(photoBase)
            .arg(date)
            .arg(idx,4,10,QLatin1Char('0'))
            .arg(extension);
}
