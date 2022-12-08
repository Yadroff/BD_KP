SELECT *
FROM Messages
WHERE (SenderID = :first OR SenderID = :second)
  AND (ReceiverID = :first OR ReceiverID = :second)
  AND Date < datetime(:date)
ORDER BY Date DESC
LIMIT :limit;