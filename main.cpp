#include<iostream>
#include<stdio.h>
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
	vector<pair<int, int> > edgeList;
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
		edgeList.push_back(pair<int, int>(from, to));
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

void appendToFile(ofstream &satInputFile, vector<vector<int> > cnfInput, int numClauses, int numVariables){
	for(int i=0;i<cnfInput.size();i++){
		for(int j=0;j<cnfInput.at(i).size();j++){
			satInputFile << to_string(cnfInput.at(i).at(j)) << " ";
		}
		satInputFile << "0" << endl;
	}
}

int main(int argc, char *argv[]){
	string methodName(argv[1]);
	if(methodName.compare("generateInput") == 0){
		int from, to;
		Graph largerGraph;
		Graph smallerGraph;
		string inputFileName(argv[2]);
		ifstream inputFile(inputFileName + ".graphs");
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


		ofstream dimensionFile;
		dimensionFile.open ("dimension.txt");
  		dimensionFile << n << " " << m << endl;
  		dimensionFile.close();


		bool mapMatrix[n][m];
		int numVariables = n*m;
		int numClauses = 0;

		vector<pair<int, int> > largerGraphEgdeList = largerGraph.edgeList;
		vector<pair<int, int> > smallerGraphEgdeList = smallerGraph.edgeList;
		
		int smallerGraphNumFrom[n] = {0};
		int largerGraphNumFrom[m] = {0};
		int smallerGraphNumTo[n] = {0};
		int largerGraphNumTo[m] = {0};

		bool smallerGraphAdjacencyMatrix[n][n] = {0};
		bool largerGraphAdjacencyMatrix[m][m] = {0};
		int tempX, tempY;

		for(int i=0; i<largerGraphEgdeList.size(); i++){
			int tempY = largerGraphEgdeList[i].second;
			int tempX = largerGraphEgdeList[i].first;
			largerGraphAdjacencyMatrix[tempX - 1][tempY - 1] = 1;
			largerGraphNumFrom[tempX - 1]++;
			largerGraphNumTo[tempY - 1]++;
		}

		cout << "Stage 1 cleared..." << endl;
		
		for(int i=0; i<smallerGraphEgdeList.size(); i++){
			int tempX = smallerGraphEgdeList[i].first;
			int tempY = smallerGraphEgdeList[i].second;
			smallerGraphAdjacencyMatrix[tempX - 1][tempY - 1] = 1;
			smallerGraphNumFrom[tempX - 1]++;
			smallerGraphNumTo[tempY - 1]++;
		}

		cout << "Stage 2 cleared..." << endl;

		string tempInputFileName = inputFileName + ".satinput";
		ofstream tempInputFile;
		tempInputFile.open(tempInputFileName, ios_base::app);
		for(int i=1; i <= n; i++){
			for(int j=1; j <= m-1; j++){
				for(int k=j+1; k <= m; k++){
					tempInputFile << -cnfCoordinate(i, j, n, m) << " " << -cnfCoordinate(i, k, n, m) << " 0" << endl;
					numClauses++;
				}
			}
			string tempString = "";
			for(int j = 1; j <= m; j++){
				tempInputFile << cnfCoordinate(i, j, n, m) << " ";
			}
			tempInputFile << "0" << endl;
			numClauses++;
		}

		cout << "Stage 3 cleared..." << endl;

		for(int i=1; i <= m; i++){
			for(int j=1; j <= n-1; j++){
				for(int k=j+1; k <= n; k++){
					tempInputFile << -cnfCoordinate(j, i, n, m) << " " << -cnfCoordinate(k, i, n, m) << " 0" << endl;
					numClauses++;
				}
			}
		}

		cout << "Stage 4 cleared..." << endl;

		for(int i=0;i<n;i++){
			for(int j=0;j<m;j++){
				if(smallerGraphNumFrom[i] > largerGraphNumFrom[j] || smallerGraphNumTo[i] > largerGraphNumTo[j]){
					tempInputFile << -cnfCoordinate(i+1, j+1, n, m) << " 0" << endl;
					numClauses++;
				}
				else{
					if(smallerGraphNumFrom[i] != 0 || smallerGraphNumTo[i] != 0){
						if(largerGraphNumFrom[j] != smallerGraphNumFrom[i]){
							tempInputFile << -cnfCoordinate(i+1, j+1, n, m) << " 0" << endl;
							numClauses++;
						}
					}
				}
			}
		}

		cout << "Stage 5 cleared..." << endl;


		for(int a = 0; a < largerGraph.edgeList.size(); a++){
			int k = largerGraphEgdeList[a].first;
			int l = largerGraphEgdeList[a].second;			
			for(int i = 1; i <= n; i++){
				if(smallerGraphNumFrom[i-1] <= largerGraphNumFrom[k-1] && smallerGraphNumTo[i-1] <= largerGraphNumTo[k-1]){	
					for(int j = 1; j <= n; j++){
						if(i != j){
							if(!smallerGraphAdjacencyMatrix[i-1][j-1]){
								tempInputFile << -cnfCoordinate(i, k, n, m) << " " << -cnfCoordinate(j, l, n, m) << " 0" << endl;
								numClauses++;
							}
						}
					}
				}
			}
		}

		cout << "Stage 6 cleared..." << endl;

		for(int a = 0; a < smallerGraphEgdeList.size(); a++){
			int k = smallerGraphEgdeList[a].first;
			int l = smallerGraphEgdeList[a].second;			
			for(int i = 1; i <= m; i++){
				if(smallerGraphNumFrom[k-1] <= largerGraphNumFrom[i-1] && smallerGraphNumTo[k-1] <= largerGraphNumTo[i-1]){
					for(int j = 1; j <= m; j++){
						if(i != j){
							if(!largerGraphAdjacencyMatrix[i-1][j-1]){
								tempInputFile << -cnfCoordinate(k, i, n, m) << " " << -cnfCoordinate(l, j, n, m) << " 0" << endl;
								numClauses++;
							}
						}
					}
				}
			}
		}

		cout << "Stage 7 cleared..." << endl;
		
		tempInputFile.close();
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
