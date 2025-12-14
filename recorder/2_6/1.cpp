#include <iostream>

using namespace std;

class Term {
private:
	int m_Coefficient;
	int m_Exponent;
	Term* m_pNext;

public:
	Term(int coeff, int exp) : m_Coefficient(coeff), m_Exponent(exp), m_pNext(nullptr) {}
	friend class Polynomial;
};

class Polynomial {
private:
	Term* m_pHead;

public:
	Polynomial( ) : m_pHead(nullptr) {}
	
	// 소멸자...
	/*~Polynomial()
	{
		Term* current = m_pHead;

		while (current)
		{
			Term* temp = current;
			current = current->m_pNext;
			delete temp;
		}
	}*/

	void addTerm(int coeff, int exp) {
		Term* newTerm = new Term(coeff, exp);

		// 다항식의 선행항은 차수가 가장 높은 항
		
		// 입력값이 차수가 가장 높은 항일 때
		if (!m_pHead || m_pHead->m_Exponent < exp)
		{
			newTerm->m_pNext = m_pHead;
			m_pHead = newTerm;
			return;
		}

		// 입력값의 노드 위치
		Term* current = m_pHead;
		Term* prev = nullptr;
		while (current && current->m_Exponent > exp)
		{
			prev = current;
			current = current->m_pNext;
		}

		// 입력값의 차수와 동일한 항이 존재할 때
		if (current && current->m_Exponent == exp)
		{
			current->m_Coefficient += coeff;
			delete newTerm;

			if (current->m_Coefficient == 0)
			{
				if (prev) prev->m_pNext = current->m_pNext;
				else m_pHead = current->m_pNext;
				delete current;
			}
		}
		else
		{
			newTerm->m_pNext = current;
			if (prev) prev->m_pNext = newTerm;
		}
	}

	void printPolynomial() {
		Term* current = m_pHead;

		while (current) {
			if (current->m_Exponent != 0)
				cout << current->m_Coefficient << "x^" << current->m_Exponent;
			else
				cout << current->m_Coefficient;
			if (current->m_pNext)
				cout << " + ";
			current = current->m_pNext;
		}
		cout << endl;
	}

	Polynomial operator+(const Polynomial& poly) {
		Polynomial result;
		Term* poly1 = this->m_pHead; // m_pHead도 가능
		Term* poly2 = poly.m_pHead;

		while (poly1)
		{
			result.addTerm(poly1->m_Coefficient, poly1->m_Exponent);
			poly1 = poly1->m_pNext;
		}

		while (poly2)
		{
			result.addTerm(poly2->m_Coefficient, poly2->m_Exponent);
			poly2 = poly2->m_pNext;
		}

		return result;
	}

	Polynomial operator-(const Polynomial& poly) {
		Polynomial result;
		Term* poly1 = this->m_pHead; // m_pHead도 가능
		Term* poly2 = poly.m_pHead;

		while (poly1)
		{
			result.addTerm(poly1->m_Coefficient, poly1->m_Exponent);
			poly1 = poly1->m_pNext;
		}

		while (poly2)
		{
			result.addTerm(-poly2->m_Coefficient, poly2->m_Exponent);
			poly2 = poly2->m_pNext;
		}

		return result;
	}

	Polynomial differentiation() {
		Polynomial result;
		Term* current = m_pHead;

		while (current->m_Exponent != 0)
		{
			result.addTerm(current->m_Coefficient * current->m_Exponent, current->m_Exponent - 1);
			current = current->m_pNext;
		}

		return result;
	}

	int calculate(int x) {
		int result = 0;
		Term* current = m_pHead;

		while (current)
		{
			int value = current->m_Coefficient;
			for (int i = 0; i < current->m_Exponent; i++)
				value *= x;

			result += value;
			current = current->m_pNext;
		}

		return result;
	}

};

int main() {

	Polynomial poly1, poly2, resultOper, resultDiff, resultCal;

	poly1.addTerm(2, 3);
	poly1.addTerm(-4, 2);
	poly1.addTerm(-7, 0);

	poly2.addTerm(-3, 3);
	poly2.addTerm(1, 2);
	poly2.addTerm(6, 1);
	poly2.addTerm(4, 0);

	cout << "Polynomial 1 : ";
	poly1.printPolynomial();
	cout << "Polynomial 2 : ";
	poly2.printPolynomial();

	resultOper = poly1 + poly2;
	cout << "Polynomial operator result : ";
	resultOper.printPolynomial();

	resultDiff = poly1.differentiation();
	cout << "Polynomial derivative result : ";
	resultDiff.printPolynomial();

	cout << "Calculate result : " << poly1.calculate(7) << endl;

	return 0;
}