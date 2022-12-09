-- получаю все сообщения из чата
WITH temp AS
         (SELECT MessageID
          FROM LastReadMessage
          WHERE UserID = :user_id
            AND ChannelID = :channel_id)
-- считем количество сообщений, добавленных позже, чем последнее прочитанное
SELECT COUNT(*)
from temp,
     Messages
WHERE temp.MessageID < Messages.MessageID
  AND ChannelID = :channel_id