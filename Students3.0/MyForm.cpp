#include "MyForm.h"
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <iomanip>
#include <msclr\marshal_cppstd.h> 
using namespace std;
using namespace Students30;
using namespace msclr::interop;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm);
	return 0;
}

static int num; //статическая переменная для учета общего числа элементов в базе данных

void ReadFile(string nameFile, Students a[]) //запись данных из файла в структуру
{
	ifstream fin(nameFile);
	int d;
	cout << "     Записей " << num << "\n";
	fin >> d;
	for (int i = 0; i < num; i++)
	{
		fin >> a[i].fam;
		fin >> a[i].name;
		fin >> a[i].faculty;
		fin >> a[i].group;
	}
	fin.close();
}

void checkFile(string nameFile) //проверка файла и считывание количество записей в ней
{
	ofstream fout(nameFile, ofstream::app);
	ifstream fin(nameFile);

	if (!fout.is_open())
	{
		MessageBox::Show("Ошибка открытия файла");
	}
	else
	{
		fin >> num;
		if (num < 0) {
			fout << "0";
			num = 0;
		}
	}
	fout.close();
	fin.close();
}

void Students30::MyForm::output(Students a[], int k)
{
	dataGridView1->Rows->Clear();
	for (int i = 0; i < k; i++)
	{
		string SN = a[i].fam;
		string LN = a[i].name;
		string F = a[i].faculty;
		int G = a[i].group;
		System::String^ SNS = marshal_as<System::String^>(SN);
		System::String^ LNS = marshal_as<System::String^>(LN);
		System::String^ FS = marshal_as<System::String^>(F);
		dataGridView1->Rows->Add();
		dataGridView1->Rows[i]->Cells[0]->Value = SNS;
		dataGridView1->Rows[i]->Cells[1]->Value = LNS;
		dataGridView1->Rows[i]->Cells[2]->Value = FS;
		dataGridView1->Rows[i]->Cells[3]->Value = G;
	}
}

void WriteFile(string nameFile, Students a[])  // запись данных из структуры в файл
{
	ofstream fout(nameFile);
	if (!fout.is_open())
	{
		cout << ("Ошибка открытия файла") << endl;
	}
	else
	{
		fout << num << "\n";
		for (int i = 0; i < num; i++)
		{
			fout << a[i].fam << "\n";
			fout << a[i].name << "\n";
			fout << a[i].faculty << "\n";
			fout << a[i].group << "\n";
		}
	}
	fout.close();
}

void sort(Students a[]) //сортировка данных по алфавиту
{
	Students min; //Создаем временную таблицу
	int n_min;
	for (int i = 0; i < num; i++)
	{
		min = a[i]; n_min = i; //Переносим все из основной таблицы в временную
		for (int j = i; j < num; j++)
			if (strcmp(a[j].fam, min.fam) < 0)
			{
				min = a[j];
				n_min = j;
			}
		//Переносим все из временной таблицы в основну
		a[n_min] = a[i];
		a[i] = min;
	}
}

int SearchByLN(Students a[], Students b[], string c) //поиск студентов по фамилии
{
	int k = 0;
	for (int i = 0; i < num; i++)
		if (a[i].fam == c) b[k++] = a[i];
	return k; //возвращаем количество записей студентов с такой фамилией
}

int IsGroup(Students a[], Students b[], int c) //фильтр по номеру группы
{
	int k = 0;
	for (int i = 0; i < num; i++)
		if (a[i].group == c) b[k++] = a[i];
	return k; //возвращаем количество записей студентов из группы
}

Students* AddEl(Students a[], String^ SN, String^ LN, String^ F, int G)
{
	Students* tempTable = new Students[num]; //Создаем временную таблицу
	for (short i = 0; i < num; i++) //Переносим все из основной таблицы в временную
		tempTable[i] = a[i];

	delete[] a; //Удаляем основную таблицу
	num++;
	a = new Students[num]; //Выделяем память для основной таблицы, но уже на +1 эл-м больше 
	for (short i = 0; i < num - 1; i++)
		a[i] = tempTable[i]; //Переносим все из временной таблицы в основну

	delete[] tempTable; //Удаляем временную таблицу
//Заполняем новый элемент:
	string SNS = marshal_as<string>(SN);
	string LNS = marshal_as<string>(SN);
	string FS = marshal_as<string>(SN);
	strcpy(a[num - 1].fam, SNS.c_str());
	strcpy(a[num - 1].name, LNS.c_str());
	strcpy(a[num - 1].faculty, FS.c_str());
	a[num - 1].group = G;
	return a;
}

System::Void Students30::MyForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	
	return System::Void();
}

System::Void Students30::MyForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (textBox8->Text == "" || textBox7->Text == "" || textBox6->Text == "" || textBox5->Text == "") {
		MessageBox::Show("Поле пустое!");
	}
	else {
		Students* S = new Students[num]; //создание основной таблицы (структуры)
		ReadFile("Students.txt", S);//запись данных из файла в структуру
		S = AddEl(S, textBox8->Text, textBox7->Text, textBox6->Text, Convert::ToInt32(textBox5->Text)); //вызов функции добавления студента
		WriteFile("Students.txt", S); //запись в файл
		output(S, num);//распечатка данных
		return System::Void();
	}
}

System::Void Students30::MyForm::button3_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (textBox1->Text == "") {
		MessageBox::Show("Поле пустое!");
	}
	else {
		Students* S = new Students[num]; //создание основной таблицы (структуры)
		ReadFile("Students.txt", S);//запись данных из файла в структуру
		Students* whisLN = new Students[num]; //создание временной таблицы
		string search = marshal_as<string>(textBox1->Text);
		int k = SearchByLN(S, whisLN, search);//поиск студента по фамилии
		output(whisLN, k);//распечатка найденных значений
		delete[] whisLN;//Удаляем временную таблицу
		return System::Void();
	}
}

System::Void Students30::MyForm::button4_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (textBox2->Text == "") {
		MessageBox::Show("Поле пустое!");
	}
	else {
		Students* S = new Students[num]; //создание основной таблицы (структуры)
		ReadFile("Students.txt", S);//запись данных из файла в структуру
		Students* isG = new Students[num];//создание временной таблицы
		int k = IsGroup(S, isG, Convert::ToInt32(textBox2->Text));//поиск студента по номеру группы
		output(isG, k);//распечатка найденных значений
		delete[] isG;//Удаляем временную таблицу
		return System::Void();
	}
}

System::Void Students30::MyForm::button1_Click_1(System::Object^ sender, System::EventArgs^ e)
{
	Students* S = new Students[num]; //создание основной таблицы (структуры)
	ReadFile("Students.txt", S);//запись данных из файла в структуру
	sort(S);//сортировка по алфавиту
	output(S, num);//распечатка отсортированной таблицы
	WriteFile("Students.txt", S); //запись отсортированной таблицы
	return System::Void();
}


System::Void Students30::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	
	checkFile("Students.txt"); //первичная проверка файла
	
	Students* S = new Students[num]; //создание основной таблицы (структуры)

	ReadFile("Students.txt", S);//запись данных из файла в структуру

	output(S, num);//распечатка данных

	return System::Void();
}