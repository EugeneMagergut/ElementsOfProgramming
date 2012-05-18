#include <iostream>
#include <vector>
#include <assert.h>
#include <math.h>
#include <algorithm>
using namespace std;

template <class T>
T Gcd(const T& a, const T& b){
	T temp;
	T a2 = a;
	T b2 = b;
	while (b2 != 0){
		temp = b2;
		b2 = a2 % b2;
		a2 = temp;
	}
	return a2;
}

template <class T>
class polynom
{
private:
	int size;
	vector <T> coef;
public:
	polynom(): size(0), coef(1, 1) {}

	polynom(int size_): size(size_), coef(size_ + 1) {}

	polynom(const polynom<T>& b){
		size = b.GetSize();
		coef.resize(size+1);
		for (int i = 0; i <= size; i++){
			coef[i] = b.GetCoef(i);
		}
	}

	T GetCoef(int index) const{
		return coef[index];
	}
	int GetSize() const{
		return size;
	}
	void SetSize(int value){
		size = value;
	}

	void SetCoef(int index, T value){
		coef[index] = value;
	}

	polynom<T> operator =(const polynom<T>& b){
		size = b.GetSize();
		coef.resize(size + 1);
		for (int i = 0; i <= size; i++){
			coef[i] = b.GetCoef(i);
		}
		return *this;
	}
	bool operator !=(int a){
		for (int i = size; i >= 0; i--){
			if(coef[i] != 0) return true;
		}
		return false;
	}

	bool operator ==(const polynom<T>& b){
		if(size != b.size) return false;
		else{
			for (int i = size; i >=0; i--){
				if ((coef[i] - b.coef[i]) > Pogreshnost())
					return false;
			}
		}
		return true;
	}

	T Pogreshnost(){
		if(typeid(T).name() == typeid(double).name())
			return 0.00001;
	}

	polynom<T> operator *(T a){
		for (int i = size; i >= 0; i--){
			coef[i] *= a; 
		}
	}

	polynom<T> operator *(const polynom<T>& a){
		polynom<T> result(GetSize() + a.GetSize());
		for (int i = size; i >= 0; i--){
			for (int j = a.size; j >= 0; j--){
				result.coef[i + j] += a.coef[j] * coef[i];
			}
		}
		return result;
	}

	polynom<T> operator %(const polynom<T>& b){
		T temp;
		for (int i = size; i >= b.GetSize(); i--){
				temp = coef[i] / b.GetCoef(b.GetSize());
				for (int j = b.GetSize(); j >= 0; j--){
					coef[i - (b.GetSize() - j)] = coef[i - (b.GetSize() - j)] - (temp * b.GetCoef(j));
				}
		}
		int k = size;
		while (k >= 0 && abs(coef[k]) <= Pogreshnost()){
			coef.erase(--coef.end());
			k--;
		}
		size = k;
		
		if (size > 0 || (size == 0 && coef[0] != 0)){
			double norm = coef[size];
			for (int i = size; i >= 0; i--){
				coef[i] /= norm;
			}
		}

		return *this;
	}
};

ostream& operator << (ostream& out, const polynom<double>& P){
	for (int i = P.GetSize(); i >= 0; i--){
		out << P.GetCoef(i);
		if (i != 0)
			out << "x^" << i << " + ";
	}
	out << endl << endl;
	return  out;
}

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


void createCoefs(polynom<double>& P){
	int size = P.GetSize();
	for (int i = 0; i <= size; i++){
		P.SetCoef(i, (rand() % 20));
	}
	while(1){
		if (P.GetCoef(size) == 0)
			P.SetCoef(size, (rand() % 20));
		else break;
	}
}

void IntergerTest(){
	int a, b, answer;
	srand(5);
	for (int i = 0; i < 100; i++){
		a = rand() % 1000;
		b = rand() % 1000;
		answer = Gcd<int>(a, b);
		assert(answer == IntergerGcd(a, b));
	}
	cout << "IntegerTest is done" << endl;
}

void PolynomExample(){
	polynom<double> P(rand()%10);
	polynom<double> Q(rand()%10);
	
	createCoefs(P);
	createCoefs(Q);

	cout << "p(x) = " << P;
	cout << "q(x) = " << Q;

	polynom<double> A;
	A = Gcd<polynom<double>> (P, Q);
	cout << "gcd(p,q) = " << A;
}


double generatingNumber(vector<double>& arrayOfNumber){
	double number;
	do{
		number = rand()%12;
	}while(arrayOfNumber.end() != find(arrayOfNumber.begin(), arrayOfNumber.end(), number));
	return number;
}

void CurrentPolynomTest(){
	polynom<double> A(1), CommonPolynom;
	A.SetCoef(1, 1);
	
	int count;
	int CommonPolynomSize = rand()%5;
	double number;
	vector<double> arrayOfNumber;
	
	for (int i = 1; i <= CommonPolynomSize; i++){
		number = generatingNumber(arrayOfNumber);
		arrayOfNumber.push_back(number);
		A.SetCoef(0, -number);
		CommonPolynom = CommonPolynom * A;
	}

	count = rand()%5;
	polynom<double> B(CommonPolynom);
	while(count > 0){
		number = generatingNumber(arrayOfNumber);
		arrayOfNumber.push_back(number);
		A.SetCoef(0, -number);
		B = B * A;
		count--;
	}

	count = rand()%5;
	polynom<double> C(CommonPolynom);
	while(count > 0){
		number = generatingNumber(arrayOfNumber);
		arrayOfNumber.push_back(number);
		A.SetCoef(0, -number);
		C = C * A;
		count--;
	}
	
	polynom<double> resultPolynom(Gcd<polynom<double>>(B, C));
	assert(resultPolynom == CommonPolynom);
}


void PolynomTest(){
	for (int i = 0; i < 1000; i++){
		CurrentPolynomTest();
	}
	cout << "PolinomTest is done" << endl;
}


int main(){
	PolynomExample();
	PolynomTest();
	IntergerTest();
	
	


	int l;
	cin >> l;
	return 0;
}