CC=g++
FLAG=-g -std=c++11
ALL: Cmain AgendaUI User Meeting Date
	$(CC) $(FLAG) -o client *.o
Cmain: Cmain.cpp
	$(CC) $(FLAG) -c Cmain.cpp
AgendaUI: AgendaUI.cpp
	$(CC) $(FLAG) -c AgendaUI.cpp
User: User.cpp
	$(CC) $(FLAG) -c User.cpp
Meeting: Meeting.cpp
	$(CC) $(FLAG) -c Meeting.cpp
Date: Date.cpp
	$(CC) $(FLAG) -c Date.cpp
clean:
	rm *.o client
