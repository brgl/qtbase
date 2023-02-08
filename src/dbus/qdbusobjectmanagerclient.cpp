// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdbusobjectmanagerclient.h"
#include "qdbusobjectmanager_p.h"

#include <QtDBus/qdbusargument.h>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

QDBusObjectManagerClientDBusHelper::QDBusObjectManagerClientDBusHelper(QDBusObjectManagerPrivate &owner)
    : m_owner(owner)
{
}

QDBusObjectManagerClientDBusHelper::~QDBusObjectManagerClientDBusHelper()
{
}

void QDBusObjectManagerClientDBusHelper::interfacesAdded(const QDBusObjectPath &path,
                                                         const QVariantMapMap &interfaces)
{
    m_owner.interfacesAdded(path, interfaces);
}

void QDBusObjectManagerClientDBusHelper::interfacesRemoved(const QDBusObjectPath &path,
                                                           const QStringList &interfaces)
{
    m_owner.interfacesRemoved(path, interfaces);
}

QDBusObjectManagerClientPrivate::QDBusObjectManagerClientPrivate(const QString &service,
                                                                 const QString &path,
                                                                 const QDBusConnection &connection)
    : QDBusObjectManagerPrivate(service, path, connection),
      m_dbusHelper(*this)
{
    bool ret;

    ret = m_connection.connect(service, path,
                               QString::fromUtf8("org.freedesktop.DBus.ObjectManager"),
                               QString::fromUtf8("InterfacesAdded"),
                               &m_dbusHelper, SLOT(interfacesAdded(QDBusObjectPath, QVariantMapMap)));
    if (!ret) {
        m_error = connection.lastError();
        return;
    }

    ret = m_connection.connect(service, path,
                               QString::fromUtf8("org.freedesktop.DBus.ObjectManager"),
                               QString::fromUtf8("InterfacesRemoved"),
                               &m_dbusHelper, SLOT(interfacesRemoved(QDBusObjectPath, QStringList)));
    if (!ret) {
        m_error = connection.lastError();
        return;
    }

    QDBusInterface intf(service, path,
                        QString::fromUtf8("org.freedesktop.DBus.ObjectManager"),
                        connection);
    if (!intf.isValid()) {
        m_error = connection.lastError();
        return;
    }

    auto reply = intf.call(QString::fromUtf8("GetManagedObjects"));
    if (reply.type() != QDBusMessage::ReplyMessage) {
        if (reply.type() == QDBusMessage::ErrorMessage) {
            m_error = QDBusError(QDBusError::Failed, reply.errorMessage());
            return;
        }
    }

    auto args = reply.arguments();
    if (args.size() != 1) {
        m_error = QDBusError(QDBusError::InvalidArgs,
                             QString::fromUtf8("invalid response format from ObjectManager"));
        return;
    }

    QMap<QDBusObjectPath, QVariantMapMap> objectMap;
    args.at(0).value<QDBusArgument>() >> objectMap;

    for (const auto &key: objectMap.keys()) {
        auto val = objectMap[key];
        QDBusInterfacePtr objIntf(new QDBusInterface(service, key.path(), QString(), connection));
        if (!objIntf->isValid()) {
            m_error = connection.lastError();
            return;
        }

        QDBusInterfaceMap intfMap;

        for (const auto &intf: val.keys()) {
            QDBusInterfacePtr intfIntf(new QDBusInterface(service, key.path(), intf, connection));
            if (!intfIntf->isValid()) {
                m_error = connection.lastError();
                return;
            }

            QStringList props;

            for (const auto &prop: val[intf].keys())
                props.append(prop);

            intfMap.insert(intf, {intfIntf, props});
        }

        m_managedObjects.insert(key, {objIntf, intfMap});
    }
}

QDBusObjectManagerClientPrivate::~QDBusObjectManagerClientPrivate()
{
}

QDBusObjectManagerClient::QDBusObjectManagerClient(const QString &service, const QString &path,
                                                   const QDBusConnection &connection, QObject *parent)
    : QDBusObjectManager(*new QDBusObjectManagerClientPrivate(service, path, connection), parent)
{
}

QDBusObjectManagerClient::~QDBusObjectManagerClient()
{
}

QT_END_NAMESPACE

#endif // QT_NO_DBUS
