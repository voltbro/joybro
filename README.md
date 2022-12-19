# Пакет ROS для джойстика JoyBro

Пакет предназначен для работы [джойстика JoyBro](http://www.voltbro.ru/joybro). С помощью данного джойстика можно управлять роботами [TurtleBro](http://www.voltbro.ru/turtlebro) и [Rover-E](http://www.voltbro.ru/rover-e).


## Первое включение джойстика

### Создание рабочего пространства

Для работы с джойстиком вам понадобится установленный ROS Noetic и сконфигурированное рабочее пространство. Подробнее об этом читайте в разделе "Создание рабочего пространства" по [ссылке](http://docs.voltbro.ru/starting-ros/install/).

### Установка пакета на компьютер

Для установки пакета, необходимо склонировать репозиторий в вашу папку `catkin_ws/src`:

```bash
cd ~/catkin_ws/src
git clone https://github.com/voltbro/joybro
```

Далее необходимо запустить процесс сборки пакета:

```bash
cd ~/catkin_ws
catkin_make --pkg=joybro
```

Для работы, может понадобиться пакет `rosserial`. Если пакет не установлен, то его нужно установить командой:

```bash
sudo apt install ros-noetic-rosserial-arduino && sudo apt install ros-noetic-rosserial
```

### Загрузка скетча (прошивки) на джойстик

В дистрибутиве пакета находится прошивка `ros_joybro.ino` [Arduino/ros_joybro/ros_joybro.ino](https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro/ros_joybro.ino) необходимая для работы джойстика. Загрузите ее на джойстик через Arduino IDE:

https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro/ros_joybro.ino

Данная прошивка создает __Издателя__ для топика /joybro, в который он публикует все данные о состоянии кнопок, слайдеров и джойстиков.


### Подключение Джойстика

На Arduino джойстика необходимо загрузить скетч [Arduino/ros_joybro/ros_joybro.ino](https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro/ros_joybro.ino)

Данный скетч с частотой 20 герц производит чтение всех контролов джойстика и передает их значения в ROS в топик `/joybro`

Тип передаваемого сообщения [msg/JoyBro.msg](https://github.com/voltbro/joybro/blob/master/msg/JoyBro.msg)

Для того, чтобы пакет JoyBro мог общаться с джойстиком, надо выдать джойстику соответствующие права в Linux выполнив команду:

```bash
sudo chmod 777 /dev/ttyACM0
```

Для того, чтобы данные из джойстика передавались в ROS, необходимо на компьютере подключенному к джойстику запустить процесс `rosserial`, который подключается к `Arduino` 
и пробрасывает все сообщения в ROS

Для этого можно запустить готовый `.launch` файл командой 

```bash
roslaunch joybro joy_serial.launch
```

После запуска `joy_serial.launch`, можно проверить правильность работы, выполнив команду
```bash
rostopic echo /joybro
```

## Управление роботом turtlebro/rover-E

Python-скрипт [/src/joy_teleop.py](https://github.com/voltbro/joybro/blob/master/src/joy_teleop.py) пакета JoyBro предназначен для управления роботами через топик `/cmd_vel` 

Для того, чтобы на компьютере появились топики с роботов необходимо указать по какому адресу находится мастер-нода (мастер-нода робота) командами:

```bash
export ROS_MASTER_URI=http://<IP-адрес робота>:11311/
export ROS_HOSTNAME=<IP-адрес компьютера>
```

Для запуска и настройки параметров скрипта есть `.launch` файл `joy_teleop.launch`, запускать его необходимо в том же терминале, в котором указан адрес мастер-нода робота: 

```bash
roslaunch joybro joy_teleop.launch
```

Файл joy_teleop.launch запускает собственный процесс `rosserial`, поэтому необходимо остановить все запущенные ранее процессы `rosserial`.

Управление перемещенияем робота, происходит при перемещении левого "стика" на джойстике, только при нажатой кнопке `btn3` (правая верхняя кнопки)

### Управление полезной нагрузкой с Arduino Mega платы turtleboard

Для управления полезной нагрузкой, необходимо создать собсвенный подписчик на топик `/joybro` и в зависимости от полученных данных производить необходимые операции.

Пример подписчика, управляющий сервоприводами на пинах 44,45,46

[Arduino/ros_joybro_subscriber/ros_joybro_subscriber.ino](https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro_subscriber/ros_joybro_subscriber.ino)


### Созданение библиотеки ros_lib

Для работы с Arduinо, необходимо произвести "сборку" библиотеки ROS для Arduino !!!на роботе!!!

```bash
cd 
rosrun rosserial_arduino make_libraries.py .
```

Команда создаст директорию `ros_lib` на RaspberyPi робота, которую необходимо переписать в папку библиотек Arduino на том компьютере на котором вы будете заливать скетчи Arduino в контроллер Arduino Mega. Обычно это папка `~/Arduino/libraries`

Необходимо выполнить пересборку библиотек Arduino, после установки пакета joybro, для того чтобы в библиотеке появился .h файл для работы с сообщением джойстика `JoyBro.msg`

Более подробно о настройке Arduino http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup
