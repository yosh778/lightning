TARGET = Lightning
OBJS = main.o menu.o game.o ppc.o tinylib.o

INCDIR =
CFLAGS = -G4 -Wall -O3 -Wno-write-strings
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)


PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin

LIBDIR =
LDFLAGS =
STDLIBS= -losl -lpsphprm -ljpeg -lpng -lz \
		-lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspaudiolib -lpspaudio -lm -lstdc++
LIBS=$(STDLIBS)$(YOURLIBS)
PSP_FW_VERSION=600
BUILD_PRX = 1
PSP_LARGE_MEMORY = 1
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Lightning
PSP_EBOOT_ICON=ICON0.PNG
PSP_EBOOT_PIC1=
PSP_EBOOT_SND0= snd0.at3

PSPSDK=$(shell psp-config --pspsdk-path)
DEFAULT_CFLAGS = $(shell $(SDL_CONFIG) --cflags)
include $(PSPSDK)/lib/build.mak