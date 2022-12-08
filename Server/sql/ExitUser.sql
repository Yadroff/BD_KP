UPDATE Users
SET Last_seen = datetime('now'),
    Online      = 0
WHERE NickName = :nickname