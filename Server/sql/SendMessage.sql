-- вставляем сообщение в таблицу сообщений
INSERT INTO Messages(ChannelID, UserID, DateCreate)
SELECT ChannelID, :sender_id, datetime(:date)
FROM Channels
WHERE ChannelName = :channel_name;
-- вставляем текст сообщения
INSERT INTO MessageContents(Content)
VALUES (:text);
-- обновляем последнее прочитанное сообщение у отправителя
UPDATE LastReadMessage
SET MessageID = last_insert_rowid()
WHERE UserID = :sender_id
  AND ChannelID = (SELECT ChannelID FROM Channels WHERE ChannelName = :channel_name);
-- обновляем последнее сообщение в канале
UPDATE Channels
SET LastMessage = max(LastMessage, datetime(:date))
WHERE ChannelName = :channel_name