// Author: Andrew Rush
// Class: CSE 609
// Date: 10/16/2014
// project2
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
// Global Variables
string name;
string testScore;
string checkTestScore;
// Struct to store student info, initiallized to 0 or ""
struct student{
  string firstName = "";
  string lastName = "";
  string testScore = "0";
};
vector<student> classList;
double mean = 0;
double deviation = 0;
// Function headers
double getMean();
double getStdDev();
string getFirstName(string);
string getLastName(string);
student newStudent;
void inputData();
void calculateGrades();
void getStudentName();
// Main Function, program organized into functions
int main(){
  getStudentName();
  getMean();
  getStdDev();
  cout << endl;
  calculateGrades();
  cout << endl;
  return 0;
}
// getStudentName method.  Returns void, but calls inputData() in order to add info to student struct
void getStudentName(){
  while (true){
    string temp;
    cout << "Please enter a student's name, or 'none' to quit: ";
    getline(cin, name);
    if (name == "none" || name == "None"){
      break;
    }
    if (name.find(" ") > 1000 || name.find(" ") == 0){
      printf("\nInvalid Input (Please Enter a First and Last Name)\n\n");
      getStudentName();
      break;
    }
    cout << endl;
    inputData();
    newStudent.testScore = testScore;
    classList.push_back(newStudent);
  }
}
// getMean() returns double mean
double getMean(){
  double total = 0;
  for (int i = 0; i < classList.size(); i++){
    total += atof((classList.at(i).testScore).c_str());
  }
  mean = total/(double)classList.size();
  return mean;
}
// getStdDev() returns double deviation
double getStdDev(){
  double tempDeviation = 0;
  for (int i = 0; i < classList.size(); i++){
    tempDeviation += pow((atof((classList.at(i).testScore).c_str()) - mean), 2.0);
  }
  deviation = sqrt(tempDeviation / (double)classList.size());
  return deviation;
}
// getFirstName() finds characters before the " " of the input name.  Used to make output cleaner.
string getFirstName(string s){
  int spaceIndex = s.find(" ");
  s.erase(s.begin() + spaceIndex, s.begin() + s.size());
  return s;
}
// getLastName() finds characters after the " " of the input name.  Used to make output cleaner.
string getLastName(string s){
  int spaceIndex = s.find(" ");
  s.erase(s.begin(), s.begin() + spaceIndex + 1);
  return s;
}
// inputData() calls getFirstName() and getLastName() and verifies that values testScore fall within parameters, loops to reenter scores if out of bounds.
void inputData(){
  checkTestScore = "";
  testScore = "";
  newStudent.firstName = getFirstName(name);
  newStudent.lastName = getLastName(name);
  cout << "Enter test score: ";
  getline(cin, testScore);
  for (int i = 0; i < testScore.length(); i++){
    if (testScore.at(i) < 48 || testScore.at(i) > 57){
      printf("\nInvalid Score Entered (Please Enter only Positive Integers)\n\n");
      inputData();
    }
  }
  if(testScore == ""){
    printf("\nPlease Enter a Score:\n\n");
    inputData();
  }
  cout << endl;
  if (atoi((testScore).c_str()) > 100 || atoi((testScore).c_str()) < 0){
    printf("Invalid Score Entered (Please Enter Score Between 0 - 100)\n\n");
    inputData();
  }
}
// calculateGrades() calculates grading scale and outputs student list with associated grades and mean/standard deviation.
void calculateGrades(){
  printf("Grading scale\n\n");
  float a = (float)(mean + 1.5*deviation);
  float b = (float)(mean + 0.5*deviation);
  float c = (float)(mean - 0.5*deviation);
  float d = (float)(mean - 1.5*deviation);
  if (a > 100){
    printf("A  above 100.0");  cout << ("%") << endl << endl;
  }
  else{
    printf("A  above %4.1f", a);  cout << ("%") << endl << endl;
  }
  if (a > 100){
    printf("B  %4.1f", b); cout << "%"; printf(" - 100.0"); cout << ("%") << endl << endl;
  }
  else{
    printf("B  %4.1f", b); cout << "%"; printf(" - %4.1f", a); cout << ("%") << endl << endl;
  }
  printf("C  %4.1f", c); cout << "%"; printf(" - %4.1f", b); cout << ("%") << endl << endl;
  if (d < 0){
    printf("D  %4.1f", b); cout << "%"; printf(" - 0.0"); cout << ("%") << endl << endl;
  }
  else{
    printf("D  %4.1f", d); cout << "%"; printf(" - %4.1f", c); cout << ("%") << endl << endl;
  }
  if (d < 0){
    printf("F  below 0.0");  cout << ("%") << endl << endl;
  }
  else{
    printf("F  below %4.1f", d);  cout << ("%") << endl << endl << endl;
  }

  printf("Name                   Score     Grade\n");
  cout << endl;
  for (int i = 0; i < classList.size(); i++){
    string tempGrade = "F";
    cout.width(2); cout << left << i+1 << ". ";
    cout.width(12); cout << classList.at(i).firstName;
    cout.width(12); cout << classList.at(i).lastName;
    cout.width(10); cout << classList.at(i).testScore;
    if (atof((classList.at(i).testScore).c_str()) > d){
      tempGrade = "D";
    }
    if (atof((classList.at(i).testScore).c_str()) > c){
      tempGrade = "C";
    }
    if (atof((classList.at(i).testScore).c_str()) > b){
      tempGrade = "B";
    } 
    if (atof((classList.at(i).testScore).c_str()) > a){
      tempGrade = "A";
    }
    cout.width(10); cout << tempGrade << endl << endl;
  }
  cout << endl;
  cout << "Class Average: " << setprecision(4) << getMean() << "%" << endl << endl;
  cout << "Standard Deviation: " << setprecision(4) << getStdDev() << endl << endl;
}
