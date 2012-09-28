
INCLUDES := -I/usr/autodesk/maya2010-x64/include -I. -I$(TOOLS)/include/jsoncpp/0.5.0

DEFINES := -DLINUX -D_BOOL -DREQUIRE_IOSTREAM

CFLAGS := -fPIC -Wall -Werror -Wextra

LIBS   := -L$(TOOLS)/lib/jsoncpp/0.5.0 -L/usr/autodesk/maya2010-x64/lib -lOpenMaya -lOpenMayaUI -lFoundation -lGL -lGLU -ljsoncpp
LFLAGS := -Wl,-Bsymbolic

all:
	mkdir -p obj/ma plugin lib

	g++ -c ma/CommandSpec.cc $(CFLAGS) $(INCLUDES) $(DEFINES) -o obj/ma/CommandSpec.o
	g++ obj/ma/CommandSpec.o $(LFLAGS) $(LIBS) -shared -o lib/libma.so

	g++ -c testsuite/ExampleCmd.cc $(CFLAGS) $(INCLUDES) $(DEFINES) -o obj/ExampleCmd.o
	
	g++ -c testsuite/plugin.cc $(CFLAGS) $(INCLUDES) $(DEFINES) -o obj/plugin.o
	g++ obj/plugin.o obj/ExampleCmd.o $(LFLAGS) $(LIBS) -Llib -lma -shared -o plugin/examplePlugin.so
	

clean:
	rm -fr obj plugin 

