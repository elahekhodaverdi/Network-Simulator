#include <QDebug>
#include "MACAddress/MACAddress.h"
#include "MACAddress/MACAddressGenerator.h"

int main()
{
    QList<MACAddress> generatedMACs;

    for (int i = 0; i < 5; ++i) {
        MACAddress randomMAC = MACAddressGenerator::getRandomMAC();
        qDebug() << "Generated MAC Address #" << (i + 1) << ":" << randomMAC.toString();

        bool isUnique = true;
        for (const auto& mac : generatedMACs) {
            if (mac == randomMAC) {
                isUnique = false;
                break;
            }
        }

        if (isUnique) {
            qDebug() << "The MAC address is unique.";
        } else {
            qDebug() << "The MAC address is not unique.";
        }

        generatedMACs.append(randomMAC);
    }
    MACAddress tempMac(generatedMACs.first().toString());
    qDebug() << "is mac assigned?: " << MACAddressGenerator::isMACAssigned(tempMac);
    return 0;
}
