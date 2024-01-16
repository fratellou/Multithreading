#ifndef STUDENTS_H
#define STUDENTS_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

struct ExamResult {  // Storing exam results
    string subject;
    int score;
};

struct Student {  // Information about the student (including his grades)
    string name;
    int age;
    int schoolNumber;
    vector<ExamResult> examResults;
};

struct School {  // Storing data about the school
    int schoolNumber;
    int perfectCount;
    vector<string> perfectStudents;
};

Student generateRandomStudent(string index);   // Generates a random student based on the passed index
bool hasPerfectScore(const Student& student);  // Checks whether the student has the maximum score
void processSingleThreaded(
    const vector<Student>& students);  // Processes a vector of students in a single thread
void processMultiThreaded(const vector<Student>& students,
                          int numThreads);  // Processes the vector of students in several streams, also takes
                                            // the number of streams as an argument

#endif