// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdbusmetatype.h"
#include "qdbusobjectmanagerserver.h"
#include "qdbusobjectmanager_p.h"

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

QDBusObjectManagerAdaptor::QDBusObjectManagerAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

QDBusObjectManagerAdaptor::~QDBusObjectManagerAdaptor()
{
}

QDBusObjPathInterfacesAndProperties QDBusObjectManagerAdaptor::GetManagedObjects() const
{
    QMap<QDBusObjectPath, QVariantMapMap> objpath_interfaces_and_properties;

    QMetaObject::invokeMethod(parent(), "getManagedObjects",
                              Q_RETURN_ARG(QDBusObjPathInterfacesAndProperties,
                                           objpath_interfaces_and_properties));

    return objpath_interfaces_and_properties;
}

QDBusObjectManagerServerDBusHelper::QDBusObjectManagerServerDBusHelper(QDBusObjectManagerServerPrivate &owner)
    : QObject(),
      m_owner(owner)
{
}

QDBusObjectManagerServerDBusHelper::~QDBusObjectManagerServerDBusHelper()
{
}

QDBusObjPathInterfacesAndProperties QDBusObjectManagerServerDBusHelper::getManagedObjects() const
{
    return m_owner.getManagedObjects();
}

QDBusObjectManagerServerPrivate::QDBusObjectManagerServerPrivate(const QString &service,
                                                                 const QString &path,
                                                                 const QDBusConnection &connection)
    : QDBusObjectManagerPrivate(service, path, connection),
      m_dbusHelper(*this)
{
    new QDBusObjectManagerAdaptor(&m_dbusHelper);
    m_connection.registerObject(path, &m_dbusHelper);
}

QDBusObjectManagerServerPrivate::~QDBusObjectManagerServerPrivate()
{
}

QDBusObjPathInterfacesAndProperties QDBusObjectManagerServerPrivate::getManagedObjects() const
{
    QDBusObjPathInterfacesAndProperties ret;

    return ret;
}

QDBusObjectManagerServer::QDBusObjectManagerServer(const QString &service, const QString &path,
                                                   const QDBusConnection &connection, QObject *parent)
    : QDBusObjectManager(*new QDBusObjectManagerServerPrivate(service, path, connection), parent)
{
}

QDBusObjectManagerServer::~QDBusObjectManagerServer()
{
}

bool QDBusObjectManagerServer::exportObject(const QString &path, const QString &interface,
                                            QObject *object, QDBusConnection::RegisterOptions options)
{
    Q_D(QDBusObjectManagerServer);

    auto ret = d->m_connection.registerObject(path, interface, object, options);
    if (!ret) {
        d->m_error = d->m_connection.lastError();
        return false;
    }

    return true;
}

void QDBusObjectManagerServer::unexportObject(const QString &path, const QString &interface,
                                              QDBusConnection::UnregisterMode mode)
{

}

QT_END_NAMESPACE

#endif // QT_NO_DBUS
