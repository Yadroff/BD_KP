-- вставка в таблицу каналов
INSERT INTO Channels(ChannelName, PrivateFlag, CreateDate)
VALUES (:channel_name, :private_flag, datetime('now'))
-- вставка создателя в таблицу пользователей канала
INSERT INTO ChannelUsers(ChannelID, UserID, CreateDate, Rights)
SELECT ChannelID, :id, CreateDate, 5
FROM Channels
WHERE ChannelName = :channel_name
-- вставка в последние прочитанные сообщения
INSERT INTO LastReadMessage(UserID, ChannelID)
SELECT UserID, C.ChannelID
FROM ChannelUsers
         INNER JOIN Channels C on ChannelUsers.ChannelID = C.ChannelID
WHERE C.ChannelName = :channel_name
  AND UserID = :id
