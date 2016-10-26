# Readme



## Installation
- Extract the files to a location of your choosing
- Open a terminal and `cd` to the extracted files
- Run `sudo make` to install
- Close and reopen the terminal to load the changes
- You can now remove or archive the extracted files
- (optional) Set your language (see below) (may get errors if you don't)
- (optional) Run `/opt/setport/testing/tester` to run the tests (may fail if above step not done)


## Setting the language
The tool checks the following environment variables to decide which language to use:
- LANGUAGE
- LC_ALL
- LC_MESSAGES
- LANG

The first variable that contains a valid locale will decide which language the tool will use. If your system already has one set correctly, then the tool will use that without issue. If none are set though, you may see an error and the tool will default to English.

To override the default language (and/or suppress the error message) select one of the variables and use the `export` command line utility to set it. Examples: `export LANGUAGE="en"` or `export LC_ALL="es_MX"`


## Use
Type `setport -h` at a terminal to view the usage page.


## Installing the tester
The tester isn't bult by default. To install it:
- Extract the files to a location of your choosing
- Open a terminal and `cd` to the extracted files
- Run `sudo make tester` to install
- You can now remove or archive the extracted files


## Running the tester
Type `/opt/setport/testing/tester` in the terminal to run the tests.


## Alternate languages
The tool can be run in multiple languages. It checks the environment variables `LANGUAGE`, `LC_ALL`, `LC_MESSAGES` and `LANG` (in that order) for a valid locale (e.g. `en_US` or `es`), extracts the language and loads the appropriate language files based on the result. By setting the value of these variables, the tool's language can be changed.


## Translating
Language files are installed to `/opt/setport/lang/`. A set of English files and a sample set of Spanish ones (machine translated) are included. These files can be modified to change the output of the tool for the curently installed languages.

Support for new languages can be added by dropping the appropriately named files in among the existing ones. To add a language with the two-letter language code `xx`, you will ned to add the files `/opt/setport/lang/xx_usage.txt` and `/opt/setport/lang/xx_strings.txt`. The existing files can be used as a guide.

Notes:
- Each line in the `xx_strings.txt` file is a different message, while the entire contents of the `xx_usage.txt` file is all one message.
- The special sequence `{0}` in the `xx_strings.txt` file is replaced with extra data relevant to the message, such as the flag for a message about unrecognized flags.
- If new language files are added to the `/lang` folder of the package, they will be automatically included in new installations.
