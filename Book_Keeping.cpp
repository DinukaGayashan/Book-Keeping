/********************************************************************
Course		:	CO2210 Programming Quest
Quest No	:	03
Title		:	Book Keeping
Author		:	Kasthuriarachchi K.A.D.G.
Index No	:	19/ENG/053
Date		:	28-09-2021
********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <stdio.h>

using namespace std;

//Date class to handle date data
class Date
{
private:
	int year, month, day;
public:
	//constructor to create date objects
	Date(int y, int m, int d) :year(y), month(m), day(d) {}

	//overloaded operator to print date objects in structured method
	friend ostream &operator << (ostream& output, Date& date)
	{
		string d = "";
		if (date.year < 1000)
			d += "0";
		d += to_string(date.year);
		if (date.month < 10)
			d += "0";
		d += to_string(date.month);
		if (date.day < 10)
			d += "0";
		d += to_string(date.day);
		output << d;
		return output;
	}

	//overloaded operator to check the equality of two date objects
	bool operator == (Date date)
	{
		if (date.year == year && date.month == month && date.day == day)
			return true;
		else
			return false;
	}
};

//Account class to handle all bank accounts
class Account
{
private:
	Date opening_date;
	int account_number;
	double balance;
public:
	//constructor to create Account objects
	Account(Date dt, int acc_no, double bal) :opening_date(dt), account_number(acc_no), balance(bal) {}
	
	//member functions to get attributes of an object
	double get_balance()
	{
		return balance;
	}
	int get_number()
	{
		return account_number;
	}
	Date get_date()
	{
		return opening_date;
	}

	//member function to edit the balance of an account object
	void edit_balance(double value)
	{
		//calculating new balance by adding the new value
		balance += value;
	}
};

//Transaction structure to handle transactions
struct Transaction
{
	Date occured_date;
	int account_number;
	int type;
	double amount;

	//constructor to create trasaction objects
	Transaction(Date dt, int acc_no, int tpe, double val) :occured_date(dt), account_number(acc_no), type(tpe), amount(val) {}
};

/*---------------------------------------------------------------------------------
Function            :	take_account
Description         :	generating Account objects from a line of inputs seperated
						by a comma
Input Parameters    :	line - line of string with data seperated by a comma
Return              :	Account object with suitable attributes
---------------------------------------------------------------------------------*/
Account take_account(string line)
{
	for (int i = 0; i < int(line.length()); i++)
		if (line[i] == ',')
			line[i] = ' ';

	stringstream data(line);
	string date;
	int account_no;
	double balance;

	data >> date;
	data >> account_no;
	data >> balance;

	return Account (Date(stoi(date.substr(0, 4)), stoi(date.substr(4, 2)), stoi(date.substr(6, 2))), account_no, balance);
}

/*---------------------------------------------------------------------------------
Function            :	take_transaction
Description         :	generating Transaction objects from a line of inputs
						seperated by a comma
Input Parameters    :	line - line of string with data seperated by a comma
Return              :	Transaction object with suitable attributes
---------------------------------------------------------------------------------*/
Transaction take_transaction(string line)
{
	for (int i = 0; i < int(line.length()); i++)
		if (line[i] == ',')
			line[i] = ' ';

	stringstream data(line);
	string date;
	int account_no;
	int type;
	double amount;

	data >> date;
	data >> account_no;
	data >> type;
	data >> amount;

	return Transaction(Date(stoi(date.substr(0, 4)), stoi(date.substr(4, 2)), stoi(date.substr(6, 2))), account_no, type, amount);
}

/*---------------------------------------------------------------------------------
Function            :	round_to_two_decimals
Description         :	rounding up a value to twa decimal place number
Input Parameters    :	num - decimal number that need to round
Return              :	rounded value for two decimal places
---------------------------------------------------------------------------------*/
double round_to_two_decimals(double num)
{
	int n = num * 100 + 0.5;
	return float(n) / 100;
}

