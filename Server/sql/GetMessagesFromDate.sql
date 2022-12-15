SELECT  U.Nickname, DateCreate, Content
FROM Channels
         INNER JOIN Messages Message on Channels.ChannelID = Message.ChannelID
         INNER JOIN MessageContents Content on Message.MessageID = Content.MessageID
         INNER JOIN Users U on Message.UserID = U.UserID
WHERE (ChannelName = :channel_name)
  AND Message.DateCreate <= datetime(:date)
ORDER BY DateCreate
LIMIT 50