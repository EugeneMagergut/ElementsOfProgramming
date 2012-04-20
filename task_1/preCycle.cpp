#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;

template <class T>
class functor{
private:
	int mod;
public:
	functor(T value){
		mod = value;
	}
	T operator ()(T& x){
		return (x + x) % mod;

	}
	int GetMod(){
		return mod;
	}
};

template<class functor, class T>
int PreCycleLength(functor& f, T x){
	int n = f.GetMod();
	T current = x;
	for(int i = 0; i <= n; i++){
		current = f(current);
	}
	T element = f(current);
	int length = 1;
	while (element != current){
		element = f(element);
		length++;
	}
	int prCycleLength = 0;
	T x2 = x;
	for(int i = 0; i < length; i++){
		x2 = f(x2);
	}
	while (x != x2){
		x = f(x);
		x2 = f(x2);
		prCycleLength++;
	}
	return prCycleLength;
}


class FunctorForTest{
private:
	vector<int> f;
	int mod;
public:
	FunctorForTest(int cycleLength, int preCycleLength){
		f.resize(preCycleLength + cycleLength);
		int currentElement = 0;
		int nextElement = 0;
		for (int i = 0; i < preCycleLength; i++){
			nextElement++;
			f[currentElement] = nextElement;
			currentElement = nextElement;
		}
		int repeatedElement = currentElement;
		for (int i = 0; i < cycleLength - 1; i++){
			nextElement++;
			f[currentElement] = nextElement;
			currentElement = nextElement;
		}
		f[currentElement] = repeatedElement;
		mod = preCycleLength;
	}
	int GetMod(){
		return mod;
	}
	int operator()(int value){
		return f[value];
	}
};

void TestForInteger(){
	srand(3);
	int cycleLength = rand() % 50;
	int preCycleLength = rand() % 50;
	FunctorForTest f(cycleLength, preCycleLength);
	assert(PreCycleLength(f, 0) == preCycleLength);
	cout << "Test is done";

}

int main(){
	int mod;
	cout << "Enter mod: ";
	cin >> mod;
	functor<int> f(mod);
	int x;
	cout << "Enter x: ";
	cin >> x;
	cout << "PreCycle_length = " << PreCycleLength<functor<int>, int>(f, x) << endl;

	TestForInteger();

	return 0;
}