#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
using namespace std;

// Car class
class Car 
{
private:
    string registrationNumber;
    string ownerName;
    string carModel;

public:
    Car(string regNum = " ", string owner = " ", string model = " ")
		{
		registrationNumber=	regNum;
		ownerName=owner;
		carModel=model;
		}

    void displayCarInfo()  
	{
        cout << "Registration Number: " << registrationNumber << endl;
        cout << "Owner Name: " << ownerName << endl;
        cout << "Car Model: " << carModel << endl;
    }

    void saveToFile(ofstream& outFile) 
	 {
        outFile << registrationNumber << "," << ownerName << "," << carModel << endl;
    }

    void loadFromFile(ifstream& inFile) 
	{
        getline(inFile, registrationNumber, ',');
        getline(inFile, ownerName, ',');
        getline(inFile, carModel);
    }

    string getRegistrationNumber()  
	{
        return registrationNumber;
    }
};

// ParkingSlot class
class ParkingSlot 
{
private:
    int slotNumber;
    bool isOccupied;
    Car parkedCar;

public:
    ParkingSlot(int num = 0)
	{
		slotNumber=num;
		isOccupied=false;
	}

    void occupy(const Car& car) {
        isOccupied = true;
        parkedCar = car;
    }

    void vacate() {
        isOccupied = false;
    }

    void saveToFile(ofstream& outFile)  {
        outFile << slotNumber;
		outFile << isOccupied;
        if (isOccupied) {
            parkedCar.saveToFile(outFile);
        } else {
            outFile << endl;
        }
    }

    void loadFromFile(ifstream& inFile) {
        string occupied;
        inFile >> slotNumber;
        inFile.ignore(); // ignore comma
        getline(inFile, occupied);
        isOccupied = (occupied == "1");
        if (isOccupied) {
            parkedCar.loadFromFile(inFile);
        }
    }

    int getSlotNumber() const {
        return slotNumber;
    }

    bool getIsOccupied() const {
        return isOccupied;
    }

    Car getParkedCar() const {
        return parkedCar;
    }
};

// Ticket class
class Ticket {
private:
    string ticketID;
    string carRegistrationNumber;
    int slotNumber;
    string issueTime;

public:
    Ticket(string id = "", string regNum = "", int slotNum = 0, string time = "")
		{
		ticketID=id;
		carRegistrationNumber=regNum;
		slotNumber=slotNum;
		issueTime=time;
		}

    void displayTicketInfo() 
	 {
        cout << "Ticket ID: " << ticketID << endl;
        cout << "Car Registration Number: " << carRegistrationNumber << endl;
        cout << "Slot Number: " << slotNumber << endl;
        cout << "Issue Time: " << issueTime << endl;
    }

    void saveToFile(ofstream& outFile) 
	{
        outFile << ticketID << "," << carRegistrationNumber << "," << slotNumber << "," << issueTime << endl;
    }

    void loadFromFile(ifstream& inFile) 
	{
        getline(inFile, ticketID, ',');
        getline(inFile, carRegistrationNumber, ',');
        inFile >> slotNumber;
        inFile.ignore(); // ignore comma
        getline(inFile, issueTime);
    }

    string getCarRegistrationNumber() 
	 {
        return carRegistrationNumber;
    }
};

// ParkingLot class
class ParkingLot {
private:
    ParkingSlot* slots;
    int numSlots;
    Ticket tickets[100]; // Simplified to use an array
    int ticketCount;

public:
    ParkingLot(int numSlots) : numSlots(numSlots), ticketCount(0) {
        slots = new ParkingSlot[numSlots];
        for (int i = 0; i < numSlots; i++) {
            slots[i] = ParkingSlot(i + 1);
        }
    }

    ~ParkingLot() {
        delete[] slots;
    }

    bool isFull() const {
        for (int i = 0; i < numSlots; i++) {
            if (!slots[i].getIsOccupied()) {
                return false;
            }
        }
        return true;
    }

    bool parkCar( Car& car,  string& ticketID,  string& issueTime) {
        if (isFull()) {
            cout << "Parking lot is full!" << endl;
            return false;
        }
        for (int i = 0; i < numSlots; i++) {
            if (!slots[i].getIsOccupied()) {
                slots[i].occupy(car);
                tickets[ticketCount++] = Ticket(ticketID, car.getRegistrationNumber(), slots[i].getSlotNumber(), issueTime);
                cout << "Car parked at slot number: " << slots[i].getSlotNumber() << endl;
                return true;
            }
        }
        return false;
    }

    bool removeCar(const string& registrationNumber) {
        for (int i = 0; i < numSlots; i++) {
            if (slots[i].getIsOccupied() && slots[i].getParkedCar().getRegistrationNumber() == registrationNumber) {
                slots[i].vacate();
                for (int j = 0; j < ticketCount; j++) {
                    if (tickets[j].getCarRegistrationNumber() == registrationNumber) {
                        tickets[j] = tickets[--ticketCount]; // Remove the ticket
                        break;
                    }
                }
                cout << "Car removed from parking lot." << endl;
                return true;
            }
        }
        cout << "Car not found!" << endl;
        return false;
    }

