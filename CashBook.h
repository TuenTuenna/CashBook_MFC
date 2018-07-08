// CashBook.h
#ifndef _CASHBOOK_H
#define _CASHBOOK_H
#include"Array.h"
#include"Date.h"
#include<string>
typedef signed long int Long;
typedef double Currency;
using namespace std;

class Account;

class CashBook {
public:
	CashBook(Long capacity = 256);
	CashBook(const CashBook& source);
	CashBook& operator =(const CashBook& source);
	~CashBook();
	Long Record(Date date, string content, Currency amount, string comment);
	void Find(Date date, Long*(*indexes), Long *count);
	void Find(string content, Long*(*indexes), Long *count);
	void Find(Date date, string content, Long*(*indexes), Long *count);
	Long Correct(Long index, Currency amount, string comment);
	void Calculate(Date beginning, Date end, Currency *sumOfIncome, Currency *sumOfOutgo, Currency *sumOfBalance);
	Account* GetAt(Long index);
	Long GetCapacity() const;
	Long GetLength() const;
private:
	Array<Account*> accounts;
	Long capacity;
	Long length;
};
inline Long CashBook::GetCapacity() const {
	return this->capacity;
}
inline Long CashBook::GetLength() const {
	return this->length;
}
int CompareDates(void *one, void *other);
int CompareContents(void *one, void *other);

#endif // _CASHBOOK_H