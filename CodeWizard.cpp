#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "nc475_Library.h"

using namespace std;
void headerFile(string className, ofstream& outFile, const vector<string> privateTypes, const vector<string> privateNames);
void implementationFile(string className, ofstream& outFile, const vector<string> privateTypes, const vector<string> privateNames);
void sampleFile();

int main(void)
{

	//----Variables---------
	string userSpecFile, impFileName, headFileName, sampleYes, ready;
	ifstream userSpec;
	ofstream impGenFile, headGenFile;
	const string delimiter = "%%%%%";

	string className;
	vector<string> privateTypes;
	vector<string> privateNames;
	string tempType, tempName, tempString;
	vector<string> standardIncludes;
	vector<string> comments;
	//----------------------

	//--------------------Greet the user-----------
	cout << "Welcome to Code Wizard! Simply point me to the right file and I will generate code for you." << endl;
	//---------------------------------------------

	//--------Generating a sample File-------------
	cout << "Do you want to generate a sample file? (Yes/ No):  ";
	cin >> sampleYes;
	if ((sampleYes == "Yes") || (sampleYes == "yes") || (sampleYes == "y") || (sampleYes == "Y"))
	{
		sampleFile();
		cout << "A sample file named \"sample.txt\" has been saved into the current directory.\n Edit this with your desired information and then continue with this program." << endl;
	}
	//---------------------------------------------

	//--------Setting up the includes--------------
	standardIncludes.push_back("#include <string>");
	standardIncludes.push_back("#include <iostream>");
	standardIncludes.push_back("#include <fstream>");
	standardIncludes.push_back("#include <vector>");
	standardIncludes.push_back("#include <sstream>");
	standardIncludes.push_back("using namespace std;");
	//---------------------------------------------

	//--------Get File name and prepare for reading------
	cout << "Enter the name of the specification file (without the extension): ";
	cin >> tempString;
	userSpecFile = tempString + ".txt";

	userSpec.open(userSpecFile);
	//---------------------------------------------------

	//--------------Read Data from the spec file---------
	userSpec >> tempString;
	userSpec.ignore();

	if (tempString == "{C}")
	{
		getline(userSpec, tempString);
		while (tempString != delimiter)
		{
			comments.push_back(tempString);
			getline(userSpec, tempString);
		}
	}

	userSpec >> tempString;

	if (tempString == "{CN}")
	{
		userSpec >> className;
	}

	userSpec >> tempString >> tempString;

	if (tempString == "{P}")
	{
		userSpec >> tempType;
		while (tempType != delimiter)
		{
			privateTypes.push_back(tempType);
			userSpec >> tempString;
			tempName = tempString + "_";
			privateNames.push_back(tempName);
			userSpec >> tempType;
		}
	}

	impFileName = className + ".cpp";
	headFileName = className + ".h";

	bool headExist = createNewFile(headGenFile, headFileName);
	bool impExist = createNewFile(impGenFile, impFileName);

	if ((headExist == false) || (impExist == false))
	{
		cout << "Feel free to edit your files and then run me again!" << endl;
		return 0;
	}

	else
	{
		//---------------------------------------------------

		//--------Set up header file to prevent duplication------
		headGenFile << "#ifndef _-" + className + "__" << endl;
		headGenFile << "#define _-" + className + "__" << endl;
		//-------------------------------------------------------

		//--------Adding user comments to the header file--------
		for (int i = 0; i < comments.size(); i++)
		{
			headGenFile << "// " << comments[i] << endl;
		}
		//-------------------------------------------------------

		//---------------Including the #includes-------------
		for (int i = 0; i < standardIncludes.size(); i++)
		{
			headGenFile << standardIncludes[i] << endl;
			impGenFile << standardIncludes[i] << endl;
		}
		//---------------------------------------------------

		//-----------Call code generation subprograms--------
		headerFile(className, headGenFile, privateTypes, privateNames);
		implementationFile(className, impGenFile, privateTypes, privateNames);
		//---------------------------------------------------

		//----------Close files------------------------------
		userSpec.close();
		impGenFile.close();
		headGenFile.close();
		//---------------------------------------------------

		cout << "Your files have been generated successfully!" << endl;
	}

	return 0;
}

