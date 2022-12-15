SELECT Channels.ChannelID, ChannelName, LastMessageDate, CU.Rights
FROM Channels
         INNER JOIN ChannelUsers CU on Channels.ChannelID = CU.ChannelID
WHERE UserID = :id
ORDER BY LastMessageDate DESC
LIMIT 50