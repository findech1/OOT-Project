#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Patient {
    string name;
    int age;
    string gender;
    string diagnosis;
};

void saveToCSV(const Patient& patient, const string& filename) {
    ofstream file;
    
    // Open in append mode to add new patients without overwriting
    file.open(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    // Write patient data as CSV row
    file << patient.name << "," << patient.age << "," 
         << patient.gender << "," << patient.diagnosis << endl;

    file.close();
}

int main() {
    Patient p;
    char another = 'y';
    string filename;

    // Prompt user for file type
    int fileType = 0;
    cout << "Choose file format to save patient data:\n";
    cout << "1. CSV file (patients.csv)\n";
    cout << "2. Excel file (patients.xlsx, CSV format)\n";
    cout << "Enter choice (1 or 2): ";
    while (!(cin >> fileType) || (fileType != 1 && fileType != 2)) {
        cout << "Invalid input! Please enter 1 or 2: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    cin.ignore(); // clear newline

    if (fileType == 1) {
        filename = "patients.csv";
    } else {
        filename = "patients.xlsx"; // still CSV format, but Excel will open it
    }

    // Write headers only once at the beginning (if file is new)
    ifstream check(filename);
    if (!check.good()) {
        ofstream headerFile(filename);
        headerFile << "Name,Age,Gender,Diagnosis" << endl;
        headerFile.close();
    }

    while (tolower(another) == 'y') {
        cout << "\nEnter patient name: ";
        getline(cin, p.name);

        cout << "Enter patient age: ";
        cin >> p.age;
        cin.ignore(); // clear newline from input buffer

        cout << "Enter patient gender: ";
        getline(cin, p.gender);

        cout << "Enter patient diagnosis: ";
        getline(cin, p.diagnosis);

        saveToCSV(p, filename);

        cout << "Patient saved successfully!\n";

        cout << "Add another patient? (y/n): ";
        cin >> another;
        cin.ignore(); // clear newline
    }

    cout << "\nAll patients saved to '" << filename << "'\n";
    return 0;
}
