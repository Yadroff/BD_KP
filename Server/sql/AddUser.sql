INSERT INTO Users (NickName, Name, Surname, Password, RegistrationDate, LastSeen, Online)
VALUES (:nickname, :name, :surname, :password, datetime('now'), datetime('now'), 1)