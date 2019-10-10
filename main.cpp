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
	bool doesAdjacencyListContain(int toVertex){
		for(int i=0;i<fromAdjacencyList.size();i++){
			if(fromAdjacencyList.at(i) == toVertex){
				return true;
			}
		}
		return false;
	}
};

class Graph{
public:
	map<int, Node*> vertexMap;
	void addEdge(int from, int to){
		for(int i=1; i <= from; i++){
			if(vertexMap.find(i) == vertexMap.end()){
				Node* newNode = new Node(i);
				vertexMap.insert(pair<int, Node*> (i, newNode));
			}
		}
		vertexMap[from]->fromAdjacencyList.push_back(to);
		for(int i=1; i <= to; i++){
			if(vertexMap.find(i) == vertexMap.end()){
				Node* newNode = new Node(i);
				vertexMap.insert(pair<int, Node*> (i, newNode));
			}
		}
		vertexMap[to]->toAdjacencyList.push_back(from);
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
	cout << line << endl;
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

int cnfCoordinate(int x, int y, int n, int m){
	return (m*(x - 1) + y);
}

void print2DVector(vector<vector<int> > v){
	for(int i=0;i<v.size();i++){
		for(int j=0;j<v.at(i).size();j++){
			cout << v.at(i).at(j) << " ";
		}
		cout<<endl;
	}
}

pair<int, int> cnfToCartesian(int cnfCoordinate, int n, int m){
	int x = (cnfCoordinate - 1)/m + 1;
	int y = (cnfCoordinate - 1)%m + 1;
	return pair<int, int>(x, y);
}

int main(int argc, char *argv[]){
	string methodName(argv[1]);
	if(methodName.compare("generateInput") == 0){
		int from, to;
		Graph largerGraph;
		Graph smallerGraph;
		string inputFileName(argv[2]);
		ifstream inputFile(inputFileName);
		string line;
		bool largeGraphInput = true;
		if(inputFile.is_open()){
			while(getline(inputFile, line)){
				if(line.compare("") != 0){
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
		}
		int n = smallerGraph.vertexMap.size();
		int m = largerGraph.vertexMap.size();
		cout << n << " " << m << endl;
		ofstream dimensionFile;
		dimensionFile.open ("dimension.txt");
  		dimensionFile << n << " " << m << endl;
  		dimensionFile.close();
		bool mapMatrix[n][m];
		vector<vector<int> > cnfInput;
		for(int i=1; i <= n; i++){
			vector<int> tempVector;
			for(int j=1; j <= m-1; j++){
				for(int k=j+1; k <= m; k++){
					tempVector.clear();
					tempVector.push_back(-cnfCoordinate(i, j, n, m));
					tempVector.push_back(-cnfCoordinate(i, k, n, m));
					cnfInput.push_back(tempVector);
				}
			}
			tempVector.clear();
			for(int j = 1; j<= m; j++){
				tempVector.push_back(cnfCoordinate(i, j, n, m));
			}
			cnfInput.push_back(tempVector);
		}
		for(int i=1; i <= m; i++){
			vector<int> tempVector;
			for(int j=1; j <= n-1; j++){
				for(int k=j+1; k <= n; k++){
					tempVector.clear();
					tempVector.push_back(-cnfCoordinate(j, i, n, m));
					tempVector.push_back(-cnfCoordinate(k, i, n, m));
					cnfInput.push_back(tempVector);
				}
			}
		}

		for(int i=1; i <= n; i++){
			vector<int> tempVector;
			for(int j=1; j <= n; j++){
				for(int k=1; k <= m; k++){
					for(int l=1; l <= m; l++){
						if(i != j && k != l){
							tempVector.clear();
							if(smallerGraph.vertexMap[i]->doesAdjacencyListContain(j) != largerGraph.vertexMap[k]->doesAdjacencyListContain(l)){
								tempVector.push_back(-cnfCoordinate(i, k, n, m));
								tempVector.push_back(-cnfCoordinate(j, l, n, m));
								cnfInput.push_back(tempVector);
							}
						}
					}
				}
			}
		}
		int numVariables = n*m;
		int numClauses = cnfInput.size();
		ofstream satInputFile;
		satInputFile.open("test.satinput");
		satInputFile << "p cnf " << numVariables << " " << numClauses << endl;
		for(int i=0;i<cnfInput.size();i++){
			for(int j=0;j<cnfInput.at(i).size();j++){
				satInputFile << to_string(cnfInput.at(i).at(j)) << " ";
			}
			satInputFile << "0" << endl;
		}
	}
	if(methodName.compare("generateOutput") == 0){
		int n, m;
		string outputFileName(argv[2]);
		ifstream dimensionFile("dimension.txt");
		dimensionFile >> n;
		dimensionFile >> m;
		dimensionFile.close();
		ifstream satOutputFile(outputFileName + ".satoutput");
		string line;
		int num;
		bool isSatisfiable = true;
		int numVariables = n*m;
		int arr[numVariables] = {0};
		if(satOutputFile.is_open()){
    		getline(satOutputFile, line);
  			if(line.compare("UNSAT") == 0){
				isSatisfiable = false;
			}
			else{
				while(getline(satOutputFile, line, ' ')){
  					num = atoi(line.c_str());
 					if(num != 0){
						if(num > 0){
							arr[num-1] = 1;
						}
					}
				}
			}
    		satOutputFile.close();
  		}
		ofstream outputFile;
		outputFile.open(outputFileName + ".mapping");
		if(!isSatisfiable){
			outputFile << to_string(0) << endl;
			return 0;
		}
		for(int i=0;i<numVariables;i++){
			if(arr[i] == 1){
				pair<int, int> tempPair = cnfToCartesian(i+1, n, m);
				outputFile << tempPair.first << " " << tempPair.second << endl;
			}
		}
	}

	return 0;
}
