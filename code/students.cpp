#include "students.h"
using namespace std;

int main() {
    int dataSize;
    cout << "Enter the count of students: ";
    cin >> dataSize;

    int numThreads;
    cout << "Enter the count of threads: ";
    cin >> numThreads;
    cout << '\n';

    // Generate random student data
    vector<Student> students;
    for (int i = 0; i < dataSize; ++i) {
        students.push_back(generateRandomStudent(to_string(i)));
    }
    cout << '\n';

    // Measure time and process data single-threaded
    auto startSingleThreaded = chrono::high_resolution_clock::now();
    processSingleThreaded(students);
    auto endSingleThreaded = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedSingleThreaded = endSingleThreaded - startSingleThreaded;

    // Measure time and process data multi-threaded
    auto startMultiThreaded = chrono::high_resolution_clock::now();
    processMultiThreaded(students, numThreads);
    auto endMultiThreaded = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedMultiThreaded = endMultiThreaded - startMultiThreaded;

    // Print execution times
    cout << fixed << setprecision(6) << "Single-threaded time: " << elapsedSingleThreaded.count()
         << " seconds\n";
    cout << fixed << setprecision(6) << "Multi-threaded time: " << elapsedMultiThreaded.count()
         << " seconds\n";

    return 0;
}

// Function to generate a random student
Student generateRandomStudent(string index) {
    int math = rand() % 101;
    int rus = rand() % 101;
    int age = rand() % 10 + 16;
    int school = rand() % 10 + 1;

    cout << "Student " << index << ": "
         << "school:" << school << ", age:" << age << " | math:" << math << " | russian:" << rus << " | ";
    vector<ExamResult> examResults;
    examResults.push_back({"russian", rus});
    examResults.push_back({"math", math});
    bool firstSubject = true;  // Flag to check if it's the first subject
    for (const char* subject : {"physics", "chemistry", "biology", "informatics"}) {
        if (rand() % 2 == 0) {
            int score = rand() % 101;
            // Output a comma only if it's not the first subject
            if (!firstSubject) {
                cout << " | ";
            }
            cout << subject << ":" << score;
            examResults.push_back({subject, score});
            firstSubject = false;
        }
    }

    cout << "\n";

    // Corrected initialization
    return {index, age, school, examResults};
}

// Predicate to check if a student has a perfect score in any exam
bool hasPerfectScore(const Student& student) {
    return any_of(student.examResults.begin(), student.examResults.end(),
                  [](const ExamResult& result) { return result.score == 100; });
}

// Function to process data single-threaded and print the top 3 schools
void processSingleThreaded(const vector<Student>& students) {
    vector<School> schools;  // Vector to store information about each school

    // Iterate through students and find those with perfect scores
    for (const auto& student : students) {
        if (hasPerfectScore(student)) {
            // Check if a school entry already exists
            auto schoolIt = find_if(schools.begin(), schools.end(), [&](const School& school) {
                return school.schoolNumber == student.schoolNumber;
            });

            // If the school entry exists, add the student to that school
            if (schoolIt != schools.end()) {
                schoolIt->perfectCount++;
                schoolIt->perfectStudents.push_back(student.name);
            } else {
                // If the school entry does not exist, create a new entry
                schools.push_back({student.schoolNumber, 1, {student.name}});
            }
        }
    }

    // Sort schools by the number of perfect students in descending order
    sort(schools.begin(), schools.end(),
         [](const School& a, const School& b) { return a.perfectCount > b.perfectCount; });

    // Print the schools with perfect scores
    cout << "Schools with perfect scores (Single-threaded):\n";
    int top = 0;
    for (const auto& school : schools) {
        cout << "School " << school.schoolNumber << ": ";
        for (size_t i = 0; i < school.perfectStudents.size(); ++i) {
            cout << school.perfectStudents[i];
            if (i != school.perfectStudents.size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
        if (++top == 3) break;
    }
    cout << "\n";
}

// Function to process data multi-threaded and print the top 3 schools
void processMultiThreaded(const vector<Student>& students, int numThreads) {
    map<int, School> schoolsMap;  // Map to store information about each school
    vector<thread> threads;
    mutex localMutex;

    int studentsPerThread = students.size() / numThreads;
    int startIndex = 0;

    // Create threads to process subsets of students in parallel
    for (int i = 0; i < numThreads; ++i) {
        int endIndex = (i == numThreads - 1) ? students.size() : startIndex + studentsPerThread;
        threads.emplace_back([&, startIndex, endIndex]() {
            for (int j = startIndex; j < endIndex; ++j) {
                if (hasPerfectScore(students[j])) {
                    auto schoolIt = schoolsMap.find(students[j].schoolNumber);

                    if (schoolIt != schoolsMap.end()) {
                        schoolIt->second.perfectCount++;
                        schoolIt->second.perfectStudents.push_back(students[j].name);
                    } else {
                        localMutex.lock();
                        schoolsMap[students[j].schoolNumber] =
                            School{students[j].schoolNumber, 1, {students[j].name}};
                        localMutex.unlock();
                    }
                }
            }
        });
        startIndex = endIndex;
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    // Convert the map to a vector for sorting
    vector<School> schools(schoolsMap.size());
    transform(schoolsMap.begin(), schoolsMap.end(), schools.begin(),
              [](const pair<int, School>& entry) { return entry.second; });

    // Sort schools by the number of perfect students in descending order
    sort(schools.begin(), schools.end(),
         [](const School& a, const School& b) { return a.perfectCount > b.perfectCount; });

    // Print the schools with perfect scores
    cout << "Schools with perfect scores (Multi-threaded):\n";
    int top = 0;
    for (const auto& school : schools) {
        cout << "School " << school.schoolNumber << ": ";
        for (size_t i = 0; i < school.perfectStudents.size(); ++i) {
            cout << school.perfectStudents[i];
            if (i != school.perfectStudents.size() - 1) {
                cout << ", ";
            }
        }
        cout << "\n";
        if (++top == 3) break;
    }
    cout << "\n";
}