#include"AgendaUI.h"
#include"Meeting.h"
#include"User.h"
#include"Date.h"
#include<iomanip>
#include<exception>
#include<sstream>
using std::setw;
using std::left;

AgendaUI::AgendaUI() {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
        // throw std::exception(“在构造函数中抛出一个异常，测试！”);
    }
    printf("create socket OK!\n");
    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    if (inet_pton(AF_INET, ipsrc, &servaddr.sin_addr) <= 0) {
        printf("inet_pton error\n");
        // throw std::exception(“在构造函数中抛出一个异常，测试！”);
    }
    printf("inet_pton OK!\n");
    if (connect(sockfd, (sockaddr *) & servaddr, sizeof (servaddr)) < 0) {
        printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
        // exit(1);
    }
    printf("connect OK!\n");
}

void AgendaUI::OperationLoop(void) {
    startAgenda();
    std::string op;
    std::cin >> op;
    if (executeOperation(op)) {
        OperationLoop();
    } else {
        quitAgenda();
    }
}

void AgendaUI::startAgenda(void) {
    if (userName_ == "") {
        std::cout << "---------------------------- Agenda ---------------------------\n"
                << "Action : \n"
                << "l   - log in Agenda by username and password\n"
                << "r   - register an Agenda account\n"
                << "q   - quit Agenda\n"
                << "---------------------------------------------------------------\n\n"
                << "Agenda : ~$ ";
    } else {
        std::cout << "\nAgenda@" << userName_ << " : # ";
    }
}

bool AgendaUI::executeOperation(std::string op) {
    if (userName_ == "" && op == "l") {
        userLogIn();
    }
    if (userName_ == "" && op == "r") {
        userRegister();
    }
    if (userName_ == "" && op == "q") {
        quitAgenda();
        return 0;
    }
    if (userName_ != "" && op == "o") {
        userLogOut();
    }
    if (userName_ != "" && op == "dc") {
        deleteUser();
    }
    if (userName_ != "" && op == "lu") {
        listAllUsers();
    }
    if (userName_ != "" && op == "cm") {
        createMeeting();
    }
    if (userName_ != "" && op == "la") {
        listAllMeetings();
    }
    if (userName_ != "" && op == "las") {
        listAllSponsorMeetings();
    }
    if (userName_ != "" && op == "lap") {
        listAllParticipateMeetings();
    }
    if (userName_ != "" && op == "qm") {
        queryMeetingByTitle();
    }
    if (userName_ != "" && op == "qt") {
        queryMeetingByTimeInterval();
    }
    if (userName_ != "" && op == "dm") {
        deleteMeetingByTitle();
    }
    if (userName_ != "" && op == "da") {
        deleteAllMeetings();
    }
    return 1;
}

