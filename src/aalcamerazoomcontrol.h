/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AALCAMERAZOOMCONTROL_H
#define AALCAMERAZOOMCONTROL_H

#include <QCameraZoomControl>

class AalCameraService;
class CameraControl;
class CameraControlListener;

class AalCameraZoomControl : public QCameraZoomControl
{
    Q_OBJECT
public:
    AalCameraZoomControl(AalCameraService *service, QObject *parent = 0);

    qreal currentDigitalZoom() const;
    qreal currentOpticalZoom() const;
    qreal maximumDigitalZoom() const;
    qreal maximumOpticalZoom() const;
    qreal requestedDigitalZoom() const;
    qreal requestedOpticalZoom() const;
    void zoomTo(qreal optical, qreal digital);

    void resetZoom();

public Q_SLOTS:
    void init(CameraControl *control, CameraControlListener *listener);

private:
    AalCameraService *m_service;

    int m_currentDigitalZoom;
    int m_maximumDigitalZoom;
    int m_pendingZoom;
};

#endif // AALCAMERAZOOMCONTROL_H
