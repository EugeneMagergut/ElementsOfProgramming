#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cassert>
using namespace std;

const int UNDEFINED = -1;

class Tree{
private:
	vector <vector <int>> adjacencyMatrix_;
	vector <int> heigh;
	int verticesNumber;
	
	class Iterator{
	private:
		Tree* Tree_;
		int currentVertex;
	public:	
		Iterator(Tree& t, int curr): Tree_(&t), currentVertex(curr) {}
		
		Iterator& operator ++(){
			currentVertex = Tree_->nextVertexDFS(currentVertex);
			return *this;
		}
		int operator *(){
			return currentVertex;
		}

		bool operator !=(Iterator& it){
			if (currentVertex != it.currentVertex)
				return true;
			else
				return false;
		}
	};

public:
	typedef Iterator iterator;
	Tree() {}
	explicit Tree(int n) : verticesNumber(n), adjacencyMatrix_(n, vector<int>(n, -1)){ }
	
	Iterator begin(){
		return Iterator(*this, 0);
	}

	Iterator end(){
		return Iterator(*this, UNDEFINED);
	}

	void addEdge(int v1, int v2){
		adjacencyMatrix_[v1][v2] = 1;
	}

	int nextVertexDFS(int currentVertex){
		int tempVertex = -1;
		if (currentVertex < 0)
			return UNDEFINED;
		else{
			while (true){
				for (int i = 0; i < verticesNumber; i++){
					if (adjacencyMatrix_[currentVertex][i] == 1 && tempVertex < i)
						return i;
				}

				if (currentVertex == 0)
					return UNDEFINED;
				for (int i = 0; i < verticesNumber; i++){
					if (adjacencyMatrix_[i][currentVertex] == 1){
						tempVertex = currentVertex;
						currentVertex = i;
						break;
					}
				}
			}
		}
	}
	

	vector<int> BFS(int t){
		vector<int> BfsList, used(verticesNumber, false);
		for (int i = 0; i < verticesNumber; i++){
			used[i] = false;
		}
		queue <int> q;
		q.push(t);
		
		used[t] = true;

		while (!q.empty()){
			int t = q.front();
			BfsList.push_back(t);
			q.pop();
			for (int i = 0; i < verticesNumber; i++){
				if (adjacencyMatrix_[t][i] != -1 && used[i] == false){
					q.push(i);
					used[i] = true;
				}
			}
		}
		return BfsList;
	}

	bool operator ==(const Tree& t) const{
		if (verticesNumber != t.verticesNumber)
			return false;
		else{
			for (int i = 0; i < verticesNumber; i++){
				for (int j = 0; j < verticesNumber; j++){
					if (adjacencyMatrix_[i][j] != t.adjacencyMatrix_[i][j]){
						return false;
					}
				}
			}
		}
		return true;
	}
};


template <class DfsIterator>
void buildTree(Tree* resultTree, DfsIterator& dfsBegin, DfsIterator& dfsEnd, const vector<int>& BfsList){
	if (BfsList.size() == 2){
		resultTree->addEdge(BfsList[0], BfsList[1]);
	}
	else if (BfsList.size() > 2){
		size_t index = 2;
		vector <int> subBfsList, elementsForSubBfsList;
		DfsIterator itBegin = dfsBegin;
		DfsIterator it = (++dfsBegin);
		DfsIterator itEnd = dfsBegin;

		while (dfsBegin != dfsEnd){
			resultTree->addEdge(*itBegin, *it);
			if (index < BfsList.size()){
				while ((BfsList[index] != *dfsBegin) && (dfsBegin != dfsEnd)){
					elementsForSubBfsList.push_back(*dfsBegin);
					++dfsBegin;
				}

				itEnd = dfsBegin;
				subBfsList = createSubBfsList(elementsForSubBfsList, BfsList);
				elementsForSubBfsList.clear();

				buildTree(resultTree, it, itEnd, subBfsList);

				index++;
				it = itEnd;
			}
			else break;
		}
	}
}

vector <int> createSubBfsList(vector <int>& elementsForSubBfsList, const vector <int>& BfsList){
	vector <int> subBfsList;
	vector <int>::iterator it;
	for (size_t i = 0; i < BfsList.size(); i++){
		it = find(elementsForSubBfsList.begin(), elementsForSubBfsList.end(), BfsList[i]);
		if (it != elementsForSubBfsList.end()){
			subBfsList.push_back(*it);
		}
	}
	return subBfsList;
}

template <class DfsIterator>
void buildTreeChecking(Tree* resultTree, DfsIterator& dfsBegin, DfsIterator& dfsEnd, const vector<int>& BfsList){
	vector<int> resultBfsList = resultTree->BFS(0);
	assert(resultBfsList == BfsList);
	Tree::iterator resultTreeIterator = resultTree->begin();
	while(dfsBegin != dfsEnd){
		assert(*resultTreeIterator == *dfsBegin);
		++dfsBegin;
		++resultTreeIterator;
	}
}

void BuildTreeExample(){
	Tree tree(9);
	tree.addEdge(0, 2);
	tree.addEdge(3, 8);
	tree.addEdge(2, 6);
	tree.addEdge(3, 7);
	tree.addEdge(0, 3);
	tree.addEdge(2, 1);
	tree.addEdge(2, 5);
	tree.addEdge(1, 4);
	vector <int> BfsList = tree.BFS(0);

	Tree resultTree(BfsList.size());
	buildTree(&resultTree, tree.begin(), tree.end(), BfsList);
	buildTreeChecking(&resultTree, tree.begin(), tree.end(), BfsList);
}


void BuildTreeCurrentTest(int i){
	srand(i);
	int treeSize = rand()%10 + 1;
	Tree tree(treeSize);
	for (int i = 1; i < treeSize; i++){
		if (i == 1)
			tree.addEdge(0, 1);
		else
			tree.addEdge(rand()%i, i);
	}
	vector <int> BfsList = tree.BFS(0);

	Tree resultTree(BfsList.size());
	buildTree(&resultTree, tree.begin(), tree.end(), BfsList);
	buildTreeChecking(&resultTree, tree.begin(), tree.end(), BfsList);
}

void BuildTreeTest(){
	for (int i = 0; i < 100; i++)
		BuildTreeCurrentTest(i);
}
int main(){
	BuildTreeExample();
	BuildTreeTest();

	return 0;
}