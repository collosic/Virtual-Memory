// driver.cpp

#include "driver.h" 



void Driver::initSegementTable(std::string input) {
    // we need to extract pairs and enter the pairs into the segement table
    vecpairs seg_pairs;
    extractPairs(input, &seg_pairs);

    // now iterate through all the pairs entering them into the segement table
    for (std::tuple<int, int> t : seg_pairs) {
        int s = std::get<0>(t);
        int f = std::get<1>(t);

        // now enter the information into the segement table
        PM.enterIntoSegementTable(s, f);
        bitmap.set(f / NUM_WORDS_PER_FRAME, 1);
    }
}

void Driver::initPageTable(std::string input) {
    vectriples v_triples;
    extractTriples(input, &v_triples);

    for(t_triples t : v_triples) {
        int p = std::get<0>(t);
        int s = std::get<1>(t);
        int f = std::get<2>(t);
        PM.enterIntoPageTable(p, s, f);
        
        // update the bitmap to reflect the newly created frame
        bitmap.set(f / NUM_WORDS_PER_FRAME, 1);
    }    
}


std::string Driver::processVirtualAddresses(std::string input, bool tlb_test) {
    std::string response;
    vecpairs v_pairs;
    t_triples trips;
    extractPairs(input, &v_pairs);

    // run the VA agains the PM and returna response
    for(t_pairs t : v_pairs) {
        int type = std::get<0>(t);
        int VA = std::get<1>(t);
        
        // extract s, p, w from the Virtual Address given 
        try {
            if (tlb_test) {
                tryTLBAccess(type, VA);   
            }
        } catch  (std::string& e) {
            response += e + " ";
            continue;
        } 
        
        extractVirtualAddress(VA, &trips);        
        
        try { 
            enterIntoPA(type, &trips);
        } catch (std::string& e) {
            response += e + " ";
        }
    }
    return response;   
}

void Driver::extractPairs(std::string input, vecpairs *v_pairs) {
    // extract two numbers at a time and place them in the tuples
    std::vector<std::string> _tuple;
    std::stringstream ss(input);
    std::string buf;
    while (ss >> buf) {
        _tuple.push_back(buf);

        // run a test to see if the temporary vector is size 2 
        if (_tuple.size() == 2) {
            v_pairs->push_back(std::make_tuple(std::stoi(_tuple.at(0)), std::stoi(_tuple.at(1))));
            _tuple.clear();
        }
    }
}

void Driver::extractTriples(std::string input, vectriples *v_triples) {
    // extract two numbers at a time and place them in the tuples
    std::vector<std::string> _tuple;
    std::stringstream ss(input);
    std::string buf;
    while (ss >> buf) {
        _tuple.push_back(buf);

        // run a test to see if the temporary vector is size 2 
        if (_tuple.size() == 3) {
            v_triples->push_back(std::make_tuple(std::stoi(_tuple.at(0)), 
                                        std::stoi(_tuple.at(1)), std::stoi(_tuple.at(2))));
            _tuple.clear();
        }
    }
}


void Driver::extractVirtualAddress(int VA, t_triples *trips) {
    // generate a new bitset with 28 bits and begin extracting
    std::bitset<VA_SIZE> virtual_address(VA);
    std::bitset<VA_SIZE> s_mask(0xFF80000);
    std::bitset<VA_SIZE> p_mask(0x007FE00);
    std::bitset<VA_SIZE> w_mask(0x00001FF);
    int s_shift = 19;
    int p_shift = 9;

    std::bitset<VA_SIZE> generate = virtual_address;

    generate &= s_mask;
    generate >>= s_shift;
    int s = (int)(generate.to_ulong());

    generate = virtual_address;
    generate &= p_mask;
    generate >>= p_shift;
    int p  = (int)(generate.to_ulong());

    generate = virtual_address;
    generate &= w_mask;
    int w = (int)(generate.to_ulong());

    *trips = std::make_tuple(s, p, w);
}

void Driver::extractForTLB(int VA, t_pairs *pair) {
    std::bitset<VA_SIZE> virtual_address(VA);
    std::bitset<VA_SIZE> sp_mask(0xFFFFE00);
    std::bitset<VA_SIZE> w_mask(0x00001FF);
    int sp_shift = 9;

    std::bitset<VA_SIZE> generate = virtual_address;

    generate &= sp_mask;
    generate >>= sp_shift;
    int sp = (int)(generate.to_ulong());

    generate = virtual_address;
    generate &= w_mask;
    int w = (int)(generate.to_ulong());

   *pair = std::make_tuple(sp, w);    
}

void Driver::enterIntoPA(int type, t_triples *trips) {
    int s = std::get<0>(*trips);
    int p = std::get<1>(*trips);
    int w = std::get<2>(*trips);
    
    if (type == 0) {
        PM.readFromMem(s, p, w);
    } else {
        PM.writeToMem(s, p , w);
    }
}


void Driver::tryTLBAccess(int type, int VA) {
    // need to extract sp and w from the VA 
    t_pairs pair;
    extractForTLB(VA, &pair);

    int sp = std::get<0>(pair);
    int w = std::get<1>(pair);
    int f = -1;

    int current_tlb_size = TLB.size();
   
    // search the tlb for sp match
    for(std::pair<int, int> p : TLB) {
        if (sp == std::get<0>(p)) {
            f = std::get<1>(p);            
            // we now need to move this to the head of the list
            std::pair<int, int> _p = p;
            TLB.remove(p);
            TLB.push_front(_p);
        }
    } 
    
    // check and see if we had a hit on the TLB 
    if (f != -1) {
        throw std::string("h ") + std::to_string(f + w); 
    }

    // if we get here it mean we got a miss and we must search the PM
    t_triples trips;
    extractVirtualAddress(VA, &trips);

    try {
        enterIntoPA(type, &trips);    
    } catch (std::string e) {
        if (e == "pf" || e == "err") {
            throw e;
        }

        // here we must place the new frame and sp into the TLB
        f = std::stoi(e);
        if (current_tlb_size == 4) {
            TLB.pop_back();
        }  
        TLB.push_front(std::make_pair(sp, f));
        throw e;
    }
}
