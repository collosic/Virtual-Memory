#ifndef DRIVER_H
#define DRIVER_H   

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <bitset>

// Header Files
#include "memory.h"

// Constants
const int pairs = 2;
const int triples = 3;
const int VA_SIZE = 28;

// Typedef
typedef std::tuple<int, int> t_pairs;
typedef std::tuple<int, int, int> t_triples;
typedef std::vector<t_pairs> vecpairs;
typedef std::vector<t_triples> vectriples;

class Driver {
  private:
    PhysicalMemory PM;
    std::bitset<NUM_FRAMES> bitmap;
    

    // Data Structures required for the driver
    

  public:
    // Constructors
    Driver() : bitmap(1) {};
    ~Driver() {};

    // Class Methods
    void initSegementTable(std::string input);
    void initPageTable(std::string input); 
    std::string processVirtualAddresses(std::string input, bool tlb_test);

    void extractPairs(std::string input, vecpairs *v_pairs);
    void extractTriples(std::string input, vectriples *v_triples);
    void extractVirtualAddress(int VA, t_triples *trips);
};
#endif
