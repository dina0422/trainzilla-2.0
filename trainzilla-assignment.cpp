#include<iostream>
#include<fstream>
#include<map>
#include<iomanip> //maths functions
#include<chrono>
#include<ctime>  //get real-timing
#include <conio.h> //hide password input
using namespace std;

const int MAX_USERS =100;
const int MAX_ADMINS =20;
const double PRICE_PER_STOP =0.30;
const int MAX_TRAIN_STOPS = 10;

string trainStops[MAX_TRAIN_STOPS]={
	"Gombak",
	"Taman Melati",
	"Wangsa Maju",
	"Sri Rampai",
	"Setiawangsa",
	"Jelatek",
	"Dato' Keramat",
	"Damai",
	"Ampang Park",
	"KlCC",
};

int numTrainStops = 10;

struct User{
	string username;
	string password;
};

User users[MAX_USERS];
User admins[MAX_ADMINS];  //Array to store users & admins

int numUsers=0;
int numAdmins=0;

int homePage();
void userHome();

void saveToFile(User userList[], int count, const string &filename){
	ofstream file(filename);
	if(file.is_open()){
		for(int i=0; i<count; ++i){
			file<<userList[i].username<<" "<<userList[i].password<<endl;
		}
		file.close();
	}else{
		cout<<"Unable to save user credentials."<<endl;
	}
}

void readFromFile(User userList[], int &count, const string &filename, int maxSize){
	ifstream file(filename);
	if(file.is_open()){
		User user;
		while(file>>user.username>>user.password){
			if(count<maxSize){
				userList[count++]=user;
			}else{
				cout<<"Maximum number of users reached."<<endl;
				break;
			}
		}
		file.close();
	}
}

void createUser(User userList[], int&count, const string &filename, int maxSize){
	if(count>=maxSize){
		cout<<"Maximum number of users reached."<<endl;
		return;
	}

	string username, password;
	cout<<"Enter username: ";
	cin>>username;

	for(int i=0; i<count; ++i){
		if(userList[i].username==username){
			system("cls");
			cout<<"Username already exists. Please choose another username."<<endl;
			return;
		}
	}

	cout<<"Enter password: ";
	cin>>password;

	User newUser;
	newUser.username=username;
	newUser.password=password;

	userList[count++]=newUser;
	saveToFile(userList, count, filename);
	cout<<"Account created successfully!"<<endl;
	system("cls");
}

bool login(User userList[], int count){
	system("cls");
	string username;
	bool validUser=false;

	while(!validUser){
		cout << "=======================" << endl;
		cout << "||       LOGIN       ||" << endl;
		cout << "=======================" << endl;

		cout << "Enter username: " ;
		cin >> username;

		cout << "Enter password: ";
		char ch;
        string password;

        while ((ch = _getch()) != '\r') {  // Read characters until Enter is pressed
            if (ch == '\b' && !password.empty()) {  // Check for backspace
                cout << "\b \b";  // Move the cursor back, erase the character, and move the cursor back again
                password.pop_back();  // Remove the last character from the string
            } else if (ch != '\b') {  // Handle regular characters
                cout << '*';  // Print asterisks instead of the actual character
                password.push_back(ch);  // Add the character to the password string
            }
        }

		for(int i=0; i<count; ++i){
			if(userList[i].username == username && userList[i].password ==password){
				cout<<"Login successful!"<<endl;
				// system("cls");
				return true;
			}
		}
		system("cls");
		cout<<"Incorrect username or password. Please try again."<<endl;
	}

	return false;
}

int homePage(){
	int choice;

	cout << "============================" << endl;
	cout << "||  WELCOME TO TRAINZILLA ||" << endl;
	cout << "|| [1] USER               ||" << endl;
	cout << "|| [2] ADMIN              ||" << endl;
	cout << "|| [3] EXIT               ||" << endl;
	cout << "============================" << endl;

	cout << "Select your choice: ";
	cin >> choice;

	return choice;
}

