UPDATE Users
SET LastSeen = datetime('now'),
    Online   = 1
WHERE NickName = :nickname