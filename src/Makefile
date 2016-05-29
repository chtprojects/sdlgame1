
CFLAGS += '-std=c99'
CFLAGS += '-g'
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
CFLAGS += -D LINUX
LDFLAGS += -lSDL2 -lGL -lGLU -lm;
ENGINE=engine_desktop
endif

ifeq ($(UNAME_S),Darwin)
CFLAGS += -D MACOSX -I /Users/charlie/Desktop/SDL_scratch/externals/include
LDFLAGS += -F/Library/Frameworks -framework SDL2 -framework OpenGL -framework GLUT -lm;
ENGINE=engine_desktop
endif

SOURCES=globals.c $(ENGINE).c main.c game.c Geometry.c Shield.c Bullet.c Player.c Collision.c Controlls.c Explosion.c Enemy.c Weapons.c Enemies_pool.c FrameRate.c
OBJS=globals.o $(ENGINE).o main.o game.o Geometry.o Shield.o Bullet.o Player.o Collision.o Controlls.o Explosion.o Enemy.o Weapons.o Enemies_pool.o FrameRate.o

game:
	$(CC) $(CFLAGS) -c $(SOURCES)
	$(CC) $(CFLAGS) -o game.exe  $(OBJS) $(LDFLAGS)

.PHONY : clean, commit
clean :
	-rm $(OBJS)
	-rm game.exe

commit :
	../scripts/git-clang-format