/*---------------------------------------------------------------------------------
Function            :	renew_accounts
Description         :	deleting existing account files and creating new files 
						corresponding to new account numbers
Input Parameters    :	accounts - vector of Account objects that are needed to 
						create files
						bank - Account object that needed to be create a file
Return              :	-
---------------------------------------------------------------------------------*/
void renew_accounts(vector<Account> accounts, Account bank)
{
	string command = "del /q Accounts\\*.txt";
	system(command.c_str());

	for (int i = 0; i < int(accounts.size()); i++)
	{
		ofstream file;
		file.open("Accounts\\" + to_string(accounts[i].get_number()) + ".txt");
		file.close();
	}
	
	ofstream file;
	file.open("Accounts\\" + to_string(bank.get_number()) + ".txt");
	file.close();
}

/*---------------------------------------------------------------------------------
Function            :	print_transaction
Description         :	printing a transaction to files corresponding to an account
Input Parameters    :	&acc - reference of the account that transaction is occuring
						trans - transaction that occurs
Return              :	-
---------------------------------------------------------------------------------*/
void print_transaction(Account &acc, Transaction trans)
{
	ofstream file;
	file.open("Accounts\\" + to_string(acc.get_number()) + ".txt", ios::app);
	file << trans.occured_date << "," << trans.type << "," << fixed << setprecision(2) << trans.amount << "," << acc.get_balance() << endl;
	file.close();
}

/*---------------------------------------------------------------------------------
Function            :	add_interests
Description         :	accounting interests corresponding to an account balance
Input Parameters    :	&acc - reference of the account that interest is occuring
						date - date of the interest
						&bank - reference of the bank valt account
Return              :	-
---------------------------------------------------------------------------------*/
void add_interests(Account &acc, Date date, Account &bank)
{
	if (acc.get_balance() > 0)
	{
		double val = round_to_two_decimals(acc.get_balance() * 0.0002);
		acc.edit_balance(val);
		print_transaction(acc, Transaction(date, acc.get_number(), 3, val));
		bank.edit_balance(val);
		print_transaction(bank, Transaction(date, acc.get_number(), 3, val));
	}
}

/*---------------------------------------------------------------------------------
Function            :	daily_trasactions
Description         :	accounting transaction corresponding to given transactions
Input Parameters    :	&acc - reference of the account that transaction is occuring
						date - date of the interest
						trans - transaction that is occuring
						&bank - reference of the bank valt account
Return              :	-
---------------------------------------------------------------------------------*/
void daily_trasactions(Account& acc, Date date, Transaction trans, Account& bank)
{
	if (trans.type == 1)
	{
		acc.edit_balance(trans.amount);
		print_transaction(acc, Transaction(date, acc.get_number(), 1, trans.amount));
		bank.edit_balance(trans.amount);
		print_transaction(bank, Transaction(date, acc.get_number(), 1, trans.amount));
	}
	else if (trans.type == 2)
	{
		acc.edit_balance(-trans.amount);
		print_transaction(acc, Transaction(date, acc.get_number(), 2, trans.amount));
		bank.edit_balance(-trans.amount);
		print_transaction(bank, Transaction(date, acc.get_number(), 2, trans.amount));
	}
}

/*---------------------------------------------------------------------------------
Function            :	apply_charges
Description         :	accounting charges corresponding to low account balances
Input Parameters    :	&acc - reference of the account that charge will occur
						date - date of the charging
						&bank - reference of the bank valt account
Return              :	-
---------------------------------------------------------------------------------*/
void apply_charges(Account& acc, Date date, Account& bank)
{
	if (acc.get_balance() < 0)
	{
		acc.edit_balance(-50);
		print_transaction(acc, Transaction(date, acc.get_number(), 4, 50));
		bank.edit_balance(-50);
		print_transaction(bank, Transaction(date, acc.get_number(), 4, 50));
	}
	else if (acc.get_balance() < 1000)
	{
		acc.edit_balance(-10);
		print_transaction(acc, Transaction(date, acc.get_number(), 4, 10));
		bank.edit_balance(-10);
		print_transaction(bank, Transaction(date, acc.get_number(), 4, 10));
	}
}

