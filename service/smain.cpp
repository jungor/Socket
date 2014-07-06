#include"AgendaService.h"
#include<sstream>
#include<iostream>
AgendaService asv;
int listenfd, connfd;
sockaddr_in servaddr;
sockaddr_in clieaddr;
char buff[MAXSIZE];
int n;

int main(int argc, char *argv[]) {
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
    } else {
        printf("create socket OK!\n");
    }
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(MYPORT);
    if (bind(listenfd, (sockaddr*) & servaddr, sizeof (servaddr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno);
    } else {
        printf("bind socket OK!\n");
    }
    connfd = 0;
    if (listen(listenfd, 10) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        // throw std::exception(“在构造函数中抛出一个异常，测试！”);
    } else {
        printf("listen socket OK!\n");
        printf("======waiting for client's request======\n");
    }
    while (connfd == 0) {
        if ((connfd = accept(listenfd, (sockaddr*) NULL, NULL)) == -1) {
            printf("accept socket error: %s(errno: %d)", strerror(errno), errno);
            return -1;
        } else {
            printf("accept socket ok!\n");
        }
    }
    std::string op;
    while (1) {
        n = read(connfd, buff, MAXSIZE);
        if (n > 0) {
            buff[n] = '\0';
            printf("read operation %s\n", buff);
            op = buff;
            if (op == "l") {
                std::string name, psw;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read psw %s\n", buff);
                    psw = buff;
                }
                if (asv.userLogIn(name, psw)) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
            if (op == "r") {
                std::string name, psw, email, phone;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read psw %s\n", buff);
                    psw = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read  email %s\n", buff);
                    email = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read phone %s\n", buff);
                    phone = buff;
                }
                if (asv.userRegister(name, psw, email, phone)) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
            if (op == "dc") {
                std::string name;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                if (asv.deleteUser(name, "")) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
            if (op == "lu") {
                std::list<User> tmp = asv.listAllUsers();
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::string name, email, phone;
                std::list<User>::iterator cur = tmp.begin();
                while (m--) {
                    name = cur->getName();
                    email = cur->getEmail();
                    phone = cur->getPhone();
                    rtn1 = name + "            " + email + "                 " + phone+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "")rtn = "There is no user!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send users " << rtn << std::endl;
            }
            if (op == "cm") {
                std::string sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read sp %s\n", buff);
                    sp = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read tt %s\n", buff);
                    tt = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read pp %s\n", buff);
                    pt = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read  st %s\n", buff);
                    st = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read et %s\n", buff);
                    et = buff;
                }
                if (asv.createMeeting(sp, tt, pt, st, et)) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
            if (op == "la") {
                std::string name, sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                std::list<Meeting> tmp = asv.listAllMeetings(name);
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::list<Meeting>::iterator cur = tmp.begin();
                while (m--) {
                    sp = cur->getSponsor();
                    tt = cur->getTitle();
                    pt = cur->getParticipator();
                    st = Date::dateToString(cur->getStartDate());
                    et = Date::dateToString(cur->getEndDate());
                    rtn1 = tt + "    " + sp + "       " + pt+"      "+st+"    "+et+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "") rtn = "There is no meeting!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send meetngs " << rtn << std::endl;
            }
            if (op == "las") {
                std::string name, sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                std::list<Meeting> tmp = asv.listAllSponsorMeetings(name);
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::list<Meeting>::iterator cur = tmp.begin();
                while (m--) {
                    sp = cur->getSponsor();
                    tt = cur->getTitle();
                    pt = cur->getParticipator();
                    st = Date::dateToString(cur->getStartDate());
                    et = Date::dateToString(cur->getEndDate());
                    rtn1 = tt + "    " + sp + "       " + pt+"      "+st+"    "+et+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "") rtn = "There is no meeting!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send meetngs " << rtn << std::endl;
            }
            if (op == "lap") {
                std::string name, sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                std::list<Meeting> tmp = asv.listAllParticipateMeetings(name);
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::list<Meeting>::iterator cur = tmp.begin();
                while (m--) {
                    sp = cur->getSponsor();
                    pt = cur->getParticipator();
                    st = Date::dateToString(cur->getStartDate());
                    et = Date::dateToString(cur->getEndDate());
                    rtn1 = tt + "    " + sp + "       " + pt+"      "+st+"    "+et+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "") rtn = "There is no meeting!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send meetngs " << rtn << std::endl;
            }
            if (op == "qm") {
                std::string name, sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read tt %s\n", buff);
                    tt = buff;
                }
                std::list<Meeting> tmp = asv.meetingQuery(name, tt);
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::list<Meeting>::iterator cur = tmp.begin();
                while (m--) {
                    sp = cur->getSponsor();
                    pt = cur->getParticipator();
                    st = Date::dateToString(cur->getStartDate());
                    et = Date::dateToString(cur->getEndDate());
                    rtn1 = sp + "       " + pt+"      "+st+"    "+et+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "") rtn = "There is no meeting!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send meetngs " << rtn << std::endl;
            }
            if (op == "qt") {
                std::string name, sp, tt, pt, st, et;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read st %s\n", buff);
                    st = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read st %s\n", buff);
                    st = buff;
                }
                std::list<Meeting> tmp = asv.meetingQuery(name, st, et);
                int m = tmp.size();
                std::string rtn1 = "", rtn = "";
                std::list<Meeting>::iterator cur = tmp.begin();
                while (m--) {
                    sp = cur->getSponsor();
                    pt = cur->getParticipator();
                    st = Date::dateToString(cur->getStartDate());
                    et = Date::dateToString(cur->getEndDate());
                    rtn1 = sp + "       " + pt+"      "+st+"    "+et+"\n";
                    rtn += rtn1;
                    cur++;
                }
                if (rtn == "") rtn = "There is no meeting!\n";
                if (send(connfd, rtn.c_str(), rtn.size(), 0) == -1)
                    perror("send error!");
                else
                    std::cout << "send meetngs " << rtn << std::endl;
            }
            if (op == "dm") {
                std::string name, tt;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read tt %s\n", buff);
                    tt = buff;
                }
                if (asv.deleteMeeting(name, tt)) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
            if (op == "da") {
                std::string name;
                int rval = read(connfd, buff, MAXSIZE);
                if (rval > 0) {
                    buff[rval] = '\0';
                    printf("read name %s\n", buff);
                    name = buff;
                }
                if (asv.deleteAllMeetings(name)) {
                    char rtn[] = "true";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                } else {
                    char rtn[] = "false";
                    if (send(connfd, rtn, strlen(rtn), 0) == -1)
                        perror("send error!");
                }
            }
        } else {
            close(connfd);
            close(listenfd);
            return -2;
        }
    }
    close(connfd);
    close(listenfd);
    return 0;
}

