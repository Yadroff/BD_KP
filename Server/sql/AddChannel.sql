-- вставка в таблицу каналов
INSERT INTO Channels
(CreatorID, ChannelName, CreateDate, LastMessageDate, PrivateFlag, TypeChannel)
VALUES (:user_id, :channel_name, datetime('now'), datetime('now'), :private_flag, 2)
