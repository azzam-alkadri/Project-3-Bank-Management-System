#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;
const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOptions {
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,
    eShowTransactionsMenue = 6, eManageUsers = 7, eExit = 8
};

enum enManageUsersMenueOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
    eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
    pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64,
    pDeposit = 128, pWithdraw = 256, pTotalBalances = 512
};

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void Login();

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;


};


struct stUser {
    string UserName;
    string Password;
    int Permissions;
    bool MarkForDelete = false; 
};

stUser CurrentUser;

vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}

void PrintClientRecordLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void PrintClientRecordBalanceLine(sClient Client)
{

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;

}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}
void GoBackToTransactionsMenue()
{
    cout << "\n\nPress any key to go back to Transactions Menue...";
    system("pause>0");
    ShowTransactionsMenue();

}

void ShowAccessDeniedMessage()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "Access Denied, \nContact Your Admin.";
    cout << "\n-----------------------------------\n";
}

void ShowAllClientsScreen()
{

    if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void ShowTotalBalances()
{

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {

            PrintClientRecordBalanceLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";

}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{

    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;

}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;

}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }

    return vClients;

}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }

}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";


        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {

            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }

            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sClient>& vClients)
{


    char Answer = 'n';


    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";

    AddNewClients();

}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";

}

void ShowDepositScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeposit))
    {
        ShowAccessDeniedMessage();
        GoBackToTransactionsMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }


    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);

}

void ShowWithDrawScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pWithdraw))
    {
        ShowAccessDeniedMessage();
        GoBackToTransactionsMenue();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n-----------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();


    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    //Validate that the amount does not exceeds the balance
    while (Amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void ShowTotalBalancesScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToTransactionsMenue();
        return;
    }

    ShowTotalBalances();

}

int ReadPermissionsToSet() {
    int Permissions = 0;
    char Answer = 'n';

    cout << "\nDo you want to give full access? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') return -1;

    cout << "\nDo you want to give access to: \n ";

    cout << "\nShow Client List? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pListClients;

    cout << "\nAdd New Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pAddNewClient;

    cout << "\nDelete Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pDeleteClient;

    cout << "\nUpdate Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pUpdateClients;

    cout << "\nFind Client? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pFindClient;

    cout << "\nTransactions? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pTranactions;

    cout << "\nManage Users? y/n? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') Permissions += enMainMenuePermissions::pManageUsers;

    cout << "Can Deposit? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') Permissions += enMainMenuePermissions::pDeposit;

    cout << "Can Withdraw? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') Permissions += enMainMenuePermissions::pWithdraw;

    cout << "Can Show Total Balances? y/n? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') Permissions += enMainMenuePermissions::pTotalBalances;

    return Permissions;
}




string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
    string stUserLine = "";

    stUserLine += User.UserName + Seperator;
    stUserLine += User.Password + Seperator;
    stUserLine += to_string(User.Permissions);

    return stUserLine;
}


stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData;
    vUserData = SplitString(Line, Seperator); // دالة Split التي درستها سابقاً

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]); // تحويل النص إلى رقم

    return User;
}

vector<stUser> LoadUsersDataFromFile(string FileName)
{
    vector<stUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in); // فتح الملف في وضع القراءة

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {
            if (Line != "") // التأكد من أن السطر ليس فارغاً
            {
                User = ConvertUserLineToRecord(Line);
                vUsers.push_back(User);
            }
        }
        MyFile.close();
    }
    return vUsers;
}

// Show User*******************************************


void PrintUserRecordLine(stUser User)
{
    cout << "| " << left << setw(15) << User.UserName;
    cout << "| " << left << setw(10) << User.Password;
    cout << "| " << left << setw(12) << User.Permissions;
   
}


