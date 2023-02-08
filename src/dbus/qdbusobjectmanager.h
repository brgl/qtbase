// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QDBUSOBJECTMANAGER_H
#define QDBUSOBJECTMANAGER_H

#include <QtCore/qvariant.h>
#include <QtCore/qmap.h>
#include <QtDBus/qdbusinterface.h>
#include <QtDBus/qtdbusglobal.h>

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

typedef QList<QDBusInterfacePtr> QDBusInterfaceList;

class QDBusObjectManagerPrivate;

class Q_DBUS_EXPORT QDBusObjectManager : public QObject
{
    Q_OBJECT

public:
    virtual ~QDBusObjectManager();

    QDBusInterfaceList getManagedObjects() const;
    QDBusInterfacePtr getObject(const QString &path) const;
    const QString &service() const;
    const QString &path() const;

    bool isValid() const;
    QDBusError lastError() const;

signals:
    void interfaceAdded(QDBusInterfacePtr& intf);
    void interfaceRemoved(QDBusInterfacePtr& intf);
    void objectAdded(QDBusInterfacePtr& intf);
    void objectRemoved(QDBusInterfacePtr &intf);

protected:
    QDBusObjectManager(QDBusObjectManagerPrivate &priv, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY_MOVE(QDBusObjectManager);
    Q_DECLARE_PRIVATE(QDBusObjectManager);
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS

#endif // QDBUSOBJECTMANAGER_H
