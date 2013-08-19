OBJECTS= Exception.o Timestamp.o Buffer.o \
EventLoop.o \
Thread.o \
Channel.o Poller.o PollPoller.o SocketsFun.o InetAddress.o Socket.o Acceptor.o TcpConnection.o \
TcpServer.o \
EventLoopTest.o 


EXECAPP=  test.x
CC_CP     = /usr/bin/g++  -c -g -rdynamic
CC_LINK   = /usr/bin/g++  -Wall -o

all: $(OBJECTS)

#base
Exception.o:
	$(CC_CP)  ../src/base/Exception.cpp
Timestamp.o:
	$(CC_CP)  ../src/base/Timestamp.cpp
Buffer.o:
	$(CC_CP)  ../src/base/Buffer.cpp

#mynet
Channel.o:
	$(CC_CP)  ../src/mynet/Channel.cpp
Poller.o:
	$(CC_CP)  ../src/mynet/Poller.cpp
PollPoller.o:
	$(CC_CP)  ../src/mynet/PollPoller.cpp
SocketsFun.o:
	$(CC_CP)  ../src/mynet/SocketsFun.cpp
InetAddress.o:
	$(CC_CP)  ../src/mynet/InetAddress.cpp
Socket.o:
	$(CC_CP)  ../src/mynet/Socket.cpp
Acceptor.o:
	$(CC_CP)  ../src/mynet/Acceptor.cpp
TcpConnection.o:
	$(CC_CP)  ../src/mynet/TcpConnection.cpp
TcpServer.o:
	$(CC_CP)  ../src/mynet/TcpServer.cpp	


#mythread
Thread.o:
	$(CC_CP)  ../src/mythread/Thread.cpp	
EventLoop.o:
	$(CC_CP)  ../src/mythread/EventLoop.cpp
	



#test
EventLoopTest.o:
	$(CC_CP)  ../test/EventLoopTest.cpp

link:
	$(CC_LINK) $(EXECAPP) $(OBJECTS) -lpthread
	
clean:	
	rm -f $(OBJECTS)

install:
	make clean
	rm $(EXECAPP)
	make all
	make link


