SELECT COUNT(*)
from Messages
WHERE ReceiverID = :receiver
  AND SenderID = :sender
  AND Read_flag = 0