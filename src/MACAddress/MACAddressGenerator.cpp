#include "MACAddressGenerator.h"
#include <QRandomGenerator>
#include <algorithm>

QList<MACAddress> MACAddressGenerator::assignedMACs = {};
const std::array<uint8_t, 3> MACAddressGenerator::OUIBytes = {123, 37, 40};

MACAddress MACAddressGenerator::generateRandomMAC()
{
    std::array<uint8_t, 3> NICBytes;
    for (size_t i = 0; i < NICBytes.size(); i++) {
        NICBytes[i] = static_cast<uint8_t>(QRandomGenerator::global()->bounded(256));
    }

    std::array<uint8_t, 6> fullMAC;
    std::copy(OUIBytes.begin(), OUIBytes.end(), fullMAC.begin());
    std::copy(NICBytes.begin(), NICBytes.end(), fullMAC.begin() + OUIBytes.size());

    return MACAddress(fullMAC);
}

bool MACAddressGenerator::isMACAssigned(const MACAddress& mac)
{
    return assignedMACs.contains(mac);
}

MACAddress MACAddressGenerator::getRandomMAC()
{
    MACAddress address = generateRandomMAC();

    while (isMACAssigned(address))
        address = generateRandomMAC();

    assignedMACs.append(address);
    return address;
}
