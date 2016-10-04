#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <vector>
#include <string>
#include <ostream>

using namespace std;

enum message {
	// failures
	HELP_EXTRA_PARAMS,
	ABOUT_EXTRA_PARAMS,
	VERSION_EXTRA_PARAMS,
	MISSING_PORT,
	ILLEGAL_PORT,
	PORT_EXTRA_PARAMS,
	ENV_EXTRA_PARAMS,
	ENV_DOES_NOT_EXIST,
	ENV_ILLEGAL_PORT,
	UNKNOWN_FLAG,
	UNKNOWN,

	// successes
	ABOUT,
	LISTENING,

	// special cases
	// must be last and in this order
	// update localization::loadStrings()
	// if changed
	VERSION,
	USAGE,
};

class localization {
	public:
		localization();
		void success(message msg, const string& insert="");
		void error(message msg, const string& insert="");
	private:
		string language;
		vector<string> localizedStrings;
		void getLanguage();
		void loadLanguage();
		void loadStrings();
		void loadVersion();
		void loadUsage();
		string replace(const string& subject, const string& find, const string& replace);
};

#endif
