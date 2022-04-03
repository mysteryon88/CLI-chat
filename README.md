# CLI-chat

OS: Oracle Linux Server Release 6.10

Сompiler: gcc version 4.4.7 20120313 (Red Hat 4.4.7-23.0.1) 

Library: SQLite

Это мультиклиентный чат на языке С, к которому подлючена база даннных SQLite, через одноименную библиотку. 
Связь клиента и сервера организована через сокеты.
Программый интерфейс представляет из себя Интерфейс командной строки (англ. Command line interface, CLI). 
Компиляция производится через Makefile и дальше можно запускать .out файлы

У Клиента есть два потока:

1. Поток ввода сообщений в функции main
2. Поток вывода принятых сообщений в функции recvmg

Сервер создает поток на каждое подключение и сохраняет номера поключений в clients_list. Там же хранится никнейм клиента, когда он пройдёт авторизацию. 

Клиенты и сервер общаются текстовыми командами, список команд можно получить командой:
 ```
 >> /help
 ```
 ```
          Hepl
/up - зарегистрироваться
/in - авторизироваться
/clear - отчистить терминал
/friends - добавить в друзья
/myfriends - вывести список друзей
/chat - начать новый чат
/dis or ^C - отключени от сервера
/about - информация о разработчике
>>
```