void bookingHistory(){
	system("cls");
	int choice;

	do{
		ifstream bookingsFile("user_bookings.txt");

		if (bookingsFile.is_open()) {
        string line;
        cout << "==================================" << endl;
        cout << "       Your Booking History       " << endl;
        cout << "==================================" << endl;

        int no = 1;
        while (getline(bookingsFile, line)) {
            stringstream ss(line);
            string dateTime, start, end, price;

            getline(ss, dateTime, ',');
            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, price);

            cout << "Booking " << no << ":" << endl;
            cout << "Date & Time: " << dateTime << endl;
            cout << "Start: " << start << endl;
            cout << "End: " << end << endl;
            cout << "Price: $" << price << endl << endl;
            ++no;
        }
        bookingsFile.close();
    } else {
        cout << "No past bookings found." << endl;
    }

		cout << "\nEnter 0 to go back: ";
		cin >> choice;
	}while(choice!=0);

	if(choice==0){
		system("cls");
		userHome();
	}
}

void displayTrainStops(){
	cout<<"========================================="<<endl;
	cout<<"||             BOOK TICKET             ||"<<endl;
	cout<<"-----------------------------------------"<<endl;
	cout<<"========================================="<<endl;
	for(int i=0; i<numTrainStops; ++i){
		cout<< i+1 <<". " <<trainStops[i]<<endl;
	}
	cout<<"========================================="<<endl;
}

void calculateFare(int start, int end){
	cout << "\nCalculating fare....\n" << endl;
	if(start <1 || start > numTrainStops || end < 1 || end > numTrainStops){
		cout << "Invalid start or end station." << endl;
		return;
	}

	cout << "Chosen Start Station: " << trainStops[start -1] << endl;
	cout << "Chosen End Station: " << trainStops[end -1] << endl;
	
	int numStops =abs(end-start) +1;
	double totalPrice = numStops * PRICE_PER_STOP;

	cout << "Number of Stops: " << numStops << endl;
	cout << "Total Price $" << fixed << setprecision(2) << totalPrice << endl;

	//Get current date and time
	auto now = std::chrono::system_clock::now();
	std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
	std::string dateTime = std::ctime(&timeNow);
	dateTime.pop_back(); //remove newline character

	char confirm;
	cout << "Confirm purchase? (Y/N): ";
	cin >> confirm;

	if (toupper(confirm) == 'Y') {
    ofstream bookingsFile("user_bookings.txt", ios::app);
		if (bookingsFile.is_open()) {	
			bookingsFile << dateTime << ","
				<< trainStops[start - 1] << ","
				<< trainStops[end - 1] << ","
				<< fixed << setprecision(2) << totalPrice << endl;
			bookingsFile.close();
			cout << "Booking confirmed and saved!" << endl;
		} else {
			cout << "Unable to save booking." << endl;
		}
	} else {
    cout << "Unable to save booking." << endl;
	}

}

void bookTicket(){
	int start, end, choice;

	do{
		system("cls");
		displayTrainStops();
		cout << "Enter start station ID: ";
		cin >> start;
		cout << "Enter end station ID: ";
		cin >> end;
		calculateFare(start,end);

		cout << "\nDo you want to rebook? (1:Yes / 0:No): ";
		cin >> choice;
	}while(choice==1);

	if(choice==0){
		system("cls");
		userHome();
	}
}

void userHome(){
	int choice;

	do{
		system("cls");
		cout << "============================" << endl;
		cout << "||          HOME          ||" << endl;
		cout << "============================" << endl;
		cout << "|| [1] BOOK A TICKET      ||" << endl;
		cout << "|| [2] VIEW HISTORY       ||" << endl;
		cout << "|| [3] LOGOUT             ||" << endl;
		cout << "============================" << endl;

		cout << "Select your choice: ";
		cin >> choice;

	switch(choice){
		case 1: bookTicket();
				break;
		case 2: bookingHistory();
				break;
		case 3: homePage();
				break;
		default: cout << "Unknown choice.Please re-enter.";
	}
	}while(choice!=3);
}

