// Array.cpp

/* 시나리오:
1. 5칸 짜리 배열을 만든다.
2. 첫번째 칸에 5를 넣는다.
3. 두번째 칸에 3을 넣는다.
4. 표를 Clear한다.
5. 첫번째 칸에 1을 넣는다.
6. 맨 앞의 칸에 8을 넣는다.
7. 맨 뒤의 칸에 3을 넣는다.
8. 첫번째 칸을 2로 바꾼다.
9. 2를 순차검색하여 찾는다.
10. 맨 앞의 칸에 3을 넣는다.
11. 3을 순차검색하여 찾는다.
12. 버블정렬한다.
13. 4칸짜리 배열을 만든다
14. 첫번째 칸에 5를 넣는다.
15. 두번째 칸에 6을 넣는다.
16. 세번째 칸에 1을 넣는다.
17. 네번째 칸에 2를 넣는다.
18. 선택정렬한다.
19. 2를 이분검색한다.
20. 네번째 칸을 5로 바꾼다.
21. 5를 이분검색하여 찾는다.
22. 3칸짜리 배열을 만든다.
23. 첫번째 칸에 3을 넣는다.
24. 두번째 칸에 6을 넣는다.
25. 세번째 칸에 4를 넣는다.
26. 삽입정렬한다.
27. one과 other 배열을 합친다.
28. 맨 앞의 칸을 지운다.
29. 맨 뒤의 칸을 지운다.
30. 3번째 칸을 지운다.
31. other 표를 지운다.
32. one 표를 지운다.
33. 표를 지운다.
*/
#if 0
#include"Array.h"
#include<iostream>
using namespace std;
int CompareIntegers(void *one, void *other);

int main(int argc, char* argv[]) {
	Long(*indexes);
	Long count;
	Long i = 0;
	Array<int> array(5);
	Long index = array.Store(0, 5);
	cout << array[index] << endl;

	index = array.Store(1, 3);
	cout << array[index] << endl;
	array.Clear();

	index = array.Insert(0, 1);
	cout << array[index] << endl;

	index = array.AppendFromFront(8);
	cout << array[index] << endl;

	index = array.AppendFromRear(3);
	cout << array[index] << endl;

	index = array.Modify(0, 2);
	cout << array[index] << endl;

	int key = 2;
	index = array.LinearSearchUnique(&key, CompareIntegers);
	cout << array[index] << endl;

	index = array.AppendFromFront(3);
	cout << array[index] << endl;

	key = 3;
	array.LinearSearchDuplicate(&key, &indexes, &count, CompareIntegers);
	while (i < count) {
		cout << array[indexes[i]] << endl;
		i++;
	}
	if (indexes != 0) {
		delete[] indexes;
	}
	array.BubbleSort(CompareIntegers);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << endl;
		i++;
	}
	Array<int> one(4);
	index = one.Store(0, 5);
	cout << one[index] << endl;
	index = one.Store(1, 6);
	cout << one[index] << endl;
	index = one.Store(2, 1);
	cout << one[index] << endl;
	index = one.Store(3, 2);
	cout << one[index] << endl;
	one.SelectionSort(CompareIntegers);
	i = 0;
	while (i < one.GetLength()) {
		cout << one[i] << endl;
		i++;
	}
	key = 2;
	index = one.BinarySearchUnique(&key, CompareIntegers);
	cout << one[index] << endl;

	cout << "바이너리서치 끝" << endl;

	index = one.Modify(3, 5);
	cout << one[index] << endl;
	key = 5;

	cout << "바이너리서치 듀플시작" << endl;
	one.BinarySearchDuplicate(&key, &indexes, &count, CompareIntegers);
	i = 0;
	while (i < count) {
		cout << one[indexes[i]] << endl;
		i++;
	}


	if (indexes != 0) {
		delete[] indexes;
	}
	cout << "ohter" << endl;
	Array<int> other(3);
	index = other.Store(0, 3);
	cout << other[index] << endl;
	index = other.Store(1, 6);
	cout << other[index] << endl;
	index = other.Store(2, 4);
	cout << other[index] << endl;


	cout << "InerstionSort" << endl;
	other.InsertionSort(CompareIntegers);
	i = 0;
	while (i < other.GetLength()) {
		cout << other[i] << endl;
		i++;
	}
	cout << "Merge" << endl;
	array.Merge(one, other, CompareIntegers);
	i = 0;
	while (i < array.GetLength()) {
		cout << array[i] << endl;
		i++;
	}

	index = array.DeleteFromFront();
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.DeleteFromRear();
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}

	index = array.Delete(3);
	if (index == -1) {
		cout << "지워졌습니다." << endl;
	}


	return 0;
}

int CompareIntegers(void *one, void *other) {
	int ret;
	if (*(static_cast<int*>(one)) == *(static_cast<int*>(other))) {
		ret = 0;
	}
	else if (*(static_cast<int*>(one)) > *(static_cast<int*>(other))) {
		ret = 1;
	}
	else if (*(static_cast<int*>(one)) < *(static_cast<int*>(other))) {
		ret = -1;
	}
	return ret;
}
#endif