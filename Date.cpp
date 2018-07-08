// Date.cpp
#define _CRT_SECURE_NO_WARNINGS
#include"Date.h"
#include<ctime>
#include<cstdio>

Date::Date() {
	this->year = 1900;
	this->month = JAN;
	this->day = 1;
	this->weekDay = MON;
}

Date::Date(int year, Month month, int day) {
	struct tm date = { 0, };
	date.tm_year = year - 1900;
	date.tm_mon = month - 1;
	date.tm_mday = day;
	mktime(&date);
	this->year = date.tm_year + 1900;
	this->month = static_cast<Month>(date.tm_mon + 1);
	this->day = date.tm_mday;
	this->weekDay = static_cast<WeekDay>(date.tm_wday);
}

Date::Date(const char(*date)) {
	struct tm date_ = { 0, };
	sscanf(date, "%4d%2d%2d", &this->year, &this->month, &this->day);
	date_.tm_year = this->year - 1900;
	date_.tm_mon = this->month - 1;
	date_.tm_mday = this->day;
	mktime(&date_);
	this->year = date_.tm_year + 1900;
	this->month = static_cast<Month>(date_.tm_mon + 1);
	this->day = date_.tm_mday;
	this->weekDay = static_cast<WeekDay>(date_.tm_wday);
}

Date::Date(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;
}

Date::~Date() {
}

Date Date::Today() {
	struct tm *today;
	time_t timer;
	Date today_;
	time(&timer);
	today = localtime(&timer);
	today_.year = today->tm_year + 1900;
	today_.month = static_cast<Month>(today->tm_mon + 1);
	today_.day = today->tm_mday;
	today_.weekDay = static_cast<WeekDay>(today->tm_wday);
	return today_;
}

Date Date::Yesterday() {
	struct tm yesterday = { 0, };
	Date yesterday_;
	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;
	mktime(&yesterday);
	yesterday_.year = yesterday.tm_year + 1900;
	yesterday_.month = static_cast<Month>(yesterday.tm_mon + 1);
	yesterday_.day = yesterday.tm_mday;
	yesterday_.weekDay = static_cast<WeekDay>(yesterday.tm_wday);
	return yesterday_;
}

Date Date::Tomorrow() {
	struct tm tomorrow = { 0, };
	Date tomorrow_;
	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;
	mktime(&tomorrow);
	tomorrow_.year = tomorrow.tm_year + 1900;
	tomorrow_.month = static_cast<Month>(tomorrow.tm_mon + 1);
	tomorrow_.day = tomorrow.tm_mday;
	tomorrow_.weekDay = static_cast<WeekDay>(tomorrow.tm_wday);
	return tomorrow_;
}

Date Date::PreviousDate(int days) {
	struct tm previousDate = { 0, };
	Date previousDate_;
	previousDate.tm_year = this->year - 1900;
	previousDate.tm_mon = this->month - 1;
	previousDate.tm_mday = this->day - days;
	mktime(&previousDate);
	previousDate_.year = previousDate.tm_year + 1900;
	previousDate_.month = static_cast<Month>(previousDate.tm_mon + 1);
	previousDate_.day = previousDate.tm_mday;
	previousDate_.weekDay = static_cast<WeekDay>(previousDate.tm_wday);
	return previousDate_;
}

Date Date::NextDate(int days) {
	struct tm nextDate = { 0, };
	Date nextDate_;
	nextDate.tm_year = this->year - 1900;
	nextDate.tm_mon = this->month - 1;
	nextDate.tm_mday = this->day + days;
	mktime(&nextDate);
	nextDate_.year = nextDate.tm_year + 1900;
	nextDate_.month = static_cast<Month>(nextDate.tm_mon + 1);
	nextDate_.day = nextDate.tm_mday;
	nextDate_.weekDay = static_cast<WeekDay>(nextDate.tm_wday);
	return nextDate_;
}

bool Date::IsEqual(const Date& other) {
	bool ret = false;
	if (this->year == other.year&&this->month == other.month&&this->day == other.day) {
		ret = true;
	}
	return ret;
}

bool Date::IsNotEqual(const Date& other) {
	bool ret = false;
	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}
	return ret;
}

bool Date::IsLesserThan(const Date& other) {
	bool ret = false;
	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month < other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day < other.day) {
		ret = true;
	}
	return ret;
}

bool Date::IsGreaterThan(const Date& other) {
	bool ret = false;
	if (this->year > other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day > other.day) {
		ret = true;
	}
	return ret;
}

Date::operator char*() {
	static char buffer[9];
	sprintf(buffer, "%4d%2d%2d", this->year, this->month, this->day);
	return buffer;
}

Date& Date::operator --() {
	struct tm yesterday = { 0, };
	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;
	mktime(&yesterday);
	this->year = yesterday.tm_year + 1900;
	this->month = static_cast<Month>(yesterday.tm_mon + 1);
	this->day = yesterday.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday.tm_wday);
	return *this;
}

Date Date::operator --(int) {
	struct tm yesterday = { 0, };
	Date temp(*this);
	yesterday.tm_year = this->year - 1900;
	yesterday.tm_mon = this->month - 1;
	yesterday.tm_mday = this->day - 1;
	mktime(&yesterday);
	this->year = yesterday.tm_year + 1900;
	this->month = static_cast<Month>(yesterday.tm_mon + 1);
	this->day = yesterday.tm_mday;
	this->weekDay = static_cast<WeekDay>(yesterday.tm_wday);
	return temp;
}

Date& Date::operator ++() {
	struct tm tomorrow = { 0, };
	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;
	mktime(&tomorrow);
	this->year = tomorrow.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow.tm_mon + 1);
	this->day = tomorrow.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow.tm_wday);
	return *this;
}