void editRoute() {
    int choice;

    do {
        system("cls");
		displayTrainStops();
        cout << "===================================" << endl;
        cout << "||      EDIT ROUTE/TICKET LIST    ||" << endl;
        cout << "===================================" << endl;
        cout << "|| [1] ADD NEW STOP               ||" << endl;
        cout << "|| [2] REMOVE A STOP              ||" << endl;
        cout << "|| [3] MODIFY EXISTING STOP       ||" << endl;
        cout << "|| [4] BACK TO ADMIN MENU         ||" << endl;
        cout << "===================================" << endl;

        cout << "Select your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (numTrainStops >= MAX_TRAIN_STOPS) {
                    cout << "Maximum number of stops reached." << endl;
                } else {
                    string newStop;
                    cout << "Enter the name of the new stop: ";
                    cin.ignore(); // Ignore newline character in buffer
                    getline(cin, newStop);

                    trainStops[numTrainStops++] = newStop;
                    cout << "New stop added successfully!" << endl;
                }
                break;
            }
            case 2: {
                int stopIndex;
                cout << "Enter the index of the stop to remove: ";
                cin >> stopIndex;

                if (stopIndex >= 1 && stopIndex <= numTrainStops) {
                    for (int i = stopIndex - 1; i < numTrainStops - 1; ++i) {
                        trainStops[i] = trainStops[i + 1];
                    }
                    numTrainStops--;
                    cout << "Stop removed successfully!" << endl;
                } else {
                    cout << "Invalid stop index." << endl;
                }
                break;
            }
            case 3: {
                int stopIndex;
                cout << "Enter the index of the stop to modify: ";
                cin >> stopIndex;

                if (stopIndex >= 1 && stopIndex <= numTrainStops) {
                    string modifiedStop;
                    cout << "Enter the modified name for the stop: ";
                    cin.ignore(); // Ignore newline character in buffer
                    getline(cin, modifiedStop);

                    trainStops[stopIndex - 1] = modifiedStop;
                    cout << "Stop modified successfully!" << endl;
                } else {
                    cout << "Invalid stop index." << endl;
                }
                break;
            }
            case 4:
                return; // Go back to admin menu
            default:
                cout << "Unknown choice. Please re-enter." << endl;
        }
        cout << "Press Enter to continue...";
        cin.ignore(); // Wait for Enter key press
        cin.get();
    } while (true);
}

void adminHome(){
	int choice;

	do{
		system("cls");
		cout << "=================================" << endl;
		cout << "||            HOME             ||" << endl;
		cout << "|| [1] EDIT ROUTE/TICKET LIST  ||" << endl;
		cout << "|| [2] LOGOUT                  ||" << endl;
		cout << "=================================" << endl;

	cout << "Select your choice: ";
	cin >> choice;
	switch(choice){
		case 1: editRoute();
				break;
		case 2: return;
		default: cout << "Unknown choice.Please re-enter.";
	}
	}while(true);
}

void user(){
	readFromFile(users, numUsers, "user_credentials.txt", MAX_USERS);
	
	int choice;

	do{
		system("cls");
		cout << "====================================================" << endl;
		cout << "||              WELCOME TO TRAINZILLA             ||" << endl;
		cout << "||  [1] LOGIN                                     ||" << endl;
		cout << "||  [2] CREATE AN ACCOUNT                         ||" << endl;
		cout << "||  [3] BACK                                      ||" << endl;
		cout << "====================================================" << endl;

	cout << "Select your choice: ";
	cin >> choice;

	switch(choice){
		case 1: if(login(users, numUsers)){
				userHome();
				}
				break;
		case 2: createUser(users, numUsers, "user_credentials.txt", MAX_USERS);
				break;
		case 3: return;
				break;
		default: cout << "Unknown choice.Please re-enter.";
	}
	}while(choice!=3);
}

void admin(){
	readFromFile(admins, numAdmins, "admin_credentials.txt", MAX_ADMINS);

	int choice;

	do{
		system("cls");
		cout << "===============================================" << endl;
		cout << "||         WELCOME TO TRAINZILLA ADMIN       ||" << endl;
		cout << "||  [1] LOGIN                                ||" << endl;
		cout << "||  [2] BACK                                 ||" << endl;
		cout << "===============================================" << endl;

		cout << "Select your choice: ";
		cin >> choice;

	switch(choice){
		case 1: if(login(admins, numAdmins)){
				adminHome();
				}
				break;
		case 2: return;
				break;
		default: cout << "Unknown choice.Please re-enter.";
	}
	}while(choice!=2);
}

int main()
{
	int choice;

	do{
	system("cls");
	choice = homePage();
	switch(choice){
		case 1: user();
				break;
		case 2: admin();
				break;
		case 3: cout << "Exiting...";;
				break;
		default: cout <<"Unknown choice.Please re-enter. \n" << endl;
	}
	}while(choice !=3);
	
	return 0;

}