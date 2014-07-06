#ifndef AGENDAUI_H
#define AGENDAUI_H

#include"Meeting.h"
#include"User.h"
#include<iostream>
#include<string>
#include<list>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#define MAXSIZE 4096
#define MYPORT 6666

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop(void);

 private:
  // task functions
  void startAgenda(void);
  std::string getOperation();
  bool executeOperation(std::string op);
  void userLogIn(void);
  void userRegister(void);
  void quitAgenda(void);
  void userLogOut(void);
  void deleteUser(void);
  void listAllUsers(void);
  void createMeeting(void);
  void listAllMeetings(void);
  void listAllSponsorMeetings(void);
  void listAllParticipateMeetings(void);
  void queryMeetingByTitle(void);
  void queryMeetingByTimeInterval(void);
  void deleteMeetingByTitle(void);
  void deleteAllMeetings(void);
  void printMeetings(std::list<Meeting> meetings);
  // dates
  std::string userName_;
  std::string userPassword_;
 public:
  int sockfd, n;
  char recvline[MAXSIZE], sendline[MAXSIZE];
  sockaddr_in servaddr;
  hostent *host;
  char ipsrc[20];
};

#endif
