#ifndef OSPF_H
#define OSPF_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QQueue>
#include <QSet>
#include "RoutingProtocol.h"

class OSPF : public RoutingProtocol
{
public:
    explicit OSPF(QObject *parent = nullptr);
    void startRouting() override;
    void addNewNeighbour(const IPv4Ptr_t &neighbourIP, PortPtr_t outPort) override;
    void processRoutingPacket(const PacketPtr_t &packet, PortPtr_t inPort) override;

private:
    struct LinkStateAdvertisement
    {
        IPv4Ptr_t originRouter;
        QMap<IPv4Ptr_t, int> linkMetrics;
        QJsonObject toJson() const
        {
            QJsonObject json;
            json["originRouter"] = originRouter->toString();
            QJsonObject metrics;
            for (auto it = linkMetrics.cbegin(); it != linkMetrics.cend(); ++it) {
                metrics[it.key()->toString()] = it.value();
            }
            json["linkMetrics"] = metrics;
            return json;
        }

        QByteArray toByteArray() const
        {
            QJsonDocument doc(toJson());
            return doc.toJson(QJsonDocument::Compact);
        }

        static LinkStateAdvertisement fromJson(const QJsonObject &json)
        {
            LinkStateAdvertisement lsa;
            lsa.originRouter = IPv4Ptr_t::create(json["originRouter"].toString());
            QJsonObject metrics = json["linkMetrics"].toObject();
            for (auto it = metrics.constBegin(); it != metrics.constEnd(); ++it) {
                lsa.linkMetrics[IPv4Ptr_t::create(it.key())] = it.value().toInt();
            }
            return lsa;
        }

        static LinkStateAdvertisement fromByteArray(const QByteArray &data)
        {
            QJsonDocument doc = QJsonDocument::fromJson(data);
            return fromJson(doc.object());
        }
    };

    QMap<IPv4Ptr_t, LinkStateAdvertisement> lsDatabase;
    QMap<IPv4Ptr_t, PortPtr_t> neighbourPorts;

    void sendLSAPacket();
    void processLSA(const LinkStateAdvertisement &lsa);
    void recomputeRoutingTable();
};

#endif // OSPF_H
