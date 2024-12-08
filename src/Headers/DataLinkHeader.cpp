#include "DataLinkHeader.h"

DataLinkHeader::DataLinkHeader(MACAddress sourceMACAddress, MACAddress destMACAddress, QObject *parent)
    : QObject{parent}, m_sourceMACAddress(sourceMACAddress), m_destinationMACAddress(destMACAddress)
{}

DataLinkHeader::DataLinkHeader(const DataLinkHeader& dataLinkHeader, QObject *parent)
    : QObject{parent},
    m_sourceMACAddress{dataLinkHeader.sourceMACAddress()},
    m_destinationMACAddress{dataLinkHeader.destinationMACAddress()}
{}

void DataLinkHeader::setSourceMACAddress(const MACAddress &address)
{
    m_sourceMACAddress = address;
}

void DataLinkHeader::setDestinationMACAddress(const MACAddress &address)
{
    m_destinationMACAddress = address;
}


MACAddress DataLinkHeader::sourceMACAddress() const
{
    return m_sourceMACAddress;
}

MACAddress DataLinkHeader::destinationMACAddress() const
{
    return m_destinationMACAddress;
}

