#include <iostream>

using namespace std;

class Stack {
private:
	int* arr;
	int capacity;
	int top;

public:
	Stack(int size) {
		capacity = size;
		arr = new int[capacity];
		top = -1;
	}

	~Stack() {
		delete[] arr;
	}

	void push(int value) {
		if (isFull() == 1)
			cout << "Stack is full\n";
		else
			arr[++top] = value;
	}

	int pop() {
		if (isEmpty() == 1)
			cout << "Stack is empty\n";
		else
			return arr[top--];
	}

	int peek() const {
		if (isEmpty() == 1)
			cout << "Stack is empty\n";
		else
			return arr[top];
	}

	bool isEmpty() const {
		if (top == -1)
			return 1;
		else
			return 0;
	}

	bool isFull() const {
		if (top == capacity - 1)
			return 1;
		else
			return 0;
	}
};

int main() {
	int size;
	cout << "Stack의 크기 : ";
	cin >> size;
	Stack myStack(size);

	while (1)
	{
		int num;
		cout << "[0] end, [1] push, [2] pop, [3] peek" << endl;
		cin >> num;

		if (num == 0)
			break;
		else if (num == 1)
		{
			int value;
			cout << "Push할 값 : ";
			cin >> value;
			myStack.push(value);
		}
		else if (num == 2)
			cout << myStack.pop() << endl;
		else if (num == 3)
			cout << myStack.peek() << endl;
		else
			cout << "다시 입력해주세요";
	}

	return 0;
}