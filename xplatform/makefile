SRC=$(wildcard *.cpp *.cc)
TMP=$(patsubst %.cpp,%.o,$(SRC) )
OBJS=$(patsubst %.cc,%.o,$(TMP) )
TARGET=libxcom.so
CPPFLAGS= -fPIC -g
LIBS=-levent -lpthread -levent_openssl
INSTALL_PATH=/usr/lib/

$(TARGET):$(OBJS)
	g++ -shared $+ -o $@ $(LIBS)
	cp *.so /usr/lib/
proto:
	protoc -I=./ --cpp_out=./ xmsg_com.proto xmsg_type.proto
install:$(TARGET) $(OBJS)
	@echo "begin install in "$(INSTALL_PATH)
	@cp *.so $(INSTALL_PATH)
	-mkdir /usr/include/xms
	cp *.h /usr/include/xms
	@echo "install success!"
uninstall:
	rm -rf *.o $(TARGET)
	rm -rf $(INSTALL_PATH)/$(TARGET)
clean:
	rm -rf *.o $(TARGET)


