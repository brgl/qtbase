// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//

#ifndef QDBUSOBJECTMANAGER_P_H
#define QDBUSOBJECTMANAGER_P_H

#include "qdbusabstractadaptor.h"
#include "qdbusconnection.h"
#include "qdbusinterface.h"
#include "qdbusobjectmanager.h"
#include "qdbusobjectmanagerclient.h"
#include "qdbusobjectmanagerserver.h"

#include "private/qobject_p.h"

#ifndef QT_NO_DBUS

QT_BEGIN_NAMESPACE

typedef QMap<QString, QPair<QDBusInterfacePtr, QStringList>> QDBusInterfaceMap;
typedef QMap<QDBusObjectPath, QPair<QDBusInterfacePtr, QDBusInterfaceMap>> QDBusManagedObjects;

typedef QMap<QString, QVariantMap> QVariantMapMap;
Q_DECLARE_METATYPE(QVariantMapMap);

typedef QMap<QDBusObjectPath, QVariantMapMap> QDBusObjPathInterfacesAndProperties;
Q_DECLARE_METATYPE(QDBusObjPathInterfacesAndProperties);

class Q_AUTOTEST_EXPORT QDBusObjectManagerPrivate : public QObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QDBusObjectManager);

    QDBusObjectManagerPrivate(const QString &service, const QString &path,
                              const QDBusConnection &connection);
    virtual ~QDBusObjectManagerPrivate();

    void interfacesAdded(const QDBusObjectPath &path, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QDBusObjectPath &path, const QStringList &interfaces);

protected:
    QDBusConnection m_connection;
    QString m_service;
    QString m_path;
    QDBusManagedObjects m_managedObjects;
    QDBusError m_error;
};

/*
 * The interfacesAdded and interfacesRemoved slots have no business being exposed in
 * a user-facing header so shouldn't be part of QDBusObjectManager's class declaration.
 * QDBusObjectManagerPrivate however is not a QObject while QDBus signals can only be
 * connected to QObjects. Let's have a helper QObject owned by QDBusObjectManagerPrivate
 * whose sole purpose is to rely the ObjectManager signals up to its owner.
 */
class QDBusObjectManagerClientDBusHelper final : public QObject
{
    Q_OBJECT

public:
    QDBusObjectManagerClientDBusHelper(QDBusObjectManagerPrivate &owner);
    ~QDBusObjectManagerClientDBusHelper();

public slots:
    void interfacesAdded(const QDBusObjectPath &path, const QVariantMapMap &interfaces);
    void interfacesRemoved(const QDBusObjectPath &path, const QStringList &interfaces);

private:
    QDBusObjectManagerPrivate &m_owner;
};

class Q_AUTOTEST_EXPORT QDBusObjectManagerClientPrivate final : public QDBusObjectManagerPrivate
{
public:
    Q_DECLARE_PUBLIC(QDBusObjectManagerClient);

    QDBusObjectManagerClientPrivate(const QString &service, const QString &path,
                                    const QDBusConnection &connection);
    ~QDBusObjectManagerClientPrivate();

private:
    QDBusObjectManagerClientDBusHelper m_dbusHelper;

    friend QDBusObjectManagerClientDBusHelper;
};

class QDBusObjectManagerAdaptor final : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.DBus.ObjectManager")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.freedesktop.DBus.ObjectManager\">\n"
"    <method name=\"GetManagedObjects\">\n"
"      <arg direction=\"out\" type=\"a{oa{sa{sv}}}\" name=\"objpath_interfaces_and_properties\"/>\n"
"    </method>\n"
"    <signal name=\"InterfacesAdded\">\n"
"      <arg direction=\"out\" type=\"o\" name=\"object_path\"/>\n"
"      <arg direction=\"out\" type=\"a{sa{sv}}\" name=\"interfaces_and_properties\"/>\n"
"    </signal>\n"
"    <signal name=\"InterfacesRemoved\">\n"
"      <arg direction=\"out\" type=\"o\" name=\"object_path\"/>\n"
"      <arg direction=\"out\" type=\"as\" name=\"interfaces\"/>\n"
"    </signal>\n"
"  </interface>\n"
    "")

public:
    QDBusObjectManagerAdaptor(QObject *parent);
    ~QDBusObjectManagerAdaptor();

public Q_SLOTS:
    QDBusObjPathInterfacesAndProperties GetManagedObjects() const;

Q_SIGNALS:
    void InterfacesAdded(const QDBusObjectPath &object_path,
                         const QVariantMapMap &interfaces_and_properties);
    void InterfacesRemoved(const QDBusObjectPath &object_path, const QStringList &interfaces);
};

class QDBusObjectManagerServerPrivate;

class QDBusObjectManagerServerDBusHelper final : public QObject
{
    Q_OBJECT

public:
    explicit QDBusObjectManagerServerDBusHelper(QDBusObjectManagerServerPrivate &owner);
    ~QDBusObjectManagerServerDBusHelper();

public Q_SLOTS:
    Q_SCRIPTABLE QDBusObjPathInterfacesAndProperties getManagedObjects() const;

Q_SIGNALS:
    Q_SCRIPTABLE void interfacesAdded(const QDBusObjectPath &object_path,
                                      const QVariantMapMap &interfaces_and_properties);
    Q_SCRIPTABLE void interfacesRemoved(const QDBusObjectPath &object_path,
                                        const QStringList &interfaces);

private:
    QDBusObjectManagerServerPrivate &m_owner;
};

class Q_AUTOTEST_EXPORT QDBusObjectManagerServerPrivate final : public QDBusObjectManagerPrivate
{
public:
    Q_DECLARE_PUBLIC(QDBusObjectManagerServer);

    QDBusObjectManagerServerPrivate(const QString &service, const QString &path,
                                    const QDBusConnection &connection);
    ~QDBusObjectManagerServerPrivate();

    QDBusObjPathInterfacesAndProperties getManagedObjects() const;

private:
    QDBusObjectManagerServerDBusHelper m_dbusHelper;
};

QT_END_NAMESPACE

#endif // QT_NO_DBUS

#endif // QDBUSOBJECTMANAGER_P_H
