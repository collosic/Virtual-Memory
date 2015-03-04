// memory.cpp

#include <memory.h>
     
void PhysicalMemory::readFromMem(int s, int p, int w) {
    
    s = PM[s];
    if (s == -1) {
        throw std::string("pf");
    }
    
    if (s == 0) {
        throw std::string("err");
    }
    
    p = PM[s + p];
    if (p == -1) {
        throw std::string("pf");
    }
    
    if (p == 0) {
        throw std::string("err");
    }

    throw std::to_string(p + w);
}

void PhysicalMemory::writeToMem(int s, int p, int w) {
    s = PM[s];
    if (s == -1) {
        throw std::string("pf");
    }

    if (s == 0) {
        s = allocateNewFrames(NUM_FRAMES_PER_PT);
        s = PM[s];
    }
    
    p = PM[s + p];
    if (p == -1) {
        throw std::string("pf");
    }

    if (p == 0) {
        p = allocateNewFrames(NUM_FRAMES_PER_DATA);
        p = PM[s + p];
    }

    throw std::to_string(p + w);
}


int PhysicalMemory::allocateNewFrames(int num_frames) {
    // find x number of free frames, x depends on the num_frames and must be consecutive
    int count = 0;
    for (std::size_t i = 0; i < b->size(); i++) {
        if (b->test(i)) {
            count++;
        } else {
            count = 0;
        }

        // test and see if we can return an address location to the caller
        if (count == num_frames) {
            if (num_frames == NUM_FRAMES_PER_PT) {
                b->set(i - 1);
                b->set(i);
                return (i - 1) * NUM_WORDS_PER_FRAME;
            } 

            b->set(i);
            return i;        
        }
    }   
    // this means no more free space
    return 0;
}



