
Tanks - Unexpected Enemies
==========================
C++/Qt-based artillery clone

Features
---------
* Singleplayer with three bot types
* Unique gameplay with surprises and unexpected enemies...
 
![PRINTSCREEN1](https://github.com/solarix95/tanks-ue/blob/master/doc/tanks-ue-1.png)
![PRINTSCREEN2](https://github.com/solarix95/tanks-ue/blob/master/doc/tanks-ue-2.png)
![PRINTSCREEN3](https://github.com/solarix95/tanks-ue/blob/master/doc/tanks-ue-3.png)

First steps
---------
```bash
$ sudo apt install qt5-default qt5-qmake qtmultimedia5-dev
$ git clone https://github.com/solarix95/tanks-ue.git
$ cd tanks-ue
$ qmake && make -j8
$ ./tanks-ue
```

Use SDL as Audio-Backend (instead of QMultimedia)
```bash
$ sudo apt install libsdl-mixer1.2-dev
$ cd tanks-ue
$ <edit tanks-ue.pro: enable "CONFIG += with_sdl", disable "CONFIG += with_multimedia">
$ qmake && make distclean && qmake && make -j8
$ ./tanks-ue
```

Play with keyboard only:
* Arrows: Menu, Angle, Force
* Space: Acivate/Fire
* Tab: Toggle statusbar
* F5: Toggle speed

