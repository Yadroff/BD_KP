INSERT INTO Users (NickName, Name, Surname, Password, Registration, Last_seen, Online)
VALUES (:nickname, :name, :surname, :password, datetime('now'), datetime('now'), 1)