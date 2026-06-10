#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
using namespace std;

// Node structure for the linked list (stack implementation)
template <typename T>
struct Node {
    T data;
    Node* next;
    Node(T data) : data(data), next(nullptr) {}
};

// Stack implemented using linked list
template <typename T>
class Stack {
private:
    Node<T>* top;

public:
    Stack() : top(nullptr) {}

    void push(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = top;
        top = newNode;
    }

    void pop() {
        if (top == nullptr) {
            cout << "Stack is empty.\n";
            return;
        }
        Node<T>* temp = top;
        top = top->next;
        delete temp;
    }

    T peek() const {
        if (top == nullptr) {
            throw runtime_error("Stack is empty.");
        }
        return top->data;
    }

    bool isEmpty() const {
        return top == nullptr;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    vector<T> toVector() const {
        vector<T> tempList;
        Node<T>* current = top;
        while (current) {
            tempList.push_back(current->data);
            current = current->next;
        }
        return tempList;
    }

    void traverse(void (*func)(const T&)) const {
        Node<T>* current = top;
        while (current) {
            func(current->data);
            current = current->next;
        }
    }

    ~Stack() {
        clear();
    }
};

// Base class: Person
class Person {
protected:
    string name;

public:
    Person() {}
    Person(string name) : name(name) {}
    string getName() const { return name; }
    void setName(const string& name) { this->name = name; }

    virtual void display() const = 0;
};

// Doctor class
class Doctor : public Person {
private:
    string specialization;
    string availability;

public:
    Doctor() {}
    Doctor(string name, string specialization, string availability)
        : Person(name), specialization(specialization), availability(availability) {}

    void display() const override {
        cout << "Doctor: " << name << ", Specialization: " << specialization
             << ", Availability: " << availability << endl;
    }
};

// Patient class
class Patient : public Person {
private:
    int age;
    string healthRecord;

public:
    Patient() {}
    Patient(string name, int age, string healthRecord)
        : Person(name), age(age), healthRecord(healthRecord) {}

    void display() const override {
        cout << "Patient: " << name << ", Age: " << age
             << ", Health Record: " << healthRecord << endl;
    }
};

// Appointment class
class Appointment {
private:
    string patientName;
    string doctorName;
    string date;
    string day;
    string time;
    string reason;

public:
    Appointment() {}
    Appointment(string patientName, string doctorName, string date, string day, string time, string reason)
        : patientName(patientName), doctorName(doctorName), date(date), day(day), time(time), reason(reason) {}

    void display() const {
        cout << "Appointment - Patient: " << patientName
             << ", Doctor: " << doctorName
             << ", Date: " << date
             << ", Day: " << day
             << ", Time: " << time
             << ", Reason: " << reason << endl;
    }

