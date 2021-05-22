/* 
	This program is to implement the database of MPN statistical technique 
	The program consists functions of List, Search, Insert, Update, Query and HTML file generator
	User can repeat to perform any function exit the program with Quit function
*/

#include <iostream>
#include <fstream>
#include <sstream> // class function for string manipulation
#include <string>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <cmath>
#include <windows.h> // Sleep() function

using namespace std;

#define MAX_DATA 216 // 6 x 6 possibilities

struct MPN_DATA
{
	string combo;
	int MPN, lower, upper;
	
	/*
		print the table with data
		Pre: library iostream and iomanip are included
	*/
	void print_table(ostream& out) { 
		out << setw(17) << "" << "|" << setw(6) << "" << setw(5) << combo << setw(8) << "|"
			<< setw(7) << "" << setw(4) << MPN << setw(9) << "|"
			<< setw(8) << "" << setw(4) << lower << setw(10) << "|"
			<< setw(9) << "" << setw(4) << upper << setw(10) << "|" << endl
			<< setw(17) << "" << "-------------------------------------------------------------------------------------\n";
	}
	
	/*
		convert positive triplet to only digit
		Pre: library iostream, sstream and string are included
	*/
	int combo_num() { 
		int num;
		string token, temp;
		
		istringstream text(combo);
		
		while(getline(text, token, '-'))
			temp += token;
		
		stringstream t(temp); 
		t >> num; // convert string to integer
		
		return num;
	}
	
	/*
		print out the data to text file
		Pre: library iostream and iomanip are included
	*/
	void out_file_table(ostream& out) {
		out << endl
			<< setw(3) << "" << setw(5) << combo
			<< "\t\t" << setw(6) << "" << setw(3) << MPN
			<< "\t\t" << setw(3) << lower
			<< "\t\t\t" << setw(3) << upper;
	}
	
	/*
		print out data in html file
		Pre: library iostream is included
	*/
	void export_to_table(ostream& out,string i) {
		string index = i + ".";
		out << "<tr>" << endl
			<< "<td style=\"text-align:right\">" << index << "</td>" << endl
			<< "<td style=\"text-align:center\">" << combo << "</td>" << endl
			<< "<td style=\"text-align:center\">" << MPN << "</td>" << endl
			<< "<td style=\"text-align:center\">" << lower << "</td>" << endl
			<< "<td style=\"text-align:center\">" << upper << "</td>" << endl
			<< "</tr>" << endl;
	}
};

bool add_file(MPN_DATA[], int&);
bool out_file(MPN_DATA[], int);
int int_validator(int);
string triplet_validator(int);
int ib_validator(int);
char y_n_validator(int, string);
void center(string);
void draw_line();
void color(string);
void heading();
void menu();
void list(MPN_DATA[], int);
int search(MPN_DATA[], int, string);
void search_operation(MPN_DATA[], int);
void swap(MPN_DATA*, MPN_DATA*);
void bubble_sort(MPN_DATA[], int);
void insert(MPN_DATA[], int&);
void update(MPN_DATA[], int&);
void query(MPN_DATA[], int);
void print_query(MPN_DATA[], int, int, string);
void export_to_html(MPN_DATA[], int&);

int main()
{	
	string trash;
	
	system("cls");
	cout << "\n\n\n\n\n\n\n";
	draw_line();
	cout << endl;
	center("Database of");
	cout << endl;
	center("Most Probable Number (MPN) method");
	cout << endl;
	center("Welcome to our program!");
	cout << endl;
		 
	draw_line();
	cout << "PRESS <ENTER> TO PROCEED =>";
	getline(cin, trash);
	system("cls");
	
	menu();
	
	cout << "\n\n\n\n\n\n\n\n";
	draw_line();
	cout << endl;
	center("Thank you for choosing our program!");
	cout << endl;
	center("Have a nice day :)");
	cout << endl;
	draw_line();
	
	system("pause");
	return 0;
}

/*
	read data from text file
	Pre: library fstream is included
*/
bool add_file(MPN_DATA data[], int& index)
{
	ifstream in_file("data_list.txt");

	if (!in_file)
		return false;
	else
	{
		in_file.ignore(256, '\n');
		in_file.ignore(256, '\n');
		while (!in_file.eof())
		{
			in_file >> data[index].combo
					>> data[index].MPN
					>> data[index].lower
					>> data[index].upper;
			index++;
		}

		bubble_sort(data, index);
		in_file.close();
	}

	return true;
}

