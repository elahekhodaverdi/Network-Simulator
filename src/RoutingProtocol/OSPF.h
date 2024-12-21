#ifndef OSPF_H
#define OSPF_H

#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>
#include "./RoutingProtocol.h"
class OSPF : public RoutingProtocol
{
public:
    explicit OSPF(QObject *parent = nullptr);
    explicit OSPF(QString routerIP, QObject *parent = nullptr);
    //virtual ~OSPF() = default;

    void initialize() override;
    void processReceivedRoutingInformation(const QByteArray &lsaData) override;
    void addNeighbor(IPv4Ptr_t neighborIP) override;
    void advertiseLinkState();
    void calculateSPF();
    struct LinkStateAdvertisement
    {
        IPv4Ptr_t routerIP;
        QList<IPv4Ptr_t> neighborIPs;

        QByteArray toByteArray() const
        {
            QJsonObject jsonObj;

            jsonObj["routerIP"] = routerIP->toString();

            QJsonArray neighborsArray;
            for (const auto &neighborIP : neighborIPs) {
                neighborsArray.append(neighborIP->toString());
            }
            jsonObj["neighborIPs"] = neighborsArray;

            QJsonDocument jsonDoc(jsonObj);
            return jsonDoc.toJson();
        }

        static LinkStateAdvertisement fromByteArray(const QByteArray &byteArray)
        {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);

            if (!jsonDoc.isObject())
                throw std::runtime_error("Invalid JSON format for LSA");

            QJsonObject jsonObj = jsonDoc.object();

            if (!jsonObj.contains("routerIP") || !jsonObj["routerIP"].isString())
                throw std::runtime_error("Missing or invalid routerIP in LSA JSON");

            IPv4Ptr_t routerIP = IPv4Ptr_t::create(jsonObj["routerIP"].toString());

            if (!jsonObj.contains("neighborIPs") || !jsonObj["neighborIPs"].isArray())
                throw std::runtime_error("Missing or invalid neighborIPs in LSA JSON");

            QJsonArray neighborsArray = jsonObj["neighborIPs"].toArray();
            QList<IPv4Ptr_t> neighborIPs;
            for (const QJsonValue &value : neighborsArray) {
                if (!value.isString())
                    throw std::runtime_error("Invalid neighborIP in neighborIPs JSON array");
                neighborIPs.append(IPv4Ptr_t::create(value.toString()));
            }

            return LinkStateAdvertisement{routerIP, neighborIPs};
        }
    };

private:
    void sendRoutingInformation(const QByteArray &lsaData) override;
    QMap<IPv4Ptr_t, LinkStateAdvertisement> LSDB;
    QMap<IPv4Ptr_t, int> shortestPathTree;
    void buildRoutingTable(const QMap<IPv4Ptr_t, IPv4Ptr_t> &previousHop);
};

typedef OSPF::LinkStateAdvertisement LSA;
#endif // OSPF_H
