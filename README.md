# Пакет ROS для джойстика joybro


### Установка пакета

Для установки пакета, необходимо склонировать репозиторий в вашу папку `catkin_ws/src`

```bash
cd ~/catkin_ws/src
git clone https://github.com/voltbro/joybro
```

Далее необходимо запустить процесс сборки пакета

```bash
cd ~/catkin_ws
sudo ./src/catkin/bin/catkin_make_isolated --install -DCMAKE_BUILD_TYPE=Release --install-space /opt/ros/melodic --pkg=joybro
```

Для работы, может понадобиться пакет `rosserial`. Если пакет не установлен, то его можно установить командой

```bash
sudo apt install ros-melodic-rosserial-arduino
```


### Подключение Джойстика

На Arduino джойстика необходимо загрузить скетч [Arduino/ros_joybro/ros_joybro.ino](https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro/ros_joybro.ino)

Данный скетч с частотой 20 герц производит чтение всех контролов джойстика и передает в ROS в топик `/joybro`

Тип передаваемого сообщения [msg/JoyBro.msg](https://github.com/voltbro/joybro/blob/master/msg/JoyBro.msg)

Для того чтобы данные из джойстика передавались в ROS, необходимо на компьютере подключенному к джойстику запустить процесс `rosserial`, который подключаеться к `Arduino` 
и пробрасывает все сообщения в ROS

Для этого удобно запустить готовый `.launch` файл командой 

```bash
roslaunch joybro joy_serial.launch
```

После запуска `joy_serial.launch`, возможно проверить правильность работы, выполнив команду
```bash
rostopic echo /joybro
```

### Управление роботом turtlebro/turtlebot3

Python скрипт [/src/joy_teleop.py](https://github.com/voltbro/joybro/blob/master/src/joy_teleop.py) уплавления роботом через топик `/cmd_vel` 

Для запуска и настройки параметров скрипта есть `.launch` файл, запустить его можно 

```bash
roslaunch joybro joy_teleop.launch
```

Файл joy_teleop.launch запускает собсвенный процесс `rosserial`, поэтому необходимо остановить все запущенные ранее процессы `rosserial`

Управление перемещенияем робота, происходит при перемещении левого "стика" на джойстике, только при нажатой кнопке `btn3` (правая верхняя кнопки)

### Управление полезной нагрузкой с Arduino Mega платы turtlebro

Для управления полезной нагрузкой, необходимо создать собсвенный подписчик на топик `/joybro` и в зависимости от полученных данных производить необходимые операции.

Пример подписчика, управляющие сервами на пинах 44,45,46

[Arduino/ros_joybro_subscriber/ros_joybro_subscriber.ino](https://github.com/voltbro/joybro/blob/master/Arduino/ros_joybro_subscriber/ros_joybro_subscriber.ino)


### Созадение библиотеки ros_lib

Для работы с Arduinо, необходимо произвести "сборку" библиотеки ROS для Arduino

```bash
cd 
rosrun rosserial_arduino make_libraries.py .
```

Команда создаст директорию `ros_lib` которую необходимо переписать в папку библиотек Arduino. Обычно это папка `~/Arduino/libraries`

Необходимо выполнить пересборку библиотек, после установки пакета joybro, для того чтобы в библиотеке появился .h файл для работы с сообщением джойстика `JoyBro.msg`

Более подробно о настройке Arduino http://wiki.ros.org/rosserial_arduino/Tutorials/Arduino%20IDE%20Setup

