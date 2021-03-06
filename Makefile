include Makefile.framework
include Makefile.demo.resources

build: bin/game $(DEMORESOURCES)

bin/game: lib/libcruftybehemoth.so lib/libdemo.so obj/main.o
	mkdir -p bin
	$(CC) -o bin/game $(LDFLAGS) obj/main.o  -ldemo -lcruftybehemoth `pkg-config --libs gdk-pixbuf-2.0 gtk+-2.0`

include Makefile.framework.targets
include Makefile.demo.targets

obj/%.o: src/%.cpp
	mkdir -p obj
	$(CC) $(CFLAGS) `pkg-config --cflags gdk-pixbuf-2.0 gtk+-2.0` -o $@ $<
	
