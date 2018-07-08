//Acount.h

#ifndef _ACCOUNT_H
#define _ACCOUNT_H

#include "Date.h"
#include <string>
typedef double Currency;
using namespace std;
class Account {
public:
	Account();
	Account(Date date, string content, Currency amount, Currency Balance, string comment);
	virtual ~Account() = 0;
	bool IsEqual(const Account& other);
	bool IsNotEqual(const Account& other);
	Account(const Account& source);
	Account& operator=(const Account& source);
	Date& GetDate() const;
	string& GetContent() const;
	Currency GetAmount() const;
	Currency GetBalance() const;
	string& GetComment() const;
private:
	Date date;
	string content;
	Currency amount;
	Currency balance;
	string comment;

};

inline Date& Account::GetDate() const {
	return const_cast<Date&> (this->date);
}
inline string& Account::GetContent() const {
	return const_cast<string&>(this->content);
}
inline Currency Account::GetAmount() const {
	return this->amount;
}
inline Currency Account::GetBalance() const {
	return this->balance;
}
inline string& Account::GetComment() const {
	return const_cast<string&>(this->comment);
}


#endif // _ACCOUNT_H
