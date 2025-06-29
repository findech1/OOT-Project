#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <limits>

const int MAX_ASSIGNMENTS = 10;

class Student {
private:
    std::string name;
    std::vector<int> marks;
    std::vector<char> grades;
    double average;
    char finalGrade;

public:
    Student() : average(0.0), finalGrade('E') {}

    Student(const std::string& studentName) : name(studentName), average(0.0), finalGrade('E') {}

    const std::string& getName() const { return name; }
    const std::vector<int>& getMarks() const { return marks; }
    const std::vector<char>& getGrades() const { return grades; }
    double getAverage() const { return average; }
    char getFinalGrade() const { return finalGrade; }

    void setName(const std::string& studentName) { name = studentName; }

    void addMark(int mark) {
        if (mark >= 0 && mark <= 100) {
            marks.push_back(mark);
            grades.push_back(calculateGrade(mark));
            calculateAverage();
        }
    }

    void updateMark(int assignmentIndex, int newMark) {
        if (assignmentIndex >= 0 && assignmentIndex < static_cast<int>(marks.size()) &&
            newMark >= 0 && newMark <= 100) {
            marks[assignmentIndex] = newMark;
            grades[assignmentIndex] = calculateGrade(newMark);
            calculateAverage();
        }
    }

    char calculateGrade(int mark) const {
        if (mark >= 70 && mark <= 100) return 'A';
        else if (mark >= 60 && mark <= 69) return 'B';
        else if (mark >= 50 && mark <= 59) return 'C';
        else if (mark >= 40 && mark <= 49) return 'D';
        else return 'E';
    }

    void calculateAverage() {
        if (marks.empty()) {
            average = 0.0;
            finalGrade = 'E';
            return;
        }

        double sum = 0;
        for (int mark : marks) {
            sum += mark;
        }
        average = sum / marks.size();
        finalGrade = calculateGrade(static_cast<int>(average));
    }

    void displayInfo() const {
        std::cout << "\nStudent: " << name << std::endl;
        std::cout << "Assignments: ";
        for (size_t i = 0; i < marks.size(); ++i) {
            std::cout << marks[i] << "(" << grades[i] << ") ";
        }
        std::cout << "\nAverage: " << std::fixed << std::setprecision(2) << average;
        std::cout << " | Final Grade: " << finalGrade << std::endl;
    }

    int getNumAssignments() const {
        return static_cast<int>(marks.size());
    }
};

class GradingSystem {
private:
    std::vector<Student> students;
    int numAssignments;

    void clearInputBuffer() const {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int getValidInteger(int min, int max) const {
        int value;
        while (!(std::cin >> value) || value < min || value > max) {
            std::cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
            clearInputBuffer();
        }
        return value;
    }

public:
    GradingSystem() : numAssignments(0) {}

    void showMenu() {
        std::cout << "\n=== KCA Students Grading System ===" << std::endl;
        std::cout << "1. Set number of assignments" << std::endl;
        std::cout << "2. Add student" << std::endl;
        std::cout << "3. Display all students" << std::endl;
        std::cout << "4. Search student" << std::endl;
        std::cout << "5. Update student marks" << std::endl;
        std::cout << "6. Delete student" << std::endl;
        std::cout << "7. Display statistics" << std::endl;
        std::cout << "8. Sort students" << std::endl;
        std::cout << "9. Export to file" << std::endl;
        std::cout << "10. Import from file" << std::endl;
        std::cout << "11. Display grade distribution" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
    }

    void setNumAssignments() {
        std::cout << "Enter number of assignments (1-" << MAX_ASSIGNMENTS << "): ";
        numAssignments = getValidInteger(1, MAX_ASSIGNMENTS);
        std::cout << "Number of assignments set to: " << numAssignments << std::endl;
    }

    void addStudent() {
        if (numAssignments == 0) {
            std::cout << "Please set number of assignments first!" << std::endl;
            return;
        }

        std::string name;
        std::cout << "Enter student name: ";
        clearInputBuffer();
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "Name cannot be empty!" << std::endl;
            return;
        }

        Student newStudent(name);

        for (int i = 0; i < numAssignments; ++i) {
            std::cout << "Enter marks for Assignment " << (i + 1) << " (0-100): ";
            int mark = getValidInteger(0, 100);
            newStudent.addMark(mark);
        }

        students.push_back(newStudent);
        std::cout << "Student added successfully!" << std::endl;
    }

