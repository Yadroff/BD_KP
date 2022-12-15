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
        Definitions.h IncludesToServer.h
        Sender.h Sender.cpp
        Cryprograph.cpp Cryptograph.h
        Server.cpp Server.h
        User.cpp User.h
        commands/Command.h
        commands/CommandReplyKey.cpp commands/CommandReplyKey.h
        commands/CommandLogin.cpp commands/CommandLogin.h
        commands/CommandRegist.cpp commands/CommandRegist.h
        commands/CommandExit.cpp commands/CommandExit.h
        commands/CommandReadMessages.cpp commands/CommandReadMessages.h
        commands/CommandSendMessage.cpp commands/CommandSendMessage.h
        commands/CommandGetUsersInChannel.cpp commands/CommandGetUsersInChannel.h
        commands/CommandInitDB.cpp commands/CommandInitDB.h
        commands/CommandCreateDialog.cpp commands/CommandCreateDialog.h commands/NotifyNewMessage.cpp commands/NotifyNewMessage.h commands/CommandSearch.cpp commands/CommandSearch.h)
target_link_libraries(Server
        Qt::Core
        Qt::Sql
        Qt::Network
        )

