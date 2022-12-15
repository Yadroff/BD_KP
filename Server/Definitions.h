#include <QString>

const QString SEPARATOR = QString(QChar(7));
const QString DIALOG_SEPARATOR = QString(QChar(8));
const QString SECRET_KEY = "I LOVE C PLUS PLUS";

#define COMMAND_SEND_KEY "SEND KEY"
#define COMMAND_SEND_MESSAGE "SEND MESSAGE"
#define COMMAND_LOGIN "LOGIN"
#define COMMAND_REGIST "REGIST"
#define COMMAND_UPDATE_ONLINE "UPDATE"
#define COMMAND_CHANNELS "CHANNELS"
#define COMMAND_UNREAD_MESSAGES "UNREAD MESSAGES"
#define COMMAND_INIT_DB "INIT DB"
#define COMMAND_EXIT "USER EXIT"
#define COMMAND_ADD_IN_TEMP_TABLE "ADD IN TEMP TABLE"
#define COMMAND_CREATE_DIALOG "CREATE DIALOG"
#define COMMAND_READ_MESSAGES "READ MESSAGES"
#define COMMAND_NOTIFY_NEW_MESSAGE "NOTIFY NEW MESSAGE"
#define COMMAND_SEARCH "SEARCH"

#define PATH_TO_LOGIN "./sql/CheckInDB.sql"
#define PATH_TO_REGIST "./sql/AddUser.sql"
#define PATH_TO_UPDATE_ONLINE "./sql/UpdateOnline.sql"
#define PATH_TO_CHANNELS "./sql/GetChannelsByNickname.sql"
#define PATH_TO_UNREAD_MESSAGES "./sql/CountUnreadMessage.sql"
#define PATH_TO_EXIT "./sql/ExitUser.sql"
#define PATH_TO_SEND_MESSAGE "./sql/SendMessage.sql"
#define PATH_TO_GET_USERS_IN_CHANNEL "./sql/GetOnlineUsersInChannel.sql"
#define PATH_TO_INIT_DB "./sql/InitDB.sql"
#define PATH_TO_ADD_IN_TEMP_TABLE "./sql/AddUserInTempTable.sql"
#define PATH_TO_CREATE_DIALOG "./sql/CreateDialog.sql"
#define PATH_TO_GET_ID "./sql/GetID.sql"
#define PATH_TO_READ_MESSAGES "./sql/GetMessagesFromDate.sql"
#define PATH_TO_SEARCH "./sql/Search.sql"

#define PATH_TO_DATABASE "database.db"

#define CAN_NOT_OPEN_QUERY_FILE_MESSAGE "ERROR: CAN NOT OPEN FILE"

#define ROOTS_OWNER 4
#define ROOTS_ADMINISTRATOR 3
#define ROOTS_MODERATOR 2
#define ROOTS_USER 1
#define ROOTS_BANNED 0

#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"

#define CREATE_DIALOG_QUERIES 5
#define SEARCH_QUERIES 2