// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDBUSOBJECTMANAGERSERVER_H
#define QDBUSOBJECTMANAGERSERVER_H

#include <QtCore/qstring.h>
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusobjectmanager.h>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

class QDBusObjectManagerServerPrivate;

class Q_DBUS_EXPORT QDBusObjectManagerServer : public QDBusObjectManager
{
    Q_OBJECT

public:
    QDBusObjectManagerServer(const QString &service, const QString &path,
                             const QDBusConnection &connection = QDBusConnection::sessionBus(),
                             QObject *parent = nullptr);
    virtual ~QDBusObjectManagerServer();

    bool exportObject(const QString &path, const QString &interface, QObject *object,
                      QDBusConnection::RegisterOptions options = QDBusConnection::ExportAdaptors);
    void unexportObject(const QString &path, const QString &interface,
                        QDBusConnection::UnregisterMode mode = QDBusConnection::UnregisterNode);

private:
    Q_DISABLE_COPY_MOVE(QDBusObjectManagerServer);
    Q_DECLARE_PRIVATE(QDBusObjectManagerServer);
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS

#endif // QDBUSOBJECTMANAGER_H
