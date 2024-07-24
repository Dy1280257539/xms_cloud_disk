SRC=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC) )
TARGET=libLXMysql.so
CPPFLAGS= -fPIC  -I/usr/include/mysql
LIBS=-lpthread 
# apt-get install libmysqlclient-dev

$(TARGET):$(OBJS)
	g++ -shared $+ -o $@ $(LIBS)
	cp *.so /usr/lib/
install:
	cp libLXMysql.so /usr/lib/
	-mkdir /usr/include/xms
	cp *.h /usr/include/xms
	@echo "install libLXMysql.so success!"
clean:
	rm -rf *.o $(TARGET)
