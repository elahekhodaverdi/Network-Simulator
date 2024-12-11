QT     += core
CONFIG += lrelease
CONFIG += no_keywords
CONFIG += c++20

INCLUDEPATH += $$PWD\Globals

SOURCES += \
    $$PWD/AutonomousSystem/AutonomousSystem.cpp \
    # $$PWD/DHCPServer/DHCPServer.cpp \
    # $$PWD/EventsCoordinator/EventsCoordinator.cpp \
    # $$PWD/DataGenerator/DataGenerator.cpp \
    # $$PWD/Headers/BGPHeader.cpp \
    # $$PWD/Headers/DataLinkHeader.cpp \
    # $$PWD/Headers/TCPHeader.cpp \
    # $$PWD/IP/IP.cpp \
    # $$PWD/MACAddress/MACAddress.cpp \
    # $$PWD/MACAddress/MACAddressGenerator.cpp \
    $$PWD/NetworkConfig/NetworkConfig.cpp \
    $$PWD/NetworkConfig/SimluationConfig.cpp \
    # $$PWD/Packet/Packet.cpp \
    # $$PWD/PortBindingManager/PortBindingManager.cpp \
    # $$PWD/Port/Port.cpp \
    $$PWD/Node/Node.cpp \
    $$PWD/PC/PC.cpp \
    $$PWD/Router/Router.cpp \
    $$PWD/Topology/TopologyBuilder.cpp \
    $$PWD/Utils/ConfigReader.cpp \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/AutonomousSystem/AutonomousSystem.h \
    # $$PWD/DHCPServer/DHCPServer.h \
    # $$PWD/EventsCoordinator/EventsCoordinator.h \
    # $$PWD/DataGenerator/DataGenerator.h \
    $$PWD/Globals/Globals.h \
    # $$PWD/Headers/BGPHeader.h \
    # $$PWD/Headers/DataLinkHeader.h \
    # $$PWD/Headers/TCPHeader.h \
    # $$PWD/IP/IP.h \
    # $$PWD/MACAddress/MACAddress.h \
    # $$PWD/MACAddress/MACAddressGenerator.h \
    $$PWD/NetworkConfig/NetworkConfig.h \
    $$PWD/NetworkConfig/SimluationConfig.h \
    # $$PWD/Packet/Packet.h \
    # $$PWD/PortBindingManager/PortBindingManager.h \
    # $$PWD/Port/Port.h \
    $$PWD/Node/Node.h \
    $$PWD/PC/PC.h \
    $$PWD/Router/Router.h \
    $$PWD/Topology/TopologyBuilder.h \
    $$PWD/Utils/ConfigReader.h