    void displayAllStudents() const {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        std::cout << "\n=== All Students ===" << std::endl;
        for (const auto& student : students) {
            student.displayInfo();
        }
    }

    void searchStudent() const {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        std::string searchName;
        std::cout << "Enter student name to search: ";
        clearInputBuffer();
        std::getline(std::cin, searchName);

        bool found = false;
        for (const auto& student : students) {
            if (student.getName().find(searchName) != std::string::npos) {
                student.displayInfo();
                found = true;
            }
        }

        if (!found) {
            std::cout << "Student not found!" << std::endl;
        }
    }

    void updateStudentMarks() {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        std::string searchName;
        std::cout << "Enter student name to update: ";
        clearInputBuffer();
        std::getline(std::cin, searchName);

        for (auto& student : students) {
            if (student.getName() == searchName) {
                std::cout << "Enter assignment number (1-" << student.getNumAssignments() << "): ";
                int assignmentNum = getValidInteger(1, student.getNumAssignments());

                std::cout << "Enter new marks (0-100): ";
                int newMark = getValidInteger(0, 100);

                student.updateMark(assignmentNum - 1, newMark);
                std::cout << "Marks updated successfully!" << std::endl;
                return;
            }
        }

        std::cout << "Student not found!" << std::endl;
    }

    void deleteStudent() {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        std::string searchName;
        std::cout << "Enter student name to delete: ";
        clearInputBuffer();
        std::getline(std::cin, searchName);

        auto it = std::find_if(students.begin(), students.end(),
            [&searchName](const Student& s) { return s.getName() == searchName; });

        if (it != students.end()) {
            students.erase(it);
            std::cout << "Student deleted successfully!" << std::endl;
        } else {
            std::cout << "Student not found!" << std::endl;
        }
    }

    void displayStatistics() const {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        double totalAverage = 0;
        double highestAverage = 0;
        double lowestAverage = 100;
        std::string topStudent, bottomStudent;

        for (const auto& student : students) {
            double avg = student.getAverage();
            totalAverage += avg;

            if (avg > highestAverage) {
                highestAverage = avg;
                topStudent = student.getName();
            }

            if (avg < lowestAverage) {
                lowestAverage = avg;
                bottomStudent = student.getName();
            }
        }

        double classAverage = totalAverage / students.size();

        std::cout << "\n=== Class Statistics ===" << std::endl;
        std::cout << "Total Students: " << students.size() << std::endl;
        std::cout << "Class Average: " << std::fixed << std::setprecision(2) << classAverage << std::endl;
        std::cout << "Highest Average: " << highestAverage << " (" << topStudent << ")" << std::endl;
        std::cout << "Lowest Average: " << lowestAverage << " (" << bottomStudent << ")" << std::endl;
    }

    void sortStudents() {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        std::sort(students.begin(), students.end(),
            [](const Student& a, const Student& b) {
                return a.getAverage() > b.getAverage();
            });

        std::cout << "Students sorted by average (highest to lowest):" << std::endl;
        displayAllStudents();
    }