    string getPatientName() const { return patientName; }
};

// Sorting Function (Only for Viewing, Not Removing)
template <typename T>
void displaySortedStack(Stack<T>& stack) {
    vector<T> tempList = stack.toVector();

    sort(tempList.begin(), tempList.end(), [](const T& a, const T& b) {
        return a.getName() < b.getName();
    });

    for (const auto& item : tempList) {
        item.display();
    }
}

// Appointment Booking
void bookAppointment(Stack<Appointment>& appointmentStack) {
    string patientName, doctorName, date, day, time, reason;
    cout << "Enter Patient Name: ";
    getline(cin, patientName);
    cout << "Enter Doctor Name: ";
    getline(cin, doctorName);
    cout << "Enter Appointment Date (DD/MM/YYYY): ";
    getline(cin, date);
    cout << "Enter Day: ";
    getline(cin, day);
    cout << "Enter Time (hh:mm AM/PM): ";
    getline(cin, time);
    cout << "Enter Reason for Checkup: ";
    getline(cin, reason);

    Appointment newAppointment(patientName, doctorName, date, day, time, reason);
    appointmentStack.push(newAppointment);
    cout << "Appointment successfully booked!\n";
}

void viewAppointments(Stack<Appointment>& appointmentStack) {
    if (appointmentStack.isEmpty()) {
        cout << "No appointments available.\n";
        return;
    }

    cout << "\n--- Appointments ---\n";
    appointmentStack.traverse([](const Appointment& appointment) { appointment.display(); });
}

// Hospital Department Location (Dijkstra's Algorithm)
void hospitalDepartmentLocation() {
    // List of departments
    vector<string> departments = {"Outpatient Department (OPD)", "Cardiology", "Dentistry", "Internal Medicine", "Gynecology"};

    // Create a simple adjacency matrix (distances between departments)
    vector<vector<int>> graph = {
        {0, 2, 0, 0, 1},  // OPD
        {2, 0, 3, 0, 0},  // Cardiology
        {0, 3, 0, 1, 0},  // Dentistry
        {0, 0, 1, 0, 4},  // Internal Medicine
        {1, 0, 0, 4, 0}   // Gynecology
    };

    int numVertices = departments.size();
    int source;
    cout << "Enter the department number to start from (0 - OPD, 1 - Cardiology, 2 - Dentistry, 3 - Internal Medicine, 4 - Gynecology): ";
    cin >> source;

    // Dijkstra's algorithm implementation
    vector<int> dist(numVertices, numeric_limits<int>::max());
    dist[source] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (int v = 0; v < numVertices; ++v) {
            if (graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\nShortest paths from department " << departments[source] << ":\n";
    for (int i = 0; i < numVertices; ++i) {
        cout << "To " << departments[i] << ": " << dist[i] << " minutes\n";
    }
}

// Doctor Information Menu
void doctorMenu(Stack<Doctor>& doctorStack, Stack<Appointment>& appointmentStack) {
    int choice;
    do {
        cout << "\n--- Doctor Information Menu ---\n";
        cout << "1. Add Doctor\n";
        cout << "2. Remove Doctor\n";
        cout << "3. View Doctors\n";
        cout << "4. View Booked Appointments\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string name, specialization, availability;
                cout << "Enter doctor name: ";
                getline(cin, name);
                cout << "Enter specialization: ";
                getline(cin, specialization);
                cout << "Enter availability: ";
                getline(cin, availability);
                doctorStack.push(Doctor(name, specialization, availability));
                break;
            }
            case 2:
                doctorStack.pop();
                cout << "Last added doctor removed.\n";
                break;
            case 3:
                cout << "\n--- Doctors (Sorted by Name) ---\n";
                displaySortedStack(doctorStack);
                break;
            case 4:
                viewAppointments(appointmentStack);
                break;
            case 5:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

// Patient Information Menu
void patientMenu(Stack<Patient>& patientStack, Stack<Appointment>& appointmentStack) {
    int choice;
    do {
        cout << "\n--- Patient Information Menu ---\n";
        cout << "1. Add Patient\n";
        cout << "2. Remove Patient\n";
        cout << "3. View Patients\n";
        cout << "4. Book Appointment\n";
        cout << "5. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                string name, healthRecord;
                int age;
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter patient age: ";
                cin >> age;
                cin.ignore();
                cout << "Enter patient health record: ";
                getline(cin, healthRecord);
                patientStack.push(Patient(name, age, healthRecord));
                break;
            }
            case 2:
                patientStack.pop();
                cout << "Last added patient removed.\n";
                break;
            case 3:
                cout << "\n--- Patients (Sorted by Name) ---\n";
                displaySortedStack(patientStack);
                break;
            case 4:
                bookAppointment(appointmentStack);
                break;
            case 5:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

// Main Menu
int main() {
    Stack<Doctor> doctorStack;
    Stack<Patient> patientStack;
    Stack<Appointment> appointmentStack;


	cout << endl << endl << "\t\t\t\t\t-------------------------------------" << endl;
    cout << "\t\t\t\t\t $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cout << "\t\t\t\t --------------------------------------------------" << endl;
    cout << "\b\t\t\t\t\tHOSPITAL MANAGEMENT SYSTEM (DSA- Project)\b" << endl;
    cout << "\t\t\t\t --------------------------------------------------" << endl;
    cout<<"\t\t\t\t\tDate: "<<__DATE__<<"\t"<<"Time: "<<__TIME__<<endl;
	cout << "\t\t\t\t---------------------------------------------------" << endl;
	cout << "\t\t\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl <<endl <<endl;
	
    int choice;
    do {
    	cout << "\t\t\t\t---------------------------------------------------" << endl;
        cout << "\t\t\t\t\t\t---Main Menu---\n";
        cout << "\t\t\t\t---------------------------------------------------" << endl;
        cout << "\t\t\t\t\t1. Doctor Information\n";
        cout << "\t\t\t\t\t2. Patient Information\n";
        cout << "\t\t\t\t\t3. Hospital Department Location\n";
        cout << "\t\t\t\t\t4. Exit\n";
        cout << "\t\t\t\t\tEnter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                doctorMenu(doctorStack, appointmentStack);
                break;
            case 2:
                patientMenu(patientStack, appointmentStack);
                break;
            case 3:
                hospitalDepartmentLocation();
                break;
            case 4:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
