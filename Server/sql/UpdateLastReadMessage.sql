UPDATE LastReadMessage
SET MessageID = :message_id
WHERE UserID = (SELECT UserID FROM Users WHERE Nickname = :nick)
  AND ChannelID = (SELECT ChannelID FROM Channels WHERE ChannelName = :channel_name)