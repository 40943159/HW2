#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;
class Polynomial
{
	friend ostream& operator<<(ostream& os, const Polynomial &p);
	friend istream& operator>>(istream& is, Polynomial &p);
	class Term {
		friend Polynomial;
		friend ostream& operator<<(ostream& os, const Polynomial &p);
	private:
		float coef;
		int exp;
	};
private:
	Term *termArray;
	int capacity;
	int terms;
public:
	Polynomial();
	Polynomial Add(Polynomial poly);
	Polynomial Mult(Polynomial poly);
	float Eval(float f);
	void Setup(int a, int b,float c[],int d[]);
};

Polynomial::Polynomial()
{
	capacity = 0;
	terms = 0;
}
Polynomial Polynomial::Add(Polynomial poly)
{
	Polynomial result;
	int maxDegree = max(this->capacity - 1, poly.capacity - 1);
	result.Setup(maxDegree + 1, 0, nullptr, nullptr);

	for (int i = 0; i <= maxDegree; i++)
	{
		result.termArray[i].coef = (i < this->capacity) ? this->termArray[i].coef : 0;
		result.termArray[i].coef += (i < poly.capacity) ? poly.termArray[i].coef : 0;
		result.termArray[i].exp = i;
	}

	return result;
}
Polynomial Polynomial::Mult(Polynomial poly)
{
	Polynomial result;
	int maxCapacity = this->capacity + poly.capacity - 1;
	result.Setup(maxCapacity, 0, nullptr, nullptr);

	for (int i = 0; i < this->capacity; i++)
	{
		for (int j = 0; j < poly.capacity; j++)
		{
			int newExp = this->termArray[i].exp + poly.termArray[j].exp;
			float newCoef = this->termArray[i].coef * poly.termArray[j].coef;
			result.termArray[newExp].coef += newCoef;
			result.termArray[newExp].exp = newExp;
		}
	}

	return result;
}
float Polynomial::Eval(float x)
{
	float result = 0;

	for (int i = 0; i < this->capacity; i++)
	{
		result += this->termArray[i].coef * pow(x, this->termArray[i].exp);
	}
	return result;
}

void Polynomial::Setup(int a,int b,float c[],int d[]) {
	this->capacity = a + 1;
	this->terms = b;
	this->termArray = new Term[capacity];
	for (int i = 0; i < capacity; i++) {
		termArray[i].coef = 0;
		termArray[i].exp = i;
	}
	for (int i = 0; i < terms; i++) {
		termArray[d[i]].coef = c[i];
		termArray[d[i]].exp = d[i];
	}
	/*
	for (int i = 0; i < capacity; i++) {
		cout << termArray[i].coef <<" "<< termArray[i].exp << ' '<< i << endl;
	}
	*/
}
std::istream& operator>>(istream& is, Polynomial &p) {
	char ch; bool negative = false;
	float coef = 0, f_coef[100] = { 0 };
	int max = 0, exp = 0, i_exp[100] = { 0 }, count = 0;

	ch = getchar();
	while (ch != '\n') {
		coef = 0; negative = false;
		while ((ch >= '0' && ch <= '9') || ch == '.' || ch == '+' || ch == '-' || ch == ' ') {
			if (ch == ' ' or ch == '+') {
				ch = getchar();
				continue;
			}
			if (ch == '-') {
				negative = true;
				ch = getchar();
			}
			while (ch != '.' && (ch >= '0' && ch <= '9')) {
				coef = coef * 10 + ch - '0';
				ch = getchar();
			}
			if(ch == '.')ch = getchar();
			int c = 1;
			while ((ch >= '0' && ch <= '9')) {
				coef = coef + pow(0.1, c) * (ch - '0');
				ch = getchar();
				c++;
			}
			if (negative) coef = coef*(-1);
			f_coef[count] = coef;
		}
		
		if (coef == 0) f_coef[count] = 1;
		if (ch == 'x' || ch == 'X') {
			ch = getchar();
			while (ch == ' ') {
				ch = getchar();
			}
			if (ch == '^') {
				exp = 0;
				while ((ch >= '0' && ch <= '9') || ch == '^' || ch == ' ') {
					if (ch == '^' || ch == ' ') {
						ch = getchar();
						continue;			
					}
					exp = exp * 10 + ch - '0';
					ch = getchar();
				}				
				i_exp[count] = exp;
				if (max < exp) max = exp;
			} else {
				i_exp[count] = 1;
				if (max < 1) max = 1;
			}
		} else {
			i_exp[count] = 0;
		}
		count++;
	}
	for (int i = 0; i < count; i++) {
		for (int j = i + 1; j < count; j++) {
			if (i_exp[i] < i_exp[j]) {
				swap(i_exp[i], i_exp[j]);
				swap(f_coef[i], f_coef[j]);
			}
		}
	}
	/*
	for (int i = 0; i < count; i++) {
		cout << f_coef[i] << " " << i_exp[i]<<endl;
	}
	*/
	p.Setup(max, count, f_coef, i_exp);
	return is;
}
std::ostream& operator<<(ostream& os, const Polynomial &p) {
	bool f = true;
	for (int i = p.capacity - 1; i > 1; i--) {
		if (p.termArray[i].coef != 0) {
			if (f) {
				if (p.termArray[i].coef == 1)
					cout << "x ^ " << p.termArray[i].exp;
				else if (p.termArray[i].coef == -1)
					cout << "-x ^ " << p.termArray[i].exp;
				else
					cout << p.termArray[i].coef << " x ^ " << p.termArray[i].exp;
			}
			else {
				if (p.termArray[i].coef == 1)
					cout << " + x ^ " << p.termArray[i].exp;
				else if (p.termArray[i].coef == -1)
					cout << " - x ^ " << p.termArray[i].exp;
				else if (p.termArray[i].coef > 0)
					cout << " + " << p.termArray[i].coef << " x ^ " << p.termArray[i].exp;
				else
					cout << " - " << p.termArray[i].coef * (-1) << " x ^ " << p.termArray[i].exp;
			}
			f = false;
		}
	}
	if (p.capacity - 1 > 0) {
		if (p.termArray[1].coef == 1) {
			if (f) cout << "x";
			else cout << " + x";
			f = false;
		}
		else if (p.termArray[1].coef == -1) {
			if (f) cout << "-x";
			else cout << " - x";
			f = false;
		}
		else if (p.termArray[1].coef > 0) {
			if (f) cout << p.termArray[1].coef << " x";
			else cout << " + " << p.termArray[1].coef << " x";
			f = false;
		}
		else if (p.termArray[1].coef < 0) {
			cout << " - " << p.termArray[1].coef * (-1) << " x";
			f = false;
		}
	}
	if (p.capacity - 1 >= 0) {
		if (p.termArray[0].coef > 0) {
			if (f) cout << p.termArray[0].coef;
			else cout << " + " << p.termArray[0].coef;
			f = false;
		}
		else if (p.termArray[0].coef < 0) {
			cout << " - " << p.termArray[0].coef * (-1);
			f = false;
		}
	}
	return os;
}

int main() {
	Polynomial a, b;
	float x;
	cout << "請輸入A(x)：";
	cin >> a;
	cout << "請輸入B(x)：";
	cin >> b;
	cout << a << endl;
	cout << b << endl;
	cout << "A(x) + B(x) = " << a.Add(b) << endl;
	cout << "A(x) * B(x) = " << a.Mult(b) << endl;
	cout << "請輸入x的值：";
	cin >> x;
	cout << "A(x) = " << a.Eval(x) << endl;
	cout << "B(x) = " << b.Eval(x) << endl;
	system("pause");
	return 0;
}