#ifndef THREADING_H
#define THREADING_H

#include <semaphore.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class Barrier {
   public:
    explicit Barrier(int count) : count_(count), original_count_(count) {}

    void Wait() {
        unique_lock<mutex> lock(mutex_);
        if (--count_ == 0) {
            count_ = original_count_;
            cv_.notify_all();
        } else {
            cv_.wait(lock, [this] { return count_ == original_count_; });
        }
    }

   private:
    mutex mutex_;
    condition_variable cv_;
    int count_;
    const int original_count_;
};

extern Barrier barrier;  // Declaration of the barrier

void fillRaceTrack(char* track, int length);  // Fill the race track with random symbols
void printRaceTrack();                        // Output the race track to the console
void mutexRace(int id);                       // Racing using Mutex
void semaphoreRace(int id);                   // Racing using Semaphore
void semaphoreSlimRace(int id);               // Racing using SemaphoreSlim
void spinLockRace(int id);                    // Racing using SpinLock
void spinWaitRace(int id);                    // Racing using SpinWait
void monitorRace(int id);                     // Racing using Monitor
void barrierRace(int id);                     // Racing using Barrier

template <typename Function>
void runRace(const string& name,
             Function raceFunction);  // Starting a race with the specified synchronization primitive

#endif