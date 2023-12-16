#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

const int STRING_SIZE = 100;

class MyString
{
private:
	char strval[STRING_SIZE];
	int strln;  // length of string value
	int frequency; 
public:
	//constructors
	MyString() : strln(0), frequency(1)
	{
		strcpy(strval, "");   //constructor for MyString class
	}

	MyString(char s[]);
	// initialize string value to s
	MyString(const MyString& s)
	{
		strcpy(strval, s.strval);
		strln = s.strln;
		frequency = s.frequency;
	}

	//accessor functions/operators
	int Length() const
	{
		return strln;
	}

	bool isAcceptable(char c) // only accept alphanumeric characters
	{
		return (((c >= 48) && (c <= 57)) || ((c >= 65) && (c <= 90)) || ((c >= 97) && (c <= 122)));
	}

	void ToUpper()  
	{
		int n = strln, m = 0;

		for (int i = 0; i < strln; i++)
		{

			if (isAcceptable(strval[i]))
			{
				strval[m] = toupper(strval[i]);
				m++;
			}
			else
				n--;
		}
		strln = n;
		strval[n] = '\0';
	}

	bool operator ==(MyString s) const;
	bool operator >(MyString s) const;
	bool operator <(MyString s) const;
	MyString operator +(MyString s) const;
	MyString operator =(MyString s);
	MyString operator ++(int)
	{
		frequency += 1;
		return *this;
	}

	char& operator [](int indx)
	{
		return strval[indx];
	}

	//file input/output functions
	friend ifstream& operator >>(ifstream& i, MyString& str);
	friend ofstream& operator <<(ofstream& o, MyString& str);
};


MyString::MyString(char c[])
{
	strcpy(strval, c);
	strln = strlen(c);
	frequency = 1;
}

bool MyString::operator ==(MyString s) const
{
	return (strcmp(strval, s.strval) == 0);
}

bool MyString::operator < (MyString s) const
{
	if (frequency == s.frequency)
		return (strcmp(strval, s.strval) > 0);
	else
		return (frequency < s.frequency);
}

bool MyString::operator > (MyString s) const
{
	if (frequency == s.frequency)
		return (strcmp(strval, s.strval) < 0);
	else
		return (frequency > s.frequency);
}

MyString MyString::operator + (MyString s) const
{
	char str[STRING_SIZE];

	strcpy(str, this->strval);
	strcat(str, " ");
	strcat(str, s.strval);

	return MyString(str);
}

MyString MyString::operator = (MyString s)
{
	strcpy(strval, s.strval);
	strln = s.strln;
	frequency = s.frequency;
	return *this;
}

ifstream& operator >>(ifstream& i, MyString& str)
{
	i >> str.strval;
	str.strln = strlen(str.strval);
	return i;
}

ofstream& operator <<(ofstream& o, MyString& str)
{
	for (int i = 0; i < str.strln; i++)
		o << str.strval[i];
	for (int j = 0; j < 60 - str.strln; j++)
		o << ' ';
	if (str.frequency > 1)
		o << "\t" << str.frequency;
	return o;
}
#endif

char* stringToCharacters(string s) {        //convert string type to char* type
	char* characters = new char[s.length()];
	for (int i = 0; i < s.length(); i++) {
		characters[i] = s[i];
	}
	return characters;
}

int getNumberOfWordsInFile(string fileName) {
	ifstream file;
	string current;
	file.open(fileName);
	int count = 0;
	while (file >> current) {
		char* arr = (char*)malloc(current.length() * sizeof(char));     //allocate space for char* array based on length of string 'current'
		for (int i = 0; i < current.length(); i++) {   
			arr[i] = current[i];
		}
		MyString s(arr);
		s.ToUpper();
		if (s.Length() > 0) {       
			count++;
		}
	}
	file.close();
	return count;
}

MyString* getWords(string fileName, int length) {
	ifstream file;
	string current;
	file.open(fileName);

	MyString* array = new MyString[length];     //create array of MyString types based on length argument
	int idx = 0;
	while (file >> current) {
		char* characters = stringToCharacters(current); //convert current to char*
		MyString s(characters);
		s.ToUpper();
		if (s.Length() > 0) {
			array[idx] = s;
			idx++;
		}
	}
	file.close();
	return array;
}

int main() {
	// Get user inputs
	cout << "Enter the source data file name: ";
	string sourceDataFile;
	cin >> sourceDataFile;
	cout << "How many Adjacent words in a phrase, enter 1-5: ";
	int adjacentWords;
	cin >> adjacentWords;
	cout << "Enter the phrase frequency file name: ";
	string phraseFrequencyFile;
	cin >> phraseFrequencyFile;

	// Get total number of words in the source data file
	int totalWords = getNumberOfWordsInFile(sourceDataFile);

	// Get all words in source data file in a sequence of type MyString 
	MyString* words = getWords(sourceDataFile, totalWords);

	int totalGroups = totalWords - adjacentWords + 1;
	MyString* groups = new MyString[totalGroups];
	for (int i = 0; i < totalGroups; i++) {
		if (words[i].Length() > 0) {
			MyString temp = words[i];
			for (int j = 1; j < adjacentWords; j++) {
				temp = temp + words[i + j];     
			}
			groups[i] = temp;   
		}
	}

	// Search and delete duplicates
	for (int i = 0; i < totalGroups; i++) {
		for (int j = i + 1; j < totalGroups; j++) {
			if (groups[i].Length() != 0 && groups[j].Length() != 0) {
				if (groups[i] == groups[j]) {
					groups[i]++;        
					groups[j] = MyString();  
				}
			}
		}
	}


	// Bubble sort for groups of words - Pearson 8.3 
	for (int i = 0; i < totalGroups; i++) {
		for (int j = i + 1; j < totalGroups; j++) {    
			if (groups[i] < groups[j]) {
				MyString temp = groups[j];
				groups[j] = groups[i];
				groups[i] = temp;
			}
		}
	}

	// Find total number of unique groups
	int totalUniqueGroups = 0;
	for (int i = 0; i < totalGroups; i++) {
		if (groups[i].Length() > 0) {
			totalUniqueGroups++;
		}
	}

	// Filtering out invalid values of type MyString
	MyString* uniqueGroups = new MyString[totalUniqueGroups];
	int idx = 0;
	for (int i = 0; i < totalGroups; i++) {
		if (groups[i].Length() > 0) {
			uniqueGroups[idx] = groups[i];
			idx++;
		}
	}

	// Create output file
	ofstream outputFile;
	outputFile.open(phraseFrequencyFile);   
	outputFile << "The file: " << phraseFrequencyFile << " contains " << totalWords << " Words.\n";     
	outputFile << "There are " << totalUniqueGroups << " unique terms.\n\n";
	for (int i = 0; i < totalUniqueGroups; i++) {       
		outputFile << " ";
		outputFile << uniqueGroups[i] << '\n';
	}
	outputFile.close();

	// Free memory that was used for sequences
	delete[] words;
	delete[] groups;
	delete[] uniqueGroups;

	return 0;
}
