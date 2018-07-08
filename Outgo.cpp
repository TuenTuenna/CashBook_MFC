//Outgo.cpp
#include "Outgo.h"

Outgo::Outgo(){
}
Outgo::Outgo(Date date, string content, Currency amount, Currency balance, string comment)
	:Account(date, content, amount, balance, comment) {
}
Outgo::Outgo(const Outgo& source)
	: Account(source) {
}
Outgo& Outgo::operator=(const Outgo& source) {
	Account::operator=(source);
	return *this;
}

Outgo::~Outgo() {

}