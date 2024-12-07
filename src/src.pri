QT     += core
CONFIG += lrelease
CONFIG += no_keywords
CONFIG += c++20

INCLUDEPATH += $$PWD\Globals

SOURCES += \
    # $$PWD/DHCPServer/DHCPServer.cpp \
    $$PWD/EventsCoordinator/EventsCoordinator.cpp \
    $$PWD/DataGenerator/DataGenerator.cpp \
    # $$PWD/Headers/BGPHeader.cpp \
    # $$PWD/Headers/DataLinkHeader.cpp \
    # $$PWD/Headers/TCPHeader.cpp \
    # $$PWD/IP/IP.cpp \
    # $$PWD/MACAddress/MACAddress.cpp \
    # $$PWD/MACAddress/MACAddressGenerator.cpp \
    # $$PWD/Packet/Packet.cpp \
    # $$PWD/PortBindingManager/PortBindingManager.cpp \
    # $$PWD/Port/Port.cpp \
     $$PWD/main.cpp

HEADERS += \
    # $$PWD/DHCPServer/DHCPServer.h \
    $$PWD/EventsCoordinator/EventsCoordinator.h \
    $$PWD/DataGenerator/DataGenerator.h \
    # $$PWD/Globals/Globals.h \
    # $$PWD/Headers/BGPHeader.h \
    # $$PWD/Headers/DataLinkHeader.h \
    # $$PWD/Headers/TCPHeader.h \
    # $$PWD/IP/IP.h \
    # $$PWD/MACAddress/MACAddress.h \
    # $$PWD/MACAddress/MACAddressGenerator.h \
    # $$PWD/Packet/Packet.h \
    # $$PWD/PortBindingManager/PortBindingManager.h \
    # $$PWD/Port/Port.h \
