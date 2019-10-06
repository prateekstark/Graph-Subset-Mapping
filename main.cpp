#include<iostream>
#include<vector>
#include<map>
#include<fstream>
using namespace std;

class Node{
public:
	int nodeID;
	vector<int> toAdjacencyList;
	vector<int> fromAdjacencyList;
	Node(int n){
		this->nodeID = n;
	}
	void printAdjacencyLists(){
		cout<<"From this vertex: ";
		for(int i=0;i<fromAdjacencyList.size();i++){
			cout<<fromAdjacencyList.at(i)<<" ";
		}
		cout<<endl;
		cout<<"To this vertex: ";
		for(int i=0;i<toAdjacencyList.size();i++){
			cout<<toAdjacencyList.at(i)<<" ";
		}
		cout<<endl;
	}
};

class Graph{
public:
	map<int, Node*> vertexMap;

	void addEdge(int from, int to){
		if(vertexMap.find(from) == vertexMap.end()){
			Node* newNode = new Node(from);
			newNode->fromAdjacencyList.push_back(to);
			vertexMap.insert(pair<int, Node*> (from, newNode));
		}
		else{
			vertexMap[from]->fromAdjacencyList.push_back(to);
		}
		if(vertexMap.find(to) == vertexMap.end()){
			Node* newNode = new Node(to);
			newNode->toAdjacencyList.push_back(from);
			vertexMap.insert(pair<int, Node*> (to, newNode));
		}
		else{
			vertexMap[to]->toAdjacencyList.push_back(from);
		}
	}

	void printGraph(){
		for(map<int, Node*>::iterator itr=vertexMap.begin(); itr != vertexMap.end(); ++itr){
			Node* tempNode = itr->second;
			cout<<tempNode->nodeID<<endl;
			tempNode->printAdjacencyLists();
		}
	}

};

vector<int> extractEdgeInformation(string line){
	string temp = "";
	vector<int> answer;
	for(int i=0;i<line.size();i++){
		if(line.at(i) == ' '){
			answer.push_back(stoi(temp));
			temp = "";
		}
		else{
			temp = temp + line.at(i);
		}
	}
	answer.push_back(stoi(temp));
	return answer;
}

int main(){
	int from, to;
	Graph largerGraph;
	Graph smallerGraph;
	ifstream inputFile("input.txt");
	string line;
	bool largeGraphInput = true;
	if(inputFile.is_open()){
		while(getline(inputFile, line)){
			vector<int> tempInfoVector = extractEdgeInformation(line);
			from = tempInfoVector.at(0);
			to = tempInfoVector.at(1);
			if(from == 0 && to == 0){
				largeGraphInput = false;
				continue;
			}
			if(largeGraphInput){
				largerGraph.addEdge(from, to);
			}
			else{
				smallerGraph.addEdge(from, to);
			}
		}
	}

	largerGraph.printGraph();
	smallerGraph.printGraph();
	return 0;
}