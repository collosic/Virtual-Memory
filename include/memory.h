#ifndef memory
#define memory 

#include <bitset>

// Constants
const int NUM_FRAMES = 1024;
const int NUM_WORDS_PER_FRAME = 512;
const int SIZE_OF_PA = NUM_FRAMES * NUM_WORDS_PER_FRAME;

// Type Definition


// Classes
class PhysicalMemory {
  private:
    int PM[SIZE_OF_PA] = {0};    

  public:
    PhysicalMemory () {};
    ~PhysicalMemory () {};

    // Class Functions
    void enterIntoSegementTable(int s, int f) { PM[s] = f; };
    void enterIntoPageTable(int p, int s, int f) { PM[PM[s] + p] = f; };
    void enterIntoVM(int type, int VA);
};

#endif
