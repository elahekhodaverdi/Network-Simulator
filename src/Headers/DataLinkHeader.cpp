#include "DataLinkHeader.h"

DataLinkHeader::DataLinkHeader(MACAddress sourceMACAddress, MACAddress destMACAddress, QObject *parent)
    : QObject{parent}, m_sourceMACAddress(sourceMACAddress), m_destinationMACAddress(destMACAddress)
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

