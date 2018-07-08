// CashBook.cpp
#include "CashBook.h"
#include "Account.h"
#include "Income.h"
#include "Outgo.h"

// Default생성자
CashBook::CashBook(Long capacity)
	:accounts(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

// 복사 생성자
CashBook::CashBook(const CashBook& source)
	:accounts(source.accounts) {
	Long i = 0;
	Account *account = (const_cast<CashBook&>(source)).GetAt(i);
	while (i < source.length) {
		if (dynamic_cast<Income*>(account)) {
			account = new Income(*(dynamic_cast<Income*>(account)));
		}
		else if (dynamic_cast<Outgo*>(account)) {
			account = new Outgo(*(dynamic_cast<Outgo*>(account)));
		}
		this->accounts.Modify(i, account);
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

// 치환 연산자
CashBook& CashBook::operator =(const CashBook& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->accounts[i] != 0) {
			delete this->accounts[i];
		}
		i++;
	}
	this->accounts = source.accounts;
	i = 0;
	Account *account = (const_cast<CashBook&>(source)).GetAt(i);
	while (i < source.length) {
		if (dynamic_cast<Income*>(account)) {
			account = new Income(*(dynamic_cast<Income*>(account)));
		}
		else if (dynamic_cast<Outgo*>(account)) {
			account = new Outgo(*(dynamic_cast<Outgo*>(account)));
		}
		this->accounts.Modify(i, account);
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
	return *this;
}

// 소멸자
CashBook::~CashBook() {
	Long i = 0;
	while (i < this->length) {
		if (this->accounts[i] != 0) {
			delete this->accounts[i];
		}
		i++;
	}
}

// Record
Long CashBook::Record(Date date, string content, Currency amount, string comment) {
	Account *account;
	Currency balance = 0.0;
	if (this->length >= 1) {
		account = this->accounts.GetAt(this->length - 1);
		balance = account->GetBalance();
	}
	if (amount > 0.0) {
		account = new Income(date, content, amount, balance + amount, comment);
	}
	else {
		account = new Outgo(date, content, -amount, balance + amount, comment);
	}
	Long index;
	if (this->length < this->capacity) {
		index = this->accounts.Store(this->length, account);
	}
	else {
		index = this->accounts.AppendFromRear(account);
		this->capacity++;
	}
	this->length++;
	return index;
}

// Find(Date)
void CashBook::Find(Date date, Long*(*indexes), Long *count) {

	this->accounts.LinearSearchDuplicate(&date, indexes, count, CompareDates);
}

// Find(Content)
void CashBook::Find(string content, Long*(*indexes), Long *count) {

	this->accounts.LinearSearchDuplicate(&content, indexes, count, CompareContents);
}

// Find(Date, Content)
void CashBook::Find(Date date, string content, Long*(*indexes), Long *count) {
	Long(*dates);
	Long dateCount = 0;
	this->accounts.LinearSearchDuplicate(&date, &dates, &dateCount, CompareDates);

	string comparison;
	if (*indexes != 0) {
		delete[] * indexes;
		*indexes = 0;
	}
	*count = 0;

	*indexes = new Long[this->length];
	Long i = 0;
	Long j = 0;
	while (i < dateCount) {
		comparison = this->GetAt(dates[i])->GetContent();
		if (comparison.compare(content) == 0) {
			(*indexes)[j] = dates[i];
			(*count)++;
			j++;
		}
		i++;
	}
	if (dates != 0) {
		delete[] dates;
	}
}

// GetAt
Account* CashBook::GetAt(Long index) {
	return this->accounts.GetAt(index);
}

// Correct
Long CashBook::Correct(Long index, Currency amount, string comment) {
	Account *account = this->accounts.GetAt(index);
	Date date = account->GetDate();
	string content = account->GetContent();
	Currency balance = 0.0;
	if (this->length > 1) {
		account = this->accounts.GetAt(index - 1);
		balance = account->GetBalance();
	}
	if (dynamic_cast<Income*>(account)) {
		account = new Income(date, content, amount, balance + amount, comment);
	}
	else if (dynamic_cast<Outgo*>(account)) {
		account = new Outgo(date, content, amount, balance - amount, comment);
	}
	delete this->accounts[index];

	index = this->accounts.Modify(index, account);
	balance = account->GetBalance();
	Long i = index + 1;
	while (i < this->length) {
		account = this->accounts.GetAt(i);
		date = account->GetDate();
		content = account->GetContent();
		comment = account->GetComment();
		amount = account->GetAmount();
		if (dynamic_cast<Income*>(account)) {
			account = new Income(date, content, amount, balance + amount, comment);
		}
		else if (dynamic_cast<Outgo*>(account)) {
			account = new Outgo(date, content, amount, balance - amount, comment);
		}
		delete this->accounts[i];
		this->accounts.Modify(i, account);
		balance = account->GetBalance();
		i++;
	}
	return index;
}

// Calculate
void CashBook::Calculate(Date beginning, Date end, Currency *sumOfIncome, Currency *sumOfOutgo, Currency *sumOfBalance) {
	Long(*dates) = 0;
	Long count;
	Long i;
	*sumOfIncome = 0.0;
	*sumOfOutgo = 0.0;
	*sumOfBalance = 0.0;
	while (beginning <= end) {
		count = 0;
		this->accounts.LinearSearchDuplicate(&beginning, &dates, &count, CompareDates);
		i = 0;
		while (i < count) {
			if (dynamic_cast<Income*>(this->accounts[dates[i]])) {
				(*sumOfIncome) += this->accounts[dates[i]]->GetAmount();
			}
			else if (dynamic_cast<Outgo*>(this->accounts[dates[i]])) {
				(*sumOfOutgo) += this->accounts[dates[i]]->GetAmount();
			}
			i++;
		}
		beginning++;
	}
	if (dates != 0) {
		delete[] dates;
		dates = 0;
	}

	*sumOfBalance = (*sumOfIncome) - (*sumOfOutgo);

}

// 비교함수
int CompareDates(void *one, void *other) {
	int ret;
	if ((*(static_cast<Account**>(one)))->GetDate() == *(static_cast<Date*>(other))) {
		ret = 0;
	}
	else if ((*(static_cast<Account**>(one)))->GetDate() > *(static_cast<Date*>(other))) {
		ret = 1;
	}
	else if ((*(static_cast<Account**>(one)))->GetDate() < *(static_cast<Date*>(other))) {
		ret = -1;
	}
	return ret;
}

int CompareContents(void *one, void *other) {
	return (*(static_cast<Account**>(one)))->GetContent().compare(*(static_cast<string*>(other)));
}

// main 함수
/*
#include<iostream>

int main(int argc, char *argv[]) {
CashBook cashBook;
Date date("20180501");
Long(*indexes) = 0;
Long count;
Currency sumOfIncome;
Currency sumOfOutgo;
Currency sumOfBalance;

Long index = cashBook.Record(date, "용돈", 400000.0, "아빠");
Account *accountLink = cashBook.GetAt(index);
cout << static_cast<char*>(accountLink->GetDate()) << endl;
cout << accountLink->GetContent() << endl;
cout << accountLink->GetAmount() << endl;
cout << accountLink->GetBalance() << endl;
cout << accountLink->GetComment() << endl;

index = cashBook.Record(date, "외장하드", -105000.0, "");
accountLink = cashBook.GetAt(index);
cout << static_cast<char*>(accountLink->GetDate()) << endl;
cout << accountLink->GetContent() << endl;
cout << accountLink->GetAmount() << endl;
cout << accountLink->GetBalance() << endl;
cout << accountLink->GetComment() << endl;

index = cashBook.Record(date, "립스틱", 27000.0, "");
accountLink = cashBook.GetAt(index);
cout << static_cast<char*>(accountLink->GetDate()) << endl;
cout << accountLink->GetContent() << endl;
cout << accountLink->GetAmount() << endl;
cout << accountLink->GetBalance() << endl;
cout << accountLink->GetComment() << endl;

cashBook.Find(date, &indexes, &count);
Long i = 0;
while (i < count) {
cout << static_cast<char*>(cashBook.GetAt(indexes[i])->GetDate()) << endl;
cout << cashBook.GetAt(indexes[i])->GetContent() << endl;
cout << cashBook.GetAt(indexes[i])->GetAmount() << endl;
cout << cashBook.GetAt(indexes[i])->GetBalance() << endl;
i++;
}
if (indexes != 0) {
delete[] indexes;
indexes = 0;
}

cashBook.Find("용돈", &indexes, &count);
i = 0;
while (i < count) {
cout << static_cast<char*>(cashBook.GetAt(indexes[i])->GetDate()) << endl;
cout << cashBook.GetAt(indexes[i])->GetContent() << endl;
cout << cashBook.GetAt(indexes[i])->GetAmount() << endl;
cout << cashBook.GetAt(indexes[i])->GetBalance() << endl;
i++;
}
if (indexes != 0) {
delete[] indexes;
indexes = 0;
}

cashBook.Find(date, "립스틱", &indexes, &count);
Long i = 0;
while (i < count) {
cout << static_cast<char*>(cashBook.GetAt(indexes[i])->GetDate()) << endl;
cout << cashBook.GetAt(indexes[i])->GetContent() << endl;
cout << cashBook.GetAt(indexes[i])->GetAmount() << endl;
cout << cashBook.GetAt(indexes[i])->GetBalance() << endl;
i++;
}
if (indexes != 0) {
delete[] indexes;
indexes = 0;
}

index = cashBook.Correct(0, 500000.0, "엄마");
accountLink = cashBook.GetAt(index);
cout << static_cast<char*>(accountLink->GetDate()) << endl;
cout << accountLink->GetContent() << endl;
cout << accountLink->GetAmount() << endl;
cout << accountLink->GetBalance() << endl;

cashBook.Calculate(date, date, &sumOfIncome, &sumOfOutgo, &sumOfBalance);
cout << sumOfIncome << endl;
cout << sumOfOutgo << endl;
cout << sumOfBalance << endl;

return 0;
}

*/