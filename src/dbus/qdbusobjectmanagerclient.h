// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDBUSOBJECTMANAGERCLIENT_H
#define QDBUSOBJECTMANAGERCLIENT_H

#include <QtCore/qstring.h>
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusobjectmanager.h>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QDBusObjectManagerClientPrivate;

class Q_DBUS_EXPORT QDBusObjectManagerClient : public QDBusObjectManager
{
    Q_OBJECT

public:
    QDBusObjectManagerClient(const QString &service, const QString &path,
                             const QDBusConnection &connection = QDBusConnection::sessionBus(),
                             QObject *parent = nullptr);
    virtual ~QDBusObjectManagerClient();

private:
    Q_DISABLE_COPY_MOVE(QDBusObjectManagerClient);
    Q_DECLARE_PRIVATE(QDBusObjectManagerClient);
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS

#endif // QDBUSOBJECTMANAGER_H
