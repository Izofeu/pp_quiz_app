#include <iostream>
#include <conio.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <time.h>
#define uint int
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
uint linecount=0;

void randomizeArray(int *array)
{
	for(int i=1; i<=linecount; i++)
	{
		array[i-1] = i;
	}
	int randNum1, randNum2, transfer;
	srand(time(0));
	for(int i=0; i<linecount*15; i++)
	{
		randNum1=rand()%linecount;
		randNum2=rand()%linecount;
		transfer=array[randNum1];
		array[randNum1] = array[randNum2];
		array[randNum2] = transfer;
	}
}

bool createSampleFile()
{
	ofstream qfile("pytania.txt");
	if(qfile)
	{
		qfile.close();
		return true;
	}
	return false;
}

int fixline(int num)
{
	int newnum=1;
	for(int i=1; i<num; i++)
	{
		newnum+=5;
	}
	return newnum;
}

bool checkAns(int ans, int num_orig)
{
	string line;
	int num=fixline(num_orig);
	// int oldpos;
	static ifstream qfile("pytania.txt");
	if(!qfile)
	{
		cout<<"Blad otwarcia pliku pytania.txt! Program zostanie zakonczony."<<endl;
		getch();
		exit(1);
	}
	qfile.clear();
	qfile.seekg(0);
	for(int i=0; i<num+1; i++)
	{
		getline(qfile, line);
	}
	// oldpos = qfile.tellg();
	// cout<<"OLDPOS: "<<oldpos<<" LINE: "<<line<<endl;
	ans--;
	for(int i=1; i<=ans; i++)
	{
		getline(qfile, line);
	}
	if(line[0] == '+')
	{
		cout<<"Odpowiedz poprawna!"<<endl;
		return true;
	}
	else
	{
		cout<<"Odpowiedz niepoprawna!";
		qfile.clear();
		qfile.seekg(0);
		for(int i=0; i<num; i++)
		{
			getline(qfile, line);
		}
		int i=1;
		do
		{
			getline(qfile, line);
			//cout<<"sprawdzanie linii "<<line<<endl;
			if(line[0] == '+')
			{
				cout<<" Poprawna odpowiedz to odpowiedz nr "<<i<<"."<<endl;
				return false;
			}
			i++;
		}
		while(i<=4);
		cout<<endl<<"To pytanie nie posiada prawidlowej odpowiedzi. Zglos to autorowi quizu."<<endl;
		return false;
	}
}

void printQuestion(int num_orig)
{
	int num=fixline(num_orig);
	string line = "";
	static ifstream qfile("pytania.txt");
	if(!qfile)
	{
		cout<<"Blad otwarcia pliku pytania.txt! Program zostanie zakonczony."<<endl;
		getch();
		exit(1);
	}
	qfile.clear();
	qfile.seekg(0);
	// cout<<"num1:"<<num<<endl;
	// cout<<"num2:"<<num<<endl;
	for(int i=0; i<num; i++)
	{
		getline(qfile, line);
	}
	cout<<"Pytanie nr "<<num_orig<<": "<<endl;
	cout<<line<<endl;
	for(int i=1; i<=4; i++)
	{
		getline(qfile, line);
		if(line[0] == '+')
		{
			line=line.substr(1,line.length()-1);
		}
		cout<<i<<" - "<<line<<endl;
	}
	cout<<"Twoja odpowiedz to: ";
}

void scoreSum(int corans, uint startTime, uint user_lines)
{
	string nickname = "";
	cout<<"Udalo Ci sie odpowiedziec poprawnie na "<<corans<<" z "<<user_lines<<" pytan. Czas quizu: ";
	time_t timenow = time(0);
	uint endTime = timenow;
	uint finishTime = endTime - startTime;
	cout<<finishTime<<" s. Wpisz swoj pseudonim jezeli chcesz dodac siebie do listy osob, ktore ukonczyly quiz (max 10 znakow) lub nacisnij enter"
	" aby zakonczyc program."<<endl;
	getline(cin, nickname);
	if(nickname.length() > 10)
	{
		nickname=nickname.substr(0, 10);
	}
	if(nickname.length() == 0)
	{
		return;
	}
	ofstream afile("wyniki.txt");
	if(!afile)
	{
		cout<<"Nie udalo sie otworzyc/utworzyc pliku wyniki.txt."<<endl;
		return;
	}
	afile<<nickname<<" odpowiedzial na "<<corans<<" z "<<user_lines<<" pytan w "<<finishTime<<" sekund."<<endl;
	afile.close();
	cout<<"Rekord dodany!"<<endl;
}