    void displayAvailableSlots() const {
        cout << "Available slots: ";
        for (int i = 0; i < numSlots; i++) {
            if (!slots[i].getIsOccupied()) {
                cout << slots[i].getSlotNumber() << " ";
            }
        }
        cout << endl;
    }

    void displayParkedCars() const {
        cout << "Parked Cars:" << endl;
        for (int i = 0; i < numSlots; i++) {
            if (slots[i].getIsOccupied()) {
                slots[i].getParkedCar().displayCarInfo();
                cout << endl;
            }
        }
    }

    void displayTickets() 
	 {
        cout << "Issued Tickets:" << endl;
        for (int i = 0; i < ticketCount; i++) {
            tickets[i].displayTicketInfo();
            cout << endl;
        }
    }

    void saveToFile( string& filename) 
	 {
        ofstream outFile("filename3.txt");
        outFile << numSlots << endl;
        for (int i = 0; i < numSlots; i++) {
            slots[i].saveToFile(outFile);
        }
        for (int i = 0; i < ticketCount; i++) {
            tickets[i].saveToFile(outFile);
        }
        outFile.close();
    }

    void loadFromFile(const string& filename) {
        ifstream inFile("filename");
        if (!inFile) {
            cout << "File not found." << endl;
            return;
        }

        // Read and load slot data
        inFile >> numSlots;
        inFile.ignore(); // ignore newline
        delete[] slots;
        slots = new ParkingSlot[numSlots];
        for (int i = 0; i < numSlots; i++) {
            slots[i].loadFromFile(inFile);
        }

        // Skip the slot data
        string line;
        for (int i = 0; i <= numSlots; i++) {
            getline(inFile, line); // Skip lines containing slot data
        }

        // Read and load ticket data
        ticketCount = 0;
        while (getline(inFile, line)) {
            //stringstream ss(line);
            Ticket newTicket;
           // newTicket.loadFromFile(ss);
            tickets[ticketCount++] = newTicket;
        }

        inFile.close(); // Close the file
    }
};

// ParkingManager class
class ParkingManager {
private:
    ParkingLot parkingLot;

public:
    ParkingManager(int numSlots) : parkingLot(numSlots) {}

    void parkCar() {
        string regNum, owner, model, ticketID, issueTime;
        cout << "Enter registration number: ";
        cin >> regNum;
        cout << "Enter owner name: ";
        cin >> owner;
        cout << "Enter car model: ";
        cin >> model;
        cout << "Enter ticket ID: ";
        cin >> ticketID;
        cout << "Enter issue time: ";
        cin >> issueTime;
        Car car(regNum, owner, model);
        parkingLot.parkCar(car, ticketID, issueTime);
    }

    void removeCar() {
        string regNum;
        cout << "Enter registration number: ";
        cin >> regNum;
        parkingLot.removeCar(regNum);
    }

    void displayAvailableSlots()  {
        parkingLot.displayAvailableSlots();
    }

    void displayParkedCars()  {
        parkingLot.displayParkedCars();
    }

    void displayTickets()  {
        parkingLot.displayTickets();
    }

    void saveData() 
	 {
        string filename;
        cout << "Enter filename to save data: ";
        cin >> filename;
        parkingLot.saveToFile(filename);
    }

    void loadData() {
        string filename;
        cout << "Enter filename to load data: ";
        cin >> filename;
        parkingLot.loadFromFile(filename);
    }

    void adminMenu() {
        int choice;
        do {
            cout << "Admin Menu:" << endl;
            cout << "1. Park Car" << endl;
            cout << "2. Remove Car" << endl;
            cout << "3. Display Available Slots" << endl;
            cout << "4. Display Parked Cars" << endl;
            cout << "5. Display Tickets" << endl;
            cout << "6. Save Data" << endl;
            cout << "7. Load Data" << endl;
            cout << "8. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    parkCar();
                    break;
                case 2:
                    removeCar();
                    break;
                case 3:
                    displayAvailableSlots();
                    break;
                case 4:
                    displayParkedCars();
                    break;
                case 5:
                    displayTickets();
                    break;
                case 6:
                    saveData();
                    break;
                case 7:
                    loadData();
                    break;
                case 8:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 8);
    }
};

bool adminLogin() {
    string username;
    string password;
    cout << "\t\t\t\tEnter admin username: ";
    cin >> username;
    cout << "\t\t\t\tEnter admin password: ";
    password.clear();
    char ch;
    while ((ch = getch()) != '\r') {
        if (ch == 8) { // Handle backspace
            if (!password.empty()) {
                cout << "\b \b";
                password.push_back(ch);
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;

    if (username == "Qureshi" && password == "aumc786") {
        cout << "\t\t\t\t--------------------------------------\n";
        cout << "\t\t\t\tLogin successful! Welcome Admin!\n";
        cout << "\t\t\t\t--------------------------------------\n";
        return true;
    } else {
        cout << "\t\t\t\t--------------------------------------\n";
        cout << "\t\t\t\tInvalid username or password! Please try again.\n";
        cout << "\t\t\t\t--------------------------------------\n";
        return false;
    }
}

int main() {
    if (!adminLogin()) 
	{
        return 0;
    }

    int numSlots;
    cout << "Enter number of parking slots: ";
    cin >> numSlots;

    ParkingManager manager(numSlots);
    manager.adminMenu();

    return 0;
}
