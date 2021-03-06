# CLI-chat

OS: Oracle Linux Server Release 6.10

Сompiler: gcc version 4.4.7 20120313 (Red Hat 4.4.7-23.0.1) 

Library: SQLite

Подробнее о реализации кода: [Wiki](https://github.com/mysteryon88/CLI-chat/wiki)

Это мультиклиентный чат на языке С, к которому подлючена база даннных SQLite, через одноименную библиотку. 
Связь клиента и сервера организована через сокеты. По умолчанию localhost.
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
```
>> /up
```
После ввода команды регистрации, необходимо будет ввести свой логин и пароль. Сервер проведет проверку на уникальность имени и, если всё хорошо, добавит в нового клиента в таблицу, выдав уникальный код, по которому можно будет добавить вас в друзья.
```
>> /in
```
После ввода команды авторизации, необходимо будет ввести свой логин и пароль. Сервер проверит зарегистрирован ли клиент, а так же верность логина и пароля. В ответ сервер пришлет уникальный код клиента.
```
>> /friends 
```
Команда на добавление в друзья, надо написать никнейм человека, которого вы хотите добавить в друзья и его уникальный код, который он должен вам сообщить.
```
>> /myfriends
```
Команда выводит добавленных друзей.
```
>> /chat
```
После ввода команды, необходимо ввести никнейм собеседника. Сервер проверит онлайн ли человек и, если онлайн, отправит ему запрос на открытие чата с вами. В вашем клиенте откроется новый терминал для чата с выбранным вами человеком.
```
>> /dis or ^C
```
Команда отключения от серверa, текстовое сообщение можно заменить на сочетание клавиш  Ctrl+C.

Так же есть две команды отдельно для окна с чатом:
```
>> /start - ответ на запрос о чате 
>> /leave - выход из чата
```
