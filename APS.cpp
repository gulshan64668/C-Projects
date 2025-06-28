#include <iostream>     // for input/output operations
#include <ctime>        // for handling time-related functions
#include <conio.h>      // for getch() used in password masking

using namespace std;

const int MAX_SLOTS = 50;  // maximum number of parking slots

// --- FSM States (Finite State Machine for main system) ---
enum State {
    START,
    LOGIN,
    MENU,
    DASHBOARD,
    VEHICLE_ENTRY,
    VEHICLE_EXIT,
    PARKING_STATUS,
    SEARCH_VEHICLE,
    SHOW_REVENUE,
    EXIT_SYSTEM
};

// --- Structure to store each vehicle's data ---
struct Vehicle {
    string numberPlate;       // vehicle's number plate
    string vehicleType;       // vehicle type (car, bike, etc.)
    time_t entryTime;         // time when vehicle entered
    bool occupied = false;    // whether slot is occupied
};

Vehicle parkingSlots[MAX_SLOTS];  // array to store all parking slots

// --- List of valid vehicle types (used for validation with DFA) ---
string validTypes[10] = {
    "car", "bike", "bus", "truck", "van",
    "scooter", "jeep", "rickshaw", "tractor", "auto"
};

double totalRevenue = 0.0;  // stores total amount collected

// --- Function to get password with masking and toggle visibility ---
string getPassword() {
    string password;
    char ch;
    bool showPassword = false; // initial state: password is hidden

    while (true) {
        ch = _getch();  // read character without showing on screen

        if (ch == '\r') {  // Enter key pressed ? accept input
            cout << endl;
            break;
        } else if (ch == '\b') {  // Backspace ? remove last character
            if (!password.empty()) {
                password.pop_back();
                cout << "\b \b";  // erase from console
            }
        } else if (ch == 19) {  // Ctrl+S ? toggle show/hide
            showPassword = !showPassword;
            cout << "\rEnter Password: ";
            if (showPassword) {
                cout << password;
            } else {
                cout << string(password.length(), '*');
            }
        } else {  // Normal character input
            password += ch;
            cout << (showPassword ? ch : '*');
        }
    }
    return password;
}

// --- Login function: accept any username, validate password ---
bool login() {
    string username, password;
    string correctPassword = "1234";  // only password is fixed

    cout << "\n================ LOGIN ================\n";
    cout << "Enter Username: ";
    getline(cin, username);  // Accept any username 
    cout << "Enter Password: ";
    password = getPassword();
    cout << "Note: Press Ctrl+S to show/hide password\n";

    if (password != correctPassword) {
        cout << "\nInvalid password!\n";
        return false;  // login failed
    }

    cout << "\nLogin successful! Welcome, " << username << "!\n";
    return true;  // login success
}

// --- Checks if entered vehicle type is valid or not ---
bool isValidVehicleType(string type) {
    for (int i = 0; i < 10; i++) {
        if (type == validTypes[i])
            return true;  // match found
    }
    return false;  // invalid vehicle type
}

// --- Handles vehicle entry: search free slot, validate type, park it ---
void vehicleEntry() {
    int slot = -1;

    // Find first free slot
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (!parkingSlots[i].occupied) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        cout << "\nNo available slots!\n";
        return;
    }

    Vehicle v;
    cout << "\n========== VEHICLE ENTRY ==========\n";
    cout << "Enter vehicle number plate: ";
    cin >> v.numberPlate;
    cout << "Enter vehicle type (car, bike, etc.): ";
    cin >> v.vehicleType;

    // Check if vehicle type is valid
    if (!isValidVehicleType(v.vehicleType)) {
        cout << "\nInvalid vehicle type!\n";
        return;
    }

    v.entryTime = time(0);    // store current time as entry time
    v.occupied = true;
    parkingSlots[slot] = v;   // assign vehicle to found slot

    // Display ticket
    cout << "\nVehicle parked at slot: " << slot + 1 << "\n";
    cout << "Ticket:\n";
    cout << "Number Plate: " << v.numberPlate << "\n";
    cout << "Time In: " << ctime(&v.entryTime);  // show entry time
}

