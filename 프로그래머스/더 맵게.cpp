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
	// 힙의 끝에 데이터 추가
	heap[++heap_count] = data;

	//  child를 parent와 비교하면서 하나씩 위로 올리는 부분
	int child = heap_count;
	int parent = child / 2;
	while (child > 1 && heap[parent] > heap[child]) {
		swap(&heap[parent], &heap[child]);
		child = parent;
		parent = child / 2;
	}
}

int pop() {
	// 힙의 첫번째 원소를 반환
	int result = heap[1];

	// 첫번째 원소를 힙의 가장 끝 원소와 바꾸고
	// 가장 끝은 이제 쓰지 않을 예정이니 heap_count를 내려준다.
	swap(&heap[1], &heap[heap_count--]);

	// child를 parent와 비교하면서 알맞은 위치로 하나씩 내리는 부분
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