// Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "tst_qdbusobjectmanager.h"

#include <QCoreApplication>
#include <QDBusObjectManagerClient>
#include <QDBusObjectManagerServer>
#include <QObject>

Receiver::Receiver(QObject *parent)
    : QObject(parent)
{
}

Receiver::~Receiver()
{
}

void Receiver::interfaceAdded(QDBusInterfacePtr &intf)
{
    qInfo() << "BGBG" << __func__ << intf->path() << intf->service() << intf->interface();
}

void Receiver::interfaceRemoved(QDBusInterfacePtr &intf)
{
    qInfo() << "BGBG" << __func__ << intf->path() << intf->service() << intf->interface();
}

void Receiver::objectAdded(QDBusInterfacePtr &intf)
{
    qInfo() << "BGBG" << __func__ << intf->path() << intf->service() << intf->interface();
}

void Receiver::objectRemoved(QDBusInterfacePtr &intf)
{
    qInfo() << "BGBG" << __func__ << intf->path() << intf->service() << intf->interface();
}
/*
MyDBusObject::MyDBusObject(QObject *parent)
    : QObject(parent)
{
}

MyDBusObject::~MyDBusObject()
{
}

void MyDBusObject::setString(const QString &str)
{
    qInfo() << "BGBG" << str;
}

void MyDBusObject::setInt(int i)
{
    qInfo() << "BGBG i" << i;
}

QString MyDBusObject::getString()
{
    return "dupa";
}
*/
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    auto bus = QDBusConnection::sessionBus();

    bus.registerService("foo.bar");

    QDBusObjectManagerServer manager("foo.bar", "/foo/bar");

/*
    MyDBusObject obj;

    auto bus = QDBusConnection::sessionBus();

    bus.registerService("foo.bar");
    new MyDBusObjectAdaptor(&obj);

    bus.registerObject("/foo/bar/obj", &obj);

    QDBusObjectManagerClient manager("io.gpiod1", "/io/gpiod1", QDBusConnection::systemBus());
    Receiver receiver;

    qInfo() << "BGBG" << __func__ << manager.getManagedObjects();

    QObject::connect(&manager, &QDBusObjectManagerClient::interfaceAdded,
                     &receiver, &Receiver::interfaceAdded);
    QObject::connect(&manager, &QDBusObjectManagerClient::interfaceRemoved,
                     &receiver, &Receiver::interfaceRemoved);
    QObject::connect(&manager, &QDBusObjectManagerClient::objectAdded,
                     &receiver, &Receiver::objectAdded);
    QObject::connect(&manager, &QDBusObjectManagerClient::objectRemoved,
                     &receiver, &Receiver::objectRemoved);
*/
    return app.exec();
}
