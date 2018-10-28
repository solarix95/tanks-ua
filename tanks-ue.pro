######################################################################
# TANKS - UNEXPECTED ENEMIES
######################################################################

TEMPLATE     = app
TARGET       = tanks-ue
DEPENDPATH  += .
INCLUDEPATH += . global game menus player entities
CONFIG      += debug

# DISABLE FOR NO SOUND
CONFIG += with_sdl

QT += widgets gui

with_sdl {
   LIBS += -lSDL_mixer -lSDL
   DEFINES += WITH_SDL1
}

# Input
SOURCES += \
    main.cc \
    game/battlefield.cc \
    game/landscape.cc \
    game/sndbackend.cc \
    game/game.cc \
    game/sky.cc \
    game/wind.cc \
    game/camera.cc \
    game/gameoption.cc \
    menus/menu.cc \
    menus/infomenu.cc \
    menus/optionmenu.cc \
    menus/playermenu.cc \
    menus/roundsmenu.cc \
    menus/featuremenu.cc \
    player/player.cc \
    player/localplayer.cc \
    player/bot.cc \
    player/vendettabot.cc \
    player/ammobot.cc \
    player/thinkerbot.cc \
    entities/entity.cc \
    entities/tank.cc \
    entities/swisstank.cc \
    entities/tree.cc \
    entities/cloud.cc \
    entities/shoot.cc \
    entities/bigbomb.cc \
    entities/explosion.cc \
    entities/particle.cc \
    entities/ammobox.cc \
    entities/hammershoot.cc \
    entities/scattershoot.cc \
    entities/atomicshoot.cc \
    entities/smoke.cc

HEADERS += \
    global/global.h \
    game/battlefield.h \
    game/landscape.h \
    game/sndbackend.h \
    game/game.h \
    game/sky.h \
    game/wind.h \
    game/camera.h \
    game/gameoption.h \
    game/bullet.h \
    menus/menu.h \
    menus/infomenu.h \
    menus/optionmenu.h \
    menus/playermenu.h \
    menus/roundsmenu.h \
    menus/featuremenu.h \
    player/player.h \
    player/localplayer.h \
    player/bot.h \
    player/vendettabot.h \
    player/ammobot.h \
    player/thinkerbot.h \
    entities/entity.h \
    entities/tank.h \
    entities/swisstank.h \
    entities/tree.h \
    entities/cloud.h \
    entities/shoot.h \
    entities/bigbomb.h \
    entities/explosion.h \
    entities/particle.h \
    entities/ammobox.h \
    entities/hammershoot.h \
    entities/scattershoot.h \
    entities/atomicshoot.h \
    entities/smoke.h \
    global/tuemath.h


RESOURCES += \
    tue.qrc
