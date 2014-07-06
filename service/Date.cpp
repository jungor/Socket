#include"Date.h"
#include<sstream>
#include<string>

inline bool islean(int y) {
    return ((y%100 == 0) && (y%400 == 0)) || ((y%100 != 0) && (y%4 == 0));
}

inline bool daysinmonth(int y, int m, int d) {
    if ((y < 1000) || (y > 9999)) return 0;
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return (d > 0) && (d <= 31);
        case 2:
        return (islean(y)&&(d > 0)&& d <= 29)||(!islean(y) && d > 0 && d <= 28);
        case 4: case 6: case 9: case 11:
        return (d > 0) && (d <= 30);
        default:
        return 0;
    }
}

Date::Date() {
    year_ = month_ = day_ = hour_ = minute_ = 0;
}

Date::Date(int y, int m, int d, int h, int mi) {
    year_ = y;
    month_ = m;
    day_ = d;
    hour_ = h;
    minute_ = mi;
}

int Date::getYear(void) const {
    return year_;
}

void Date::setYear(int year) {
    year_  = year;
}

int Date::getMonth(void) const {
    return month_;
}

void Date::setMonth(int month) {
    month_ = month;
}

int Date::getDay(void) const {
    return day_;
}

void Date::setDay(int day) {
    day_ = day;
}

int Date::getHour(void) const {
    return hour_;
}

void Date::setHour(int hour) {
    hour_ = hour;
}

int Date::getMinute(void) const {
    return minute_;
}

void Date::setMinute(int minute) {
    minute_ = minute;
}

bool Date::isValid(Date date) {
    return daysinmonth(date.year_, date.month_, date.day_) && (date.hour_ > 0)
            && (date.hour_ < 24) && (date.minute_ > 0) && (date.minute_ < 60);
}

Date Date::stringToDate(std::string dateString) {
    std::string y, m, d, h, mi;
    int y_, m_, d_, h_, mi_;
    std::stringstream ss;
    y = dateString.substr(0, 4);
    dateString.erase(0, 5);
    m = dateString.substr(0, 2);
    dateString.erase(0, 3);
    d = dateString.substr(0, 2);
    dateString.erase(0, 3);
    h = dateString.substr(0, 2);
    dateString.erase(0, 3);
    mi = dateString.substr(0, 2);
    ss << y << " " << m << " " << d << " " << h << " " << mi;
    ss >> y_ >> m_ >> d_ >> h_ >> mi_;
    return Date(y_, m_, d_, h_, mi_);
}

std::string Date::dateToString(Date date) {
    std::string str;
    std::stringstream ss;
    ss << date.year_ << '-'
       << (date.month_ < 9 ? "0" : "") << date.month_ << '-'
       << (date.day_ < 9 ? "0" : "") << date.day_ << '/'
       << (date.hour_ < 9 ? "0" : "") << date.hour_ << ':'
       << (date.minute_ < 9 ? "0" : "") << date.minute_;
       ss >> str;
       return str;
}

Date &Date::operator=(const Date& date) {
    if (this == &date) {
        return *this;
    } else {
        year_ = date.year_;
        month_ = date.month_;
        day_ = date.day_;
        hour_ = date.hour_;
        minute_ = date.minute_;
        return *this;
    }
}

bool Date::operator==(const Date& date) const {
    return Date::dateToString(*this) == Date::dateToString(date);
}

bool Date::operator>(const Date& date) const {
    return Date::dateToString(*this) > Date::dateToString(date);
}

bool Date::operator<(const Date& date) const {
    return Date::dateToString(*this) < Date::dateToString(date);
}

bool Date::operator>=(const Date& date) const {
    return Date::dateToString(*this) >= Date::dateToString(date);
}

bool Date::operator<=(const Date& date) const {
    return Date::dateToString(*this) <= Date::dateToString(date);
}
