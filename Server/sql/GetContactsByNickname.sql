WITH temp AS (SELECT FIRSTID, SecondID
              FROM Contacts
                       INNER JOIN Users ON ID = FirstID
              WHERE NickName = :nickname)

SELECT NickName, ID
FROM temp,
     Users
WHERE ID = SecondID