/*SELECT *
FROM Messages
WHERE (SenderID = :first OR SenderID = :second)
  AND (ReceiverID = :first OR ReceiverID = :second)
  AND Date < datetime(:date)
ORDER BY Date DESC
LIMIT :limit;*/

SELECT ChannelName, Channels.ChannelId, Message.MessageID, UserID, DateCreate, Content
FROM Channels
         INNER JOIN Messages Message on Channels.ChannelID = Message.ChannelID
         INNER JOIN MessageContents Content on Message.MessageID = Content.MessageID
WHERE (ChannelName = :channel_name)
  AND Message.DateCreate < datetime(:date)
ORDER BY DateCreate DESC
LIMIT :limit