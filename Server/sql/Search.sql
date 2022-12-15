SELECT Nickname
FROM Users
WHERE Nickname LIKE '%' || :name || '%'
ORDER BY (CASE WHEN Nickname = :name THEN 1 WHEN Nickname LIKE :name || '%' THEN 2 ELSE 3 END), Nickname
LIMIT 25;
SELECT ChannelName
FROM Channels
         INNER JOIN ChannelUsers CU on Channels.ChannelID = CU.ChannelID
WHERE ChannelName LIKE '%' || :name || '%' AND UserID = :id
   OR PrivateFlag = 0
ORDER BY (CASE WHEN ChannelName = :name THEN 1 WHEN ChannelName LIKE :name || '%' THEN 2 ELSE 3 END), ChannelName
LIMIT 25;