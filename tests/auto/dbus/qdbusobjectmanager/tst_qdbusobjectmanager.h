// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef TST_QDBUSOBJECTMANAGER_H
#define TST_QDBUSOBJECTMANAGER_H

#include <QObject>
#include <QDBusInterface>

class Receiver : public QObject
{
    Q_OBJECT

public:
    Receiver(QObject *parent = nullptr);
    virtual ~Receiver();

public slots:
    void interfaceAdded(QDBusInterfacePtr &intf);
    void interfaceRemoved(QDBusInterfacePtr &intf);
    void objectAdded(QDBusInterfacePtr &intf);
    void objectRemoved(QDBusInterfacePtr &intf);
};
/*
class MyDBusObject : public QObject
{
    Q_OBJECT

public:
    MyDBusObject(QObject *parent = nullptr);
    virtual ~MyDBusObject();

public slots:
    Q_SCRIPTABLE void setString(const QString &str);
    Q_SCRIPTABLE void setInt(int i);
    Q_SCRIPTABLE QString getString();

signals:
    Q_SCRIPTABLE void sigString(const QString &s);
};
*/
#endif // TST_QDBUSOBJECTMANAGER_H

