//============================================================================
// Name        : Project Two.cpp
// Author      : Dylan Dunagan CS 300 June 22, 2025
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Course structure
struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

// BST node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class CourseBST {
private:
    Node* root;

    // Helper for inserting a course
    Node* insert(Node* node, Course course) {
        if (node == nullptr) return new Node(course);
        if (course.number < node->course.number) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }
        return node;
    }

    // Helper for in-order printing
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.number << ": " << node->course.title << endl;
        inOrder(node->right);
    }

    // Helper for searching
    Node* search(Node* node, const string& courseNumber) {
        if (node == nullptr || node->course.number == courseNumber)
            return node;
        if (courseNumber < node->course.number)
            return search(node->left, courseNumber);
        else
            return search(node->right, courseNumber);
    }

    // Helper to delete tree
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    CourseBST() : root(nullptr) {}

    ~CourseBST() {
        clear(root);
    }

    void insert(Course course) {
        root = insert(root, course);
    }

    void printCourses() {
        if (!root) {
            cout << "No courses loaded yet.\n";
            return;
        }
        inOrder(root);
    }

    Node* findCourse(const string& number) {
        return search(root, number);
    }
};

// Helper: Trim function
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, (last - first + 1));
}

// Global course tree and prereq map
CourseBST courseTree;
vector<Course> loadedCourses; // Temporary list to hold courses before inserting
map<string, Course> courseMap; // For prereq lookup only

// Load course data from file
void loadCoursesFromFile(const string& filename) {
    courseTree = CourseBST();  // Reset tree
    loadedCourses.clear();
    courseMap.clear();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string number, title, prereq1, prereq2;

        getline(ss, number, ',');
        getline(ss, title, ',');
        getline(ss, prereq1, ',');
        getline(ss, prereq2, ',');

        Course course;
        course.number = trim(number);
        course.title = trim(title);
        if (!prereq1.empty()) course.prerequisites.push_back(trim(prereq1));
        if (!prereq2.empty()) course.prerequisites.push_back(trim(prereq2));

        loadedCourses.push_back(course);
        courseMap[course.number] = course; // for later prereq title lookup
    }

    file.close();

    for (Course& c : loadedCourses) {
        courseTree.insert(c);
    }

    cout << "Courses loaded successfully into binary search tree.\n";
}

// Display a course's info and prereqs
void displayCourseInfo(const string& courseNumber) {
    Node* node = courseTree.findCourse(courseNumber);
    if (!node) {
        cout << "Course not found: " << courseNumber << endl;
        return;
    }

    Course course = node->course;
    cout << "\n" << course.number << ": " << course.title << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None\n";
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            string prereqNum = course.prerequisites[i];
            if (courseMap.count(prereqNum)) {
                cout << prereqNum << " (" << courseMap[prereqNum].title << ")";
            }
            else {
                cout << prereqNum;
            }
            if (i < course.prerequisites.size() - 1)
                cout << ", ";
        }
        cout << endl;
    }
}

// Menu loop
void displayMenu() {
    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "  1. Load data file\n";
        cout << "  2. Print course list\n";
        cout << "  3. Print course\n";
        cout << "  9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            string filename;
            cout << "Enter file name: ";
            cin >> filename;
            loadCoursesFromFile(filename);
            break;
        }
        case 2:
            courseTree.printCourses();
            break;
        case 3: {
            string courseNum;
            cout << "Enter course number (e.g., CSCI101): ";
            cin >> courseNum;
            displayCourseInfo(courseNum);
            break;
        }
        case 9:
            cout << "Goodbye!\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    }
}

// Entry point
int main() {
    displayMenu();
    return 0;
}