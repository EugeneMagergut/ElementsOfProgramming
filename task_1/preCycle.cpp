#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
using namespace std;

vector<int> operator ++(vector<int>& element, int){
	for (size_t i = 0; i < element.size(); i++){
		element[i] += 1;
	}
	return element;
}

vector<int> operator +(const vector<int>& firstElement, const vector<int>& secondElement){
	vector<int> result(firstElement);
	for (size_t i = 0; i < result.size(); i++){
		result[i] = firstElement[i] + secondElement[i];
	}
	return result;
}

vector<int> operator %(const vector<int>& element, int mod){
	vector<int> result(element);
	for (size_t i = 0; i < element.size(); i++){
		result[i] = element[i] % mod;
	}
	return element;
}

ostream& operator << (ostream& out, const vector<int>& element){
	out << "(";
	for (size_t i = 0; i < element.size(); i++){
		out << element[i] << ",";
	}
	out << ")";
	return out;
}

template <class T>
class Transformation{
private:
	int mod;
public:
	explicit Transformation(int value_): mod(value_) {}

	T operator()(const T& x) const{
		return (x + x) % mod;
	}

	int getMod() const{
		return mod;
	}
};

template<class Transformation, class T>
int preCycleLengthSearch(const Transformation& f, T x){
	int n = f.getMod();
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
	int preCycleLength = 0;
	T x2 = x;
	for(int i = 0; i < length; i++){
		x2 = f(x2);
	}
	while (x != x2){
		x = f(x);
		x2 = f(x2);
		preCycleLength++;
	}
	return preCycleLength;
}

template <class T>
class chainTransformation{
private:
	map<T, T> function_;
	int mod;
public:
	chainTransformation(int cycleLength, int precycleLength, const T& firstElement): mod(precycleLength) {
		T currentElement = firstElement;
		T nextElement = firstElement;
		for (int i = 0; i < precycleLength; i++){
			nextElement++;
			function_[currentElement] = nextElement;
			currentElement = nextElement;
		}
		T repeatedElement = currentElement;
		for (int i = 0; i < cycleLength - 1; i++){
			nextElement++;
			function_[currentElement] = nextElement;
			currentElement = nextElement;
		}
		function_[currentElement] = repeatedElement;
	}
	int getMod() const{
		return mod;
	}
	T operator() (const T& element) const{
		return (function_.find(element))->second;
	}
}; 


void IntegerExample(){
	int mod = rand();
	int x = rand();
	cout << "mod=" << mod << ", x=" << x << endl;
	Transformation<int> f(mod);
	cout << "PreCycle_length = " << preCycleLengthSearch(f, x) << endl;
}

void VectorExample(){
	int mod = rand();
	int vectorDimension = rand()%10;
	vector<int> firstElement(vectorDimension);
	for (int i = 0; i < vectorDimension; i++){
		firstElement.push_back(rand());
	}
	cout << "mod=" << mod << ", x=" << firstElement << endl;
	Transformation<vector<int>> f(mod);
	cout << "PreCycle_length = " << preCycleLengthSearch(f, firstElement) << endl;
}

void TestForInteger(){
	int cycleLength = rand() % 50;
	int preCycleLength = rand() % 50;
	chainTransformation<int> f(cycleLength, preCycleLength, 0);
	assert(preCycleLengthSearch(f, 0) == preCycleLength);
}

void TestForVector(){
	int vectorDimension = rand();
	vector<int> firstElement(vectorDimension);
	for (int i = 0; i < vectorDimension; i++){
		firstElement.push_back(rand());
	}

	int cycleLength = rand() % 50;
	int preCycleLength = rand() % 50;
	chainTransformation<vector<int>> f(cycleLength, preCycleLength, firstElement);
	assert(preCycleLengthSearch(f, firstElement) == preCycleLength);
}

int main(){
	srand(5);
	IntegerExample();
	VectorExample();

	TestForInteger();
	TestForVector();

	return 0;
}