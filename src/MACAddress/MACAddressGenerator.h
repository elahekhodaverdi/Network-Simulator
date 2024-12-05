#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include "MACAddress.h"
#include <QList>
class MACAddressGenerator
{
public:
    MACAddressGenerator() = delete;

    static MACAddress generateRandomMAC();
    static bool isMACAssigned(const MACAddress& mac);
    static MACAddress getRandomMAC();

private:
    static QList<MACAddress> assignedMACs;
    static const std::array<uint8_t, 3> OUIBytes = {123, 37, 40};
};

#endif // MACADDRESSGENERATOR_H
