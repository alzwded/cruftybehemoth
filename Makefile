include Makefile.framework

build: framework-lib demo-lib demo-res src/main.cpp 
	mkdir -p bin
	$(CC) $(CFLAGS) `pkg-config --cflags gdk-pixbuf-2.0 gtk+-2.0` -o obj/main.o src/main.cpp
	$(CC) -o bin/game $(LDFLAGS) obj/main.o  -ldemo -lcruftybehemoth `pkg-config --libs gdk-pixbuf-2.0 gtk+-2.0`

include Makefile.demo.targets
include Makefile.framework.targets
