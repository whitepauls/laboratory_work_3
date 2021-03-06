/* File contains part of code borrowed from condition of Labaratory Work 3 (page 14),
that represents template of handling -output, -stat commands.
From 12 line to 18 line
*/

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstring>
#include "Info.h"
#include "Builder.h"
#include "writersInStream.h"

const char *  copyright() {
	return "Author: Pavlo Bilinskyi, student of Taras Shevchenko National University of Kyiv. Faculty of CSC, specialization Applied Mathematics";
}

void help() {
	std::cout<< "Call the program with a string like <input_file_name> {<command> <name_of_file>}, "<<std::endl;
	std::cout << "where part in parentheses {} can appear arbitrary many times." << std::endl;
	std::cout << "Here are possible commands: " << std::endl;
	std::cout << "	-output (call it for printing the information into file)." << std::endl;
	std::cout << "	-stat (call it for printing into file some statistics from input file data)." << std::endl;
}

//post: if loading failed, exception is thrown
void input(Info& inf, const char* filename) {
	Builder bild;
	std::cout << "input " << filename << " : ";
	bild.loadData(inf, filename);
	std::cout << "OK" << std::endl;
}

using InfoResults = std::ostream & (std::ostream&, Info&);

bool do_command(Info& inf, const char* fname, InfoResults infRes) {
	if (std::strcmp(fname, "#con") == 0) return bool(infRes(std::cout, inf));
	std::ofstream os(fname);
	bool res = bool(infRes(os, inf)); 
	os.close(); 
	if (os.fail()) res = false;
	return res;
}

void commandHandler(const char* command, const char* filename, Info& inf) {
	std::cout << (++command) << " " << filename << " : ";
	if (std::strcmp(filename, "#con") == 0) std::cout << std::endl;
	bool flag = false;
	if (std::strcmp(command, "output") == 0) {
		flag = do_command(inf, filename, outputInStream);
	}
	if (std::strcmp(command, "stat") == 0) {
		flag = do_command(inf, filename, statInStream);
	}

	if (std::strcmp(filename, "#con") != 0) {
		if (flag) std::cout << "OK" << std::endl;
		else std::cout << "UPS" << std::endl;
	}
}


void ignoreCommandMessage(const char* command) {
	std::cout << command << " : ignored" << std::endl;
}

bool isCommand(const char* command) {
	return ((std::strcmp(command, "-output") == 0) || (std::strcmp(command, "-stat") == 0));
}

void unknownCommandMessage(const char* command) {
	std::cout << command << " : ignored"<<  std::endl;
}

void notEnoughArguments(const char* command) {
	std::cout <<++command <<" undefined" << std::endl;
}

void doNothing() {
	std::cout << "nothing to do" << std::endl;
}

int main(int argc, char** argv) {
	std::cout << copyright() << std::endl;
	help();
	std::cout<<"*****"<<std::endl;
	//=======================

	if (argc == 1) doNothing();
	else {
		try {
			Info inf;
			input(inf, argv[1]);

			int i = 2;
			const char* curCommand;
			while (i < argc){
				curCommand = argv[i];
				if (isCommand(curCommand)) {
					++i;
					if (i < argc) commandHandler(curCommand, argv[i], inf);
					else notEnoughArguments(curCommand);
				}
				else unknownCommandMessage(curCommand);
				++i;
			}
		}
		catch (std::invalid_argument &ia) {
			std::cout << "UPS" << std::endl;
			std::cout << ia.what() << std::endl;
		}
	}
	//=======================
	std::cout << "*****" << std::endl;
	system("pause");
	return 0;
}