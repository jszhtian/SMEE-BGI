// BGI-ScriptReplace.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;


int main(int argc, char *argv[])
{
	//def const
	//const int Scripthead = 0x40; Change to Autodetect
	const int Scriptidentify = 0x1C;
	char Flag01[] = {0x3,0x0,0x0,0x0};
	char HeadFlag[] = {0x1,0x0,0x0,0x0};
	const int sizeofFlag01 = 4;
	const int sizeofAdd = 4;
	const int sizeofheadflag = 4;



	if (argc==1)
    {
		cout << "Usage:" << argv[0] << " BGIScriptFile" << endl;
		return 0;
    }
	if (argc>=3)
	{
		cout << "Incorrect parameter!";
		return 0;
	}//Parameter Check


	string OutFile;
	string TMPName;
	TMPName = argv[1];
	OutFile = TMPName + ".txt";
	ofstream fout(OutFile);


	fstream TFile(argv[1], ios::binary | ios::in | ios::out);
	if (TFile.fail())
	{
		cout << "Open Fail:File " << argv[1];
		fout << "Open Fail:File " << argv[1];
		return 0;
	}
	cout << "Target File:" << argv[1] << " Open Success!" << endl;//Open File
	fout << "Target File:" << argv[1] << " Open Success!" << endl;//Open File
	int Filesize;
	TFile.seekg(0, ios::end);
	Filesize = TFile.tellg();
	cout << "File Size:" << Filesize<<"Byte"<<endl;
	fout << "File Size:" << Filesize << "Byte" << endl;
	//file size
	CHAR* TMPBlock;
	TMPBlock=new char[Scriptidentify];
	TFile.seekg(0x0,ios::beg);
	
	try
	{
		TFile.read(TMPBlock, Scriptidentify);
	}
	catch (exception ex)
	{
		cout<<"Read operation Failed!";
		return -1;
	}
	
	if (strcmp(TMPBlock, "BurikoCompiledScriptVer1.00") == 0)
	{
		cout << "Target File is a BGI V2 Script File!" << endl;
		fout << "Target File is a BGI V2 Script File!" << endl<<endl;
	}
	else
	{
		cout << "Illegal File or BGI V1 Script File!" << endl;
		return 0;
	}
	delete[] TMPBlock;
	//Check File Format

	char* TMPhead;
	int headptr = 0;
	int Scripthead = 0;
	TFile.seekg(0, ios::beg);//set ptr to 0 pos
	do 
	{
		TFile.seekg(headptr, ios::beg);
		TMPhead = new char[sizeofheadflag];
		TFile.read(TMPhead, sizeofheadflag);
		if (memcmp(TMPhead,HeadFlag,4)==0)
		{
			Scripthead = headptr;
			break;
		}
		delete[] TMPhead;
		headptr++;
	} while (headptr<Filesize);

	
	TFile.seekg(0, ios::beg);//reset FSptr
	int ptrN = 0;//def FSPtr Now position
	int PtrB = 0;//def FSPtr before position
	char* TMPFLAGBLK;
	char* TMPTEXT;
	
	ptrN = Scripthead;
	do 
	{
	
	
		TMPFLAGBLK = new char[sizeofFlag01];
		TFile.seekg(ptrN, ios::beg);
		TFile.read(TMPFLAGBLK, sizeofFlag01);
		if (memcmp(TMPFLAGBLK,Flag01,4)==0)
		{
			cout << "Find Target!"<<endl;
			fout << "Find Target! ";
			PtrB = ptrN;//save ptr
			cout << "FSPTR:0x" << hex << ptrN<<endl;
			//fout << "FSPTR:0x" << hex << ptrN<<" "; Do not need this
			TFile.seekg(ptrN + sizeofAdd, ios::beg);
			TFile.read(TMPFLAGBLK, sizeofAdd);
			int Add = NULL;
			memcpy(&Add, TMPFLAGBLK, sizeof(TMPFLAGBLK));
			double pa1 = Add;
			double pa2 = Scripthead;
			double pa3 = Filesize;
			double res = pa1 + pa2 - pa3;
			if (res>0||pa1<0)
			{
				cout << "Out of Range:0x" << hex << Add + Scripthead << endl;
				fout << "Out of Range:0x" << hex << Add + Scripthead<<" "<<endl<<endl;
				ptrN=ptrN+5;
			}
			else
			{
				cout << "Text Address:0x" << hex << Add + Scripthead << endl;
				fout << "Text Address:0x" << hex << Add + Scripthead <<" ";
				TFile.seekg(Add + Scripthead, ios::beg);//set FSptr to Text Sector
				int sizeoftext = 0;

				int ptr1;//start address
				int ptr2;//ptr FSstream
				ptr1 = Add + Scripthead;//init
				ptr2 = Add + Scripthead;//init


				try
				{
					char* TMP;//set TMP
					char* CMP[5] = { 0x0,0x0,0x0,0x0,0x0 };
					TMP = new char[5];
					do
					{
						TFile.seekg(ptr2, ios::beg);//set FSposition			
						TFile.read(TMP, 1);
						if (memcmp(TMP, CMP, 1) == 0)
						{
							sizeoftext = ptr2 - ptr1;
							break;
						}
						else
						{
							ptr2++;
						}
					} while (!TFile.eof());
					delete[] TMP;
				}
				catch (const std::exception&)
				{
					cout << "Error" << endl;
				}
				//check size
				cout << "Text Size:" << sizeoftext << endl;
				fout << "Allocation Size:" << sizeoftext <<" ";
				TFile.seekg(Add + Scripthead, ios::beg);
				TMPTEXT = new char[sizeoftext];
				char HEXtext[1024] = "";
				TFile.read(TMPTEXT, sizeoftext);
				memcpy(&HEXtext, TMPTEXT, sizeoftext);
				cout << "Text:" << HEXtext << endl << endl;
				fout << "Text:" << HEXtext << endl << endl;
				delete[] TMPTEXT;


				ptrN++;
				delete[] TMPFLAGBLK;
			}
			
		} 
		else
		{
			//cout << "Not Find" << endl;
			ptrN=ptrN+1;
		}

	} while (ptrN <= Filesize);
	
	




	TFile.close();//Close File
	cout << "Target File:" << argv[1] << " Closed!" << endl;
	return 0;
}
