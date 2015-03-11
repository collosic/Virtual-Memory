#ifndef DRIVER_H
#define DRIVER_H   

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <bitset>
#include <list>

// Header Files
#include "memory.h"

// Constants
const int PAIR = 2;
const int TRIPLE = 3;
const int VA_SIZE = 28;

// Typedef
typedef std::pair<int, int> pairs;
typedef std::tuple<int, int, int> triples;
typedef std::vector<pairs> vecpairs;
typedef std::vector<triples> vectriples;
typedef std::list<pairs> lru;

class Driver {
  private:
    std::bitset<NUM_FRAMES> bitmap;
    PhysicalMemory PM;
    lru TLB; 

  public:
    // Constructors
    Driver() : bitmap(1), PM(&bitmap) {};
    ~Driver() {};

    // Class Methods
    void initSegementTable(std::string input);
    void initPageTable(std::string input); 
    std::string processVirtualAddresses(std::string input, bool tlb_test);

    void extractPairs(std::string input, vecpairs *v_pairs);
    void extractTriples(std::string input, vectriples *v_triples);
    void extractVirtualAddress(int VA, triples *trips);
    void extractForTLB(int VA, t_pairs *pair);
    void enterIntoPA(int type, triples *trips);
    void tryTLBAccess(int type, int VA);
};
#endif