void headerFile(string className, ofstream& outFile, const vector<string> privateTypes, const vector<string> privateNames)
{

	vector<string> inspectorNames;

	char date[9];
	_strdate_s(date);
	outFile << "//Code generated by Code Wizard (Copyright Nischaal Cooray 2014)" << endl;
	outFile << "//Generated on " << date << endl;

	outFile << endl << endl;

	outFile << "class " << className << endl;
	outFile << "{" << endl;
	//-----------Public Methods--------------
	outFile << "public: " << endl << endl;

	//-----------Constructors----------------
	outFile << "//-----------Constructors----------------" << endl;
	outFile << "//-----------Add more if necessary-------" << endl;

	outFile << "             " << className << "();" << endl;
	outFile << "             " << className << "(";
	for (int i = 0; i < privateTypes.size(); i++)
	{
		int lastPos = privateNames[i].find('_');
		if (i == 0)
			outFile << privateTypes[i] << " " << privateNames[i].substr(0,lastPos);
		else
			outFile << ", " << privateTypes[i] << " " << privateNames[i].substr(0,lastPos) << " = 0";
	}
	outFile << ");		//Change the default values as needed." << endl;
	
	outFile << "             " << className << "(" << className << "& " << className.substr(0, 3) << ");" << endl;
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;

	//----------Destructor-------------------
	outFile << "//----------Destructor-------------------" << endl;
	outFile << "             " << "~" << className << "();" << endl;
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;

	//-----------Inspectors------------------
	outFile << "//-----------Inspectors------------------" << endl;
	for (int i = 0; i < privateTypes.size(); i++)
	{
		int lastPos = privateNames[i].find('_');
		outFile << "             " << privateTypes[i] << " get" << privateNames[i].substr(0, lastPos) << "() const;" << endl;
	}
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;

	//-----------Mutators--------------------
	outFile << "//-----------Mutators--------------------" << endl;
	for (int i = 0; i < privateTypes.size(); i++)
	{
		int lastPos = privateNames[i].find('_');
		outFile << "             " << "void " << "set" << privateNames[i] << "(const " << privateTypes[i] << "& " << privateNames[i].substr(0, lastPos) << ");" << endl;
	}
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;

	//--------Facilitators-------------------
	outFile << "//--------Facilitators-------------------" << endl;
	outFile << "             " << "void output(ostream& out);" << endl;
	outFile << "             " << "string toString(ostream& out) const;" << endl;
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;
	//------End of Public Methods------------

	//-----------Private Data----------------
	outFile << "private: " << endl;
	for (int i = 0; i < privateTypes.size(); i++)
	{
		outFile << "             " << privateTypes[i] << " " << privateNames[i] << ";" << endl;
	}
	//-----------End of Private Data---------

	outFile << "}" << endl;

	outFile << endl << endl;

	//----------Operator Overloads-----------
	outFile << "//----------Operator Overloads-----------" << endl;
	outFile << "ostream& operator<<(ostream & out, const " << className << "& " << className.substr(0, 2) << ");" << endl;
	outFile << "//---------------------------------------" << endl;
	//---------------------------------------

	outFile << endl << endl;

	outFile << "#endif";
}