/*
	print data to text file
	Pre: library iomanip and fstream are included
*/
bool out_file(MPN_DATA data[], int index)
{
	ofstream out_file("data_list.txt");

	if (!out_file)
		return false;
	else {
		out_file << "Combination of\t\t" << "MPN Index/100ml\t\t" << "95 percent Confidence Limits\n"
			<< "  Positives\t\t\t\t\t" << "Lower\t\t" << setw(7) << "" << "Upper";
		for (int i = 0; i < index; i++)
			data[i].out_file_table(out_file);
		out_file.close();
	}

	return true;
}

/*
	read input and only accept positive integer value
	Pre: library iostream, iomanip, sstream, cctype and string are included
*/
int int_validator(int space)
{
	string text;
	int num(0);
	bool flag;

	do {
		getline(cin, text);

		flag = true;

		color("red");

		if (text.empty()) {
			cout << endl << setw(space) << "" << "Text is currently empty" << endl << setw(space) << "" << "=>";
			flag = false;
		}

		for (int i = 0; i < text.length(); i++) {
			if (!isdigit(text[i])) {
				cout << endl << setw(space) << "" << "Please enter a positive digit" << endl << setw(space) << "" << "=>";
				flag = false;
				break;
			}
		}
		
		color("reset");

	} while (!flag);

	stringstream t(text); 
	t >> num; // string to integer
	return num;
}

/*
	read input and only accept X-X-X triplet format (X must be digit)
	Pre: library iostream, iomanip, cctype and string are included
*/
string triplet_validator(int space) 
{
	string choice;
	bool flag;
	do 
	{
		getline(cin, choice);
		
		color("red");
		
		if (choice.length() != 5) 
		{
			cout << endl << setw(space) << "" << "Error! Wrong length! It should be only 5 digits!!" 
				 << endl << setw(space) << "" << "=>";
			flag = false;
			color("reset");
			continue;
		}
		
		flag = isdigit(choice[0]) && (choice[1] == '-') && isdigit(choice[2]) && (choice[3] == '-') && isdigit(choice[4]); // format of triplet
		
		if (!flag) 
			cout << endl << setw(space) << "" << "Error! Wrong format! It should be X-X-X format!" 
				 << endl << setw(space) << "" << "=>";
				 
		color("reset");

	} while (!flag);

	return choice;
}

/*
	index and bound validation
	read input and only accept positive 4 digit integer value
	Pre: library iostream, iomanip, sstream, cctype and string are included
*/
int ib_validator(int space)
{
	int num;
	string choice;
	bool flag;
	
	do 
	{
		flag = true;
		getline(cin, choice);
		
		color("red");
		
		if (choice.length() <= 0 || choice.length() > 4)
			flag = false;
			
		for (int i = 0; i < choice.length(); i++)
			if (!isdigit(choice[i]))
				flag = false;
		
		if (!flag)
			cout << endl << setw(space) << "" << "Error! Number has to be larger than 0 and smaller than 10 000!"
				 << endl << setw(space) << "" << "=>";
				 
		color("reset");

	} while (!flag);

	stringstream t(choice); 
	t >> num; // string to integer
	
	return num;
}

/*
	read input and validate character input 'y' and 'n'
	Pre: library iostream, iomanip and string are included
*/
char y_n_validator(int space, string msg) 
{
	string choice;
	bool flag;
	
	cout << endl
		 << setw(space) << "" << "Do you want to continue to " << msg <<"? Press 'y' - YES / 'n' - NO: ";
	
		
	do {
		flag = true;
		getline(cin, choice);
		
		color("red");
		
		if (choice.length() != 1){
			cout << endl << setw(space - 5) << "" << "Invalid input! Only 1 input character is accepted!"
				 << endl << setw(space - 5) << "" << "=>";
				 
			flag = false;
			color("reset");
			continue;
		}

		if (choice[0] != 'y' && choice[0] != 'Y' && choice[0] != 'n' && choice[0] != 'N'){
			cout << endl << setw(space - 5) << "" << "Invalid input! Please enter y or n only!"
				 << endl << setw(space - 5) << "" << "=>";
				 
			flag = false;
		}
		color("reset");
		
	} while (!flag);

	return choice[0];
}

