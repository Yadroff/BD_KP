-- создаем приватный канал
INSERT INTO Channels(ChannelName, PrivateFlag, CreateDate, LastMessageDate, CreatorID, TypeChannel)
VALUES (:name, 1, datetime('now'), datetime('now'), :first, 0);
-- добавляем первого дэбила в чат
INSERT INTO ChannelUsers(ChannelID, UserID, CreateDate, Rights)
SELECT ChannelID, :first, CreateDate, 4
FROM Channels
WHERE ChannelName = :name;
-- добавляем второго дэбила в чат
INSERT INTO ChannelUsers(ChannelID, UserID, CreateDate, Rights)
SELECT ChannelID, :second, CreateDate, 4
FROM Channels
WHERE ChannelName = :name;
-- добавляем последние прочитанные сообщения первого дэбила
INSERT INTO LastReadMessage(ChannelID, UserID)
SELECT ChannelID, :first
FROM Channels
WHERE ChannelName = :name;
-- добавляем последние прочитанные сообщения второго дэбила
INSERT INTO LastReadMessage(ChannelID, UserID)
SELECT ChannelID, :second
FROM Channels
WHERE ChannelName = :name;