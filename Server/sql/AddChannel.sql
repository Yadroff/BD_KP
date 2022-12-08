INSERT INTO Users(NickName, Registration)
VALUES (:nickname, datetime('now'))
INSERT
INTO Channels(ID, Name, OwnerID, Private_Flag)
VALUES (last_insert_rowid(), :nickname, :ownerID, :privateFlag)
INSERT INTO AdministratorsInChannels SELECT ID, OwnerID FROM Channels WHERE Name = :nickname