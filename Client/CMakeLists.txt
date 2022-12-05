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
        REQUIRED)

add_executable(Client
        main.cpp
        CommonHeader.h
        Connector.h Connector.cpp
        CryptographAlice.cpp CryptographAlice.h
        Manager.cpp Manager.h
        SenderReceiver.cpp SenderReceiver.h
        )
target_link_libraries(Client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        )

