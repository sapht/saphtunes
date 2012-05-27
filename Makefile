# vim: set noexpandtab:
CFLAGS = -O0 -ggdb -Wall -std=gnu99

st: init obj/album.o obj/cache.o obj/git.o obj/main.o obj/song.o obj/ui.o obj/ui_common.o obj/ui_page1.o obj/ui_page2.o obj/util.o
	gcc -o build/st obj/*.o $(CFLAGS) `pkg-config --libs gtk+-2.0`

init:
	@mkdir -p build 
	@mkdir -p obj

obj/album.o: src/album.c src/song.h src/git.h src/album.h src/util.h src/main.c
	gcc -c src/album.c -o obj/album.o $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/cache.o: src/cache.c src/main.h src/cache.h
	gcc -c src/cache.c -o obj/cache.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/git.o: src/git.c src/git.h src/util.h src/cache.h
	gcc -c src/git.c -o obj/git.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/main.o: src/main.c src/main.h src/cache.h src/ui.h src/song.h src/git.h \
  src/album.h
	gcc -c src/main.c -o obj/main.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/song.o: src/song.c src/song.h src/git.h src/util.h src/cache.h
	gcc -c src/song.c -o obj/song.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/ui.o: src/ui.c src/main.h src/cache.h src/album.h src/song.h src/git.h \
  src/ui.h src/ui_common.h src/ui_page1.h src/ui_page2.h
	gcc -c src/ui.c -o obj/ui.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/ui_common.o: src/ui_common.c src/ui_common.h src/song.h src/git.h \
  src/album.h src/main.h src/cache.h
	gcc -c src/ui_common.c -o obj/ui_common.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/ui_page1.o: src/ui_page1.c src/main.h src/cache.h src/ui_common.h \
  src/song.h src/git.h src/ui_page1.h src/album.h
	gcc -c src/ui_page1.c -o obj/ui_page1.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/ui_page2.o: src/ui_page2.c src/main.h src/cache.h src/ui_common.h \
  src/song.h src/git.h src/ui_page2.h src/album.h
	gcc -c src/ui_page2.c -o obj/ui_page2.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

obj/util.o: src/util.c src/util.h
	gcc -c src/util.c -o obj/util.o  $(CFLAGS) `pkg-config --cflags gtk+-2.0`

clean:
	echo obj/*.o | xargs rm
	@rm -f build/st

gui:
	cat guiscripts/_common.applescript \
        guiscripts/_reasonbatch.applescript \
        guiscripts/resave.applescript | osacompile -o build/resave.scpt
	cat guiscripts/_common.applescript \
        guiscripts/_reasonbatch.applescript \
        guiscripts/render.applescript | osacompile -o build/render.scpt