/*---------------------------------------------------------------------------------
Function            :	extract_balance
Description         :	extracting the balance of a account by a line of transaction
						data occured on the account
Input Parameters    :	line - string line of transaction data seperated by a comma
Return              :	double value of the balance
---------------------------------------------------------------------------------*/
double extract_balance(string line)
{
	for (int i = 0; i < int(line.length()); i++)
		if (line[i] == ',')
			line[i] = ' ';
	
	stringstream bal(line);
	double balance;
	bal >> balance >> balance >> balance >> balance;
	return balance;
}

/*---------------------------------------------------------------------------------
Function            :	print_balance
Description         :	printing starting and ending account balances of a date
Input Parameters    :	&transactions - vector of transactions that are occuring on 
						the particular account
						date - date of the balance checking
						start_index - index of the first transaction of the date
Return              :	-
---------------------------------------------------------------------------------*/
void print_balance(vector<string> &transactions, string date, int start_index)
{
	if (start_index > 0)
		start_index--;

	int end_index = -1;
	for (int i = transactions.size() - 1; i >= 0; i--)
	{
		if (transactions[i].substr(0, 8) == date)
		{
			end_index = i;
			break;
		}
	}

	cout << fixed << setprecision(2) << endl << "Starting balance : " << extract_balance(transactions[start_index]) << endl;
	cout << "Ending balance : " << extract_balance(transactions[end_index]) << endl << endl;
}

