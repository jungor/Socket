CC=g++
FLAG=-g -std=c++11
ALL: Smain AgendaService Storage User Meeting Date
	$(CC) $(FLAG) -o service *.o
Smain: smain.cpp
	$(CC) $(FLAG) -c smain.cpp
AgendaService: AgendaService.cpp
	$(CC) $(FLAG) -c AgendaService.cpp
Storage: Storage.cpp
	$(CC) $(FLAG) -c Storage.cpp
User: User.cpp
	$(CC) $(FLAG) -c User.cpp
Meeting: Meeting.cpp
	$(CC) $(FLAG) -c Meeting.cpp
Date: Date.cpp
	$(CC) $(FLAG) -c Date.cpp
clean:
	rm *.o service
