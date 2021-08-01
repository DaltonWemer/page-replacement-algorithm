//********************************************************************
//
// Dalton Wemer
// Operating Systems
// Programming Project #5: Page Replacement Simulation
// April 1, 2021
// Instructor: Dr. Siming Liu
//
//********************************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <algorithm>
#include <deque> 
#include <time.h> 
#include <unordered_map>

using namespace std;

//********************************************************************
//
// First In First Out Algorithm Implementation
//
// This fucntion simulates page replacement by replacing the element in memory
// that was first to arrive.
//
// Return Value
// ------------
// int                        integer representing the number of page faults
//
// Value Parameters
// ----------------
// pages		integer array		The Pages
// pagenum	int	 The amount of pages the algorithm will be simulating
//
//*******************************************************************

int fifoPageFaults(vector<int> pages, int pageNum) 
{ 
    int page_faults = 0; 

    // Queue to represent our pages
    deque<int> frame; 

    // Itterate through each page in our reference string
    for (int i = 0; i < pages.size(); i++)
    {
        // If we have not reached the max page size, push the page into memory and increment faults
        if(pageNum > frame.size())
        {
            frame.push_back(pages[i]);
            page_faults++;

        } else {
            // Once capacity has been met, check to see if the page is already in memory
            // and if not we remove the first element and add the new page to the last position
            if ( find(frame.begin(), frame.end(), pages[i]) == frame.end())
            {
                page_faults++;
                frame.pop_front();
                frame.push_back(pages[i]);
            }else {
                // The page exists in memory and we have no page fault, the program does nothing
            }
        }   
    }
    return page_faults; 
} 


//********************************************************************
//
// Least Recently Used Algorithm Implementation
//
// The Least Recently used page replacement strategy is a is a
// greedy algorithm which, like the name implies, replaces the page
// that has been least recently used
//
// Return Value
// ------------
// int                        integer representing the number of page faults
//
// Value Parameters
// ----------------
// pages		integer array		The Pages
// pagenum	int	 The amount of pages the algorithm will be simulating
//
//*******************************************************************

int lruPageFaults(vector<int> pages, int pageNum) 
{ 
    // Set the inital value of page faults to 0
    int page_faults = 0; 

    // Queue to represent our pages
    deque<int> frame;

    // We are using a hash map to store the indexes of the
    // last time our number in our
    unordered_map<int, int> indexesOfLRU;

    int lru = 1;

    // Itterate through each page in our reference string
    for (int i = 0; i < pages.size(); i++)
    {
        // If we have not reached the max page size, push the page into memory and increment faults
        // we are also incrementing our least recently used offset
        if(pageNum > frame.size())
        {
            frame.push_front(pages[i]);
            page_faults++;
            lru++;
            indexesOfLRU[pages[i]] = i;
        } else {

            // Once capacity has been met, check to see if the page is already in memory
            // and if not we remove the least recently used page
            if ( find(frame.begin(), frame.end(), pages[i]) == frame.end())
            {
                int lru = INT_MAX, val;
                
                for (auto it=frame.begin(); it!=frame.end(); it++)
                {
                    if (indexesOfLRU[*it] < lru)
                    {
                        lru = indexesOfLRU[*it];
                        val = *it;
                    }
                }
                // Remove the least recently used page
                frame.erase(frame.begin() + val);
                // Insert the current page
                frame.insert(frame.begin() + lru, pages[i]);
                // Increment page faults
                page_faults++;

                if (lru < pageNum)
                {
                    lru++; 
                }else {
                    lru = 1;
                }

                // Update the indexes map
                indexesOfLRU[pages[i]] = i;
            }else {
                // The page exists in memory and we have no page fault, the program does nothing
            }
        }   
    }
    return page_faults;  
} 

//********************************************************************
//
// Optimal Page Replacement Algorithm Implementation
//
// This fucntion simulates page replacement by replacing the element in memory
// that will not be used for longest in the future. This relies on future knowlege
// which is not something that can be used in practice often.
//
// Return Value
// ------------
// int                        integer representing the number of page faults
//
// Value Parameters
// ----------------
// pages		integer array		The Pages
// pagenum	int	 The amount of pages the algorithm will be simulating
//
//*******************************************************************


