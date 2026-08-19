#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cctype>


using namespace std;

const string ClientsFileName = "client_file.txt";

enum enMainMenueOptions { enShowClient = 1, enAddClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enTransactionClient = 6, enExit = 7 };
enum enTransactionsMenueOption { enDepositClient = 1, enWithdrawClient = 2, enTotalBalancesClients = 3, enMainMenue = 4 };
enum enTransactionType { enIsDeposit = 1, enIsWithdraw = 2, };

struct sClient
{
    string AccountNumber;
    string Name;
    string Phone;
    double AccountBalance;
    int PinCode;
    bool MarkClientDelete = false;
};

void showMainMenueScreen();

void ShowTransactionScreen();

vector<string> SplitString(string S1, string seperator = "#//#")
{
    short pos = 0;
    string sword = "";
    vector<string> vsword;

    while ((pos = S1.find(seperator)) != string::npos)
    {
        sword = S1.substr(0, pos);

        if (sword != "")
        {
            vsword.push_back(sword);
        }
        S1.erase(0, (pos + seperator.length()));
    }

    if (S1 != "")
    {
        vsword.push_back(S1);
    }
    return vsword;
}

sClient ConvertLineToRecord(string Line)
{
    sClient Client;

    vector<string> vS1;

    vS1 = SplitString(Line);

    Client.AccountNumber = vS1[0];
    Client.PinCode = stoi(vS1[1]);
    Client.Name = vS1[2];
    Client.Phone = vS1[3];
    Client.AccountBalance = stod(vS1[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string seperator = "#//#")
{
    string S1;

    S1 = Client.AccountNumber + seperator
        + to_string(Client.PinCode) + seperator
        + Client.Name + seperator
        + Client.Phone + seperator
        + to_string(Client.AccountBalance) + seperator;
    return S1;
}

vector<sClient> LoadDataFromFile(string FileName)
{
    fstream Fstream;
    vector<sClient> vsClients;
    sClient Client;

    Fstream.open(FileName, ios::in);

    if (Fstream.is_open())
    {
        string Line;
        while (getline(Fstream, Line))
        {
            Client = ConvertLineToRecord(Line);
            vsClients.push_back(Client);
        }
        Fstream.close();
    }
    return vsClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientRecord(sClient Client)
{

    cout << "\nThe following are the client details:";
    cout << "\n-----------------------------------\n";
    cout << "\nClient.AccountNumber: " << Client.AccountNumber;
    cout << "\nClient.PinCode: " << Client.PinCode;
    cout << "\nClient.Name: " << Client.Name;
    cout << "\nClient.Phone: " << Client.Phone;
    cout << "\nClient.AccountBalance: " << Client.AccountBalance;
    cout << "\n-----------------------------------";
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line = "";
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (AccountNumber == Client.AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

bool FindClientByAccountNumber(sClient& Client, string AccountNumber, vector<sClient> vClients)
{
    for (sClient& C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

void ShowAllClientsScreen()
{
    vector<sClient>  vClients = LoadDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Clients.";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient& C : vClients)
        {

            PrintClientRecordLine(C);
            cout << endl;
        }
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

sClient ReadNewClient()
{
    sClient Client;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient Wiht [ " << Client.AccountNumber << " ] is already exists, Enter Another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    cin >> Client.PinCode;

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter PinCode? ";
    cin >> Client.PinCode;

    cout << "Enter Name? ";
    getline(cin >> ws, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

void AddClientDatatoFile(sClient Client, string FileName)
{
    fstream MyFile;

    MyFile.open(FileName, ios::app);

    if (MyFile.is_open())
    {
        string S1 = "";

        S1 = ConvertRecordToLine(Client);
        MyFile << S1 << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddClientDatatoFile(Client, ClientsFileName);
}

void AddNewClients()
{
    cout << "===========================================\n";
    cout << "\t\Add New Clients Screen\n";
    cout << "===========================================\n";

    char AddMore = ' ';

    do
    {
        cout << "\nAdd new Client:\n\n";

        AddNewClient();

        cout << "\n Client Added Successfully, do you want to add more client? Y/N? ";
        cin >> AddMore;

    } while (tolower(AddMore) == 'y');

}

string ReadAccountNumber()
{
    string AccountNumber = "";

    cout << "Enter Account Number? ";
    getline(cin >> ws, AccountNumber);

    return AccountNumber;
}

void SaveClientDataToFile(vector<sClient>& vClients)
{
    fstream MyFile;

    MyFile.open(ClientsFileName, ios::out);

    if (MyFile.is_open())
    {
        string Line = "";

        for (sClient& C : vClients)
        {
            if (C.MarkClientDelete == false)
            {
                Line = ConvertRecordToLine(C);

                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
}

bool MarkClientDeleteByAccountNumber(vector<sClient>& vClients, string AccountNumber)
{
    for (sClient& C : vClients)
    {
        if (AccountNumber == C.AccountNumber)
        {
            C.MarkClientDelete = true;
            return C.MarkClientDelete;
        }
    }
    return false;
}

bool DeleteClientByAccountNumber(vector<sClient>& vClients, string AccountNumber)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        PrintClientRecord(Client);

        cout << "\nAre you sure you want delete this client? y/n? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkClientDeleteByAccountNumber(vClients, AccountNumber);
            SaveClientDataToFile(vClients);
            vClients = LoadDataFromFile(ClientsFileName);
            cout << "\n Client Deleted successfully,";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }

}

bool UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        PrintClientRecord(Client);

        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            for (sClient& C : vClients)
            {
                if (Client.AccountNumber == C.AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveClientDataToFile(vClients);
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

double ReadAmountNumber(string message)
{
    double Amount = 0;

    cout << message;
    cin >> Amount;

    return Amount;
}

double CalculateNewBalance(double AccountBalance, double AmountNumber)
{  
    return (AccountBalance + AmountNumber);
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<sClient> vClient)
{
    char answer = 'n';

    cout << "\nAre yoy sure you want perfrom this transaction? Y/N ? ";
    cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        for (sClient& C : vClient)
        {
            if (AccountNumber == C.AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientDataToFile(vClient);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
}
 
void ShowDeleteClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\t\Delete Clients Screen\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    DeleteClientByAccountNumber(vClients, AccountNumber);
}

void ShowUpdateClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\t\tUpdate Client info Screen\n";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);

}

void ShowFindClientScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    string AccountNumber = ReadAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(Client, AccountNumber, vClients))
        PrintClientRecord(Client);
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
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients =  LoadDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadAccountNumber();

    
        //IsClientExist = TransactionClientByAccountNumber(AccountNumber, vClients, true);
    while (!FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        cout << "\nClient with [" << AccountNumber << "] does nont exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientRecord(Client);

    double amount = 0;
    
    cout << "\nPlease enter deposit amount? ";
    cin >> amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, amount, vClients);

    

}

void ShowWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Withdraw Screen";
    cout << "\n-----------------------------------\n";

    vector<sClient> vClients = LoadDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(Client, AccountNumber, vClients))
    {
        cout << "\nClient with [" << AccountNumber << "] does nont exist.\n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientRecord(Client);

    double Amount = 0;

    cout << "\nPlease enter Withdraw amount ?";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(AccountNumber, Amount * (-1), vClients);

}

double CalculateTotalOfAllBalancesAmount(vector<sClient> vClients)
{
    double SumBalances = 0;

    for (sClient& C : vClients)
    {
        SumBalances += C.AccountBalance;
    }
    return SumBalances;
}

void ShowTotalBalancesScreen()
{
    vector<sClient>  vClients = LoadDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Clients.";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else
    {
        for (sClient& Client : vClients)
        {

            cout << "| " << setw(15) << left << Client.AccountNumber;
            cout << "| " << setw(40) << left << Client.Name;
            cout << "| " << setw(12) << left << Client.AccountBalance;
            cout << endl;
        }
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "\t\t\t Total Balances = " << CalculateTotalOfAllBalancesAmount(vClients) << endl;
}

void GoBackToTransactionScreen()
{
    cout << "\n\nPress any key to go back to Transaction Menue...";
    system("pause>0");
    ShowTransactionScreen();
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    showMainMenueScreen();
}

short ReadMainMenueOptions()
{
    cout << "Choose what do you want to do? [1 to 7]? ";

    short choice = 0;
    cin >> choice;
    return choice;

}

short ReadTransactionMenueOption()
{
    cout << "Choose what do you want to do? [1 to 4]? ";

    short choice = 0;
    cin >> choice;
    return choice;

}

void PerformTransactionMenueOption(enTransactionsMenueOption  TransactionsMenueOption)

{
    switch (TransactionsMenueOption)
    {
    case enTransactionsMenueOption::enDepositClient:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enWithdrawClient:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enTotalBalancesClients:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionScreen();
        break;
    }
    case enTransactionsMenueOption::enMainMenue:
    {
        showMainMenueScreen();
    }
    }
}

void ShowTransactionScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransaction Menus Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";

    PerformTransactionMenueOption((enTransactionsMenueOption)ReadTransactionMenueOption());
}

void PerformMainMenueOption(enMainMenueOptions  MainMenueOptions)

{
    switch (MainMenueOptions)
    {
    case enMainMenueOptions::enShowClient:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enAddClient:
    {
        system("cls");
        AddNewClients();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enFindClient:
    {
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::enTransactionClient:
    {
        system("cls");
        ShowTransactionScreen();
        break;
    }
    case enMainMenueOptions::enExit:
    {
        system("cls");
        ShowEndScreen();
        break;
    }
    }
}

void showMainMenueScreen()
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
    cout << "\t[6] Transaction Client.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";

    PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOptions());
}

int main()
{
    //SplitString("A150#//#1234#//#mohamed ashraf#//#+201111111111#//#1500.000000", "#//#");
    showMainMenueScreen();
    system("pause>0");
    return 0;
}