#include "MACAddress.h"
#include <QDebug>
#include <QRegularExpression>

MACAddress::MACAddress(std::array<uint8_t, 6> address)
    : m_address(address)
{}

MACAddress::MACAddress(QString address)
{
    if (isValid(address)) {
        QStringList byteStrings = address.split(':');
        for (int i = 0; i < 6; i++) {
            m_address[i] = static_cast<uint8_t>(byteStrings[i].toUInt(nullptr, 16));
        }
    } else {
        qWarning() << "Invalid MAC Address!";
        m_address.fill(0);
    }
}

QString MACAddress::toString() const
{
    QString string;
    for (size_t i = 0; i < m_address.size(); ++i) {
        if (i != 0)
            string.append(":");
        string.append(QString("%1").arg(m_address[i], 2, 16, QChar('0')).toUpper());
    }
    return string;
}



bool MACAddress::isValid(const QString& address)
{
    QRegularExpression macRegex("^(?:[0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$");
    return macRegex.match(address).hasMatch();
}


bool MACAddress::operator==(const MACAddress& other) const
{
    for (int i = 0; i < 6; i++) {
        if (this->m_address[i] != other.m_address[i])
            return false;
    }
    return true;
}
