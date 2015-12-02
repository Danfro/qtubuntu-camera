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

#include "aalcamerazoomcontrol.h"
#include "aalcameracontrol.h"
#include "aalcameraservice.h"

#include <QDebug>

#include <hybris/camera/camera_compatibility_layer.h>
#include <hybris/camera/camera_compatibility_layer_capabilities.h>

AalCameraZoomControl::AalCameraZoomControl(AalCameraService *service, QObject *parent)
    : QCameraZoomControl(parent),
      m_service(service),
      m_currentDigialZoom(1),
      m_maximalDigitalZoom(1),
      m_pendingZoom(-1)
{
}

qreal AalCameraZoomControl::currentDigitalZoom() const
{
    return (qreal)m_currentDigialZoom;
}

qreal AalCameraZoomControl::currentOpticalZoom() const
{
    return 1.0;
}

qreal AalCameraZoomControl::maximumDigitalZoom() const
{
    return (qreal)m_maximalDigitalZoom;
}

qreal AalCameraZoomControl::maximumOpticalZoom() const
{
    return 1.0;
}

qreal AalCameraZoomControl::requestedDigitalZoom() const
{
    return (qreal)m_pendingZoom;
}

qreal AalCameraZoomControl::requestedOpticalZoom() const
{
    return 1.0;
}

void AalCameraZoomControl::zoomTo(qreal optical, qreal digital)
{
    Q_UNUSED(optical);

    if (!m_service->androidControl())
        return;

    if (digital < 1.0 || digital > m_maximalDigitalZoom) {
        qWarning() << "Invalid zoom value:" << digital;
        return;
    }

    m_pendingZoom = static_cast<int>(digital);

    if (m_pendingZoom == m_currentDigialZoom)
        return;

    android_camera_set_zoom(m_service->androidControl(), m_pendingZoom);
    m_currentDigialZoom = m_pendingZoom;
    Q_EMIT currentDigitalZoomChanged(m_currentDigialZoom);
}

void AalCameraZoomControl::init(CameraControl *control, CameraControlListener *listener)
{
    Q_UNUSED(control);
    Q_UNUSED(listener);

    resetZoom();
}

/*!
 * \brief AalCameraZoomControl::reset sets the current zoom value to 1 and the
 * maximum zoom value to the maximum zoom level that the hardware reports as
 * supporting
 */
void AalCameraZoomControl::resetZoom()
{
    if (!m_service->androidControl()) {
        return;
    }

    if (m_currentDigialZoom != 1) {
        m_currentDigialZoom = 1;
        Q_EMIT currentDigitalZoomChanged(m_currentDigialZoom);
    }

    android_camera_set_zoom(m_service->androidControl(), m_currentDigialZoom);

    int maxValue = 1;
    android_camera_get_max_zoom(m_service->androidControl(), &maxValue);
    if (maxValue < 1) {
        return;
    }

    if (maxValue != m_maximalDigitalZoom) {
        m_maximalDigitalZoom = maxValue;
        Q_EMIT maximumDigitalZoomChanged(m_maximalDigitalZoom);
    }
}
