#include <iostream>;
#include <cctype>;
#include <fstream>;
#include <string>;
#include <random>;
#include <ctime>;
#include <map>
#include <cstdio>;
#include <vector>;
#include <iomanip>;
#include <sstream>;

using namespace std;

void MainMenu();
void LoginPage();

bool IsNumber(string str) {
	for (char& c: str) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
}

bool FindAccountNumber(string AccountNumber = "") {
	if (AccountNumber == "") { return false; };

	ifstream AccountListDataR("AccountListData.txt");
	string AccountNumbers;
	float Balance;
	while (AccountListDataR >> AccountNumbers >> Balance)
	{
		if (AccountNumbers == AccountNumber) {
			AccountListDataR.close();
			return true;
		}
	}
	AccountListDataR.close();
	return false;

}
float GetBalance(string AccountNumber = "") {
	if (AccountNumber == "") { return 0.0f; };

	ifstream AccountListDataR("AccountListData.txt");
	string AccountNumbers;
	float Balance;
	while (AccountListDataR >> AccountNumbers >> Balance)
	{
		if (AccountNumbers == AccountNumber) {
			AccountListDataR.close();
			return Balance;
		}
	}
	AccountListDataR.close();
	return Balance;

}

map<string,map<string,string>> FindAccount(string FindUsername = "", string FindPassword = "") {
	if (FindUsername == "" or FindPassword == "") return {};

	string Ic;
	string AccountNumber;
	string Name;
	string Pin;
	string Username;
	string Password;

	ifstream AccountDetailR("AccountDetail.txt");

	while (AccountDetailR >> AccountNumber >> Ic >> Name >> Username >> Password >> Pin)
	{
		if (FindUsername == Username && FindPassword == Password) {
			AccountDetailR.close();
			return {
				{"Account", {
					{"AccountNumber",AccountNumber},
					{"Ic",Ic},
					{"Name",Name},
					{"Username",Username},
					{"Password", Password},
					{"Pin",Pin},
				}} };
		}
	}
	return {};
}

map<string, map<string, string>> FindAccountByAccountNumber(string AccountNumberDetail = "") {
	if (AccountNumberDetail == "") return {};

	string Ic;
	string AccountNumber;
	string Name;
	string Pin;
	string Username;
	string Password;

	ifstream AccountDetailR("AccountDetail.txt");

	while (AccountDetailR >> AccountNumber >> Ic >> Name >> Username >> Password >> Pin)
	{
		if (AccountNumberDetail == AccountNumber) {
			AccountDetailR.close();
			return {
				{"Account", {
					{"AccountNumber",AccountNumber},
					{"Ic",Ic},
					{"Name",Name}
				}} };
		}
	}
	return {};

}

string AccountNumberGenarator() {
	string AccountNumber = "";

	for (size_t i = 0; i < 8; i++)
	{
		mt19937 engine(random_device{}());
		uniform_int_distribution<> distance(1, 9);

		int RandomNum = distance(engine);
		AccountNumber = AccountNumber + to_string(RandomNum);
	}
	if (FindAccountNumber(AccountNumber))  return AccountNumberGenarator(); 

	return AccountNumber;
}
int parseAge(string YearBirth = "") {
	if (YearBirth == "" || !IsNumber(YearBirth)) return -1; 

 	time_t timestamp = time(NULL);
	struct tm localTime;
	localtime_s(&localTime, &timestamp);
	

	int CurrentYear = localTime.tm_year + 1900;
	int Age = CurrentYear;
	if (stoi(YearBirth) >= 60) {
		YearBirth = "19" + YearBirth;
	}
	else
	{
		YearBirth = "20" + YearBirth;
	}

	return CurrentYear - stoi(YearBirth); 
}

