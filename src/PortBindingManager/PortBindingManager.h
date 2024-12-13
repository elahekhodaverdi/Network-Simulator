#ifndef PORTBINDINGMANAGER_H
#define PORTBINDINGMANAGER_H

#include "../Port/Port.h"

#include <QList>
#include <QMap>
#include <QObject>

class PortBindingManager : public QObject
{
    Q_OBJECT

public:
    PortBindingManager(QObject *parent = nullptr);
    ~PortBindingManager() override = default;

    static void bind(const PortPtr_t &port1, const PortPtr_t &port2);
    static bool unbind(const PortPtr_t &port1, const PortPtr_t &port2);
    static bool isBounded(const PortPtr_t &port);
Q_SIGNALS:
    // for testing purposes
    // static void bindingChanged(
    //     const QString &router1, uint8_t port1, const QString &router2, uint8_t port2, bool bind);

private:
    static QMap<PortPtr_t, QList<PortPtr_t>> bindings;
};

#endif    // PORTBINDINGMANAGER_H
