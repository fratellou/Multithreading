#include "banker.h"

int main() {
    int numProcesses, numResources;

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    cout << "Enter the number of resources: ";
    cin >> numResources;

    BankerAlgorithm banker(numProcesses, numResources);

    // Entering the maximum amount of resources that each process can request
    cout << "Enter the maximum matrix for each process:" << endl;
    vector<vector<int>> maximum(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        cout << "Process " << i << ": ";
        for (int j = 0; j < numResources; ++j) {
            cin >> maximum[i][j];
        }
    }

    // Entering the number of resources allocated to each process
    cout << "Enter the allocation matrix for each process:" << endl;
    vector<vector<int>> allocation(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; ++i) {
        cout << "Process " << i << ": ";
        for (int j = 0; j < numResources; ++j) {
            cin >> allocation[i][j];
        }
    }

    // Entering available resources
    cout << "Enter the available vector:" << endl;
    vector<int> available(numResources);
    for (int i = 0; i < numResources; ++i) {
        cin >> available[i];
    }

    banker.setMaximum(maximum);
    banker.setAllocation(allocation);
    banker.setAvailable(available);

    banker.isSafe();

    return 0;
}