//
int predictFuture(vector<int> pages, vector<int>& frame, int amountOfPages, int pos)
{
    int result = -1;
    int farthestFromCurrentElement = pos;
    for (int i = 0; i < frame.size(); i++) {
        int j;
        for (j = pos; j < amountOfPages; j++) {
            if (frame[i] == pages[j]) {
                if (j > farthestFromCurrentElement) {
                    farthestFromCurrentElement = j;
                    result = i;
                }
                break;
            }
        }
        if (j == amountOfPages)
            return i;
    }
    if(result == -1)
    {
        return 0;
    }else {
        return result;
    }
 
}

int optPageFaults(vector<int> pages, int pageNum) 
{ 
    int page_faults = 0; 
    
    // Queue to represent our pages
    vector<int> frame;

    // Itterate through each page in our reference string
    for (int i = 0; i < pages.size(); i++)
    {
        // If we have not reached the max page size, push the page into memory and increment faults
        if(pageNum > frame.size())
        {
            frame.push_back(pages[i]);
            page_faults++;
            
        } else {
            // Once capacity has been met, check to see if the page is already in memory
            if (find(frame.begin(), frame.end(), pages[i]) == frame.end())
            {
              int opt = predictFuture(pages, frame, pages.size(), i - 1);
              frame[opt] = pages[i];
              page_faults++;
            }else {
                // The page exists in memory and we have no page fault, the program does nothing
            }
        }   
    }
    return page_faults;  
} 


//********************************************************************
//
// Random Page Replacement Algorithm Implementation
//
// The random page replacment strategy is a simple and low overhead
// solution for page replacement. Each time a page needs to be replaced, 
// all pages currently in memory have an equal chance at being selected 
// for replacment. While this approach is fast, it is also very hit or 
// miss and is rarely used.
//
// Return Value
// ------------
// int                        integer representing the number of page faults
//
// Value Parameters
// ----------------
// pages		integer array		The Pages
// pagenum	int	 The amount of pages the algorithm will be simulating
//
//*******************************************************************

int randPageFaults(vector<int> pages, int pageNum) 
{ 
    int page_faults = 0; 

    // Queue to represent our pages
    deque<int> frame;

    // Itterate through each page in our reference string
    for (int i = 0; i < pages.size(); i++)
    {
    
        // If we have not reached the max page size, push the page into memory and increment faults
        if(pageNum > frame.size())
        {
            frame.push_front(pages[i]);
            page_faults++;
            
        } else {
            // Once capacity has been met, check to see if the page is already in memory
            // and if not we remove random element and push the new on in its place
            if (find(frame.begin(), frame.end(), pages[i]) == frame.end())
            {
                page_faults++;
                int randNum = rand() % pageNum;
                frame.erase(frame.begin() + randNum);
                frame.insert(frame.begin() + randNum, pages[i]);
            }else {
                // The page exists in memory and we have no page fault, the program does nothing
            }
        }   
    }
    return page_faults;  
} 



int main ()
{
    string pageRefString;
    vector <int> pageRef;
    string stringNumOfFrames;
    string fileName;
    fstream inputFile;

    // Prompts user to enter the name of a data file
    cout << "Please enter the name of the file you wish to be sorted. You do not need to enter the extension." << "\n";
    cin >> fileName;
    cout << endl;
    fileName = fileName + ".txt";
    
    cout << "Attempting to open " << fileName << "..." << endl;
    // Open file for reading 
    inputFile.open(fileName,ios::in); 
    if (inputFile.is_open())
    {   
        // Read in the page reference string
        getline (inputFile, pageRefString);

        // Convert the read in string as a vector of integers
        stringstream iss(pageRefString);
        int ref;
        
        while (iss >> ref)
        {
            pageRef.push_back(ref);
        }

        cout << "Page Reference String: " << endl;
        cout << pageRefString << endl;

        // Read in the number of Frames
        getline (inputFile, stringNumOfFrames);

        stringstream numOfFrames(stringNumOfFrames); 

        int frameCount; 
        numOfFrames >> frameCount;

        // Used for seeding our random number generator for the random 
        // page replacement strategy
        srand(time(NULL)); 

        cout << "Number Of Frames: " << frameCount << endl;

        cout << "FIFO: " << fifoPageFaults(pageRef, frameCount) << endl;
        cout << "LRU: " << lruPageFaults(pageRef, frameCount) << endl;
        cout << "OPT: " << optPageFaults(pageRef, frameCount) << endl;
        cout << "RAND: " << randPageFaults(pageRef, frameCount) << endl;

        inputFile.close();
    }else{
      cout << "Failed to open specified data file, please check your file name and try again!" << endl;
    }
    
}