void CreateLog(string AccountNumber = "", string Message = "Test Ping By Admin!") {
	if (AccountNumber == "") return;

	if (!FindAccountNumber(AccountNumber)) return;

	time_t timestamp = time(NULL);
	struct tm localTime;

	localtime_s(&localTime, &timestamp);

	char Timestamp[100];
	strftime(Timestamp, sizeof(Timestamp), "%Y-%m-%d %H:%M:%S", &localTime);

	ofstream LogAccount("LogAccount.txt", ios::app);
	if (LogAccount.is_open()) {
		LogAccount << AccountNumber << "|" << Timestamp << "|" << Message<< "\n";
		LogAccount.close();
	}
}

vector<string> ParseLog(string AccountNumber = "") {
	if (AccountNumber == "") return {};

	ifstream LogAccount("LogAccount.txt");

	vector<string> ToReturn = {};

	while (LogAccount)
	{
		string ToPush = "";

		string Line;
		getline(LogAccount, Line);

		size_t p1 = Line.find("|");
		size_t p2 = Line.find("|", p1 + 1);

		string FinderAccountNumber = Line.substr(0, p1);
		string Timestamp = Line.substr(p1 + 1, p2 - p1 - 1);
		string Message = Line.substr(p2 + 1);

		if (AccountNumber == FinderAccountNumber) {
			ToReturn.push_back("[" + Timestamp + "] " + Message);
		}
	}
	LogAccount.close();

	return ToReturn;

}
void Intialize_Account(string Name, string Ic, string Username, string Password) {
	string AccountNumber = AccountNumberGenarator();
	float Balance = 0.00f;


	ofstream AccountListDataW("AccountListData.txt", ios::app);

	if (AccountListDataW.is_open()) {
		AccountListDataW << AccountNumber << " " << Balance<<"\n";
		AccountListDataW.close();
	}

	string Pin;

	string Birthday = "";
	string YearBirth = "";

	YearBirth = YearBirth + Ic[0] + Ic[1];
	Birthday = Birthday + Ic[4] + Ic[5] + '/' + Ic[2] + Ic[3] + '/' + (stoi(YearBirth) >= 60 ? "19" : "20") + Ic[0] + Ic[1];

	int Age = parseAge(YearBirth);

	cout << "Your Account has successfully Created"<< endl<< endl;
	cout << "Account Number  : " << AccountNumber<<endl;
	cout << "Account Balance : Rm " << Balance << endl;
	cout << "Birthday        : " << Birthday<<endl;
	cout << "Age             : " << Age << endl<<endl;
	cout << "Please Enter 6 Digit Pin : "; cin >> Pin;

	while (Pin.length() != 6)
	{
		cout << "Please enter 6 Digit Pin : "; cin >> Pin;

	}

	ofstream AccountDetailW("AccountDetail.txt", ios::app);

	if (AccountDetailW.is_open()){
		AccountDetailW<< AccountNumber << " " << Ic << " " << Name << " " << Username << " " << Password << " " << Pin << "\n";
		AccountDetailW.close();
	}

	int Option;
	cout<< ""<<endl<<endl;
	cout << "Yeayy Your account Initialization Has been Complete!" << endl;
	cout << "[0] Exit\n[1] Main Menu \n Choose Option : "; cin >> Option;
	switch (Option)
	{
	case 1:
		MainMenu();
		break;
	default:
		exit(0);
		break;
	}
}

