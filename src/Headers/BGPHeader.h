#ifndef BGPHEADER_H
#define BGPHEADER_H

#include <QObject>

class BGPHeader : public QObject
{
    Q_OBJECT
public:
    explicit BGPHeader(QObject *parent = nullptr);
    explicit BGPHeader(QByteArray &marker, quint16 length, quint8 type);
    BGPHeader(const QByteArray &data);

    QByteArray marker() const;
    quint16 length() const;
    quint8 type() const;


    QByteArray toByteArray() const;
    bool fromByteArray(const QByteArray &data);

Q_SIGNALS:

private:
    QByteArray m_marker;
    quint16 m_length;
    quint8 m_type;
};

#endif // BGPHEADER_H

