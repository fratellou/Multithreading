# Multithreading
To compile a program, you must use -lpthread
Example:
g++ -std=c++11 main.cpp -o main -lpthread

## Task 1
Implement parallel launch of a specified number of streams
containing random characters from an ASCII table in the race format.
To analyze the features of synchronization primitives
(comparative analysis):
• Mutexes
• Semaphore
• SemaphoreSlim
• Barrier
• SpinLock
• SpinWait
• Monitor
Analyze the speeds of primitives using
StopWatch and BenchmarkDotNet.

## Task 2 
Data is set: the size of the data array, the number of parallel
streams, the values of the structure attributes, additional data.
The result of the program is the output of processing time without
using multithreading, processing time using
multithreading and processing results.
The structure contains information about students who have passed the Unified State Exam (full name,
age, school number, list of exam results). The exam result
includes the name of the subject and the number of points. It is necessary to withdraw
The TOP 3 schools in terms of the largest number of students who have passed
at least one exam for 100 points.