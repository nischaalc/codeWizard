/*
File: nc475_Library.h
Author: Nischaal Cooray
Date: 2014/02/24
Version: 1.0
*/

/* Copyright 2014 Nischaal Cooray*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#pragma once

using namespace std;

/*
	Prompts the user for specific input and returns the input string to the calling program.
	@param Prompt:	the string to be displayed to the user to request specific input
	@return:		the string entered by the user after the prompt

*/

static string promptForString(string Prompt)
{
	string userInput;

	cout << Prompt;
	cin.ignore();
	getline(cin, userInput);

	return userInput;

}

bool createNewFile(ofstream& out, string filename)
{
	ifstream fin;
	fin.open(filename);
	int userAnswer;

	if (fin.good())
	{
		cout << "The file " << filename << " already exists." << endl;
		cout << "Do you want to overwrite it or cancel? (1 for overwrite, 2 for cancel): ";
		cin >> userAnswer;

		if (userAnswer == 1)
		{
			out.open(filename);
			return true;
		}
		else
			return false;

	}
	else
	{
		out.open(filename);
		return true;
	}

}
