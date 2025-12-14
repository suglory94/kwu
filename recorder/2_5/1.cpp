#include <iostream>

using namespace std;

struct Node {
	int data;
	Node* next;
};

class LinkedList {
private:
	Node* head;
	Node* tail;

public:
	LinkedList() {
		head = NULL;
		tail = NULL;
	}

	void push_front(int data) {
		Node* newNode = new Node;
		newNode->data = data;
		newNode->next = NULL;

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			newNode->next = head;
			head = newNode;
		}
	}

	void push_back(int data) {
		Node* newNode = new Node;
		newNode->data = data;
		newNode->next = NULL;

		if (head == NULL)
		{
			head = newNode;
			tail = newNode;
		}
		else
		{
			tail->next = newNode;
			tail = newNode;
		}
	}

	void pop_front() {
		if (head == NULL)
			return;
		else
		{
			Node* temp = head;
			head = head->next;

			if (head == NULL)
				tail = NULL;

			delete temp;
		}
	}

	void pop_back() {
		if (head == NULL)
			return;

		if (head == tail)
		{
			delete head;
			head = NULL;
			tail = NULL;
		}
		else
		{
			Node* current = head;

			while (current->next != tail)
				current = current->next;
			
			delete tail;
			tail = current;
			tail->next = NULL;
		}
	}

	void remove(int value) {
		if (head == NULL)
			return;
		
		if (head->data == value)
		{
			Node* temp = head;
			head = head->next;
			delete temp;
		}
		else
		{
			Node* current = head;

			while (current->next != NULL)
			{
				if (current->next->data == value)
				{
					Node* temp = current->next;
					current->next = current->next->next;

					if (temp == tail)
						tail = current;

					delete temp;
				}
				else
					current = current->next;
			}
		}
	}

	Node* find(int value) {
		Node* current = head;

		while (current != NULL)
		{
			if (current->data == value)
				return current;
			current = current->next;
		}

		cout << value << " 데이터가 없습니다" << endl;
		return NULL;
	}

	void display() {
		Node* current = head;

		while (current != NULL)
		{
			cout << current->data << " ";
			current = current->next;
		}
		cout << endl;
	}

	~LinkedList() {
		Node* current = head;
		while (current != NULL)
		{
			Node* temp = current;
			current = current->next;
			delete temp;
		}
	}
};

int main() {
	LinkedList linkedList = LinkedList();
	linkedList.display();
	linkedList.push_front(7);
	linkedList.display();
	linkedList.push_front(1);
	linkedList.display();
	linkedList.push_front(3);
	linkedList.display();

	linkedList.push_back(2);
	linkedList.display();
	linkedList.push_back(4);
	linkedList.display();

	linkedList.pop_front();
	linkedList.display();

	linkedList.pop_back();
	linkedList.display();

	linkedList.remove(7);
	linkedList.display();

	Node* findNode = linkedList.find(0);
	if (findNode != NULL)
		cout << findNode->data;

	return 0;
}