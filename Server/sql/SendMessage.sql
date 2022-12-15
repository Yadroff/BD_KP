-- вставляем сообщение в таблицу сообщений
INSERT INTO Messages(ChannelID, UserID, DateCreate)
SELECT ChannelID, :sender_id, datetime(:date)
FROM Channels
WHERE ChannelName = :channel_name;
-- вставляем текст сообщения
INSERT INTO MessageContents(Content)
VALUES (:text);