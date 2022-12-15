INSERT INTO Channels
(CreatorID, ChannelName, CreateDate, LastMessageDate, PrivateFlag, TypeChannel)
VALUES (:user_id, :chat_name, datetime('now'), datetime('now'), 1, 1)