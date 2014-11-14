// Author: Andrew Rush
// Class: CSE 609
// Date: 11/6/2014
#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;

// Function Headers
tuple<string,list<string>,string> parseFastaFile(string filepath);
map<string, int> digramFreqScores(string);
list<list<int>> digramFreqMatrix(map<string, int>);
list<list<int>> parseScoreFile(string);
pair<int,int> scoreSequence(string, string, list<list<int>>);
tuple<int, int, string> findHighScore(string, list<string>, list<list<int>>);
int main(int argc, char** argv){
	tuple<string, list<string>, string> myTuple;
	tuple<int, int, string> finalTuple;
	list<string> seqs;
	map<string, int> myMap;
	list<list<int>> scoreMatrix;	
	list<list<int>> firstMatrix;
	string fileName;
	fileName = argv[1];
	myTuple = parseFastaFile(fileName);
	cout << "        Digram Frequency Matrix" << endl;
	cout << "           " << "A  G  C  T" << endl;
	myMap = digramFreqScores(get<2>(myTuple));
      	firstMatrix = digramFreqMatrix(myMap);
	cout << "        A  ";
	vector<string> tempString = {"G","C","T","","",""};
	int tempStringIndex = 0;
	for (list<list<int>>::iterator IT = firstMatrix.begin(); IT != firstMatrix.end(); IT++){
		for (list<int>::iterator IT2 = (*IT).begin(); IT2 != (*IT).end(); IT2++){
			cout << *IT2 << " ";
		}
		cout << endl << "        " << tempString.at(tempStringIndex) << "  ";
		tempStringIndex++;
	}
	tempStringIndex = 0;
	cout << endl << endl << "        Scoring Matrix" << endl;
	cout << "           " << "A  G  C  T" << endl;
	scoreMatrix = parseScoreFile(argv[2]);
	cout << "        A  ";
	for (list<list<int>>::iterator IT = scoreMatrix.begin(); IT != scoreMatrix.end(); IT++){
          for (list<int>::iterator IT2 = (*IT).begin(); IT2 != (*IT).end(); IT2++){
            cout << *IT2 << " ";
          }
          cout << endl << "        " << tempString.at(tempStringIndex) << "  ";
          tempStringIndex++;
        }
	cout << endl;
	int num_seq = 0;
	cout << "How many sequences would you like to score?" << " ";
	cin >> num_seq;
	int i = 0;
	string tempseq = "";
	while ( i < num_seq){
	  cout << "Enter Sequence #" << i+1 << " on a single line:" << endl;
		cin >> tempseq;
		seqs.push_back(tempseq);
		i++;
	}
	i = 0;
	cout << endl << endl;
	list<string>::iterator seqIT = seqs.begin();
	while(i < num_seq){
	  cout << "Sequence:" << endl << *seqIT << endl;
	  cout << "Score: " << scoreSequence(get<2>(myTuple),*seqIT,scoreMatrix).second << " at position " << scoreSequence(get<2>(myTuple),*seqIT,scoreMatrix).first << endl << endl;
	  i++;
	  seqIT++;
	}
	finalTuple = findHighScore(get<2>(myTuple), seqs, scoreMatrix); 
	return 0;
}
tuple<string, list<string>, string> parseFastaFile(string filepath){
	tuple<string, list<string>, string> tempTuple;
	list<string> tempList;
	string line = "";
	ifstream myfile(filepath);
	if (myfile.is_open()){
		while (getline(myfile, line)){
			if (line.at(0) == '>'){
				get<0>(tempTuple) = line;
			}
			if (line.at(0) != '>' && line.at(0) != ';'){
				get<2>(tempTuple) += line;
			}
			if (line.at(0) == ';'){
				tempList.push_back(line);
			}
		}
		get<1>(tempTuple) = tempList;
		myfile.close();
	}
	return tempTuple;
}
map<string, int> digramFreqScores(string sequence){
	map<string, int> tempMap;
	int numAA = 0;
	int numAC = 0;
	int numGT = 0;
	int numAG = 0;
	int numCC = 0;
	int numCA = 0;
	int numCG = 0;
	int numTT = 0;
	int numGG = 0;
	int numGC = 0;
	int numAT = 0;
	int numGA = 0;
	int numTG = 0;
	int numTA = 0;
	int numTC = 0;
	int numCT = 0;
	for (int i = 0; i < sequence.size() - 1; i++){
		if (sequence.at(i) == 'A' && sequence.at(i+1) == 'A'){
			numAA++;
		}
		if (sequence.at(i) == 'A' && sequence.at(i + 1) == 'C'){
			numAC++;
		}
		if (sequence.at(i) == 'G' && sequence.at(i + 1) == 'T'){
			numGT++;
		}
		if (sequence.at(i) == 'A' && sequence.at(i + 1) == 'G'){
			numAG++;
		}
		if (sequence.at(i) == 'C' && sequence.at(i + 1) == 'C'){
			numCC++;
		}
		if (sequence.at(i) == 'C' && sequence.at(i + 1) == 'A'){
			numCA++;
		}
		if (sequence.at(i) == 'C' && sequence.at(i + 1) == 'G'){
			numCG++;
		}
		if (sequence.at(i) == 'T' && sequence.at(i + 1) == 'T'){
			numTT++;
		}
		if (sequence.at(i) == 'G' && sequence.at(i + 1) == 'G'){
			numGG++;
		}
		if (sequence.at(i) == 'G' && sequence.at(i + 1) == 'C'){
			numGC++;
		}
		if (sequence.at(i) == 'A' && sequence.at(i + 1) == 'T'){
			numAT++;
		}
		if (sequence.at(i) == 'G' && sequence.at(i + 1) == 'A'){
			numGA++;
		}
		if (sequence.at(i) == 'T' && sequence.at(i + 1) == 'G'){
			numTG++;
		}
		if (sequence.at(i) == 'T' && sequence.at(i + 1) == 'A'){
			numTA++;
		}
		if (sequence.at(i) == 'C' && sequence.at(i + 1) == 'T'){
			numCT++;
		}
		if (sequence.at(i) == 'T' && sequence.at(i + 1) == 'C'){
			numTC++;
		}
		tempMap["AA"] = numAA;
		tempMap["AC"] = numAC;
		tempMap["GT"] = numGT;
		tempMap["AG"] = numAG;
		tempMap["CC"] = numCC;
		tempMap["CA"] = numCA;
		tempMap["CG"] = numCG;
		tempMap["TT"] = numTT;
		tempMap["GG"] = numGG;
		tempMap["GC"] = numGC;
		tempMap["AT"] = numAT;
		tempMap["GA"] = numGA;
		tempMap["TG"] = numTG;
		tempMap["TA"] = numTA;
		tempMap["TC"] = numTC;
		tempMap["CT"] = numCT;
	}
	return tempMap;
}
list<list<int>> digramFreqMatrix(map<string, int> d){
	list<list<int>> m;
	list<int> rowMatrix;
	list<list<int>>::iterator row = m.begin();
	list<int>::iterator col = rowMatrix.begin();
	vector<string> pairs{ "AA", "AG", "AC", "AT", "GA", "GG", "GC", "GT", "CA", "CG", "CC", "CT", "TA", "TG", "TC", "TT" };
	for (int i = 0; i < 4; i++){
		rowMatrix.push_back(d.at(pairs.at(4 * i)));
		rowMatrix.push_back(d.at(pairs.at(4 * i + 1)));
		rowMatrix.push_back(d.at(pairs.at(4 * i + 2)));
		rowMatrix.push_back(d.at(pairs.at(4 * i + 3)));
		m.push_back(rowMatrix);
		col = rowMatrix.begin();
		col = rowMatrix.erase(col);
		col = rowMatrix.erase(col);
		col = rowMatrix.erase(col);
		col = rowMatrix.erase(col);
	}
	return m;
}
list<list<int>> parseScoreFile(string filepath){
	list<list<int>> m;
	list<int> rowMatrix;
	list<list<int>>::iterator row = m.begin();
	list<int>::iterator col = rowMatrix.begin();
	ifstream file(filepath);
	string line;
	string value;
	while (file.good()){
		getline(file, line);
		stringstream input(line);
		while (getline(input, value, ',')){
			rowMatrix.push_back(stoi(value));
		}
		
		m.push_back(rowMatrix);
		col = rowMatrix.begin();
		while (rowMatrix.size() != 0){
			col = rowMatrix.erase(col);
		}
	}
	return m;
}
pair<int,int> scoreSequence(string haystack, string needle, list<list<int>> score_m){
	pair<int,int> max_score;
	int tempScore = 0;
	int topScore = 0;
	vector<int> v1;
	max_score.first = 0;
	max_score.second = 0;
	int secondStart = 0;
	list<list<int>>::iterator IT = score_m.begin();
	list<int>::iterator IT2 = (*IT).begin();
	for (int j = 0; j < (haystack.size() - needle.size()+1); j++){
		secondStart = j;
		for (int i = 0; i < needle.size(); i++){
			if (haystack.at(j+i) == 'A'){
				IT = score_m.begin();
			}
			if (haystack.at(j+i) == 'G'){
				IT = score_m.begin();
				IT++;
			}
			if (haystack.at(j+i) == 'C'){
				IT = score_m.begin();
				IT++;
				IT++;
			}
			if (haystack.at(j+i) == 'T'){
				IT = score_m.begin();
				IT++;
				IT++;
				IT++;
			}
			if (needle.at(i) == 'A'){
				IT2 = (*IT).begin();
				tempScore = *IT2;
			}
			if (needle.at(i) == 'G'){
				IT2 = (*IT).begin();
				IT2++;
				tempScore = *IT2;
			}
			if (needle.at(i) == 'C'){
				IT2 = (*IT).begin();
				IT2++;
				IT2++;
				tempScore = *IT2;
			}
			if (needle.at(i) == 'T'){
				IT2 = (*IT).begin();
				IT2++;
				IT2++;
				IT2++;
				tempScore = *IT2;
			}
			topScore += tempScore;
		}
		if (topScore >= max_score.second){
			max_score.second = topScore;
			max_score.first = secondStart;
		}
		topScore = 0;
		secondStart = 0;
	}		
	return max_score;
}
tuple<int, int, string> findHighScore(string haystack, list<string> needles, list<list<int>> scoring_m){
	tuple<int, int, string> high_scorer;
	int topScore = 0;
	int newScore = 0;
	int newScoreIndex = 0;
	int topScoreIndex = 0;
	for (list<string>::iterator IT = needles.begin(); IT != needles.end(); IT++){
		newScore = scoreSequence(haystack, *IT, scoring_m).second;
		newScoreIndex = scoreSequence(haystack, *IT, scoring_m).first;
		if (newScore >= topScore){
			topScore = newScore;
			topScoreIndex = newScoreIndex;
			get<2>(high_scorer) = *IT;
		}
		get<0>(high_scorer) = topScoreIndex;
		get<1>(high_scorer) = topScore;
	}
	return high_scorer;
}
