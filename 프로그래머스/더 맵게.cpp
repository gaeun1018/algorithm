#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

const int HEAP_SIZE = 1000000;

int heap[HEAP_SIZE];
int heap_count = 0;

void swap(int* a, int* b) {
	int tmp = *a; *a = *b; *b = tmp;
}

int first() {
	return heap[1];
}

int size() {
	return heap_count;
}

void push(int data) {
	// ���� ���� ������ �߰�
	heap[++heap_count] = data;

	//  child�� parent�� ���ϸ鼭 �ϳ��� ���� �ø��� �κ�
	int child = heap_count;
	int parent = child / 2;
	while (child > 1 && heap[parent] > heap[child]) {
		swap(&heap[parent], &heap[child]);
		child = parent;
		parent = child / 2;
	}
}

int pop() {
	// ���� ù��° ���Ҹ� ��ȯ
	int result = heap[1];

	// ù��° ���Ҹ� ���� ���� �� ���ҿ� �ٲٰ�
	// ���� ���� ���� ���� ���� �����̴� heap_count�� �����ش�.
	swap(&heap[1], &heap[heap_count--]);

	// child�� parent�� ���ϸ鼭 �˸��� ��ġ�� �ϳ��� ������ �κ�
	int parent = 1;
	int child = parent * 2;
	if (child + 1 <= heap_count) {
		child = (heap[child] < heap[child + 1]) ? child : child + 1;
	}

	while (child <= heap_count && heap[parent] > heap[child]) {
		swap(&heap[parent], &heap[child]);

		parent = child;
		child = child * 2;
		if (child + 1 <= heap_count) {
			child = (heap[child] < heap[child + 1]) ? child : child + 1;
		}
	}

	return result;
}

int solution(vector<int> scoville, int K) {
	int answer = 0;
	for (int i = 0; i < scoville.size(); i++) {
		push(scoville[i]);
	}
	if ((heap[0] == 0 and heap[1] == 0))
		return -1;
	while (first() < K) {
		if (size() == 1)
			return -1;
		push(pop() + pop() * 2);
		answer++;
	}
	return answer;
}