Date Date::operator ++(int) {
	struct tm tomorrow = { 0, };
	Date temp(*this);
	tomorrow.tm_year = this->year - 1900;
	tomorrow.tm_mon = this->month - 1;
	tomorrow.tm_mday = this->day + 1;
	mktime(&tomorrow);
	this->year = tomorrow.tm_year + 1900;
	this->month = static_cast<Month>(tomorrow.tm_mon + 1);
	this->day = tomorrow.tm_mday;
	this->weekDay = static_cast<WeekDay>(tomorrow.tm_wday);
	return temp;
}

Date Date::operator -(int days) {
	struct tm previousDate = { 0, };
	Date previousDate_;
	previousDate.tm_year = this->year - 1900;
	previousDate.tm_mon = this->month - 1;
	previousDate.tm_mday = this->day - days;
	mktime(&previousDate);
	previousDate_.year = previousDate.tm_year + 1900;
	previousDate_.month = static_cast<Month>(previousDate.tm_mon + 1);
	previousDate_.day = previousDate.tm_mday;
	previousDate_.weekDay = static_cast<WeekDay>(previousDate.tm_wday);
	return previousDate_;
}

Date Date::operator +(int days) {
	struct tm nextDate = { 0, };
	Date nextDate_;
	nextDate.tm_year = this->year - 1900;
	nextDate.tm_mon = this->month - 1;
	nextDate.tm_mday = this->day + days;
	mktime(&nextDate);
	nextDate_.year = nextDate.tm_year + 1900;
	nextDate_.month = static_cast<Month>(nextDate.tm_mon + 1);
	nextDate_.day = nextDate.tm_mday;
	nextDate_.weekDay = static_cast<WeekDay>(nextDate.tm_wday);
	return nextDate_;
}

bool Date::operator ==(const Date& other) {
	bool ret = false;
	if (this->year == other.year&&this->month == other.month&&this->day == other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator !=(const Date& other) {
	bool ret = false;
	if (this->year != other.year || this->month != other.month || this->day != other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator <(const Date& other) {
	bool ret = false;
	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month < other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day < other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator <=(const Date& other) {
	bool ret = false;
	if (this->year < other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month < other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day <= other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator >(const Date& other) {
	bool ret = false;
	if (this->year > other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day > other.day) {
		ret = true;
	}
	return ret;
}

bool Date::operator >=(const Date& other) {
	bool ret = false;
	if (this->year > other.year) {
		ret = true;
	}
	else if (this->year == other.year&&this->month > other.month) {
		ret = true;
	}
	else if (this->year == other.year&&this->month == other.month&&this->day >= other.day) {
		ret = true;
	}
	return ret;
}

Date& Date::operator =(const Date& source) {
	this->year = source.year;
	this->month = source.month;
	this->day = source.day;
	this->weekDay = source.weekDay;
	return *this;
}

// Main

/* 시나리오:
1. Create 한다.
2. Today 한다.
3. Yesterday 한다.
4. Tomorrow 한다.
5. Today의 전위증가 한다.
6. Today의 전위감소 한다.
7. Today의 후위증가 한다.
8. Today의 후위감소 한다.
9. 이전 일자를 구한다.
10. 이후 일자를 구한다.
11. 오늘과 이전일자를 비교한다.
12. 오늘과 이전일자를 비교한다.
13. 오늘과 이후일자를 비교한다.
14. 오늘과 이후일자를 비교한다.
15. 끝낸다
*/

#include<iostream>
using namespace std;

int main(int argc, char *argv[]) {
	Date date("20180430");
	cout << static_cast<char*>(date) << endl;
	Date date2(2018, APR, 30);
	cout << date2.GetYear() << "-" << date2.GetMonth() << "-" << date2.GetDay() << endl;
	Date today = Date::Today();
	cout << today.GetYear() << "-" << today.GetMonth() << "-" << today.GetDay() << endl;
	Date date3(today);
	cout << date3.GetYear() << "-" << date3.GetMonth() << "-" << date3.GetDay() << endl;
	Date yesterday = today.Yesterday();
	cout << yesterday.GetYear() << "-" << yesterday.GetMonth() << "-" << yesterday.GetDay() << endl;
	Date tomorrow = today.Tomorrow();
	cout << tomorrow.GetYear() << "-" << tomorrow.GetMonth() << "-" << tomorrow.GetDay() << endl;
	Date temp = ++today;
	if (temp == tomorrow) {
		cout << "같은 일자입니다." << endl;
	}
	temp = --today;
	if (temp == tomorrow) {
		cout << "같은 일자입니다." << endl;
	}
	temp = today++ - 1;
	if (temp == tomorrow) {
		cout << "같은 일자입니다." << endl;
	}
	if (today == tomorrow) {
		cout << "같은 일자입니다." << endl;
	}
	temp = today-- - 1;
	if (temp == date) {
		cout << "같은 일자입니다." << endl;
	}
	if (today != tomorrow) {
		cout << "다른 일자입니다." << endl;
	}
	Date previousDate = yesterday.PreviousDate(1);
	cout << previousDate.GetYear() << "-" << previousDate.GetMonth() << "-" << previousDate.GetDay() << endl;
	Date nextDate = today.NextDate(6);
	cout << nextDate.GetYear() << "-" << nextDate.GetMonth() << "-" << nextDate.GetDay() << endl;
	if (today < nextDate) {
		cout << "이전 일자입니다." << endl;
	}
	if (today <= nextDate) {
		cout << "같거나 이전 일자입니다." << endl;
	}
	if (today > previousDate) {
		cout << "이후 일자입니다." << endl;
	}
	if (today >= previousDate) {
		cout << "같거나 이전 일자입니다." << endl;
	}
	return 0;
}