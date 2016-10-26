all:
	make translation
	make setport
	make shellAlias

translation:
	sudo mkdir -p /opt/setport
	sudo cp -r lang /opt/setport/lang
	sudo chmod -R a=rX /opt/setport/lang

setport: main.cpp localization.cpp
	sudo g++-5 -Wall -std=c++11 main.cpp localization.cpp -o /opt/setport/main

shellAlias:
	touch ~/.bash_aliases
	sudo grep -q -F 'alias setport="/opt/setport/main"' ~/.bash_aliases || echo 'alias setport="/opt/setport/main"' >> ~/.bash_aliases

tester: tester.cpp
	sudo mkdir -p /opt/setport/testing
	sudo g++-5 -Wall -std=c++11 tester.cpp -o /opt/setport/testing/tester
	sudo cp tests.txt /opt/setport/testing/tests.txt
