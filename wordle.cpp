#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm>
#include <map>
//#include <set>
#include <vector>
#include <queue>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generateWords(string curr, queue<char> floating, set<string>& words, size_t idx, const set<std::string>& dict);

// Definition of primary wordle function
std::set<std::string> wordle(
        const std::string& in,
        const std::string& floating,
        const std::set<std::string>& dict)
{
    // Add your code here
    queue<char> floatQueue;
    for (size_t i=0; i<floating.size(); i++){
        floatQueue.push(floating[i]);
    }
    set<string> wordMatches;
    generateWords(in, floatQueue, wordMatches, 0, dict);
    return wordMatches;
}

// Define any helper functions here
void generateWords(string curr, queue<char> floating, set<string>& words, size_t idx, const set<std::string>& dict){
    set<char> usedChars; // for each recursive function, create set of letters already used
    // when applying a floating letter then remove it from list of floating letters
    // pass an index to see what the current index of word is to get base case
    if (idx == curr.size()){ // Base case
        // if curr is in dictionary, add to words;
        if (dict.find(curr) != dict.end()){
            words.insert(curr);
        }
        return;
    }
    size_t numBlanks = 0;
    for (size_t i=idx; i<curr.size(); i++){
        if (curr[i] == '-') numBlanks++;
    }
    if (curr[idx] == '-'){
        // remove floating letter each time you add, but when backtracking then add it back
        if (numBlanks < floating.size()) return;
        size_t floatSize = floating.size();
        for (size_t i=0; i<floatSize; i++){ // Iterate through floating letters
            curr[idx] = floating.front();
            char currChar = floating.front();
            floating.pop();
            // each time using one of the floating letters, remove the first floating letter and pass to next recursive call
            if (usedChars.find(currChar) == usedChars.end()) {
                generateWords(curr, floating, words, idx+1, dict);
                usedChars.insert(currChar);
            }
            floating.push(currChar);
            curr[idx] = '-';
        }
        // if length of floating equals length of missing then don't do a-z search
        if (numBlanks == floating.size()) return;
        for (char i='a'; i<='z'; i++){ // Iterate through whole alphabet
            curr[idx] = i;
            // if idx is a floating letter, remove floating character then add it back if unsuccessful
            if (usedChars.find(i) == usedChars.end()){
                generateWords(curr, floating, words, idx+1, dict);
            }
            curr[idx] = '-';
        }
    } else {
        generateWords(curr, floating, words, idx+1, dict); // Skip fixed letters
    }
}