#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
using namespace std;


template <class T>
T Gcd(T& a, T& b){
	T temp;
	while (b != 0){
		temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

template <class T>
class polynom{
private:
	int size;
	vector <T> coef;
public:
	polynom(){
		size = 0;
		coef.resize(1, 1);
	}

	polynom(int size_){
		size = size_;
		coef.resize(size + 1);
	}
	polynom(polynom& b){
		size = b.GetSize();
		coef.resize(size+1);
		for (int i = 0; i <= size; i++){
			coef[i] = b.GetCoef(i);
		}
	}

	void CreatePolynom(int j){
		srand(j);
		for (int i = 0; i <= size; i++){
			coef[i] = (rand() % 20);
		}
		while(1){
			if (coef[size] == 0)
				coef[size] = rand() % 20;	
			else break;
		}

	}

	void PrintOnScreen(){
		for (int i = size; i >= 0; i--){
			cout << coef[i];
			if (i != 0)
				cout << "x^" << i << " + ";
		}
		cout << endl;
	}

	T GetCoef(int i){
		return coef[i];
	}
	int GetSize(){
		return size;
	}
	int SetSize(int value){
		size = value;
	}

	T SetCoef(int i, T value){
		return coef[i] = value;
	}

	polynom<T> operator =(polynom<T>& b){
		size = b.GetSize();
		coef.resize(size + 1);
		for (int i = 0; i <= size; i++){
			coef[i] = b.GetCoef(i);
		}
		return *this;
	}
	bool operator !=(int a){
		if (size <= 0) return false;
		for (int i = size; i >= 0; i--){
			if(coef[i] != 0) return true;
		}
		return true;
	}

	bool operator ==(polynom<T>& b){
		if(size != b.size) return false;
		else{
			for (int i = size; i >=0; i--){
				if (coef[i] != b.coef[i]) return false;
			}
		}
		return true;
	}

	polynom<T> operator *(T a){
		for (int i = size; i >= 0; i--){
			coef[i] *= a; 
		}
	}

	polynom<T> operator *(polynom<T>& a){
		polynom<T> result(GetSize() + a.GetSize());
		for (int  i = size; i >= 0; i--){
			for (int j = a.size; j >= 0; j--){
				result.coef[i + j] += a.coef[j] * coef[i];
			}
		}
		return result;
	}

	polynom<T> operator %(polynom<T>& b){
		polynom<T> result(*this);
		double temp;
		for (int i = size; i >= b.GetSize(); i--){
				temp = coef[i] / b.GetCoef(b.GetSize());
				for (int j = b.GetSize(); j >= 0; j--){
					//cout << coef[i - (b.GetSize() - j)] << " "<<(temp * b.GetCoef(j)) << endl;
					coef[i - (b.GetSize() - j)] = coef[i - (b.GetSize() - j)] - (temp * b.GetCoef(j));
				}
		}
		//(*this).PrintOnScreen();
		int k = size;
		while (k >= 0 && abs(coef[k]) <= 0.00000001){
			//cout << "t";
			coef.erase(--coef.end());
			k--;
		}
		size = k;
		return *this;
	}
};

int ReminderRecursive(int a, int b){
	if (a - b >= b){
		a = ReminderRecursive(a, b + b);
		if (a < b) return a;
	}
	return a - b;
}

int ReminderNonnegative(int a, int b){
	if (a < b) return a;
	return ReminderRecursive(a, b);
}

int IntergerGcd(int a, int b){
	while (true){
		if (b == 0) return a;
		a = ReminderNonnegative(a, b);
		if (a == 0) return b;
		b = ReminderNonnegative(b, a);
	}
}

void IntergerTest(){
	int a, b;
	cout << "Enter a and b: ";
	cin >> a >> b;
	cout << "gdc(" << a << ", " << b << ") = ";
	int answer = Gcd<int>(a, b);
	cout << answer << endl;
	assert(answer == IntergerGcd(a, b));
}

void PolynomExample(){
	polynom<double> P(3);
	polynom<double> Q(2);
	
	P.CreatePolynom(4);
	Q.CreatePolynom(6);
	
	cout << "p(x) = ";
	P.PrintOnScreen();
	cout << "q(x) = ";
	Q.PrintOnScreen();
	
	polynom<double> A;
	A = Gcd<polynom<double>> (P, Q);
	cout << "gcd(p, q) = ";
	A.PrintOnScreen();
}

void PolynomTest(){
	polynom<double> CommonPolynom;
	polynom<double> A(1);
	for (int i = 1; i <= 5; i++){
		A.SetCoef(0, -i);
		A.SetCoef(1, 1);
		CommonPolynom = CommonPolynom * A;
	}
	CommonPolynom.PrintOnScreen();

	polynom<double> B(CommonPolynom);
	for(int i = 6; i <= 7; i++){
		A.SetCoef(0, -i);
		B = B * A;
	}
	B.PrintOnScreen();


	polynom<double> C(CommonPolynom);
	for(int i = 8; i <= 10; i++){
		A.SetCoef(0, -i);
		C = C * A;
	}
	C.PrintOnScreen();

	Gcd<polynom<double>>(B, C).PrintOnScreen();
	//assert(Gcd<polynom<double>>(B, C) == CommonPolynom);
}

int main(){
	IntergerTest();
	PolynomExample();
	PolynomTest();

	return 0;
}