#ifndef BANKER_H
#define BANKER_H

#include <iostream>
#include <vector>
using namespace std;

class BankerAlgorithm {
   private:
    int processes;  // Number of processes
    int resources;  // Number of resources

    vector<vector<int>> maximum;     // The maximum number of resources that each process can request
    vector<vector<int>> allocation;  // The number of resources allocated to each process
    vector<vector<int>> need;        // The remaining amount of resources that the process can request
    vector<int> available;           // Number of available resources

   public:
    BankerAlgorithm(int numProcesses, int numResources)
        : processes(numProcesses),
          resources(numResources),
          maximum(numProcesses, vector<int>(numResources)),
          allocation(numProcesses, vector<int>(numResources)),
          need(numProcesses, vector<int>(numResources)),
          available(numResources) {}

    void setMaximum(const vector<vector<int>>& max) { maximum = max; }

    void setAllocation(const vector<vector<int>>& alloc) {
        allocation = alloc;
        updateNeedMatrix();
    }

    void setAvailable(const vector<int>& avail) { available = avail; }

    bool isSafe() {  // Checks whether the system is in a safe state using the banker's algorithm
        vector<int> work = available;
        vector<bool> finish(processes, false);
        vector<int> safeSequence;

        int count = 0;
        while (count < processes) {
            bool found = false;
            for (int i = 0; i < processes; ++i) {
                if (!finish[i] && checkNeedLessOrEqual(i, work)) {
                    for (int j = 0; j < resources; ++j) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSequence.push_back(i);
                    found = true;
                    ++count;
                }
            }

            if (!found) {
                cout << "System is not in safe state!" << endl;
                return false;
            }
        }

        cout << "System is in safe state. Safe sequence: ";
        for (int i = 0; i < processes; ++i) {
            cout << safeSequence[i] << " ";
        }
        cout << endl;

        return true;
    }

   private:
    void updateNeedMatrix() {  // Updates the need matrix based on the current values in the maximum and
                               // allocation matrices
        for (int i = 0; i < processes; ++i) {
            for (int j = 0; j < resources; ++j) {
                need[i][j] = maximum[i][j] - allocation[i][j];
            }
        }
    }

    bool checkNeedLessOrEqual(int process,
                              const vector<int>& work) {  // Checks whether the process can meet its needs
                                                          // with the current available resources
        for (int i = 0; i < resources; ++i) {
            if (need[process][i] > work[i]) {
                return false;
            }
        }
        return true;
    }
};

#endif