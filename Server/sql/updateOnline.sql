UPDATE Users
SET Last_seen = datetime('now'),
    Online      = 1
WHERE NickName = :nickname