void AgendaUI::userLogIn(void) {
    std::string name, pwd, rtn = "false";
    char op[] = "l";
    std::cout << "\n[log in] [user name] [password]\n"
            << "[log in] ";
    write(sockfd, op, sizeof (op));
    scanf("%s", sendline);
    name = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    pwd = sendline;
    write(sockfd, sendline, sizeof (sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        userName_ = name;
        userPassword_ = pwd;
        std::cout << "[log in] succeed!\n\n"
                << "---------------------------- Agenda ---------------------------\n"
                << "Action : \n"
                << "o   - log out Agenda\n"
                << "dc  - delete Agenda account\n"
                << "lu  - list al Agenda user\n"
                << "cm  - creat a meeting\n"
                << "la  - list all meetings\n"
                << "las - list all sponsor meetings\n"
                << "lap - list all participator meetings\n"
                << "qm  - query meeting by title\n"
                << "qt  - query meeting by time interval\n"
                << "dm  - delete meeting by title\n"
                << "da  - delete all meetings\n"
                << "---------------------------------------------------------------\n"
                << std::endl;
    } else {
        std::cout << "[error] log in fail!\n";
    }
}

void AgendaUI::userRegister(void) {
    std::string name, pwd, email, phone;
    char op[] = "r";
    std::string rtn = "false";
    std::cout << "\n[register] [user name] [password] [email] [phone]\n"
            << "[register] ";
    write(sockfd, op, sizeof (op));
    scanf("%s", sendline);
    name = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    pwd = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    email = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    phone = sendline;
    write(sockfd, sendline, sizeof (sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        std::cout << "[register] succeed!\n\n";
    } else {
        std::cout << "[error] register fail!\n\n";
    }
}

void AgendaUI::quitAgenda(void) {
}

void AgendaUI::userLogOut(void) {
    userName_ = userPassword_ = "";
}

void AgendaUI::deleteUser(void) {
    char op[] = "dc";
    std::string rtn = "false";
    write(sockfd, op, sizeof (op));
    strncpy(sendline, userName_.c_str(), userName_.size());
    write(sockfd, sendline, sizeof (sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        userName_ = userPassword_ = "";
        std::cout << "\n[delete agenda account] succeed!\n\n";
    } else {
        std::cout << "\n[error]delete user fail!\n\n";
    }
}

void AgendaUI::listAllUsers(void) {
    char op[] = "lu";
    write(sockfd, op, sizeof (op));
    std::cout << "\n[list all users]\n\n"
            << setw(20) << left << "name" << " "
            << setw(20) << left << "email" << " "
            << setw(20) << left << "phone\n";
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }

    std::cout << std::endl;
}

void AgendaUI::createMeeting(void) {
    std::string tt, pt, st, et;
    char op[] = "cm";
    std::string rtn = "false";
    std::cout << "\n[creat meeting] [title] [participator] "
            << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
            << "[creat meeting] ";
    write(sockfd, op, strlen(op));
    write(sockfd, userName_.c_str(), userName_.size());
    scanf("%s", sendline);
    tt = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    pt = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    st = sendline;
    write(sockfd, sendline, sizeof (sendline));
    scanf("%s", sendline);
    et = sendline;
    write(sockfd, sendline, sizeof (sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        std::cout << "[creat meeting] succeed!\n\n";
    } else {
        std::cout << "[error] creat meeting fail!\n\n";
    }
}

void AgendaUI::listAllMeetings(void) {
    char op[] = "la";
    write(sockfd, op, sizeof (op));
    write(sockfd, userName_.c_str(), userName_.size());
    std::cout << "\n[list all meetings]\n\n"
            << setw(14) << left << "title" << " "
            << setw(14) << left << "sponsor" << " "
            << setw(14) << left << "participator" << " "
            << setw(15) << left << "start time" << " "
            << setw(15) << left << "end time"
            << std::endl;
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }
    std::cout << std::endl;
}

void AgendaUI::listAllSponsorMeetings(void) {
    char op[] = "las";
    write(sockfd, op, sizeof (op));
    write(sockfd, userName_.c_str(), userName_.size());
    std::cout << "\n[list all sponsor meetings]\n\n"
            << setw(14) << left << "title" << " "
            << setw(14) << left << "sponsor" << " "
            << setw(14) << left << "participator" << " "
            << setw(15) << left << "start time" << " "
            << setw(15) << left << "end time"
            << std::endl;
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }
    std::cout << std::endl;
}

void AgendaUI::listAllParticipateMeetings(void) {
    char op[] = "lap";
    write(sockfd, op, sizeof (op));
    write(sockfd, userName_.c_str(), userName_.size());
    std::cout << "\n[list all participate meetings]\n\n"
            << setw(14) << left << "title" << " "
            << setw(14) << left << "sponsor" << " "
            << setw(14) << left << "participator" << " "
            << setw(15) << left << "start time" << " "
            << setw(15) << left << "end time"
            << std::endl;
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTitle(void) {
    std::string tt;
    char op[] = "qm";
    write(sockfd, op, sizeof (op));
    write(sockfd, userName_.c_str(), userName_.size());
    std::cout << "\n[query  meeting] [title]\n\n]"
              << "[query  meeting] ";
    scanf("%s", sendline);
    tt = sendline;
    write(sockfd, sendline, sizeof(sendline));
    printf("%d\n", sizeof(sendline));
    std::cout << "\n[query meeting]\n\n"
            << setw(14) << left << "sponsor" << " "
            << setw(14) << left << "participator" << " "
            << setw(15) << left << "start time" << " "
            << setw(15) << left << "end time"
            << std::endl;
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::string st, et;
    char op[] = "qt";
    write(sockfd, op, sizeof (op));
    write(sockfd, userName_.c_str(), userName_.size());
    std::cout << "\n[query meetings] "
            << "[start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]\n"
            << "[query meetings] ";
    scanf("%s", sendline);
    st = sendline;
    write(sockfd, sendline, sizeof(sendline));
    scanf("%s", sendline);
    et = sendline;
    write(sockfd, sendline, sizeof(sendline));
    std::cout << "\n[query meetings]\n\n"
            << setw(14) << left << "sponsor" << " "
            << setw(14) << left << "participator" << " "
            << setw(15) << left << "start time" << " "
            << setw(15) << left << "end time"
            << std::endl;
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        std::cout << recvline;
    }
    std::cout << std::endl;
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::string title;
    char op[] = "da";
    std::string rtn = "false";
    write(sockfd, op, sizeof (op));
    strncpy(sendline, userName_.c_str(), userName_.size());
    write(sockfd, sendline, sizeof (sendline));
    std::cout << "[delete meeting] [title]\n"
            << "[delete meeting] ";
    scanf("%s", sendline);
    title = sendline;
    write(sockfd, sendline, sizeof(sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        std::cout << "\n[delete meeting by title] succeed!\n\n";
    } else {
        std::cout << "\n[error] delete meeting fail!\n\n";
    }
}

void AgendaUI::deleteAllMeetings(void) {
    char op[] = "da";
    std::string rtn = "false";
    write(sockfd, op, sizeof (op));
    strncpy(sendline, userName_.c_str(), userName_.size());
    write(sockfd, sendline, sizeof (sendline));
    if ((n = recv(sockfd, recvline, MAXSIZE, 0)) == -1) {
        perror("recv error!");
    } else {
        recvline[n] = '\0';
        rtn = recvline;
    }
    if (rtn == "true") {
        userName_ = userPassword_ = "";
        std::cout << "\n[delete agenda account] succeed!\n\n";
    } else {
        std::cout << "\n[error]delete user fail!\n\n";
    }
}