void isFileValid()
{
	ifstream qfile("pytania.txt");
	if(!qfile)
	{
		cout<<"Plik pytania.txt nie istnieje! Tworzenie pliku pytania.txt..."<<endl<<endl;
		qfile.close();
		if(createSampleFile())
		{
			cout<<"Plik pytania.txt stworzony! ";
			cout<<"Plik ten musi zostac uzupelniony\nzanim aplikacja zacznie dzialac. Potrzebne jest przynajmniej jedno pytanie"
			" oraz\ncztery odpowiedzi do niego. Skladnia pliku powinna byc nastepujaca:\npytanie\nodpowiedz1\nodpowiedz2"
			"\nodpowiedz3\nodpowiedz4.\nOdpowiedz poprawna poprzedz znakiem + (plus).\n"
			"Powroc do programu po uzupelnieniu pliku przynajmniej jednym pytaniem i nacisnij dowolny "
			"klawisz, aby kontynuowac."<<endl;
			getch();
			qfile.open("pytania.txt");
			if(!qfile)
			{
				cout<<"Plik pytania.txt nie istnieje!"<<endl;
				getch();
				exit(1);
			}
		}
		else
		{
			cout<<"Brak uprawnien do stworzenia pliku pytania.txt. Nadaj pelne uprawnienia dla folderu aplikacji i sprobuj ponownie."<<endl;
			getch();
			exit(1);
		}
	}
	string curline;
	uint count = 0;
	while(getline(qfile, curline))
	{
		count++;
	}
	if(count % 5 != 0 || count == 0)
	{
		cout<<"Blad skladni pliku pytania.txt - ilosc linii powinna byc rowna mnoznikowi liczby 5, ilosc linii w pliku: "<<count<<endl;
		getch();
		exit(1);
	}
	qfile.close();
	linecount=count/5;
}

int main(int argc, char** argv) {
	cout<<"=============================="<<endl<<"QUIZ PYTAN"<<endl<<"=============================="<<endl<<endl;
	cout<<"Nacisnij dowolny klawisz, aby rozpoczac"<<endl<<endl;
	getch();
	isFileValid();
	uint user_lines;
	while(true)
	{
		cout<<"Wpisz ilosc pytan, ktora chcesz otrzymac (maksimum: "<<linecount<<")"<<endl;
		cin>>user_lines;cin.ignore();
		if(user_lines > linecount)
		{
			cout<<"Plik nie zawiera takiej ilosci pytan! Nacisnij dowolny klawisz aby kontynuowac..."<<endl;
			getch();
			continue;
		}
		if(user_lines == 0)
		{
			cout<<"Wprowadzona ilosc jest nieprawidlowa! Nacisnij dowolny klawisz aby kontynuowac..."<<endl;
			getch();
			continue;
		}
		break;
	}
	int corans = 0, qlist[linecount];
	uint ans;
	time_t timenow = time(0);
	uint startTime = timenow;
	randomizeArray(qlist);
	for(int i=0; i<=user_lines-1; i++)
	{
		printQuestion(qlist[i]);
		cin>>ans;cin.ignore();
		if(ans == 0 || ans > 4)
		{
			cout<<"Odpowiedz nie istnieje, pytanie zostanie wyswietlone jeszcze raz"<<endl<<endl;
			i--;
			continue;
		}
		if(checkAns(ans, qlist[i]))
		{
			corans++;
		}
		cout<<"Nacisnij dowolny klawisz, aby kontynuowac..."<<endl;
		getch();
	}
	scoreSum(corans, startTime, user_lines);
	cout<<"Nacisnij dowolny klawisz aby zakonczyc dzialanie programu."<<endl;
	getch();
	return 0;
}
