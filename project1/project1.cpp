// Author: Andrew Rush
// Class: CSE 609
// Date: 9/21/2014

// Import Statements
#include <iostream>
#include <vector>

// Standard Namespace
using namespace std;

// Main Statement (Start Here)
int main(){
  // Main Function Variables
  vector<int> m1;
  vector<int> m2;
  vector<int> mResult;
  int m1Rows;
  int m2Rows;
  int m1Cols;
  int m2Cols;
  int sum;
  int inputValue;

  // Prompt User for Row/Column Info
  cin >> m1Rows;
  cin >> m1Cols;
  cin >> m2Rows;
  cin >> m2Cols;

  // Check matrix 1 columns is equal to matrix 2 rows. Fail: Give error text
  if(m1Cols == m2Rows){
    // Prompt User for Matrix 1 Input Values
    for (int i = 0; i < m1Rows; i++){
      for (int j = 0; j < m1Cols; j++){
        cin >> inputValue;
        m1.push_back(inputValue);
      }
    }
    // Prompt User for Matrix 2 Input Values
    for (int i = 0; i < m2Rows; i++){
      for (int j = 0; j < m2Cols; j++){
        cin >> inputValue;
        m2.push_back(inputValue);
      }
    }
    cout << endl;
    int counter = 0;
    // Print Matrix 1
    for (int i = 0; i < m1Rows; i++){
      for (int j = 0; j < m1Cols; j++){
        cout << m1.at(counter) << " ";
        counter++;
      }
    cout << endl;
    }
    cout << endl;
    counter = 0;
    // Print Matrix 2
    for (int i = 0; i < m2Rows; i++){
      for (int j = 0; j < m2Cols; j++){
        cout << m2.at(counter) << " ";
        counter++;
      }
      cout << endl;
    }
    sum = 0;
    // Multiply Matrix 1 x Matrix 2
    for (int i = 0; i < m1Rows; i++){
      for (int j = 0; j < m2Cols; j++){
	for (int k = 0; k < m1Cols; k++){
	  sum += (m1.at(k+m1Cols*i)*(m2.at(j+m2Cols*k)));
	}
      mResult.push_back(sum);
      sum = 0;
      }
    }
    cout << endl;
    counter = 0;
    // Print Matrix 2
    for (int i = 0; i < m1Rows; i++){
      for (int j = 0; j < m2Cols; j++){
       	cout << mResult.at(counter) << " ";
	counter++;
      }
      cout << endl;
    }
  }
  // Error Message if Matrix Dimensions not Compatible
  else{
    cout << "Invalid Input\n";
  }
  cout << endl;
  return 1;
}
