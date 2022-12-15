UPDATE Users
SET LastSeen = datetime('now'),
    Online      = 0
WHERE NickName = :nickname;