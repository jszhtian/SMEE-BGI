// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;


int main(int argc, char *argv[])
{
	const char Splitflag1[1] = {0x20};//def
	const char Splitflag2[2] = {0x0d,0x0a};
	const int MAX = 65536;
	const int Scripthead = 0x40;
	const int Scriptidentify = 0x1C;
	char Flag01[] = { 0x3,0x0,0x0,0x0 };
	const int sizeofFlag01 = 4;
	const int sizeofAdd = 4;
	string addide = "Text Address:";
	string allocide = "Allocation Size:";
	string textide = "Text:";




	ofstream flog("Report.log");

	if (argc == 1)
	{
		cout << "Usage:" << argv[0] << " BGIScriptFile" <<" GBKTXTFile"<< endl;
		return 0;
	}
	if (argc == 2|argc>=4)
	{
		cout << "Incorrect parameter!";
		return 0;
	}//Parameter Check

	cout << "This Application will directly copy the RAW data. Please Check Splitflag!" << endl;
	fstream TFile1(argv[1], ios::binary | ios::in | ios::out);
	fstream TFile2(argv[2], ios::in | ios::out);
	if (TFile1.fail())
	{
		cout << "Open Script Fail:File " << argv[1];
		flog << "Open Script Fail:File " << argv[1];
		return 0;
	}
	if (TFile2.fail())
	{
		cout << "Open Text Fail:File " << argv[2];
		flog << "Open Text Fail:File " << argv[2];
		return 0;
	}
	cout << "Target Script File:" << argv[1] << " Open Success!" << endl;
	flog << "Target Script File:" << argv[1] << " Open Success!" << endl;
	cout << "Target Text File:" << argv[2] << " Open Success!" << endl;
	flog << "Target Text File:" << argv[2] << " Open Success!" << endl;//Open File

	int Filesize;
	TFile1.seekg(0, ios::end);
	Filesize = TFile1.tellg();
	cout << "File Script Size:" << Filesize << "Byte" << endl;
	flog << "File Script Size:" << Filesize << "Byte" << endl;
	//file size
	CHAR* TMPBlock;
	TMPBlock = new char[Scriptidentify];
	TFile1.seekg(0x0, ios::beg);

	try
	{
		TFile1.read(TMPBlock, Scriptidentify);
	}
	catch (exception ex)
	{
		cout << "Read operation Failed!";
		return -1;
	}

	if (strcmp(TMPBlock, "BurikoCompiledScriptVer1.00") == 0)
	{
		cout << "Target Script File is a BGI V2 Script File!" << endl;
		flog << "Target Script File is a BGI V2 Script File!" << endl << endl;
	}
	else
	{
		cout << "Illegal File or BGI V1 Script File!" << endl;
		flog << "Illegal File or BGI V1 Script File!" << endl;
		return 0;
	}
	delete[] TMPBlock;
	//Check File Format



	cout << "Replace Text!" << endl;//start Replace!
	flog << "Replace Text!" << endl;
	char buffer[MAX];
	string Proc;
	while (!TFile2.eof())
	{
		cout << endl;
		int sizeofreplacetext = 0;
		TFile2.getline(buffer, MAX);
		int addressoffset = 0;
		int addressend = 0;
		Proc = buffer;
		sizeofreplacetext = strlen(buffer);
		cout <<"Total Size:"<<hex<< sizeofreplacetext << endl;
		cout <<"Processing Command:"<<Proc;
		cout << endl;
		addressoffset = Proc.find(addide);
		if (addressoffset==Proc.npos)
		{
			cout << "Illegal Command detected!" << endl;
			flog << "Illegal Command detected!" << endl;
			cout << "Target:" << Proc<<endl;
			flog << "Target:" << Proc << endl;
			break;
		}
		//cout << "Address Offset:" << hex << addressoffset+addide.length()<<endl;
		addressend = Proc.find(allocide);
		//cout << "Address End:" << hex << addressend-1 << endl;
		//cout << "Address length:" << hex << addressend - addressoffset - addide.length() -1<< endl;
		char address[128] = {0};
		Proc.copy(address, addressend - addressoffset - addide.length() - 3, addressoffset + addide.length()+2);
		address[addressend - addressoffset - addide.length() - 1] = '\0';
		//cout << "Address(Char):" << address<<endl;
		int Adds = 0;
		Adds = strtol(address,NULL,16);//Char2HEX
		cout << "Address(Int):0x" << hex << Adds << endl;
		int TextAllo = 0;
		TextAllo = Proc.find(textide);
		char Allo[128] = { 0 };
		Proc.copy(Allo, TextAllo - 1 - addressend - allocide.length(),addressend+allocide.length());
		int tex = 0;
		tex = strtol(Allo, NULL, 16);
		cout << "Allocation(Int):0x"<<hex<<tex<< endl;
		char Text[65536] = { 0 };
		int TextSize = 0;
		TextSize = sizeofreplacetext - TextAllo - textide.length();
		cout << "Replace Text Size(int): 0x" << hex << TextSize << endl;

		if (TextSize>tex)
		{
			cout << "Out of Range! Please check the text length! Target:" << Proc << endl;
			flog << "Out of Range! Please check the text length! Target:" << Proc << endl;
			break;
		}

		Proc.copy(Text, TextSize, TextAllo + textide.length());
		cout << "Replace Text:" << Text << endl;
		
		cout << "Writing to BGI Script!!" << endl;

		char* CHK[3] = { 0x0,0x0,0x0 };
		char* CHKPT;
		CHKPT = new char[3];

		TFile1.seekg(Adds + tex, ios::beg);
		TFile1.read(CHKPT, 1);
		if (memcmp(CHKPT, CHK, 1) != 0)
		{
			cout << "Mismatched Script! Target:" << Proc << endl;
			flog << "Mismatched Script! Target:" << Proc << endl;
			break;
		}//File check


		TFile1.seekp(Adds, ios::beg);
		TFile1.write(Text, tex);



		TFile1.seekg(Adds + tex, ios::beg);
		TFile1.read(CHKPT, 1);
		if (memcmp(CHKPT,CHK,1)!=0)
		{
			cout << "Unexpected Error! please check the File! Target:" << Proc << endl;
			flog << "Unexpected Error! please check the File! Target:" << Proc << endl;
			break;
		}//Write Check





	}




	TFile1.close();
	TFile2.close();
	cout << "File Closed!" << endl;
	flog << "File Closed!" << endl;
    return 0;
}

