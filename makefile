all:
	make lang	
	make setport
	make shellAlias

lang:
	sudo mkdir -p /opt/setport
	sudo cp -r lang /opt/setport/lang
	sudo chmod -R a=rX /opt/setport/lang

setport: main.cpp localization.cpp
	sudo g++ -Wall -std=c++11 main.cpp localization.cpp -o /opt/setport/main

shellAlias:
	touch ~/.bash_aliases
	sudo grep -q -F 'alias setport="/opt/setport/main"' ~/.bash_aliases || echo 'alias setport="/opt/setport/main"' >> ~/.bash_aliases

tester: tester.cpp
	sudo mkdir /opt/setport/testing
	sudo g++ -Wall -std=c++11 tester.cpp -o /opt/setport/testing/tester
	sudo cp tests.txt /opt/setport/testing/tests.txt

