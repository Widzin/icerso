copy:
	cp /home/oskar/workspace/cpp/rso-ice/pro/* .

slice:
	slice2cpp chat.ice

client: Client.cpp chat.cpp chatI.cpp
	g++ -I. chat.cpp chatI.cpp Client.cpp -lIce -lIceUtil -lpthread -o client

server: Server.cpp chat.cpp chatI.cpp
	g++ -I. chat.cpp chatI.cpp Server.cpp -lIce -lIceUtil -lpthread -o server

clean:
	-rm chat.cpp chat.h client server
all:
	make copy
	make clean
	make slice
	make client
	make server