string NewTransfer(string Sender_AccountNumber = "", string Reciever_AccountNumber = "", float Amount = 0.0f) {
	if (Sender_AccountNumber == "" or Reciever_AccountNumber == "") return "Account Not Found!";

	if (!FindAccountNumber(Sender_AccountNumber)) return "Account Not Found";
	if (!FindAccountNumber(Reciever_AccountNumber)) return "Invalid Reciever Account!";
	if (GetBalance(Sender_AccountNumber) < Amount) return "Not Enough Credit!";

	map<string, map<string, string>> SenderDetail = FindAccountByAccountNumber(Sender_AccountNumber);
	map<string, map<string, string>> RecieverDetail = FindAccountByAccountNumber(Reciever_AccountNumber);

	string AccountNumber;
	float Balance;

	
	ifstream AccountListDataR("AccountListData.txt");
	ofstream temp("temp.txt");

	if (AccountListDataR.is_open()) {
		while (AccountListDataR >> AccountNumber >> Balance)
		{
			if (AccountNumber == Sender_AccountNumber) {
				Balance = Balance - Amount;
			}
			else if (AccountNumber == Reciever_AccountNumber) {
				Balance = Balance + Amount;
			}
			temp << AccountNumber << " " << Balance << "\n";
		}

		AccountListDataR.close();
		temp.close();

		remove("AccountListData.txt");
		rename("temp.txt", "AccountListData.txt");

		stringstream FormatAmount;

		FormatAmount << fixed << setprecision(2) << Amount;

		CreateLog(Sender_AccountNumber, "Transfered RM " + FormatAmount.str() + " To " + RecieverDetail["Account"]["Name"]);
		CreateLog(Reciever_AccountNumber, "Recieved RM " + FormatAmount.str() + " From " + SenderDetail["Account"]["Name"]);

		return "Successfully Transfer Money To Reciever Account!";

	}
	return "Error!";
}

void TransferPage(map<string, map<string, string>> Sender_AccountDetail = {}) {
	string Reciever_AccountNumber;

	cout << "Please Enter Receiver Account Number : "; cin >> Reciever_AccountNumber;
	cout << "Searching ...."<<endl <<endl;

	if (!FindAccountNumber(Reciever_AccountNumber)) { cout << "Account Not Found!"; return; };
	map<string, map<string, string>> Receiver_AccountDetail = FindAccountByAccountNumber(Reciever_AccountNumber);
	
	if (Receiver_AccountDetail.empty()) { cout << "Account Not Found!"; return; };

	cout << "Reciever Information"<<endl;
	cout << "Account Number : " << Reciever_AccountNumber << endl;
	cout << "Reciever Name  : " << Receiver_AccountDetail["Account"]["Name"]<<endl<< endl;


	int Option;
	string Pin;
	float Amount;
	float Balance = GetBalance(Sender_AccountDetail["Account"]["AccountNumber"]);

	cout << "[1] Enter Amount \n";
	cout << "[2] New Transfer \n";
	cout << "[0] Main Menu \n";
	cout << "Select Option : "; cin >> Option;

	switch (Option)
	{
	case 1:
		cout << "Your Balance : RM " << Balance << endl;
		cout << "Enter Amount : RM "; cin >> Amount;
		if (Balance >= Amount) {
			cout << "Enter 6 Digit Pin To Complete Transfer : "; cin >> Pin;
			while (Pin != Sender_AccountDetail["Account"]["Pin"])
			{
				cout << "Enter 6 Digit Pin To Complete Transfer : "; cin >> Pin;
			}
			cout << NewTransfer(Sender_AccountDetail["Account"]["AccountNumber"], Reciever_AccountNumber, Amount)<< endl;
			LoginPage();
		}
		else
		{
			MainMenu();
		}
		break;
	case 2:
		TransferPage(Sender_AccountDetail);
		break;
	default:
		MainMenu();
		break;
	}

}

void InformationDetail(map<string, map<string, string>> AccountDetail = {}) {
	if (AccountDetail.empty()) return;
	system("cls");

	string Birthday = "";
	string YearBirth = "";
	string Ic = AccountDetail["Account"]["Ic"];

	YearBirth = YearBirth + Ic[0] + Ic[1];
	Birthday = Birthday + Ic[4] + Ic[5] + '/' + Ic[2] + Ic[3] + '/' + (stoi(YearBirth) >= 60 ? "19" : "20") + Ic[0] + Ic[1];

	int Age = parseAge(YearBirth);

	cout << "Full Name : " << AccountDetail["Account"]["Name"] << endl;
	cout << "Ic        : " << Ic << endl;
	cout << "Username  : " << AccountDetail["Account"]["Username"] << endl;
	cout << "Password  : " << AccountDetail["Account"]["Password"] << endl;
	cout << "Birthday  : " << Birthday << endl;
	cout << "Age       : " << Age << endl <<endl;

	int Option;

	cout << "[0] Back\nSelect Option : "; cin >> Option;
	switch (Option)
	{
	default:
		LoginPage();
		break;
	}
}

