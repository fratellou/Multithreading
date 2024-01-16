# Multithreading

1. Implement parallel launch of a specified number of streams containing random characters from an ASCII table in the race format. To analyze the features of synchronization primitives. Analyze the speeds of the primitives.
2. Data is set: the size of the data array, the number of parallel streams, the values of the structure attributes, additional data. The result of the program is the output of processing time without using multithreading, processing time using multithreading and processing results. 
3. Implement the banker's algorithm.

## Contents

1. [Task 1](#task-1) \
    1.1. [Task 1. Information](#task-1.-information) \
    1.2. [Task 1. Code](#task-1.-code)
2. [Task 2](#task-2) \
    2.1. [Task 2. Code](#task-2.-code)
3. [Task 3](#task-3) \
    3.1. [Task 3. Information](#task-3.-information) \
    3.2. [Task 3. Code](#task-3.-code)

# Task 1

Implement parallel launch of a specified number of streams containing random characters from an ASCII table in the race format. 
To analyze the features of synchronization primitives (comparative analysis): 
- Mutexes 
- Semaphore 
- SemaphoreSlim 
- Barrier 
- SpinLock 
- SpinWait 
- Monitor 
Analyze the speeds of primitives using StopWatch and BenchmarkDotNet.

## Task 1. Information

A race is a competition between threads for access to shared resources. In the task of implementing a race, threads compete for access to a race track represented by an array of symbols.

Synchronization primitives:

- Mutex - provides exclusive access to a resource. Only one thread can own a mutex at a given time. A thread that cannot capture a mutex is blocked and waits for the mutex to be released. It is often used when a critical section is required that is accessible to only one thread. It is easy to use, but can cause performance problems due to blocking.
- Semaphore - allows you to limit access to a resource to a certain number of threads. The number of available permissions is set when creating a semaphore. It is suitable when a resource can be used by several threads, but with a limit on their number. It is more flexible than a mutex, but more difficult to use.
- SemaphoreSlim (Thin Semaphore) is a simplified version of the semaphore with additional optimizations to improve performance. Provides a permission counter similar to a semaphore, but with an additional lightweight implementation. It is used when access control to resources with a minimum overhead is required. It is less expensive than a conventional semaphore, but may have limited capabilities.
- Barrier - synchronizes a group of threads, waiting until all of them reach the barrier before continuing execution. Provides a collection point for threads before continuing execution. It is used when it is necessary to wait for the completion of execution of several threads before continuing. It is suitable for the coordinated execution of a group of tasks.
- SpinLock - provides a lock using active standby (spin) instead of passive. Threads repeat attempts to capture the lock without going into a waiting state. Suitable for short critical sections. Fast in case of short delays, but can cause problems with long waits.
-SpinWait (Waiting for a spin) - allows the thread to actively wait for a condition change instead of blocking. Threads repeat attempts to wait for a condition change without going into a waiting state. It is suitable for optimizing the waiting conditions without actively using the processor.
- Monitor - provides exclusive access to a resource, similar to a mutex, but also provides additional functionality such as waiting for a signal and notifying other threads. It is used for simple synchronization cases with minimal need.

Comparative analysis:

- The fastest primitive depends on the specific usage scenario and system features.
- Usage also depends on the context. For example, mutexes and semaphores are often used, but in some cases spin-locking may be more effective.
- Each primitive has its own strengths and weaknesses, and the choice depends on the specific requirements of the task.

## Task 1. Code

`fillRaceTrack` - fills the racetrack array with random symbols.

`printRaceTrack` - displays the current state of the race track in the console.

`mutexRace` - simulates a race using a mutex for synchronization. id: ID of the stream participating in the race.

`semaphoreRace` - simulates a race using a semaphore for synchronization. 

`semaphoreSlimRace` - simulates a race using a "thin" semaphore for synchronization.

`spinLockRace` - simulates a race using spinLockRace for synchronization.

`spinWaitRace` - simulates a race using SpinWait for synchronization.

`monitorRace` - simulates a race using a monitor for synchronization.

`barrierRace` - simulates a race using a barrier for synchronization.

`run Race` - starts the race with the specified synchronization primitive and outputs the elapsed time.
- name: The name of the synchronization primitive;
- race Function: A function representing a race with a specific primitive.

# Task 2 

Data is set: the size of the data array, the number of parallel streams, the values of the structure attributes, additional data. 
The result of the program is the output of processing time without using multithreading, processing time using multithreading and processing results. 
The structure contains information about students who have passed the Unified State Exam (full name, age, school number, list of exam results). The exam result includes the name of the subject and the number of points. It is necessary to bring out the TOP 3 schools in terms of the largest number of students who have passed at least one exam for 100 points.

## Task 2. Code

`main`:
- Asks the user for the number of students (dataSize) and the number of threads (numThreads).
- Generates random data about students using the generateRandomStudent function and stores it in the students vector.
- Measures execution time and processes data first in single-threaded mode using processSingleThreaded, then in multithreaded mode using processMultiThreaded.
- Displays the results, including the execution time for each mode.

`generateRandomStudent` - random student generation function:
- Accepts an index as a string and generates random data about a student, including grades in mathematics and Russian, age, school number and exam results in several subjects.
- Displays the generated data on the screen.
- Creates and returns the Student structure with the generated data.

`hasPerfectScore` - checking the student's maximum grade:
- Accepts the student as an argument.
- Returns true if the student has at least one maximum score (100), otherwise returns false.

`processSingleThreaded` - data processing function in single-threaded mode:
- Creates a vector of schools, which stores information about each school (school number, number of students with maximum scores and their names).
- It goes through the vector of students and updates the relevant information in the vector of schools for each student with the maximum score.
- Sorts schools by the number of students with the highest scores in descending order.
- Displays the top 3 schools with the highest number of students with the highest scores.

`processMultiThreaded` - data processing function in multithreaded mode:
- Uses a set (schoolsMap) to store information about schools instead of a vector to avoid the problem of data races when accessed from multiple streams.
- Creates multiple threads to process parts of the student vector in parallel.
- After the threads are completed, it converts the set into a vector and sorts it in the same way as in processSingleThreaded.
- Displays the top 3 schools with the highest number of students with the highest scores.
processMultiThreaded uses a mutex to ensure security when updating data on the total number of students with maximum scores in all streams.

# Task 3

Implement the banker's algorithm.

## Task 3. Information

The banker's algorithm is an algorithm that is used to prevent resource shortages in multitasking systems.

Let's say we have a certain amount of resources, for example, processor time or memory, and several processes that can request these resources. The purpose of the banker's algorithm is to prevent situations where processes request more resources than are available, which can lead to a system lockdown.

The banker's algorithm works as follows:

1. At the beginning, each process announces how many resources it can consume during its operation. This is called maximum consumption.
2. When a process requests resources, the system checks whether the requested amount does not exceed its maximum consumption. If so, the request may be rejected.
3. If the request meets the requirements, the system temporarily allocates resources to the process. However, the system does not allow the allocation of resources if this may lead to a situation in which there will not be enough resources to satisfy the requests of other processes.
4. If the system allocates resources to a process, the process uses them. When the process has finished using resources, it returns them to the system.
5. If the system cannot allocate resources to a process, then the process must wait until resources become available.

The banker's algorithm ensures that if the system is in a safe state before allocating resources, then it will remain safe after allocation. In other words, the banker's algorithm prevents the system from being blocked and ensures proper resource management. This is especially important in multitasking systems where multiple processes are running simultaneously and competing for resources.

Basic concepts in the banker's algorithm:
- The maximum number of resources (Maximum): This is the maximum amount of resources that a process can request during execution.
- Allocated resources (Allocation): This is the amount of resources that have already been allocated to the process.
- Remaining (unsatisfied) needs (Need): This is the difference between the maximum number of resources and the allocated resources. Thus, Need[i, j] = Maximum[i, j] - Allocation[i, j].
- Available resources (Available): This is the amount of resources that are currently available to the system for allocation to processes.

## Task 3. Code

`is Safe` - checks whether the system is in a safe state using the banker's algorithm.

- Uses the work vector to track available resources during validation.
- Uses the finish vector to track completed processes.
- Uses the save Sequence vector to form a safe execution sequence.
- Uses a while loop to check each process for the possibility of allocating resources to it.

If the check is successful, it outputs a safe execution sequence and returns true.
If a suitable process cannot be found, it outputs a message and returns false.

`updateNeedMatrix`- updates the need matrix based on the current values in the maximum and allocation matrices. Uses two nested loops to iterate through each element of the matrix.

`checkNeedLessOrEqual`- checks whether the process can meet its needs with the current available resources.

>
>fratellou, 2024