void implementationFile(string className, ofstream& outFile, const vector<string> privateTypes, const vector<string> privateNames)
{
	//----------------------------------------------
	outFile << "#include \"" << className << ".h\"";
	//----------------------------------------------
	outFile << endl << endl;

	//-----------Constructors----------------
	outFile << "//-----------Constructors----------------" << endl;
	outFile << className << "::" << className << "() \n{			//Default Constructor" << endl;
	for (int i = 0; i < privateNames.size(); i++)
	{
		outFile << "		" << privateNames[i] << " = 0;	//Edit value as necessary" << endl;
	}

	outFile << "};" << endl;

	outFile << endl << endl;

	outFile << className << "::" << className << "(const " << className << "& " << className.substr(0, 3) << ")\n{			//Copy constructor" << endl;
	outFile << endl;
	outFile << "//Shallow copy" << endl;
	for (int i = 0; i < privateNames.size(); i++)
	{
		outFile << "          " << privateNames[i] << " = " << className.substr(0, 3) << "." << privateNames[i] << ";" << endl;
	}
	outFile << "}" << endl;
	outFile << "//-------------------------------------------------" << endl;
	//----------------------------------------------

	outFile << endl << endl;

	//----------Destructor-------------------
	outFile << "//----------Destructor-------------------" << endl;
	outFile << className << "::~" << className << "()\n{" << endl;
	outFile << "//Insert any code here to be run when the destructor is called." << endl;
	outFile << "}" << endl;
	outFile << "//-------------------------------------------------" << endl;
	//----------------------------------------------

	outFile << endl << endl;

	//-----------Inspectors------------------
	outFile << "//----------------Inspectors-----------------------" << endl;

	for (int i = 0; i < privateTypes.size(); i++)
	{
		int lastPos = privateNames[i].find('_');
		outFile << privateTypes[i] << " " << className << "::get" << privateNames[i].substr(0, lastPos) << "() const\n{		//Inspector for the " << privateNames[i].substr(0, lastPos) << " variable" << endl;
		outFile << "          " << "return " << privateNames[i] << endl;
		outFile << "}" << endl << endl;
	}
	outFile << "//-------------------------------------------------" << endl;
	//----------------------------------------------

	//-----------Mutators--------------------
	outFile << "//--------------------Mutators---------------------" << endl;

	for (int i = 0; i < privateTypes.size(); i++)
	{
		int lastPos = privateNames[i].find('_');
		outFile << "void " << className << "::set" << privateNames[i].substr(0, lastPos) << "(const " << privateTypes[i] << "& " << privateNames[i].substr(0, lastPos) << ")\n{			//Mutator for the " << privateNames[i].substr(0, lastPos) << " variable" << endl << endl;

		if (privateTypes[i].substr(0, 6) == "vector")
		{
			outFile << "          " << privateNames[i] << ".push_back(" << privateNames[i].substr(0, lastPos) << ");" << endl;
		}
		else
		{
			outFile << "          " << privateNames[i] << " = " << privateNames[i].substr(0, lastPos) << ";" << endl;
		}
		outFile << "}" << endl << endl;
	}
	outFile << "//-------------------------------------------------" << endl;
	//----------------------------------------------

	//--------Facilitators-------------------
	outFile << "//--------------------Facilitators---------------------" << endl;

	outFile << "void " << className << "::toString(ostream& out) const \n{" << endl;
	outFile << "          " << "//Insert code here to output your desired data." << endl;
	outFile << "}" << endl;

	outFile << endl << endl;

	outFile << "void " << className << "::output(ostream& out) const\n{" << endl;
	outFile << "          " << "out << &" << className << "::toString;" << endl;
	outFile << "}" << endl;

	outFile << endl << endl;

	outFile << "ostream& operator<<(ostream& out, const " << className << "& outputObject)\n{" << endl;
	outFile << "          " << "outputObject.output(out);" << endl;
	outFile << "          " << "return out;\n}" << endl;
	outFile << "//-------------------------------------------------" << endl;
	//----------------------------------------------

}

void sampleFile()
{
	ofstream sampleFile("sample.txt");

	sampleFile << "[Use this sample file to detail the data necessary to construct your class.\n Delete any statements surrounded by parentheses (\"[]\").]" << endl;
	sampleFile << "{C}" << endl;
	sampleFile << "[Enter any information here that you would like to display at the top of your generated header file.]" << endl;
	sampleFile << "%%%%%" << endl;
	sampleFile << "{CN}		[Enter the name of your class.]" << endl;
	sampleFile << "%%%%%" << endl;
	sampleFile << "{P}" << endl;
	sampleFile << "[Enter the names of any private data for your class, each on a new line.]" << endl;
	sampleFile << "%%%%%";

	sampleFile.close();
}