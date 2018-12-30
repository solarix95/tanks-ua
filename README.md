
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
$ sudo apt install qt5-default qt5-qmake libsdl-mixer1.2-dev
$ git clone https://github.com/solarix95/tanks-ue.git
$ cd tanks-ue
$ qmake && make -j8
$ ./tanks-ue
```

Use QMultimedia as Audio-Backend (instead of SDL)
```bash
$ sudo apt install qtmultimedia5-dev
$ cd tanks-ue
$ <edit tanks-ue.pro: disable "CONFIG += with_sdl", enable "CONFIG += with_multimedia">
$ qmake && make distclean && qmake && make -j8
$ ./tanks-ue
```

Play with keyboard only:
* Arrows: Menu, Angle, Force
* Space: Acivate/Fire
* Tab: Toggle statusbar
* F5: Toggle speed

