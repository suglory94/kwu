#include <iostream>

using namespace std;

template <typename T>
class Stack;

template <typename T>
class Node {
private:
	Node* m_Next;
	T m_Data;

public:
	Node(T data) : m_Data(data), m_Next(nullptr) {}
	~Node() {}

	friend class Stack<T>;
};

template <typename T>
class Stack {
private:
	Node<T>* m_Top;

public:
	Stack() : m_Top(nullptr) {}
	~Stack() {
		while (!isEmpty())
			pop();
	}

	void push(T data) {
		Node<T>* newNode = new Node<T>(data);
		newNode->m_Next = m_Top;
		m_Top = newNode;
	}

	T pop() {
		if (isEmpty())
		{
			cout << "스택이 비어있습니다." << endl;
			return T();
		}

		Node<T>* temp = m_Top;
		T data = temp->m_Data;
		m_Top = m_Top->m_Next;
		
		delete temp;

		return data;
	}

	bool isEmpty() {
		return m_Top == nullptr;
	}

	T top() {
		if (isEmpty())
		{
			cout << "스택이 비어있습니다." << endl;
			return T();
		}

		return m_Top->m_Data;
	}

	void print() {
		if (isEmpty())
		{
			cout << "스택이 비어있습니다." << endl;
			return;
		}

		Node<T>* current = m_Top;

		while (current)
		{
			cout << current->m_Data << " ";
			current = current->m_Next;
		}
		cout << endl;
	}
};

int main() {
	Stack<int> intStack;

	intStack.push(7);
	intStack.push(13);
	
	cout << "Stack: ";
	intStack.print();

	cout << "Top : " << intStack.top() << endl;

	while (!intStack.isEmpty())
		cout << "Pop : " << intStack.pop() << endl;
	
	cout << "Empty : " << (intStack.isEmpty() ? "Yes" : "No") << endl;

	cout << endl;

	Stack<char> charStack;

	charStack.push('K');
	charStack.push('H');

	cout << "Stack: ";
	charStack.print();

	cout << "Top : " << charStack.top() << endl;

	while (!charStack.isEmpty())
		cout << "Pop : " << charStack.pop() << endl;

	cout << "Empty : " << (charStack.isEmpty() ? "Yes" : "No") << endl;

	return 0;
}