void ShowListUsersScreen() {

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "User Name";
    cout << "| " << left << setw(10) << "Password";
    cout << "| " << left << setw(12) << "Permissions";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    for (stUser User : vUsers) 
    {
        PrintUserRecordLine(User);

        cout << endl;
    }
    cout << "_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}


//Add User *********************************************************

bool IsUserExists(string UserName, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        stUser User;
        while (getline(MyFile, Line)) {
            User = ConvertUserLineToRecord(Line);
            if (User.UserName == UserName) {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}


void ShowAddNewUserScreen() {
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n-----------------------------------\n";

    stUser User;
    char AddMore = 'Y';

    do {
        cout << "\nEnter Username: ";
        cin >> User.UserName;

        while (IsUserExists(User.UserName, UsersFileName)) {
            cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username: ";
            cin >> User.UserName;
        }

        cout << "Enter Password: ";
        cin >> User.Password;

        User.Permissions = ReadPermissionsToSet();

        // حفظ المستخدم في الملف
        AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));

        cout << "\nUser Added Successfully, do you want to add more users? y/n? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

bool MarkUserForDeleteByUsername(string UserName, vector <stUser>& vUsers)
{
    for (stUser& U : vUsers)
    {
        if (U.UserName == UserName)
        {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out); // Overwrite
    
    if (MyFile.is_open())
    {
        for (stUser U : vUsers)
        {
            if (!U.MarkForDelete)
            {
                MyFile << ConvertUserRecordToLine(U) << endl;
            }
        }
        MyFile.close();
    }
}

string ReadUserName()
{
    string UserName = "";
    cout << "\nPlease enter Username? ";
    cin >> UserName;
    return UserName;
}

stUser ChangeUserRecord(string UserName)
{
    stUser User;
    User.UserName = UserName;

    cout << "\n\nEnter Password? ";
    cin >> User.Password;

    User.Permissions = ReadPermissionsToSet();

    return User;
}


//Delete User************************************************

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the user details:\n";
    cout << "-----------------------------------";
    cout << "\nUsername   : " << User.UserName;
    cout << "\nPassword   : " << User.Password;
    cout << "\nPermissions: " << User.Permissions;
    cout << "\n-----------------------------------\n";
}

void ShowDeleteUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDelete User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
   
    string UserName = ReadUserName();

    if (UserName == "Admin") // حماية المستخدم الأساسي
    {
        cout << "\n\nYou cannot delete this user.";
        return;
    }

    stUser User;
    if (IsUserExists(UserName, UsersFileName))
    {
        
        for (stUser U : vUsers) 
        { 
            if (U.UserName == UserName) 
            User = U; 
        }

        PrintUserCard(User);

        char Answer = 'n';
        cout << "\n\nAre you sure you want delete this user? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkUserForDeleteByUsername(UserName, vUsers);
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Deleted Successfully.";
        }
    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
    }
}

// Update User*************************************************

void ShowUpdateUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate User Screen";
    cout << "\n-----------------------------------\n";

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
   
    string UserName = ReadUserName();

    if (IsUserExists(UserName, UsersFileName))
    {
        char Answer = 'n';
        cout << "\n\nAre you sure you want update this user? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            for (stUser& U : vUsers)
            {
                if (U.UserName == UserName)
                {
                    PrintUserCard(U);

                    U = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUsersDataToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
        }
    }
    else
    {
        cout << "\nUser with Username (" << UserName << ") is Not Found!";
    }
}

// Find User***********************************************

void ShowFindUserScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n-----------------------------------\n";

    string UserName = ReadUserName();

    vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
    for (stUser U : vUsers)
    {
        if (U.UserName == UserName)
        {
            PrintUserCard(U);
            return;
        }
    }
    cout << "\nUser with Username [" << UserName << "] is not found!";
}


bool CheckAccessPermission(enMainMenuePermissions Permission) {
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
        return true;

    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}


short ReadManageUsersMenueOption() {
    cout << "Choose what do you want to do? [1 to 6]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void GoBackToManageUsersMenue() {
    cout << "\n\nPress any key to go back to Manage Users Menue...";
    system("pause>0");
    ShowManageUsersMenue(); // استدعاء القائمة مرة أخرى
}

void PerfromManageUsersMenueOption(enManageUsersMenueOptions ManageUsersMenueOption) {
    switch (ManageUsersMenueOption) {
    case enManageUsersMenueOptions::eListUsers:
        system("cls");
        ShowListUsersScreen(); // دالة عرض المستخدمين
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eAddNewUser:
        system("cls");
        ShowAddNewUserScreen(); // دالة إضافة مستخدم
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;
    case enManageUsersMenueOptions::eMainMenue:
        ShowMainMenue(); // العودة للقائمة الرئيسية
        break;
    }
}

void ShowManageUsersMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tManage Users Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.\n";
    cout << "===========================================\n";
    PerfromManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}




short ReadTransactionsMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionsMenue();
        break;
    }

    case enTransactionsMenueOptions::eWithdraw:
    {
        system("cls");
        ShowWithDrawScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowTotalBalance:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionsMenue();
        break;
    }


    case enTransactionsMenueOptions::eShowMainMenue:
    {

        ShowMainMenue();

    }
    }

}

void ShowTransactionsMenue()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransactions Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";
    cout << "===========================================\n";
    PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}

short ReadMainMenueOption()
{
    cout << "Choose what do you want to do? [1 to 8 ]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eShowTransactionsMenue:
        system("cls");
        ShowTransactionsMenue();
        break;

    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenue();
        break;

    case enMainMenueOptions::eExit:
        system("cls");
       // ShowEndScreen();
        Login();
        break;
    }

}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Exit.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool FindUserByUsernameAndPassword(string UserName, string Password, stUser& User) {

    vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);

    for (stUser U : vUsers) {
        if (U.UserName == UserName && U.Password == Password) {
            User = U;
            return true;
        }
    }
    return false;
}

void Login() {
    bool LoginFailed = false;
    string UserName, Password;

    do {
        system("cls");
        cout << "-----------------------------------\n";
        cout << "\tLogin Screen\n";
        cout << "-----------------------------------\n";

        if (LoginFailed) {
            cout << "\nInvalid Username/Password!\n";
        }

        cout << "Enter Username? ";
        cin >> UserName;
        cout << "Enter Password? ";
        cin >> Password;

        LoginFailed = !FindUserByUsernameAndPassword(UserName, Password, CurrentUser);

    } while (LoginFailed);

    ShowMainMenue(); // الدخول للقائمة الرئيسية بعد النجاح
}


int main()

{
    Login();
    system("pause>0");
    return 0;
}