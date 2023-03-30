#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedHelper(const AvailabilityMatrix& avail,
                 const size_t dailyNeed,
                 const size_t maxShifts,
                 DailySchedule& sched,
                 size_t day,
                 map<Worker_T, size_t> workerToAvail, // Maps worker ID to their number of shifts scheduled
                 size_t slot);


// Add your implementation of schedule() and other helper functions here

bool schedule(
        const AvailabilityMatrix& avail,
        const size_t dailyNeed,
        const size_t maxShifts,
        DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below
    map<Worker_T, size_t> workerAvail;
    // Initialize the schedule and map to correct size
    for (size_t i=0; i<avail.size(); i++){
        vector <Worker_T> days(dailyNeed, INVALID_ID);
        sched.push_back(days);
    }
    for (Worker_T i=0; i<avail[0].size(); i++){
        workerAvail.insert(pair<Worker_T ,size_t>(i,0));
    }
    return schedHelper(avail, dailyNeed, maxShifts, sched, 0, workerAvail, 0);
}

bool schedHelper(const AvailabilityMatrix& avail,
                 const size_t dailyNeed,
                 const size_t maxShifts,
                 DailySchedule& sched,
                 size_t day,
                 map<Worker_T, size_t> workerToAvail,
                 size_t slot)
{
    if (day == avail.size()) return true; // Base case
    for (Worker_T i=0; i<workerToAvail.size(); i++){ // Iterate through total number of workers
        if (avail[day][i] && workerToAvail[i] < maxShifts){
            // Try adding this user at this specific day/shift
            workerToAvail[i]++;
            sched[day][slot] = i;
            bool result = false;
            if (slot+1 == dailyNeed){ // Iterate through days (rows) in schedule matrix
                result = schedHelper(avail, dailyNeed, maxShifts, sched, day+1, workerToAvail, 0);
            } else { // Iterate through slots (columns) in schedule matrix
                result = schedHelper(avail, dailyNeed, maxShifts, sched, day, workerToAvail, slot+1);
            }
            if (result) return true; // Done if a solution is found
            else { // Backtrack
                workerToAvail[i]--;
                sched[day][slot] = INVALID_ID;
            }
        }
    }
    return false;
}