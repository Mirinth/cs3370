#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <fstream>

#include "localization.h"

const string VERSION_STRING = "3";
const string REPLACE_TOKEN = "{0}";
const string STRINGS_TEMPLATE = "/opt/setport/lang/strings_{0}.txt";
const string USAGE_TEMPLATE =   "/opt/setport/lang/usage_{0}.txt";

using namespace std;

localization::localization() {
	getLanguage();
	loadLanguage();
}

void localization::success(message msg, const string& insert) {
	string output = localizedStrings[msg];
	output = replace(output, REPLACE_TOKEN, insert);
	cout << output << endl;
}

void localization::error(message msg, const string& insert) {
	success(msg, insert);
	success(USAGE, "");
}

void localization::getLanguage() {
	std::regex re("([a-z]{2})(_[A-Z]{2})?(\\.UTF-8)?");
	std::smatch match;

	const char* const vars[] = {
		"LANGUAGE",
		"LC_ALL",
		"LC_MESSAGES",
		"LANG",
		nullptr
	};

	for (int i = 0; vars[i] != nullptr; ++i) {
		string var = getenv(vars[i]) != nullptr ? getenv(vars[i]) : "";

		if (var == "" || var == "C" || var == "C.UTF-8") {
			continue;
		}

		if (std::regex_match(var, match, re)) {
			language = match.str(1);
			return;
		}
	}

	cout << "Error: Could not determine locale; using default: en" << endl;
	language = "en";
}

void localization::loadLanguage() {
	// these will break if they're changed without
	// updating the messages enum to match
	loadStrings();
	loadVersion();
	loadUsage();
}

void localization::loadStrings() {
	string fileName = replace(STRINGS_TEMPLATE, REPLACE_TOKEN, language);
	
	ifstream languageFile;
	languageFile.open(fileName);

	// translating the coutput below would be nice, but since they
	// only happen when loading the translation files failed, it
	// isn't exactly practical..
	if (languageFile.fail() && language == "en") {
		cout << "Fatal error: default language missing" << endl;
		exit(-1);
	}

	if (languageFile.fail()) {
		cout << "'" << language << "' language files not found; using 'en'" << endl;
		language = "en";
		loadLanguage();
	}

	while (languageFile.good()) {
		string line;
		getline(languageFile, line);
		if (line.size() > 0) { localizedStrings.push_back(line); }
	}
}

void localization::loadVersion() {
	localizedStrings.push_back(VERSION_STRING);
}

void localization::loadUsage() {
	string fileName = replace(USAGE_TEMPLATE, REPLACE_TOKEN, language);
	ifstream usageFile;
	usageFile.open(fileName);

	// translating the coutput below would be nice, but since they
	// only happen when loading the translation files failed, it
	// isn't exactly practical..
	if (usageFile.fail() && language == "en") {
		cout << "Fatal error: default language missing" << endl;
		exit(-1);
	}

	if (usageFile.fail()) {
		cout << "'" << language << "' language files not found; using 'en'" << endl;
		language = "en";
		loadLanguage();
	}

	string contents = "";
	string line;
	while (getline(usageFile, line)) {
		if (contents == "") { contents = line; }
		else { contents += "\n" + line; }
	}

	localizedStrings.push_back(contents);
}

string localization::replace(const string& subject, const string& find, const string& replace) {
	string str = subject.substr(0, subject.length());

	size_t location = subject.find(find);
	while (location != string::npos) {
		str.replace(location, find.length(), replace);
		location = str.find(find, location + find.length());
	}

	return str;
}
