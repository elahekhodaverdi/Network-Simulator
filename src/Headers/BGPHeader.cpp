#include "BGPHeader.h"
#include <QDebug>
BGPHeader::BGPHeader(QObject *parent)
    : QObject{parent}
{
    m_marker.fill(0xFF, 16);
}

BGPHeader::BGPHeader(QByteArray &marker, quint16 length, quint8 type)
    : m_marker(marker)
    , m_length(length)
    , m_type(type)
{}

BGPHeader::BGPHeader(const QByteArray &data)
{
    if (data.size() != 19) {
        qWarning() << "Invalid BGPHeader";
    }
    m_marker = data.left(16);
    m_length = (static_cast<quint8>(data[16]) << 8) | static_cast<quint8>(data[17]);
    m_type = static_cast<quint8>(data[18]);
}

QByteArray BGPHeader::marker() const
{
    return m_marker;
}
quint16 BGPHeader::length() const
{
    return m_length;
}
quint8 BGPHeader::type() const
{
    return m_type;
}


QByteArray BGPHeader::toByteArray() const
{
    QByteArray data;
    data.append(m_marker);
    data.append(quint8(m_length >> 8));
    data.append(quint8(m_length));
}
