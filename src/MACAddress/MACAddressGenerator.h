#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include "MACAddress.h"
#include <array>
#include <QList>

class MACAddressGenerator
{
public:
    MACAddressGenerator() = delete;

    static MACAddress generateRandomMAC();
    static bool isMACAssigned(const MACAddress& mac);
    static MACAddress getRandomMAC();

private:
    static QList<MACAddress> m_assignedMACs;
    static const std::array<uint8_t, 3> m_OUIBytes;
};

#endif // MACADDRESSGENERATOR_H
