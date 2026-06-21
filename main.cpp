#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

class Person {
protected:
    string name;
    int age;
    string id;

public:
    Person(string n, int a, string i)
        : name(n), age(a), id(i) {}

    Person(const Person& p)
        : name(p.name), age(p.age), id(p.id) {}

    string getName() const { return name; }
    int getAge() const { return age; }
    string getId() const { return id; }

    void setName(const string& n) { name = n; }
    void setAge(int a) { age = a; }

    virtual void display() const {
        cout << "Name: " << name
             << " | Age: " << age
             << " | ID: " << id;
    }

    virtual ~Person() {}
};

class Student : public Person {
private:
    string major;
    double gpa;

public:
    Student(string n, int a, string i, string m, double g)
        : Person(n, a, i), major(m), gpa(g) {}

    Student(const Student& s)
        : Person(s), major(s.major), gpa(s.gpa) {}

    string getMajor() const { return major; }
    double getGpa() const { return gpa; }

    void display() const override {
        Person::display();
        cout << " | Major: " << major
             << " | GPA: " << gpa << endl;
    }

    ~Student() {}
};

// FILE HANDLING FUNCTION

void saveToFile(Student* students[], int totalStudents) {
    ofstream outFile("students.txt");
    
    if (!outFile) {
        cout << "  [!] Error: Could not open file to save data." << endl;
        return;
    }

    for (int i = 0; i < totalStudents; i++) {
        outFile << students[i]->getName() << endl;
        outFile << students[i]->getAge() << endl;
        outFile << students[i]->getId() << endl;
        outFile << students[i]->getMajor() << endl;
        outFile << students[i]->getGpa() << endl;
    }

    outFile.close();
}

void loadFromFile(Student* students[], int& totalStudents) {
    ifstream inFile("students.txt");
    
    if (!inFile) {
        return; 
    }

    string name, id, major;
    int age;
    double gpa;

    while (getline(inFile, name)) {
        if (name.empty()) continue;

        inFile >> age;
        inFile.ignore(10000, '\n'); 

        getline(inFile, id);
        getline(inFile, major);

        inFile >> gpa;
        inFile.ignore(10000, '\n');

        if (totalStudents < 100) {
            students[totalStudents] = new Student(name, age, id, major, gpa);
            totalStudents++;
        }
    }

    inFile.close();
    
    if (totalStudents > 0) {
        cout << "  [*] Automatically loaded " << totalStudents << " students from database.\n" << endl;
    }
}

// VALIDATION & UTILITY FUNCTIONS

int getValidatedAge() {
    int age;
    while (true) {
        cout << "  Enter Age       : ";
        cin >> age;
        if (cin.fail() || age <= 0 || age > 150) {
            cin.clear();
            cin.ignore(10000, '\n'); 
            cout << "  [!] Invalid age. Try again." << endl;
        } else {
            cin.ignore(10000, '\n');
            return age;
        }
    }
}

double getValidatedGpa() {
    double gpa;
    while (true) {
        cout << "  Enter GPA (0-4) : ";
        cin >> gpa; 
        if (cin.fail() || gpa < 0.0 || gpa > 4.0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  [!] Invalid GPA. Enter a value between 0.0 and 4.0." << endl;
        } else {
            cin.ignore(10000, '\n');
            return gpa;
        }
    }
}

void displayAll(Student* students[], int totalStudents) {
    if (totalStudents == 0) {
        cout << "\n  [!] No students recorded yet.\n" << endl;
        return;
    }

    cout << "\n===============================" << endl;
    cout << "     ALL STUDENT RECORDS      " << endl;
    cout << "===============================" << endl;

    for (int i = 0; i < totalStudents; i++) {
        cout << "  [" << i + 1 << "] ";
        students[i]->display();
    }

    cout << "===============================" << endl;
}

void addStudent(Student* students[], int& totalStudents) {
    if (totalStudents >= 100) {
        cout << "\n  [!] Storage full! Cannot add more students.\n" << endl;
        return;
    }

    string name, id, major;

    cout << "\n--- Enter Student Details ---" << endl;

    cout << "  Enter Name      : ";
    getline(cin, name);

    int age = getValidatedAge();

    cout << "  Enter ID        : ";
    getline(cin, id);

    cout << "  Enter Major     : ";
    getline(cin, major);

    double gpa = getValidatedGpa();

    students[totalStudents] = new Student(name, age, id, major, gpa);
    totalStudents++;

    saveToFile(students, totalStudents);

    cout << "  [?] Student added and saved to database successfully!\n" << endl;
}

void freeMemory(Student* students[], int& totalStudents) {
    for (int i = 0; i < totalStudents; i++) {
        delete students[i];
    }
    totalStudents = 0;
}

int main() {
    Student* students[100]; 
    int totalStudents = 0;
    int choice;

    cout << "===============================" << endl;
    cout << "   STUDENT MANAGEMENT SYSTEM  " << endl;
    cout << "===============================" << endl;

    loadFromFile(students, totalStudents);

    while (true) {
        cout << "\n  [1] Add Student" << endl;
        cout << "  [2] Display All" << endl;
        cout << "  [3] Exit" << endl;
        cout << "  Enter choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n  [!] Invalid input. Enter 1, 2, or 3." << endl;
            continue;
        }

        cin.ignore(10000, '\n');

        switch (choice) {
            case 1:
                addStudent(students, totalStudents);
                break;

            case 2:
                displayAll(students, totalStudents);
                break;

            case 3:
                saveToFile(students, totalStudents);
                freeMemory(students, totalStudents);
                cout << "\n Goodbye!\n" << endl;
                return 0;

            default:
                cout << "\n  [!] Invalid choice. Enter 1, 2, or 3." << endl;
        }
    }
}