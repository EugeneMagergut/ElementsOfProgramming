#include <iostream>
#include <vector>
#include <queue>
#include <assert.h>
using namespace std;

class Tree{
private:
	vector <vector <int>> v;
	vector <int> used;
	int VertexNumber;
	vector<int> DfsList;

	class Iterator{
	private:
		Tree* Tree_;
		int currentVertex;
	public:	
		Iterator(Tree& t, int curr): Tree_(&t), currentVertex(curr) {}
		
		Iterator operator ++(){
			currentVertex = Tree_->NextVertexDFS(currentVertex);
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
	Tree() {}
	Tree(int n){
		VertexNumber = n;
		v.resize(n);
		used.resize(n);
		for (int i = 0; i < n; i++){
			v[i].resize(n);
			used[i] = false;
			for (int j = 0; j < n; j++){
				v[i][j] = -1;
			}
		}
	}
	
	Iterator begin(){
		return Iterator(*this, 0);
	}

	Iterator end(){
		return Iterator(*this, -1);
	}

	void AddEdge(int v1, int v2){
		v[v1][v2] = 1;
	}

	int NextVertexDFS(int v){
		for (int i = 0; i < VertexNumber; i++){
			if (DfsList[i] == v){
				if(i == VertexNumber - 1)
					return -1;
				else
					return DfsList[++i];
			}
		}
		return -1;
	}

	void DFS(int t){
		for (int i = 0; i < VertexNumber; i++){
			used[i] = false;
		}
		DfsList.push_back(t);
		used[t] = true;
		for (int i = 0; i < VertexNumber; i++){
			if (v[t][i] != -1 && used[i] == false)
				DFS(i);
		}
	}

	vector<int> BFS(int t){
		vector<int> BfsList;
		for (int i = 0; i < VertexNumber; i++){
			used[i] = false;
		}
		queue <int> q;
		q.push(t);
		
		used[t] = true;

		while (!q.empty()){
			int t = q.front();
			BfsList.push_back(t);
			q.pop();
			for (int i = 0; i < VertexNumber; i++){
				if (v[t][i] != -1 && used[i] == false){
					q.push(i);
					used[i] = true;
				}
			}
		}
		return BfsList;
	}

	bool operator ==(Tree& t){
		if (VertexNumber != t.VertexNumber)
			return false;
		else{
			for (int i = 0; i < VertexNumber; i++){
				for (int j = 0; j < VertexNumber; j++){
					if (v[i][j] != t.v[i][j]){
						cout << i << " "<< j;
						return false;
					}
				}
			}
		}
		return true;
	}
};


template <class DfsIterator>
void buildTree(Tree& ResultTree, DfsIterator& dfsBegin, DfsIterator& dfsEnd, vector<int>& BfsList){
	if (BfsList.size() == 2){
		ResultTree.AddEdge(BfsList[0], BfsList[1]);
	}
	else if (BfsList.size() > 2){
		size_t index = 2;
		vector <int> subBfsList, elementsForSubBfsList;
		DfsIterator itBegin = dfsBegin;
		DfsIterator it = (++dfsBegin);
		DfsIterator itEnd = dfsBegin;

		while (dfsBegin != dfsEnd){
			ResultTree.AddEdge(*itBegin, *it);
			if (index < BfsList.size()){
				while ((BfsList[index] != *dfsBegin) && (dfsBegin != dfsEnd)){
					elementsForSubBfsList.push_back(*dfsBegin);
					++dfsBegin;
				}

				itEnd = dfsBegin;
				subBfsList = createSubBfsList(elementsForSubBfsList, BfsList);
				elementsForSubBfsList.clear();

				buildTree(ResultTree, it, itEnd, subBfsList);

				index++;
				it = itEnd;
			}
			else break;
		}
	}
}

vector <int> createSubBfsList(vector <int>& elementsForSubBfsList, vector <int>& BfsList){
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

int main(){
	Tree NewTree(9);
	NewTree.AddEdge(0, 2);
	NewTree.AddEdge(3, 8);
	NewTree.AddEdge(2, 6);
	NewTree.AddEdge(3, 7);
	NewTree.AddEdge(0, 3);
	NewTree.AddEdge(2, 1);
	NewTree.AddEdge(2, 5);
	NewTree.AddEdge(1, 4);
	NewTree.DFS(0);
	vector <int> BfsList = NewTree.BFS(0);

	Tree ResultTree(BfsList.size());
	buildTree(ResultTree, NewTree.begin(), NewTree.end(), BfsList);
	assert(NewTree == ResultTree);

	return 0;
}