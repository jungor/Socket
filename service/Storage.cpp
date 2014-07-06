#include"Storage.h"
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<list>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
Storage *Storage::instance_ = 0;
Storage::Storage() {
    readFromFile("agenda.data");
}
bool Storage::readFromFile(const char *fpath) {
    std::string user, meeting;
    std::string nm, pw, em, ph, sp, pt, sd, ed, tt;
    int u, m;
    std::ifstream infile;
    infile.open(fpath);
    if (infile.is_open()) {
        std::stringstream ss1, ss2;
        infile >> user;
        if (user == "") return 0;
        user = user.substr(user.find("total")+6,
            user.size()-user.find("total")-1);
        ss1 << user;
        ss1 >> u;
        while (u--) {
            infile >> user;
            nm = user.substr(user.find(':')+2, user.find(',')-user.find(':')-3);
            user.erase(user.find(':'), nm.size()+4);
            pw = user.substr(user.find(':')+2, user.find(',')-user.find(':')-3);
            user.erase(user.find(':'), pw.size()+4);
            em = user.substr(user.find(':')+2, user.find(',')-user.find(':')-3);
            user.erase(user.find(':'), em.size()+4);
            ph = user.substr(user.find(':')+2, user.find('}')-user.find(':')-3);
            User tmp(nm, pw, em, ph);
            userList_.push_back(tmp);
        }
        infile >> meeting;
        meeting = meeting.substr(meeting.find("total")+6,
            meeting.size()-meeting.find("total")-1);
        ss2 << meeting;
        ss2 >> m;
        while (m--) {
            infile >> meeting;
            sp = meeting.substr(meeting.find(':')+2,
                meeting.find(',')-meeting.find(':')-3);
            meeting.erase(meeting.find(':'), sp.size()+4);
            pt = meeting.substr(meeting.find(':')+2,
                meeting.find(',')-meeting.find(':')-3);
            meeting.erase(meeting.find(':'), pt.size()+4);
            sd = meeting.substr(meeting.find(':')+2,
                meeting.find(',')-meeting.find(':')-3);
            meeting.erase(meeting.find(':'), sd.size()+4);
            ed = meeting.substr(meeting.find(':')+2,
                meeting.find(',')-meeting.find(':')-3);
            meeting.erase(meeting.find(':'), ed.size()+4);
            tt = meeting.substr(meeting.find(':')+2,
                meeting.find('}')-meeting.find(':')-3);
            Meeting tmp(sp, pt, Date::stringToDate(sd),
                        Date::stringToDate(ed), tt);
            meetingList_.push_back(tmp);
        }
        infile.close();
        return 1;
    } else {
        return 0;
    }
}
bool Storage::writeToFile(const char *fpath) {
    ofstream outfile;
    outfile.open(fpath);
    if (outfile.is_open()) {
        int n1 = userList_.size(), n2 = meetingList_.size();
        outfile << "{collection:\"User\",total:"
                << userList_.size() << "}\n";
        std::list<User>::iterator cur1 = userList_.begin();
        while (n1--) {
            outfile << "{name:\""<< cur1->getName()
                    << "\",password:\"" << cur1->getPassword()
                    << "\",email:\"" << cur1->getEmail()
                    << "\",phone:\""<< cur1->getPhone()
                    << "\"}\n";
            cur1++;
        }
        outfile << "{collection:\"Meeting\",total:"
                << meetingList_.size() << "}\n";
        std::list<Meeting>::iterator cur2 = meetingList_.begin();
        while (n2--) {
            outfile << "{sponsor:\"" <<  cur2->getSponsor()
                    << "\",participator:\"" << cur2->getParticipator()
                    << "\",sdate:\"" << Date::dateToString(cur2->getStartDate())
                    << "\",edate:\"" << Date::dateToString(cur2->getEndDate())
                    << "\",title:\"" << cur2->getTitle()
                    << "\"}";
            if (n2) outfile << endl;
            cur2++;
        }
        outfile.close();
        return 1;
    } else {
        return 0;
    }
}
Storage *Storage::getInstance(void) {
    if (instance_ == 0) {
        instance_ = new Storage;
    }
     return instance_;
}
Storage::~Storage() {
    sync();
    instance_ = 0;
}
void Storage::createUser(const User& user) {
    userList_.push_back(user);
}
std::list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    std::list<User> tmp;
    std::list<User>::iterator cur = userList_.begin();
    int n = userList_.size();
    while (n--) {
        if (filter(*cur)) {
            tmp.push_back(*cur);
        }
        cur++;
    }
    return tmp;
}
int Storage::updateUser(std::function<bool(const User&)> filter,
    std::function<void(User&)> switcher) {
    int s = 0;
    std::list<User>::iterator cur = userList_.begin();
    int n = userList_.size();
    while (n--) {
        if (filter(*cur)) {
            switcher(*cur);
            s++;
        }
        cur++;
    }
    return s;
}
int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int s = 0;
    std::list<User>::iterator cur = userList_.begin();
    int n = userList_.size();
    while (n--) {
        if (filter(*cur)) {
            userList_.erase(cur);
            cur--;
            s++;
        }
        cur++;
    }
    return s;
}
void Storage::createMeeting(const Meeting& meeting) {
    meetingList_.push_back(meeting);
}
std::list<Meeting> Storage::queryMeeting(
                   std::function<bool(const Meeting&)> filter) {
    std::list<Meeting> tmp;
    std::list<Meeting>::iterator cur = meetingList_.begin();
    int n = meetingList_.size();
    while (n--) {
        if (filter(*cur)) {
            tmp.push_back(*cur);
        }
        cur++;
    }
    return tmp;
}
int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
    std::function<void(Meeting&)> switcher) {
    int s = 0;
    std::list<Meeting>::iterator cur = meetingList_.begin();
    int n = userList_.size();
    while (n--) {
        if (filter(*cur)) {
            switcher(*cur);
            s++;
        }
        cur++;
    }
    return s;
}
int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int s = 0;
    std::list<Meeting>::iterator cur = meetingList_.begin();
    int n = meetingList_.size();
    while (n--) {
        if (filter(*cur)) {
            meetingList_.erase(cur);
            cur--;
            s++;
        }
        cur++;
    }
    return s;
}
bool Storage::sync(void) {
    return writeToFile("agenda.data");
}
