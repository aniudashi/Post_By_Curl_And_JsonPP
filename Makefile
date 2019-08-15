all: TestPostJson
INCLUDE_PATH=-I/usr/local/include/jsoncpp/
LINK_LIBS = -lstdc++ -lcurl -ljsoncpp
DEFS= -DDBUILDING_LIBCURL
TestPostJson:
	gcc -o TestPostJson Main.cpp $(DEFS) $(INCLUDE_PATH) $(LINK_LIBS)
clean:
	rm -rf TestPostJson