int main()
{
	//initializing the end date
	const int year = 2021;
	const int month = 01;
	const int date = 11;
	
	//initializing the bank valt account number
	const int bank_valt_account_number = 000000;

	//initializing vectors to take inputs from the files
	vector<Account> opening_accounts;
	vector<Transaction> transactions;

	//taking inputs from the text file and converting them into Account objects
	ifstream balances("balance.txt");
	string line = "";
	while (getline(balances, line))
		opening_accounts.push_back(take_account(line));
	balances.close();

	//calculating the opening balance of all user accounts
	double total = 0;
	for (int i = 0; i < int(opening_accounts.size()); i++)
		total += opening_accounts[i].get_balance();

	//creating bank valt account for the bank and initializing with the total
	Account bank_valt_account(Date(year, month, 1), bank_valt_account_number, total);
	
	//taking inputs from the text file and converting them into Transaction objects
	ifstream transaction("transaction.txt");
	while (getline(transaction, line))
		transactions.push_back(take_transaction(line));
	transaction.close();

	//deleting past files and creating files for new accounts
	renew_accounts(opening_accounts, bank_valt_account);

	//printing opening balances of all accounts to the files
	for (int i = 0; i<int(opening_accounts.size()); i++)
		print_transaction(opening_accounts[i], Transaction(opening_accounts[i].get_date(), opening_accounts[i].get_number(), 0, opening_accounts[i].get_balance()));
	print_transaction(bank_valt_account, Transaction(bank_valt_account.get_date(), bank_valt_account.get_number(), 0, bank_valt_account.get_balance()));


	//iterating through the days passed
	for (int d = 1; d < date+1; d++)
	{
		//accounting interests for user accounts at the begining of everyday
		for (int i = 0; i<int(opening_accounts.size()); i++)
			add_interests(opening_accounts[i], Date(year, month, d), bank_valt_account);

		//choosing transactions within a day
		vector<Transaction> transactions_of_the_day;
		for (int i = 0; i<int(transactions.size()); i++)
			if (Date(year, month, d) == transactions[i].occured_date)
				transactions_of_the_day.push_back(transactions[i]);
		//accounting each transaction in corresponding accounts
		for (int i = 0; i<int(transactions_of_the_day.size()); i++)
			for (int j = 0; j<int(opening_accounts.size()); j++)
				if (opening_accounts[j].get_number() == transactions_of_the_day[i].account_number)
					daily_trasactions(opening_accounts[j], transactions_of_the_day[i].occured_date, transactions_of_the_day[i], bank_valt_account);

		//accounting charges for the relevant accounts
		for (int i = 0; i<int(opening_accounts.size()); i++)
			apply_charges(opening_accounts[i], Date(year, month, d), bank_valt_account);
	}


	//iterating while exiting
	while (true)
	{
		//initial display
		system("cls");
		cout << "Book Keeping System\n\n";

		//displaying and choosing options
		int option = 0;
		cout << "1. View an account balance" << endl;
		cout << "2. View account balance of a day" << endl;
		cout << "3. Exit" << endl;
		cin >> option;
		cout << endl;

		//proceeding with viewing account balance
		if (option == 1)
		{
			//iterating while entering an existing account number
			bool acc_avail = 0;
			int acc_no;		
			while (acc_avail == 0)
			{
				acc_no = -1;
				cout << "Enter bank account number : ";
				cin >> acc_no;
				//checking for existing account numbers
				for (int i = 0; i<int(opening_accounts.size()); i++)
				{
					if (acc_no == opening_accounts[i].get_number() || acc_no == bank_valt_account_number)
					{
						acc_avail = 1;
						break;
					}
				}
				//displaying error if such account number is not existing
				if (acc_avail == 0)
					cerr << "Wrong account number entered.\n\n";
			}
			//taking the balance history of the account
			vector<string> acc_transactions;
			string line = "";
			ifstream account_log("Accounts\\" + to_string(acc_no) + ".txt");
			while (getline(account_log, line))
				acc_transactions.push_back(line);
			account_log.close();

			//displaying the last balance of the account
			cout << fixed << setprecision(2) << endl << "Account balance : " << extract_balance(acc_transactions[acc_transactions.size() - 1]) << endl << endl;
			
			//waiting for user input
			system("pause");
		}
		//proceeding with viewing account balance of a day
		else if (option == 2)
		{
			//iterating while entering an existing account number
			bool acc_avail = 0;
			int acc_no;
			while (acc_avail == 0)
			{
				acc_no = -1;
				cout << "Enter bank account number : ";
				cin >> acc_no;
				//checking for existing account numbers
				for (int i = 0; i<int(opening_accounts.size()); i++)
				{
					if (acc_no == opening_accounts[i].get_number() || acc_no == bank_valt_account_number)
					{
						acc_avail = 1;
						break;
					}
				}
				//displaying error if such account number is not existing
				if (acc_avail == 0)
					cerr << "Wrong account number entered.\n\n";
			}
			//taking the balance history of the account
			vector<string> acc_transactions;
			string line = "";
			ifstream account_log("Accounts\\" + to_string(acc_no) + ".txt");
			while (getline(account_log, line))
				acc_transactions.push_back(line);
			account_log.close();
			cout << endl;

			//iterating while entering an existing date
			bool date_avail = 0;
			int transaction_no = -1;
			string date;
			while (date_avail == 0)
			{
				date = "";
				cout << "Enter date (YYYYMMDD) : ";
				cin >> date;
				//checking for existing dates
				for (int i = 0; i<int(acc_transactions.size()); i++)
				{
					if (acc_transactions[i].substr(0, 8) == date)
					{
						date_avail = 1;
						transaction_no = i;
						break;
					}
				}
				//displaying error if such date is not existing
				if (date_avail == 0)
					cerr << "Unavailable date entered.\n\n";
			}
			//printing opening and closing balances of the day
			print_balance(acc_transactions, date, transaction_no);

			//waiting for user input
			system("pause");
		}
		//exiting on user choice
		else if (option == 3)
			return 0;
	}
}