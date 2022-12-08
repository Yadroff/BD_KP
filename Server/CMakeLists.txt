cmake_minimum_required(VERSION 3.23)
project(Server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Sql
        Network
        REQUIRED)

add_executable(Server main.cpp
        Sender.h Sender.cpp
        Cryprograph.cpp Cryptograph.h
        Server.cpp Server.h
        User.cpp User.h
        commands/Command.h
        commands/CommandReplyKey.h commands/CommandReplyKey.cpp
        commands/CommandLogin.cpp commands/CommandLogin.h
        commands/CommandRegist.cpp commands/CommandRegist.h
        )
target_link_libraries(Server
        Qt::Core
        Qt::Sql
        Qt::Network
        )

