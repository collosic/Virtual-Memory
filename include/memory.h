#ifndef memory
#define memory 

#include <bitset>
#include <tuple>

// Constants
const int NUM_FRAMES = 1024;
const int NUM_WORDS_PER_FRAME = 512;
const int SIZE_OF_PA = NUM_FRAMES * NUM_WORDS_PER_FRAME;
const int NUM_FRAMES_PER_PT = 2;
const int NUM_FRAMES_PER_DATA = 1;

// Type Definition
typedef std::tuple<int, int> t_pairs;
typedef std::tuple<int, int, int> t_triples;

// Classes
class PhysicalMemory {
  private:
    int PM[SIZE_OF_PA];    
    std::bitset<NUM_FRAMES> *b;

  public:
    PhysicalMemory(std::bitset<NUM_FRAMES> *bitmap) : PM{0}, b(bitmap) {};
    ~PhysicalMemory () {};

    // Class Functions
    void enterIntoSegementTable(int s, int f) { PM[s] = f; };
    void enterIntoPageTable(int p, int s, int f) { PM[PM[s] + p] = f; };
    void readFromMem(int s, int p, int w);
    void writeToMem(int s, int p, int w);

    // Helper functions
    int allocateNewFrames(int num_frames);
};

#endif
