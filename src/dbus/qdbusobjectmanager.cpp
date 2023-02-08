// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qdbusobjectmanager.h"
#include "qdbusobjectmanager_p.h"

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

QDBusObjectManagerPrivate::QDBusObjectManagerPrivate(const QString &service, const QString &path,
                                                     const QDBusConnection &connection)
    : QObjectPrivate(),
      m_connection(connection),
      m_service(service),
      m_path(path),
      m_managedObjects(),
      m_error()
{
}

QDBusObjectManagerPrivate::~QDBusObjectManagerPrivate()
{
}

void QDBusObjectManagerPrivate::interfacesAdded(const QDBusObjectPath &path,
                                                const QVariantMapMap &interfaces)
{
    Q_Q(QDBusObjectManager);

    if (!m_managedObjects.contains(path)) {
        QDBusInterfacePtr objIntf(new QDBusInterface(m_service, path.path(), QString(), m_connection));
        if (!objIntf->isValid()) {
            qCritical() << "unable to create a new merged DBus interface:" << m_connection.lastError().message();
            return;
        }

        m_managedObjects.insert(path, {objIntf, QDBusInterfaceMap()});
        emit q->objectAdded(objIntf);
    }

    auto &intfMap = m_managedObjects[path].second;

    for (const auto &intf: interfaces.keys()) {
        if (!intfMap.contains(intf)) {
            QDBusInterfacePtr intfIntf(new QDBusInterface(m_service, path.path(), intf, m_connection));
            if (!intfIntf->isValid()) {
                qCritical() << "unable to create a new DBus interface:" << m_connection.lastError().message();
                return;
            }

            QStringList props;

            intfMap.insert(intf, {intfIntf, props});
            emit q->interfaceAdded(intfIntf);
        }
    }
}

void QDBusObjectManagerPrivate::interfacesRemoved(const QDBusObjectPath &path,
                                                  const QStringList &interfaces)
{
    Q_Q(QDBusObjectManager);

    if (!m_managedObjects.contains(path)) {
        qCritical() << "InterfacesRemoved DBus signal received for object not managed by this manager:" << path;
        return;
    }

    auto &intfMap = m_managedObjects[path].second;

    for (const auto &intf: interfaces) {
        if (!intfMap.contains(intf)) {
            qCritical() << "InterfacesRemoved DBus signal received for an interface not associated with object" <<
                           path << ":" << intf;
            return;
        }

        emit q->interfaceRemoved(intfMap[intf].first);
        intfMap.remove(intf);
    }

    if (intfMap.empty()) {
        emit q->objectRemoved(m_managedObjects[path].first);
        m_managedObjects.remove(path);
    }
}

QDBusObjectManager::QDBusObjectManager(QDBusObjectManagerPrivate &priv, QObject *parent)
    : QObject(priv, parent)
{
}

QDBusObjectManager::~QDBusObjectManager()
{
}

QDBusInterfaceList QDBusObjectManager::getManagedObjects() const
{
    Q_D(const QDBusObjectManager);

    QDBusInterfaceList ret;

    for (auto const &path: d->m_managedObjects.keys())
        ret.append(d->m_managedObjects[path].first);

    return ret;
}

QDBusInterfacePtr QDBusObjectManager::getObject(const QString &path) const
{
    Q_D(const QDBusObjectManager);

    QDBusObjectPath objPath(path);

    if (!d->m_managedObjects.contains(objPath))
        return nullptr;

    return d->m_managedObjects[objPath].first;
}

const QString &QDBusObjectManager::service() const
{
    Q_D(const QDBusObjectManager);

    return d->m_service;
}

const QString &QDBusObjectManager::path() const
{
    Q_D(const QDBusObjectManager);

    return d->m_path;
}

bool QDBusObjectManager::isValid() const
{
    Q_D(const QDBusObjectManager);

    return d->m_error.isValid();
}

QDBusError QDBusObjectManager::lastError() const
{
    Q_D(const QDBusObjectManager);

    return d->m_error;
}

QT_END_NAMESPACE

#endif // QT_NO_DBUS