/*
	locate string in center alignment of the console window
	Pre: library iostream is included
*/
void center(string msg)
{
	int l = msg.length();
	int space = 60 - (int)l / 2;
	cout << setw(space) << " " << "\033[3;104;97m " << msg << " \u001b[0m" << endl; // ASCII code for text color
}

/*
	print out 120 equal line 
	Pre: library iostream is included
*/
void draw_line()
{
	string str(120, '=');
	cout << str << endl;
}

/*
	print string in color
	Pre: library iostream is included
*/
void color(string change)
{
	string color[8] = { "black","red","green","yellow","blue","magenta","cyan", "reset" };
	int num(-1);
	for (int i = 0; i < sizeof(color) / sizeof(string); i++)
		if (change == color[i])
			num = i;
	switch (num) 
	{
		case 0:cout << "\u001b[30m"; break;
		case 1:cout << "\u001b[31m"; break;
		case 2:cout << "\u001b[32m"; break;
		case 3:cout << "\u001b[33m"; break;
		case 4:cout << "\u001b[34m"; break;
		case 5:cout << "\u001b[35m"; break;
		case 6:cout << "\u001b[36;1m"; break;
		case 7:cout << "\u001b[0m"; break;
	}
}

/*
	print out heading of the table list
	Pre: library iostream and iomanip are included
*/
void heading()
{
	cout << endl
		<< setw(17) << "" << "-------------------------------------------------------------------------------------\n"
		<< setw(17) << "" << "|" << setw(2) << "" << "Combination of" << setw(3)
		<< "|" << setw(2) << "" << "MPN Index/100ml" << setw(3)
		<< "|" << setw(8) << "" << "95 percent Confidence Limits" << setw(10) << "|\n"
		<< setw(17) << "" << "|" << setw(4) << "" << "Positives" << setw(6)
		<< "|" << setw(20)
		<< "|" << setw(8) << "" << "Lower" << setw(9)
		<< "|" << setw(9) << "" << "Upper" << setw(10) << "|\n"
		<< setw(17) << "" << "-------------------------------------------------------------------------------------\n";
}

