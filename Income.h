//Income.h
#ifndef _INCOME_H
#define _INCOME_H
#include "Account.h"


class Income :public Account {
public:
	Income();
	Income(Date date, string content, Currency amount, Currency balance, string comment);
	Income(const Income& source);
	Income& operator = (const Income& source);
	virtual ~Income();
};

#endif // _INCOME_H

