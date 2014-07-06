#include"AgendaService.h"
#include<iostream>
#include<exception>
AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
    std::list<User> users = storage_->queryUser(
            [](const User&) {return true;});
    int n = users.size();
    std::list<User>::iterator cur = users.begin();
    while (n--) {
        if (cur->getName() == userName && cur->getPassword() == password) {
            return 1;
        } else {
            cur++;
        }
    }
    return 0;
}

bool AgendaService::userRegister(std::string userName, std::string password,
                  std::string email, std::string phone) {
    std::list<User> users = storage_->queryUser(
            [](const User&) {return true;});
    User tmp(userName, password, email, phone);
    int n = users.size();
    std::list<User>::iterator cur = users.begin();
    while (n--) {
        if (cur->getName() == userName) {
            return 0;
        } else {
            cur++;
        }
    }
    storage_->createUser(tmp);
    return 1;
}

bool AgendaService::deleteUser(std::string userName, std::string password) {
    storage_->deleteMeeting(
        [&](const Meeting& meeting)
        {return meeting.getParticipator() == userName
            || meeting.getSponsor() == userName;});
    return storage_->deleteUser(
        [&](const User& user) {return user.getName() == userName;});
}

std::list<User> AgendaService::listAllUsers(void) {
    return storage_->queryUser(
        [](const User& user) {return true;});
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                   std::string participator,
                   std::string startDate, std::string endDate) {
    if (!(Date::isValid(Date::stringToDate(startDate))
            && Date::isValid(Date::stringToDate(endDate))
            && endDate > startDate)) {
        return 0;
    }
    std::list<User> users = storage_->queryUser(
        [&](const User& user) {return user.getName() == participator;
    });
    std::list<Meeting> meetings = storage_->queryMeeting(
        [&](const Meeting& meeting){return (userName == meeting.getSponsor() || userName == meeting.getParticipator() || participator == meeting.getSponsor() || participator == meeting.getParticipator()) && (title == meeting.getTitle() || !(Date::dateToString(meeting.getEndDate()) <= startDate || Date::dateToString(meeting.getStartDate()) >= endDate));});
    if (userName != participator && users.size() && meetings.empty()) {
        Meeting tmp(userName, participator, Date::stringToDate(startDate),
                Date::stringToDate(endDate), title);
        storage_->createMeeting(tmp);
        return 1;
    } else {
        return 0;
    }
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName,
                                            std::string title) {
    return storage_->queryMeeting(
        [&](const Meeting& meeting) {return (
        meeting.getSponsor() == userName
        || meeting.getParticipator() == userName)
        && meeting.getTitle() == title;});
}
std::list<Meeting> AgendaService::meetingQuery(std::string userName,
                                 std::string startDate, std::string endDate) {
    return storage_->queryMeeting(
        [&](const Meeting& meeting) {
            return Date::isValid(Date::stringToDate(startDate))
            && Date::isValid(Date::stringToDate(endDate))
            && (!(Date::dateToString(meeting.getStartDate()) > endDate
            || Date::dateToString(meeting.getEndDate()) < startDate))
            && (meeting.getParticipator() == userName
            || meeting.getSponsor() == userName);});
}
std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
    return storage_->queryMeeting(
        [&](const Meeting& meeting) {return meeting.getSponsor() == userName
        || meeting.getParticipator() == userName;});
}
std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
    return storage_->queryMeeting(
        [&](const Meeting& meeting) {return meeting.getSponsor() == userName;});
}
std::list<Meeting> AgendaService::listAllParticipateMeetings(
                                    std::string userName) {
    return storage_->queryMeeting(
        [&](const Meeting& meeting) {
            return meeting.getParticipator() == userName;});
}
bool AgendaService::deleteMeeting(std::string userName, std::string title) {
    return storage_->deleteMeeting(
        [&](const Meeting& meeting) {return meeting.getSponsor() == userName
        && meeting.getTitle() == title;}  );
}
bool AgendaService::deleteAllMeetings(std::string userName) {
    return storage_->deleteMeeting(
        [&](const Meeting& meeting) {return meeting.getSponsor() == userName;});
}
void AgendaService::startAgenda(void) {
    storage_ = Storage::getInstance();
}
void AgendaService::quitAgenda(void) {
    if (storage_) {
        storage_->sync();
        storage_ = 0;
    }
}
