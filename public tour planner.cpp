#include <iostream>
#include <limits>  // Add this at the top

using namespace std;

// Node structure for Linked List (Tours)
struct Node {
    string name;//price
    int price;
    string details;//e.g Beautiful hill station
    Node* next;
    
     Node(string n, int p, string d) {
        name = n;
        price = p;
        details = d;
        next = nullptr;
    }
};

// Custom Stack implementation for feedback
class Stack {
private:
    string stackArray[100];  // Array to store stack elements
    int top;  // Index of the top element in the stack

public:
    Stack() : top(-1) {}  // Stack constructor initializing top to -1 (empty stack)

    void push(string val) {
        if (top < 99) {  // Stack overflow check
            stackArray[++top] = val;  // Increment top and push the element
        } else {
            cout<<"Stack overflow";
        }
    }

    string pop() {
        if (top == -1) {  // Stack underflow check
            cout<<"No feedback available";
        }
        return stackArray[top--];  // Return top element and decrement top
    }

    bool isEmpty() {
        return top == -1;  // Stack is empty if top is -1
    }

    string peek() {
        if (top == -1) {  // Check if the stack is empty
            cout<<"Stack is empty";
        }
        return stackArray[top];  // Return the top element without popping
    }
};

// Custom Queue implementation fro tour booking
class Queue {
private:
    struct Node {
        string data;
        Node* next;
        Node(string val) : data(val), next(nullptr) {}
    };
    Node* front;
    Node* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(string val) {
        Node* newNode = new Node(val);
        if (rear) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
    }

    string dequeue() {
        if (front == nullptr) {
            throw runtime_error("Queue is empty");
        }
        Node* temp = front;
        string dequeuedData = front->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return dequeuedData;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    string peek() {
        if (front == nullptr) {
            throw runtime_error("Queue is empty");
        }
        return front->data;
    }
};

// HashMap for Graph adjacency list
const int TABLE_SIZE = 10;

struct HashMapNode {
    string key;
    string value;
    HashMapNode* next;

    HashMapNode(string k, string v) : key(k), value(v), next(nullptr) {}
};

class HashMap {
private:
    HashMapNode* table[TABLE_SIZE];

    int hashFunction(const string& key) {
        int hashValue = 0;
        for (char ch : key) {
            hashValue = (hashValue + ch) % TABLE_SIZE;
        }
        return hashValue;
    }

public:
    HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void put(const string& key, const string& value) {
        int index = hashFunction(key);
        HashMapNode* newNode = new HashMapNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    string get(const string& key) {
        int index = hashFunction(key);
        HashMapNode* current = table[index];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        return "";
    }

    bool contains(const string& key) {
        int index = hashFunction(key);
        HashMapNode* current = table[index];
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Add a public method to allow access to the adjacency list (table)
    HashMapNode** getTable() {
        return table;
    }
};

// Graph implementation using HashMap
class Graph {
private:
    HashMap adjList;

public:
    void addEdge(const string& u, const string& v) {
        adjList.put(u, v);
        adjList.put(v, u); //  undirected graph
    }

    void displayGraph() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            for (HashMapNode* node = adjList.getTable()[i]; node != nullptr; node = node->next) {
                cout << node->key << " -> " << node->value << endl;
            }
        }
    }
};

// Linked List for Available Tours
class Tour {
private:
    Node* head;

public:
    Tour() : head(nullptr) {}