void Transaction(string AccountNumber = "") {
	vector<string> Log = ParseLog(AccountNumber);

	for (size_t i = 0; i < Log.size(); i++)
	{
		string Data = Log[i];
		cout << Data << endl;
	}

	int Option;
	cout << "" << endl << endl;
	cout << "[0] Back\nSelect Option : "; cin >> Option;
	switch (Option)
	{
	default:
		LoginPage();
		break;
	}

}

void LoginPage() {
	system("cls");
	string Username;
	string Password;

	cout << "WELCOME TO MY BANK"<<endl<<endl;
	cout << "Username : "; cin >> Username;
	cout << "Password : "; cin >> Password;

	system("cls");

	map<string, map<string, string>> AccountDetail = FindAccount(Username, Password);

	if (AccountDetail.empty()) return LoginPage();

	system("cls");

	string UsernameDetail = AccountDetail["Account"]["Username"];
	string AccountNumberDetail = AccountDetail["Account"]["AccountNumber"];
	float AccountBalanceDetail = GetBalance(AccountNumberDetail);

	int Option;

	cout << "WELCOME BACK " << UsernameDetail << endl<<endl;
	
	cout << "Account Number : " << AccountNumberDetail<< endl;
	cout << "Account Balance : Rm " << AccountBalanceDetail << endl<< endl;

	cout << "[1] Information Detail" << endl;
	cout << "[2] Transfer Money" << endl;
	cout << "[3] Transaction" << endl;
	cout << "[0] Log Out" << endl<< endl;

	cout << "Select Option : "; cin >> Option;

	switch (Option)
	{
	case 1:
		InformationDetail(AccountDetail);
		break;
	case 2:
		system("cls");
		TransferPage(AccountDetail);
		break;
	case 3:
		system("cls");
		Transaction(AccountNumberDetail);
	default:
		MainMenu();
		break;
	}

}
void CreateAccount() {
	system("cls");
	string Name;
	string Ic;
	string Username;
	string Password;
	int Option;
	cout << "WELCOME TO MY BANK"<<endl<<endl;

	cout << "Enter Your Name :"; cin >> Name;

	cout << "Enter Your Ic :"; cin >> Ic;
	while (Ic.length() != 12 || !IsNumber(Ic))
	{
		cout << "" << endl;
		cout << "Ic Must be 12 and Only Number!"<<endl;
		cout << "Enter Your Ic :"; cin >> Ic;
	}

	cout << "Enter Your Username :"; cin >> Username;

	cout << "Enter Your Password :"; cin >> Password;

	system("cls");

	cout << "Name : "<<Name<<endl;
	cout << "Ic : " << Ic<<endl;
	cout << "Username : " << Username<<endl;
	cout << "Password : " << Password << endl<<endl;

	cout << "[0] Main Menu\n[1]Confirm"<<endl;

	cout << "Are you sure To Proceed :"; cin >> Option;

	switch (Option)
	{
	case 1:
		system("cls");
		Intialize_Account(Name,Ic,Username,Password);

		break;
	default:
		MainMenu();
		break;
	}


}
void MainMenu() {
	system("cls");
	int Option;
	cout << "Choose Option\n";
	cout << "[1] Log In\n";
	cout << "[2] Create New Acoount\n";
	cout << "[0] Exit\n" << endl;

	cout << "Select Option to Proceed :";
	cin >> Option;

	switch (Option)
	{
	case 0:
		exit(0);
		break;
	case 1:
		LoginPage();
		break;
	case 2:
		CreateAccount();
		break;
	default:
		MainMenu();
		break;
	}
}
int main() {
	MainMenu();
	system("pause");
	return 0;
}

