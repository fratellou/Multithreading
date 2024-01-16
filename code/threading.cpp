#include "threading.h"

using namespace std;

const int numThreads = 15;
const int raceLength = 50;

// An array to represent the race track
char raceTrack[raceLength];

// Synchronization primitives
mutex mtx;
sem_t semaphore;
sem_t semaphoreSlim;
atomic_flag spinLock = ATOMIC_FLAG_INIT;
atomic<int> spinWaitCount(0);
int monitorLock = 0;
Barrier barrier(numThreads);

int main() {
    sem_init(&semaphore, 0, 1);
    sem_init(&semaphoreSlim, 0, 1);

    runRace("Mutex", mutexRace);
    runRace("Semaphore", semaphoreRace);
    runRace("SemaphoreSlim", semaphoreSlimRace);
    runRace("Barrier", barrierRace);
    runRace("SpinLock", spinLockRace);
    runRace("SpinWait", spinWaitRace);
    runRace("Monitor", monitorRace);

    sem_destroy(&semaphore);
    sem_destroy(&semaphoreSlim);

    return 0;
}

// Fill the race track with random symbols
void fillRaceTrack(char* track, int length) {
    for (int i = 0; i < length; ++i) {
        track[i] = ' ';
    }
}

// Output the race track to the console
void printRaceTrack() {
    for (int i = 0; i < raceLength; ++i) {
        cout << raceTrack[i];
    }
    cout << "\n\n";
}

// Racing using Mutex
void mutexRace(int id) {
    int position = 0;
    while (position < raceLength) {
        unique_lock<mutex> lock(mtx);
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        lock.unlock();
        usleep(10000);  // Симуляция работы
    }
}

// Racing using Semaphore
void semaphoreRace(int id) {
    int position = 0;
    while (position < raceLength) {
        sem_wait(&semaphore);
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        sem_post(&semaphore);
        usleep(10000);  // Симуляция работы
    }
}

// Racing using SemaphoreSlim
void semaphoreSlimRace(int id) {
    int position = 0;
    while (position < raceLength) {
        sem_wait(&semaphoreSlim);
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        sem_post(&semaphoreSlim);
        usleep(10000);  // Симуляция работы
    }
}

// Racing using SpinLock
void spinLockRace(int id) {
    int position = 0;
    while (position < raceLength) {
        while (spinLock.test_and_set(memory_order_acquire))
            ;  // Spin until we acquire the lock
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        spinLock.clear(memory_order_release);
        usleep(10000);  // Симуляция работы
    }
}

// Racing using SpinWait
void spinWaitRace(int id) {
    int position = 0;
    while (position < raceLength) {
        while (spinWaitCount.exchange(1, memory_order_acquire))
            ;  // Spin until we acquire the lock
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        spinWaitCount.store(0, memory_order_release);
        usleep(10000);  // Симуляция работы
    }
}

// Racing using Monitor
void monitorRace(int id) {
    int position = 0;
    while (position < raceLength) {
        lock_guard<mutex> lock(mtx);
        raceTrack[position] = static_cast<char>('0' + id);
        ++position;
        usleep(10000);  // Симуляция работы
    }
}

// Racing using Barrier
void barrierRace(int id) {
    int position = 0;
    while (position < raceLength) {
        {
            lock_guard<mutex> lock(mtx);
            raceTrack[position] = static_cast<char>('0' + id);
            ++position;
        }
        barrier.Wait();  // All threads wait at the barrier
        usleep(10000);   // Simulate work
    }
}

// Starting a race with the specified synchronization primitive
template <typename Function>
void runRace(const string& name, Function raceFunction) {
    fillRaceTrack(raceTrack, raceLength);  // Очистка дорожки
    auto start = chrono::high_resolution_clock::now();
    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(raceFunction, i);
    }
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = chrono::high_resolution_clock::now();
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << name << " Elapsed time: " << elapsed.count() << "ms" << endl;
    printRaceTrack();  // Вывод конечного состояния гоночной дорожки
}