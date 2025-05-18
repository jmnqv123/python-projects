#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Student {
public:
    int id;
    string name;
    float gpa;

    Student(int i, string n, float g) : id(i), name(n), gpa(g) {}

    string to_string() const {  // const because it doesn't modify the object
        return std::to_string(id) + "," + name + "," + std::to_string(gpa);
    }

    static Student from_string(const string &line) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        int id = stoi(line.substr(0, pos1));
        string name = line.substr(pos1 + 1, pos2 - pos1 - 1);
        float gpa = stof(line.substr(pos2 + 1));
        return Student(id, name, gpa);
    }

    void display() {
        cout << "ID: " << id << ", Name: " << name << ", GPA: " << gpa << endl;
    }
};

class DBMS {
private:
    const string filename = "students.txt";

public:
    void insert_student(const Student &s) {
        ofstream fout(filename, ios::app);
        fout << s.to_string() << endl;
        fout.close();
        cout << "✅ Student inserted!" << endl;
    }

    void display_all_students() {
        ifstream fin(filename);
        string line;
        cout << "📄 Student Records:\n";
        while (getline(fin, line)) {
            Student s = Student::from_string(line);
            s.display();
        }
        fin.close();
    }
};

int main() {
    DBMS db;
    int choice;

    while (true) {
        cout << "\n📚 Student DBMS\n";
        cout << "1. Insert Student\n";
        cout << "2. Display All\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id;
            string name;
            float gpa;
            cout << "Enter ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter GPA: ";
            cin >> gpa;
            db.insert_student(Student(id, name, gpa));
        } else if (choice == 2) {
            db.display_all_students();
        } else {
            break;
        }
    }

    return 0;
}
