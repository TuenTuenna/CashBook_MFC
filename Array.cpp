// Array.cpp

/* �ó�����:
1. 5ĭ ¥�� �迭�� �����.
2. ù��° ĭ�� 5�� �ִ´�.
3. �ι�° ĭ�� 3�� �ִ´�.
4. ǥ�� Clear�Ѵ�.
5. ù��° ĭ�� 1�� �ִ´�.
6. �� ���� ĭ�� 8�� �ִ´�.
7. �� ���� ĭ�� 3�� �ִ´�.
8. ù��° ĭ�� 2�� �ٲ۴�.
9. 2�� �����˻��Ͽ� ã�´�.
10. �� ���� ĭ�� 3�� �ִ´�.
11. 3�� �����˻��Ͽ� ã�´�.
12. ���������Ѵ�.
13. 4ĭ¥�� �迭�� �����
14. ù��° ĭ�� 5�� �ִ´�.
15. �ι�° ĭ�� 6�� �ִ´�.
16. ����° ĭ�� 1�� �ִ´�.
17. �׹�° ĭ�� 2�� �ִ´�.
18. ���������Ѵ�.
19. 2�� �̺а˻��Ѵ�.
20. �׹�° ĭ�� 5�� �ٲ۴�.
21. 5�� �̺а˻��Ͽ� ã�´�.
22. 3ĭ¥�� �迭�� �����.
23. ù��° ĭ�� 3�� �ִ´�.
24. �ι�° ĭ�� 6�� �ִ´�.
25. ����° ĭ�� 4�� �ִ´�.
26. ���������Ѵ�.
27. one�� other �迭�� ��ģ��.
28. �� ���� ĭ�� �����.
29. �� ���� ĭ�� �����.
30. 3��° ĭ�� �����.
31. other ǥ�� �����.
32. one ǥ�� �����.
33. ǥ�� �����.
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

	cout << "���̳ʸ���ġ ��" << endl;

	index = one.Modify(3, 5);
	cout << one[index] << endl;
	key = 5;

	cout << "���̳ʸ���ġ ���ý���" << endl;
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
		cout << "���������ϴ�." << endl;
	}

	index = array.DeleteFromRear();
	if (index == -1) {
		cout << "���������ϴ�." << endl;
	}

	index = array.Delete(3);
	if (index == -1) {
		cout << "���������ϴ�." << endl;
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