    void exportToFile() const {
        if (students.empty()) {
            std::cout << "No students to export!" << std::endl;
            return;
        }

        std::ofstream file("student_results.csv");
        if (!file.is_open()) {
            std::cout << "Error opening file for writing!" << std::endl;
            return;
        }

        file << "Student Name";
        for (int i = 0; i < numAssignments; ++i) {
            file << ",Assignment " << (i + 1) << ",Grade";
        }
        file << ",Average,Final Grade\n";

        for (const auto& student : students) {
            file << student.getName();

            const auto& marks = student.getMarks();
            const auto& grades = student.getGrades();

            for (size_t i = 0; i < marks.size(); ++i) {
                file << "," << marks[i] << "," << grades[i];
            }

            file << "," << std::fixed << std::setprecision(2) << student.getAverage()
                 << "," << student.getFinalGrade() << "\n";
        }

        file << "\nGrading Criteria:\n";
        file << "Score Range,Grade\n";
        file << "70-100,A\n";
        file << "60-69,B\n";
        file << "50-59,C\n";
        file << "40-49,D\n";
        file << "0-39,E\n";

        file.close();
        std::cout << "Data exported successfully to 'student_results.csv'" << std::endl;
    }

    void importFromFile() {
        std::ifstream file("student_results.csv");
        if (!file.is_open()) {
            std::cout << "Error opening file for reading! Make sure 'student_results.csv' exists." << std::endl;
            return;
        }

        students.clear();
        std::string line;

        if (!std::getline(file, line)) {
            std::cout << "File is empty or corrupted!" << std::endl;
            file.close();
            return;
        }

        while (std::getline(file, line) && !line.empty()) {
            if (line.find("Grading Criteria:") != std::string::npos) break;

            std::stringstream ss(line);
            std::string name;

            if (!std::getline(ss, name, ',') || name.empty()) continue;

            Student student(name);
            std::string markStr, gradeStr;

            while (std::getline(ss, markStr, ',') && std::getline(ss, gradeStr, ',')) {
                try {
                    int mark = std::stoi(markStr);
                    if (mark >= 0 && mark <= 100) {
                        student.addMark(mark);
                    }
                } catch (const std::exception&) {
                    break;
                }
            }

            if (student.getNumAssignments() > 0) {
                students.push_back(student);
            }
        }

        file.close();
        std::cout << "Data imported successfully! Loaded " << students.size() << " students." << std::endl;
    }

    void displayGradeDistribution() const {
        if (students.empty()) {
            std::cout << "No students found!" << std::endl;
            return;
        }

        int gradeCount[5] = {0};

        for (const auto& student : students) {
            char grade = student.getFinalGrade();
            switch (grade) {
                case 'A': gradeCount[0]++; break;
                case 'B': gradeCount[1]++; break;
                case 'C': gradeCount[2]++; break;
                case 'D': gradeCount[3]++; break;
                case 'E': gradeCount[4]++; break;
            }
        }

        std::cout << "\n=== Grade Distribution ===" << std::endl;
        char grades[] = {'A', 'B', 'C', 'D', 'E'};
        for (int i = 0; i < 5; ++i) {
            double percentage = (gradeCount[i] * 100.0) / students.size();
            std::cout << "Grade " << grades[i] << ": " << gradeCount[i]
                      << " students (" << std::fixed << std::setprecision(1)
                      << percentage << "%)" << std::endl;
        }
    }

    void run() {
        int choice;

        std::cout << "Welcome to KCA Students Grading System!" << std::endl;

        do {
            showMenu();
            choice = getValidInteger(0, 11);

            switch (choice) {
                case 1: setNumAssignments(); break;
                case 2: addStudent(); break;
                case 3: displayAllStudents(); break;
                case 4: searchStudent(); break;
                case 5: updateStudentMarks(); break;
                case 6: deleteStudent(); break;
                case 7: displayStatistics(); break;
                case 8: sortStudents(); break;
                case 9: exportToFile(); break;
                case 10: importFromFile(); break;
                case 11: displayGradeDistribution(); break;
                case 0: std::cout << "Thank you for using KCA Grading System!" << std::endl; break;
                default: std::cout << "Invalid choice! Please try again." << std::endl;
            }
        } while (choice != 0);
    }
};

int main() {
    GradingSystem system;
    system.run();
    return 0;
}