    void add(string name, int price, string details) {
        Node* newNode = new Node(name, price, details);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() {
        Node* temp = head;
        cout << "Available Tours:" << endl;
        while (temp) {
            cout << "Name: " << temp->name << ", Price: " << temp->price
                 << ", Details: " << temp->details << endl;
            temp = temp->next;
        }
    }

    Node* find(string name) {
        Node* temp = head;
        while (temp) {
            if (temp->name == name) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
};

// TourGuider class for distances between destinations
class TourGuider {
private:
    string distances[5][2] = {
        {"Swat", "Islamabad: 250 km"},
        {"Islamabad", "Murree: 70 km"},
        {"Murree", "Naran: 300 km"},
        {"Naran", "Kalam: 250 km"}
    };

public:
    void showAllDistances() {
        cout << "--- Tour Distances ---\n";
        for (int i = 0; i < 4; i++) {
            cout << distances[i][0] << " " << distances[i][1] << endl;
        }
    }
};

// TransportVendor class for available vehicles
class TransportVendor {
private:
    struct VehicleNode {
        string vehicleName;
        string vehicleType;
        VehicleNode* left;
        VehicleNode* right;

        VehicleNode(string name, string type) : vehicleName(name), vehicleType(type), left(nullptr), right(nullptr) {}
    };

    VehicleNode* root;

    void insert(VehicleNode*& node, const string& vehicleName, const string& vehicleType) {
        if (node == nullptr) {
            node = new VehicleNode(vehicleName, vehicleType);
        } else if (vehicleName < node->vehicleName) {
            insert(node->left, vehicleName, vehicleType);
        } else {
            insert(node->right, vehicleName, vehicleType);
        }
    }

    void display(VehicleNode* node) {
        if (node != nullptr) {
            display(node->left);
            cout << "Vehicle Name: " << node->vehicleName 
                 << ", Type: " << node->vehicleType << endl;
            display(node->right);
        }
    }

public:
    TransportVendor() : root(nullptr) {}

    void addVehicle(const string& vehicleName, const string& vehicleType) {
        insert(root, vehicleName, vehicleType);
    }

    void showAvailableVehicles() {
        if (root == nullptr) {
            cout << "No vehicles available." << endl;
            return;
        }
        cout << "Available Vehicles:\n";
        display(root);
    }
};
bool login() {
    string userName, password;
    cout << "Enter your name: ";
    cin >> userName;

    for (char ch : userName) {
        if (isdigit(ch)) {
            cout << "Invalid username! how can be your name is in digits" << endl;
            return false;
        }
    }

    cout << "Enter password: ";
    cin >> password;

    return true;  // Any password is accepted
}
// Booking class 
class Booking {
private:
    Queue bookedTours;

public:
    void bookTour(Node* tour) {
        bookedTours.enqueue(tour->name);
    }

    void cancelTour(string tourName) {
        Queue tempQueue;
        bool canceled = false;

        while (!bookedTours.isEmpty()) {
            string tour = bookedTours.dequeue();
            if (tour == tourName) {
                canceled = true;
                cout << "Tour " << tourName << " has been canceled." << endl;
            } else {
                tempQueue.enqueue(tour);
            }
        }

        bookedTours = tempQueue;

        if (!canceled) {
            cout << "Tour not found in your bookings." << endl;
        }
    }

    void viewBookedTours() {
        if (bookedTours.isEmpty()) {
            cout << "No tours booked yet." << endl;
            return;
        }
        Queue tempQueue = bookedTours;
        cout << "Booked Tours:" << endl;
        while (!tempQueue.isEmpty()) {
            cout << tempQueue.dequeue() << endl;
        }
    }
};

// Tourist class for user interaction
class Tourist {
private:
    string userName;
    Tour availableTours;
    Stack feedbacks;

    void initializeTours() {
        availableTours.add("Murree", 5000, "Beautiful hill station.");
        availableTours.add("Swat", 8000, "Known as the Switzerland of Pakistan.");
        availableTours.add("Kalam", 10000, "Serene valley with rivers.");
        availableTours.add("Naran", 12000, "Home to Saif-ul-Malook Lake.");
        availableTours.add("Islamabad", 3000, "Capital city with modern facilities.");
    }

public:
    Tourist(string name) : userName(name) {
        initializeTours();
    }

    void viewTours() {
        availableTours.display();
    }

    void bookTour(string tourName, Booking& booking) {
        Node* tour = availableTours.find(tourName);
        if (tour) {
            booking.bookTour(tour);
            cout << userName << " booked the tour: " << tourName << "." << endl;
        } else {
            cout << "Tour not found." << endl;
        }
    }

    void cancelTour(string tourName, Booking& booking) {
        booking.cancelTour(tourName);
    }

    void viewBookedTours(Booking& booking) {
        booking.viewBookedTours();
    }

    void provideFeedback(string feedback) {
        feedbacks.push(feedback);
        cout << "Thank you for your feedback!" << endl;
    }

    void viewFeedbacks() {
        if (feedbacks.isEmpty()) {
            cout << "No feedbacks yet." << endl;
        } else {
            cout << "Feedbacks provided:" << endl;
            while (!feedbacks.isEmpty()) {
                cout << feedbacks.pop() << endl;
            }
        }
    }
};

// Main function
int main() {
    string userName;
    //
if (!login()) return 0;
    cout << "Login successful!" << endl;
    

    Tourist planner(userName);
 cout << "\n---🛤🛣🏞🚗🚋 Welcome to 🛤🛣🏞🚗PTP ---\n";
    // classes
    Booking booking;
    Graph graph;
    TourGuider guider;
    TransportVendor transport;

    // Adding some sample vehicles
    transport.addVehicle("Boeing 747", "Aeroplane");
    transport.addVehicle ("Volvo", "Bus");
    transport.addVehicle("Toyota", "Car");
    transport.addVehicle("Honda", "Car");
    transport.addVehicle("Suzuki", "Bike");
    
 
    // Menu loop for user
    int choice;
    do {
        
       
        cout << "\n--- 🧚Tourist Dashboard 🧚---\n";
    
        cout << "1. View Available Tours🏞️\n";
        cout << "2. Book a Tour🔖\n";
        cout << "3. Cancel a Tour😒\n";
        cout << "4. View Booked Tours️👀\n";
        cout << "5. View Tour Distances🚗\n";
        cout << "6. Show Available Vehicles🚗🚲🚋🚌✈️\n";
        cout << "7. Provide Feedback📝💡\n";
        cout << "8. View Feedbacks👀️️\n";
        cout << "9. Exit😊\n";
        cout << "Enter your choice: ";
        cin >> choice;
        
        while(cin.fail() || choice < 1 || choice > 9) {
    cout << "Invalid choice. Please enter a number between 1 and 9: ";
    cin.clear(); // clear the error flag
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore incorrect input
    cin >> choice;
}  

        switch (choice) {
            case 1:
                planner.viewTours();
                break;
            case 2: {
                string tourName;
                cout << "Enter the tour name to book: ";
                cin >> tourName;
                planner.bookTour(tourName, booking);
                break;
            }
            case 3: {
                string tourName;
                cout << "Enter the tour name to cancel: ";
                cin >> tourName;
                planner.cancelTour(tourName, booking);
                break;
            }
            case 4:
                planner.viewBookedTours(booking);
                break;
            case 5:
                guider.showAllDistances();
                break;
            case 6:
                transport.showAvailableVehicles();
                break;
            case 7: {
                string feedback;
                cout << "Enter your feedback: ";
                cin.ignore();  // To ignore any leftover newline character in input buffer
                getline(cin, feedback);
                planner.provideFeedback(feedback);
                break;
            }
            case 8:
                planner.viewFeedbacks();
                break;
            case 9:
                cout << "Goodbye, " << userName << "!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 9);

    return 0;
}