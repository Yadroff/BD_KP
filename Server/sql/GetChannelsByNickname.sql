SELECT ChannelName, CU.Rights, CU.ChannelID
FROM Channels
         INNER JOIN ChannelUsers CU on Channels.ChannelID = CU.ChannelID
WHERE UserID = :id
ORDER BY LastMessage DESC
LIMIT 50