/*
	print out menu and perform 7 functions of program
	Pre: library iostream, iomanip and string are included
*/
void menu()
{
	MPN_DATA data[MAX_DATA];
	int choice(0), index(0);
	string trash;

	if (!add_file(data, index))
		cout << "Error opening input file\n";
	else
	{
		do
		{
			cout << endl;
			center("MPN Data Bank");
			center("Main Menu");
			draw_line();
	
			cout << endl
				<< setw(50) << "" << "<1> List\n"
				<< setw(50) << "" << "<2> Search\n"
				<< setw(50) << "" << "<3> Insert\n"
				<< setw(50) << "" << "<4> Update\n"
				<< setw(50) << "" << "<5> Query\n"
				<< setw(50) << "" << "<6> HTML file generator\n"
				<< setw(50) << "" << "<7> Quit\n\n";
			draw_line();
	
			cout << setw(40) << "" << "Enter your choice --> ";
			choice = int_validator(30);
			system("cls");
	
			switch (choice)
			{
			case 1:
				list(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 2:
				search_operation(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 3:
				insert(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 4:
				update(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 5:
				query(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 6:
				export_to_html(data, index);
				getline(cin, trash);
				system("cls");
				break;
			case 7:
				if (!out_file(data, index))
					cout << "Error opening input file\n";
			}
		} while (choice != 7);
	}
	
}

/*
	print out list of the MPN data
	Pre: library iostream and iomanip are included
*/
void list(MPN_DATA arr[], int index)
{
	cout << endl;
	center("Table of Bacterial Concentrations for Most Probable Number Method");
	draw_line();
	heading();

	for (int i = 0; i < index; i++)
		arr[i].print_table(cout);

	color("green");
	cout << setw(17) << "" << "Total MPN data = " << index
		<< "\n\n*Combination-of-Positive Triplets are based on three different dilutions of sample.\n\n"
		<< "**Three sets of sample which are 10ml, 1ml and 0.1ml of sample per tube.\n"
		<< "**Each sets of sample contain 5 tubes.\n\n"
		<< "***Example: 5-3-2 means all five tubes receiving 10 ml of sample show bacterial growth, "
		<< "only three tubes in the 1 ml \n   show growth, and two of the 0.1 ml group is positive.\n";
	color("reset");

	cout << "\n\nEND OF LIST FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";
}

/*
	compare user input with every positive triplet in the array
*/
int search(MPN_DATA arr[], int index, string user_input)
{
	for (int i = 0; i < index; i++)
		if (arr[i].combo == user_input)
			return i;
	return -1;
}

/*
	read positive triplet from user and search through every positive triplet of the array
	display the result after searching
	Pre: library iostream and iomanip are included
*/
void search_operation(MPN_DATA arr[], int index)
{
	int target;
	string user_input;
	char again;

	cout << endl;
	center("MPN Data");
	center("Search Operation");
	draw_line();
	cout << endl;

	do // user-defined whether repeat function
	{
		cout << setw(30) << "" << "Combination-of-Positives Triplet to search [Example: 3-2-0]\n"
			 << setw(30) << "" << "---> ";
			
		fflush(stdin);
		user_input = triplet_validator(25);
		cout << endl;

		target = search(arr, index, user_input);

		if (target != -1) // check whether is existing data
		{
			cout << setw(30) << "----> "
				 << "For " << arr[target].combo
				 << ", MPN = " << arr[target].MPN
				 << "; 95% of samples contain between "
				 << arr[target].lower << " and "
				 << arr[target].upper << " bacterial/ ml.\n";
		}
		else {
			color("red");
			cout << setw(30) << "----> " << "Combination-of-Positives Triplet is not found!\n";
			color("reset");
		}
		
		again = y_n_validator(30, "search") ;
		cout << endl;

	} while (again == 'y' || again == 'Y');

	cout << endl << "END OF SEARCH FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";
	
}

/*
	swap a and b
*/
void swap(MPN_DATA* a, MPN_DATA* b)
{
	MPN_DATA temp = *a;
	*a = *b;
	*b = temp;
}

/*
	sort the array of all MPN data in ascending order
*/
void bubble_sort(MPN_DATA arr[], int index)
{
	for (int i = 0; i < index - 1; i++)
		for (int j = 0; j < index - i - 1; j++)
			if (arr[j].combo_num() > arr[j + 1].combo_num())
				swap(&arr[j], &arr[j + 1]);
}

/*
	insert the data from user input to the MPN data of array
	Pre: library iostream and iomanip are included
*/
void insert(MPN_DATA arr[], int& index)
{
	int duplication;
	char again;
	MPN_DATA temp;

	if (index <= 216) // check array of MPN_DATA whether is full
	{
		do // user-defined whether repeat function
		{
			system("cls");
			cout << endl;
			center("MPN Data");
			center("New Data Input");
			draw_line();
			cout << endl;

			cout << setw(40) << "" << "Combination-of-Positives Triplet [Example: 3-2-0]\n"
				 << setw(40) << "" << "---> ";

			fflush(stdin);
			temp.combo = triplet_validator(36);
			duplication = -1;

			for (int i = 0; i < index; i++)
				if (temp.combo == arr[i].combo)
				{
					duplication = i;
					break;
				}

			if (duplication == -1) // check whether is existing data
			{
				cout << endl
					 << setw(40) << "" << "Most Probable Number(MPN)\n"
					 << setw(40) << "" << "---> ";

				temp.MPN = ib_validator(36);

				cout << endl
					 << setw(40) << "" << "95 percent confidence range\n"
					 << setw(40) << "" << "---------------------------\n";

				do // validation of lower bound
				{
					cout << setw(40) << "" << "Lower bounds\n"
						 << setw(40) << "" << "---> ";

					temp.lower = ib_validator(36);

					color("red");

					if (temp.lower < 0)
						cout << endl << setw(36) << "" << "Error! Lower can not be lower than 0" << endl;

					color("reset");

				} while (temp.lower < 0);

				do // validation of upper bound
				{
					cout << endl
						<< setw(40) << "" << "Upper bounds\n"
						<< setw(40) << "" << "---> ";

					temp.upper = ib_validator(36);

					color("red");

					if (temp.lower > temp.upper)
						cout << endl << setw(36) << "" << "Error! Lower can not be larger than Upper" << endl;

					color("reset");

				} while (temp.lower > temp.upper);

				arr[index++] = temp;

				bubble_sort(arr, index);
			}
			else 
			{
				color("red");
				
				cout << endl << setw(36) << "" << "Error! Combination-of-Positives Triplet " << arr[duplication].combo << " already exists!" << endl;
				
				color("reset");
			}

			again = y_n_validator(31, "insert data");
			cout << endl << endl;

		} while (again == 'y' || again == 'Y');

	}
	else 
	{
		cout << "\n\n\n\n\n\n\n\n";
		draw_line();
		cout << endl;
		center("Database has reached it's limit. [ERROR 426]\n\n");
		center("Please contact admin to repair the problem.\n");
		draw_line();
	}

	cout << "END OF INSERT FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";
	
}

/*
	update the data from user input to the MPN data of array
	Pre: library iostream and iomanip are included
*/
void update(MPN_DATA arr[], int& index)
{
	int target;
	char again;
	MPN_DATA temp;

	cout << endl;
	center("MPN Data");
	center("Data Update");
	draw_line();
	cout << endl;

	do // user-defined whether repeat function
	{
		cout << setw(30) << "" << "Combination-of-Positives Triplet to update: ";
		fflush(stdin);
		temp.combo = triplet_validator(25);
		cout << endl;

		target = search(arr, index, temp.combo);

		if (target != -1) // check whether is existing Combination-of-Positives
		{
			color("yellow");
			
			cout << setw(30) << "" << "----------------------------\n"
				 << setw(30) << "" << "Current data for " << temp.combo << ": " << endl
				 << setw(30) << "" << "MPN = " << arr[target].MPN << endl
				 << setw(30) << "" << "Lower bound = " << arr[target].lower << endl
				 << setw(30) << "" << "Upper bound = " << arr[target].upper << endl
				 << setw(30) << "" << "----------------------------\n";
				 
			color("reset");

			cout << endl << setw(30) << "" << "Enter the updated MPN: ";
			temp.MPN = ib_validator(25);

			cout << endl
				 << setw(30) << "" << "95 percent confidence range\n"
				 << setw(30) << "" << "---------------------------\n";

			do // validation of lower bound
			{
				cout << setw(30) << "" << "Enter the updated lower bounds\n"
					 << setw(30) << "" << "---> ";

				temp.lower = ib_validator(25);

				color("red");
				
				if (temp.lower < 0)
					cout << endl << setw(25) << "" << "Error! Lower bound can not be lower than 0" << endl;
					
				color("reset");

			} while (temp.lower < 0);

			do // validation of upper bound
			{
				cout << endl
					 << setw(30) << "" << "Enter the updated upper bounds\n"
					 << setw(30) << "" << "---> ";

				temp.upper = ib_validator(25);

				color("red");
				
				if (temp.lower > temp.upper)
					cout << endl << setw(25) << "" << "Error! Lower bound can not be larger than Upper bound" << endl;
					
				color("reset");

			} while (temp.lower > temp.upper);

			arr[target] = temp;

		}
		else {
			color("red");
			
			cout << setw(30) << "----> " << "Combination-of-Positives temp.combo is not found!\n\n";
			
			color("reset");
		}

		again = y_n_validator(30, "update");
		cout << endl << endl;

	} while (again == 'y' || again == 'Y');

	cout << "END OF SEARCH FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";

}

/*
	get user input of group of dilution sample and number of tubes
	print out query list
	Pre: library iostream, iomanip and sstream are included
*/
void query(MPN_DATA arr[], int index)
{
	int group, tube;
	char again;
	string text;

	do // user-defined whether repeat function
	{
		system("cls");

		cout << endl;
		center("MPN Data");
		center("Query Search");
		draw_line();
		cout << endl;

		cout << setw(30) << "" << "Which group of dilution sample to search?\n"
			 << setw(30) << "" << "[Press '1' - 10ml group;   Press '2' - 1ml group;   Press '3' - 0.1ml group]\n"
			 << setw(30) << "" << "Enter your choice --> ";

		do // validation of 3 groups
		{
			group = int_validator(25);

			color("red");
			
			if (1 > group || group > 3)
				cout << endl << setw(25) << "" << "Error! Please enter 1, 2 or 3 only."
					 << endl << setw(25) << "" << "=>";
					 
			color("reset");

		} while (1 > group || group > 3);

		group = 2 * (group - 1); // convert to group array no. i.e. 0, 2, 4

		cout << endl << setw(30) << "" << "How many tubes in the group?\n"
			 << setw(30) << "" << "[Min: 0tube;    Max: 5 tubes]\n"
			 << setw(30) << "" << "Enter your choice --> ";

		do // validation of number of tube
		{
			tube = int_validator(25);

			color("red");
			
			if (0 > tube || tube > 5)
				cout << endl << setw(25) << "" << "Error! Maximum number of tube is 5.\n"
					 << setw(25) << "" << "=>";
					 
			color("reset");

		} while (0 > tube || tube > 5);

		stringstream t;
		t << tube;
		t >> text; // int to string
		
		print_query(arr, index, group, text);

		again = y_n_validator(30, "search");
		cout << endl;

	} while (again == 'y' || again == 'Y');

	cout << "END OF SEARCH FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";
	
}

/*
	print out list of data based on the group of dilution sample and number of tubes
	Pre: library iostream, iomanip, cmath and cstring are included
*/
void print_query(MPN_DATA arr[], int index, int g, string t)
{
	string temp;
	int count(0);
	double g2 = pow(10, 1 - 0.5 * g); // convert group array no. to actual value of its group, i.e. 10, 1, 0.1

	color("yellow");
	
	cout << endl << setw(17) << ""
		 << "The combinations that have " + t + " tubes in the " << g2 << "-ml group:";

	heading();

	for (int i = 0; i < index; i++)
	{
		temp = arr[i].combo;

		if (strncmp(&temp[g], &t[0], 1) == 0)
		{
			arr[i].print_table(cout);
			count++;
		}
	}

	color("red");
	
	if (count == 0)
		cout << setw(17) << "" << "-----------------------------------NO RECORD FOUND-----------------------------------\n";
		
	color("reset");
}

/*
	print out list in HTML format
	Pre: library iostream, iomanip, windows.h and sstream are included
*/
void export_to_html(MPN_DATA arr[], int& index)
{
	string str;
	
	cout << endl;
	center("MPN Data");
	center("HTML Table generator");
	draw_line();
	cout << endl << setw(40) << "" << "We are exporting the MPN data list to HTML file.\n"
		 << endl << setw(50) << "" << "Please wait for a moment.\n"
		 << endl << setw(55) << "" << "Exporting";
	Sleep(300);
	cout << ".";
	Sleep(300);
	cout << ".";
	Sleep(300);
	cout << ".\n";
	Sleep(700);
	color("cyan");
	cout << endl << setw(53) << "" << "Export complete!\n";
	Sleep(500);
	cout << endl << setw(40) << ""
		 << "mpn_records.html has been successfully created!\n\n";
	color("reset");
	draw_line();
	cout << "\n\n\n\n" << "END OF SEARCH FUNCTION" << endl;
	draw_line();
	cout << "PRESS <ENTER> TO GO BACK MAIN MENU";
	
	ofstream output("mpn_records.html"); // export to html file
	output << "<!DOCTYPE html>" << endl
		<< "<html lang=\"en\">" << endl
		<< "<head>" << endl
		<< "<meta charset=\"UTF - 8\">" << endl
		<< "<meta name=\"viewport\" content=\"width = device-width, initial-scale = 1.0\">" << endl
		<< "<title>Table of Bacterial Concentrations for MPN</title>" << endl
		<< "<style>" << endl
		<< "table, th, td{\nborder: 1px solid black;\n}" << endl
		<< "th, td{\npadding: 18px;\n}" << endl
		<< "tr:nth-child(even) {background-color: #f2f2f2;}" << endl
		<< "</style>" << endl
		<< "</head>" << endl
		<< "<body>" << endl
		<< "<h1 style=\"text-align:center\">Table of Bacterial Concentrations for Most Probable Number Method</h1>" << endl
		<< "<table align=\"center\" style=\"border-collapse: collapse;\">" << endl
		<< "<tr style=\"background-color:#93FFE8;\">" << endl
		<< "<th rowspan=\"2\"></th>" << endl
		<< "<th rowspan=\"2\" style=\"font-size:20px;\">Combination of<br>Positives</th>" << endl
		<< "<th rowspan=\"2\" style=\"font-size:20px;\">MPN Index/100ml</th>" << endl
		<< "<th scope = \"col\"colspan=\"2\" style=\"font-size:20px;\">95 percent Confidence Limits</th>" << endl
		<< "<tr style=\"background-color:#93FFE8;\">" << endl
		<< "<th style=\"font-size:15px;\">Lower</th>" << endl
		<< "<th style=\"font-size:15px;\">Upper</th>" << endl;
		
	for (int i = 0; i < index; i++) 
	{
		stringstream scase;
		scase << i + 1;
		scase >> str; // int to string
		
		arr[i].export_to_table(output, str);
	}
	
	output << "</table>" << endl
		<< "</body>" << endl
		<< "</html>" << endl;
	output.close();
}
