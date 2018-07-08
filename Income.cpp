//Income.cpp
#include "Income.h"

Income::Income() {
}
Income::Income(Date date, string content, Currency amount, Currency balance, string comment)
	:Account(date, content, amount, balance, comment) {
}
Income::Income(const Income& source)
	: Account(source) {
}
Income& Income::operator=(const Income& source) {
	Account::operator=(source);
	return *this;
}

Income::~Income() {

}