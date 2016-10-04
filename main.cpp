#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <deque>

#include "localization.h"

using namespace std;

const int MIN_PORT = 1;
const int MAX_PORT = 65535;
const int MAX_PORT_DIGITS = 5;

deque<string> argsToDeque(int argc, char* args[]);
int handleHelpFlag(deque<string> args, localization& loc);
int handleAboutFlag(deque<string> args, localization& loc);
int handleVersionFlag(deque<string> args, localization& loc);
int handlePortFlag(deque<string> args, localization& loc);
int handleEnvPort(deque<string> args, localization& loc);
bool isPort(const string& port);

int main(int argc, char* args[]) {

	localization loc;
	deque<string> dArgs = argsToDeque(argc, args);
	dArgs.pop_front(); // skip the program's name
		
	string flag;
	
	if (dArgs.empty()) {
		flag = "-h";
	} else {
		flag = dArgs.front();
		dArgs.pop_front();
	}

	if (flag == "-h" || flag == "--help" || flag == "-?") {
		return handleHelpFlag(dArgs, loc);
	}

	if (flag == "-!") {
		return handleAboutFlag(dArgs, loc);
	}

	if (flag == "-v" || flag == "--version") {
		return handleVersionFlag(dArgs, loc);
	}

	if (flag == "-p" || flag == "--port") {
		return handlePortFlag(dArgs, loc);
	}

	loc.error(UNKNOWN_FLAG, flag);
	return 1;
}

deque<string> argsToDeque(int argc, char* args[]) {
	deque<string> dArgs;

	for (int i = 0; i < argc; ++i) {
		dArgs.push_back(string(args[i]));
	}

	return dArgs;
}

int handleHelpFlag(deque<string> args, localization& loc) {
	if (args.empty()) {
		loc.success(USAGE);
		return 0;
	}

	loc.error(HELP_EXTRA_PARAMS);
	return 1;
}

int handleAboutFlag(deque<string> args, localization& loc) {
	if (args.empty()) {
		loc.success(ABOUT);
		return 0;
	}

	loc.error(ABOUT_EXTRA_PARAMS);
	return 1;
}

int handleVersionFlag(deque<string> args, localization& loc) {
	if (args.empty()) {
		loc.success(VERSION);
		return 0;
	}

	loc.error(VERSION_EXTRA_PARAMS);
	return 1;
}

int handlePortFlag(deque<string> args, localization& loc) {
	if (args.empty()) {
		loc.error(MISSING_PORT);
		return 1;
	}

	string port = args.front();
	args.pop_front();

	if (port == "-e") { return handleEnvPort(args, loc); }
	if (isPort(port) == false) {
		loc.error(ILLEGAL_PORT, port);
		return 1;
	}
	if (args.empty() == false) {
		loc.error(PORT_EXTRA_PARAMS);
		return 1;
	}

	loc.success(LISTENING, port);
	return 0;
}

int handleEnvPort(deque<string> args, localization& loc) {
	string envName;

	if (args.empty()) {
		envName = "PORT";
	} else {
		envName = args.front();
		args.pop_front();
	}

	if (args.empty() == false) {
		loc.error(ENV_EXTRA_PARAMS);
		return 1;
	}

	char* cPort = getenv(envName.c_str());
		
	if (cPort == nullptr) {
		loc.error(ENV_DOES_NOT_EXIST, envName);
		return 1;
	}
	
	string port = cPort;
	
	if (isPort(port) == false) {
		loc.error(ENV_ILLEGAL_PORT, port);
		return 1;
	}
	
	loc.success(LISTENING, port);
	return 0;
}

bool isPort(const string& port) {
	for (auto digit : port) {
		if (isdigit(digit) == false) { return false; }
	}
	
	// sooner or later someone will try to set port
	// 10295612093581723049182356102938479821651098
	// and bad things will happen to stoi()
	if (port.length() > MAX_PORT_DIGITS) { return false; }
	
	int numericPort = stoi(port);
	
	if (numericPort < MIN_PORT || numericPort > MAX_PORT) { return false; }
	
	return true;
}
