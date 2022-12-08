SELECT Name
FROM Channels
         INNER JOIN AdministratorsInChannels ON ChannelID = ID
WHERE AdministratorID = :ID