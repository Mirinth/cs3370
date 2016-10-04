#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

const bool PASS = true;
const bool FAIL = false;
const string WORKING_DIRECTORY = "/opt/setport/lang";
const string TESTER_DIRECTORY = "/opt/setport/testing";
const string PREDICTED = "/tmp/predicted.txt";
const string ACTUAL = "/tmp/actual.txt";
bool runTest(const string& params, const string& compareFile, int expect);


struct test {
	string command;
	string output;
	int exitCode;
};

vector<test> loadTests();

void prepOutput(test t);
int runTest(test t);
bool compareResults(test t, int exitCode);


int main(int argc, char* args[]) {
	chdir(WORKING_DIRECTORY.c_str());
	vector<test> tests = loadTests();
	bool result = PASS;

	for (auto t : tests) {
		prepOutput(t);
		int exitCode = runTest(t);
		result = compareResults(t, exitCode);
		if (result == FAIL) { break; }
	}

	cout << endl << endl;
	if (result == PASS) {
		cout << "All tests passed" << endl;
		return 0;
	} else {
		cout << "Tests failed" << endl;
		return -1;
	}
}

vector<test> loadTests() {
	vector<test> tests;
	ifstream testFile;
	testFile.open(TESTER_DIRECTORY + "/tests.txt");

	while (testFile.good() && !testFile.eof()) {
		test t;
		string line;

		getline(testFile, line);
		t.command = line;

		getline(testFile, line);
		t.output = line;

		getline(testFile, line);
		t.exitCode = stoi(line);

		getline(testFile, line); // discard blank line between records

		tests.push_back(t);
	}

	return tests;
}

void prepOutput(test t) {
	string outputGenerator = t.output + " > " + PREDICTED;
	system(outputGenerator.c_str());
}

int runTest(test t) {
	string testCommand = t.command + " > " + ACTUAL;

	// note: system() puts the program's exit code in the
	// high byte of its own return value, so the result of
	// calls to system() have to be divided by 256 to get
	// at the exit code.
	int exitCode = system(testCommand.c_str()) / 256;

	return exitCode;
}

bool compareResults(test t, int exitCode) {
	string diffCommand = "cmp " + PREDICTED + " " + ACTUAL;
	int diff = system(diffCommand.c_str()) / 256;
    
	if (exitCode != t.exitCode) {
		cout << "[fail] " << t.command << " | bad return: " << exitCode << endl;
		return FAIL;
	}

	if (diff != 0) {
		string catPredicted = "cat " + PREDICTED;
		string catActual = "cat " + ACTUAL;
		cout << "[fail] " << t.command << " | expected output: " << endl;
		system(catPredicted.c_str());
		cout << endl << "Got output: " << endl;
		system(catActual.c_str());
		cout << endl << endl << endl;
		return FAIL;
	}

	cout << "[pass] " << t.command << endl;
	return PASS;
}
