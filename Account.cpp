//Account.cpp
#include "Account.h"

Account::Account()
	: content(""), comment("") {
	this->amount = 0.0;
	this->balance = 0.0;
}

Account::Account(Date date, string content, Currency amount, Currency balance, string comment)
	: date(date), content(content), comment(comment) {
	this->amount = amount;
	this->balance = balance;

}

Account:: ~Account() {
}

bool Account::IsEqual(const Account& other) {
	bool ret = false;
	if (this->date == other.date && this->content.compare(other.content) == 0 && this->amount == other.amount && this->balance == other.balance &&
		this->comment.compare(other.comment) == 0) {
		ret = true;
	}
	return ret;
}

bool Account::IsNotEqual(const Account& other) {
	bool ret = false;
	if (this->date != other.date || this->content.compare(other.content) != 0 && this->amount != other.amount && this->balance != other.balance &&
		this->comment.compare(other.comment) != 0) {
		ret = true;
	}
	return ret;
}

Account::Account(const Account& source)
	:date(source.date), content(source.content), comment(source.comment) {
	this->amount = source.amount;
	this->balance = source.balance;
}

Account& Account::operator=(const Account& source) {
	this->date = source.date;
	this->content = source.content;
	this->comment = source.comment;
	this->amount = source.amount;
	this->balance = source.balance;
	return *this;
}