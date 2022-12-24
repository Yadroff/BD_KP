UPDATE Users
SET LastSeen = datetime('now'),
    Online      = 0
WHERE NickName = :nickname;
DELETE FROM OnlineUsersInChannels WHERE Nickname = :nickname;