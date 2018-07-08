//Outgo.h
#ifndef _OUTGO_H
#define _OUTGO_H
#include "Account.h"


class Outgo :public Account {
public:
	Outgo();
	Outgo(Date date, string content, Currency amount, Currency balance, string comment);
	Outgo(const Outgo& source);
	Outgo& operator = (const Outgo& source);
	virtual ~Outgo();
};

#endif // _OUTGO_H
