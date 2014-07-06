#ifndef AGENDASERVICE_H
#define AGENDASERVICE_H

#include "Storage.h"
#include <list>
#include <string>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define MAXSIZE 4096
#define MYPORT 6666

class AgendaService {
 public:
  AgendaService();
  ~AgendaService();
  bool userLogIn(std::string userName, std::string password);
  bool userRegister(std::string userName, std::string password,
                    std::string email, std::string phone);
  bool deleteUser(std::string userName, std::string password); // a user can only delete itself
  std::list<User> listAllUsers(void);

  bool createMeeting(std::string userName, std::string title,
                     std::string participator,
                     std::string startDate, std::string endDate);
  std::list<Meeting> meetingQuery(std::string userName, std::string title);
  std::list<Meeting> meetingQuery(std::string userName, std::string startDate,
                                  std::string endDate);
  std::list<Meeting> listAllMeetings(std::string userName);
  std::list<Meeting> listAllSponsorMeetings(std::string userName);
  std::list<Meeting> listAllParticipateMeetings(std::string userName);
  bool deleteMeeting(std::string userName, std::string title);
  bool deleteAllMeetings(std::string userName);
  void startAgenda(void);
  void quitAgenda(void);
 private:
  Storage *storage_;
};

#endif