// --- Handles vehicle exit: verify plate and slot, calculate fee ---
void vehicleExit() {
    string numberPlate;
    int slot;
    cout << "\n========== VEHICLE EXIT ==========\n";
    cout << "Enter slot number: ";
    cin >> slot;
    slot--;  // to match array index (0-based)

    // Check if slot is valid and occupied
    if (slot < 0 || slot >= MAX_SLOTS || !parkingSlots[slot].occupied) {
        cout << "\nInvalid slot or slot is empty!\n";
        return;
    }

    cout << "Enter number plate: ";
    cin >> numberPlate;

    // Match number plate with record
    if (parkingSlots[slot].numberPlate != numberPlate) {
        cout << "\nNumber plate doesn't match the record!\n";
        return;
    }

    time_t exitTime = time(0);  // get current time
    double duration = difftime(exitTime, parkingSlots[slot].entryTime) / 60;
    double amount = duration * 2;  // Rs. 2 per minute
    totalRevenue += amount;

    cout << "\nTime stayed: " << duration << " minutes\n";
    cout << "Amount to pay: Rs. " << amount << endl;

    parkingSlots[slot] = Vehicle();  // empty the slot
    cout << "Slot " << slot + 1 << " is now free.\n";
}

// --- Shows total and available slots ---
void dashboard() {
    int occupied = 0;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (parkingSlots[i].occupied) occupied++;
    }
    cout << "\n========== DASHBOARD ==========\n";
    cout << "Total Slots:     " << MAX_SLOTS << endl;
    cout << "Available Slots: " << (MAX_SLOTS - occupied) << "\n";
}

// --- Displays status of all slots (occupied or free) ---
void parkingStatus() {
    cout << "\n========== PARKING SLOT STATUS ==========\n";
    for (int i = 0; i < MAX_SLOTS; i++) {
        cout << "Slot " << i + 1 << ": ";
        if (parkingSlots[i].occupied) {
            cout << "Occupied by " << parkingSlots[i].numberPlate << " (" << parkingSlots[i].vehicleType << ")\n";
        } else {
            cout << "Available\n";
        }
    }
}

// --- Search for a vehicle by its number plate ---
void searchVehicle() {
    string plate;
    cout << "\n========== SEARCH VEHICLE ==========\n";
    cout << "Enter number plate to search: ";
    cin >> plate;

    bool found = false;
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (parkingSlots[i].occupied && parkingSlots[i].numberPlate == plate) {
            cout << "\nVehicle found at slot " << i + 1 << endl;
            cout << "Type: " << parkingSlots[i].vehicleType << endl;
            cout << "Entry Time: " << ctime(&parkingSlots[i].entryTime);
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nVehicle not found!\n";
    }
}

// --- Shows total revenue collected so far ---
void showRevenue() {
    cout << "\n========== TOTAL REVENUE ==========\n";
    cout << "Rs. " << totalRevenue << " collected so far.\n";
}

// --- Wrapper for login (for clarity) ---
bool authentication() {
    return login();
}

// --- Main Controller FSM ---
int main() {
    State currentState = START;  // initial state

    while (currentState != EXIT_SYSTEM) {
        switch (currentState) {
            case START:
                currentState = LOGIN;
                break;

            case LOGIN:
                if (authentication()) {
                    currentState = MENU;
                } else {
                    currentState = LOGIN;
                }
                break;

            case MENU: {
                int choice;
                cout << "\n========== PARKING SYSTEM MENU ==========\n";
                cout << "1. Dashboard\n";
                cout << "2. Vehicle Entry\n";
                cout << "3. Vehicle Exit\n";
                cout << "4. Parking Status\n";
                cout << "5. Search Vehicle by Number Plate\n";
                cout << "6. Show Total Revenue\n";
                cout << "7. Exit\n";
                cout << "=========================================\n";
                cout << "Choose option (1-7): ";
                cin >> choice;
                cin.ignore();  // clear newline character

                switch (choice) {
                    case 1: currentState = DASHBOARD; break;
                    case 2: currentState = VEHICLE_ENTRY; break;
                    case 3: currentState = VEHICLE_EXIT; break;
                    case 4: currentState = PARKING_STATUS; break;
                    case 5: currentState = SEARCH_VEHICLE; break;
                    case 6: currentState = SHOW_REVENUE; break;
                    case 7: currentState = EXIT_SYSTEM; break;
                    default:
                        cout << "\nInvalid option! Please choose between 1 to 7.\n";
                        currentState = MENU;
                        break;
                }
                break;
            }

            case DASHBOARD:
                dashboard();
                currentState = MENU;
                break;

            case VEHICLE_ENTRY:
                vehicleEntry();
                currentState = MENU;
                break;

            case VEHICLE_EXIT:
                vehicleExit();
                currentState = MENU;
                break;

            case PARKING_STATUS:
                parkingStatus();
                currentState = MENU;
                break;

            case SEARCH_VEHICLE:
                searchVehicle();
                currentState = MENU;
                break;

            case SHOW_REVENUE:
                showRevenue();
                currentState = MENU;
                break;

            case EXIT_SYSTEM:
                break;
        }
    }

    cout << "\nTHANK YOU FOR USING THE PARKING SYSTEM!\n";
    return 0;
}

