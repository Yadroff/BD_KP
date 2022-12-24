cmake_minimum_required(VERSION 3.23)
project(Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        Multimedia
        REQUIRED)

add_executable(Client
        main.cpp
        CommonHeader.h
        Connector.h Connector.cpp
        CryptographAlice.cpp CryptographAlice.h
        SenderReceiver.cpp SenderReceiver.h
        window.h window.cpp window.ui
        res.qrc
        ChannelItem.cpp ChannelItem.h
        Channel.cpp Channel.h
        Message.cpp Message.h
        MessageLabel.cpp MessageLabel.h
        MessageMenu.cpp MessageMenu.h)
target_link_libraries(Client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Multimedia
        )

