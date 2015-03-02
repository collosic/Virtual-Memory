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


std::string Driver::processVirtualAddresses(std::string input, bool tld_test) {
    std::string response;
    vecpairs v_pairs;
    t_triples trips;
    extractPairs(input, &v_pairs);

    // run the VA agains the PM and returna response
    for(t_pairs t : v_pairs) {
        int type = std::get<0>(t);
        int VA = std::get<1>(t);
        
        // extract s, p, w from the Virtual Address given 
        extractVirtualAddress(VA, &trips);        

        try {
            //PM.enterIntoVM(type, VA);
        } catch (std::string e) {
            response += e;
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
    int s_shift = 19;
    int p_shift = 9;
    int s;
    int p;
    int w;

    std::bitset<VA_SIZE> virtual_address(VA);
    std::bitset<VA_SIZE> s_mask(0xFF80000);
    std::bitset<VA_SIZE> p_mask(0x007FE00);
    std::bitset<VA_SIZE> f_mask(0x00001FF);

    std::bitset<VA_SIZE> generate = virtual_address;

    generate &= s_mask;
    generate >>= s_shift;
    s = (int)(generate.to_ulong());

    generate = virtual_address;
    generate &= p_mask;
    generate >>= p_shift;
    p  = (int)(generate.to_ulong());

    generate = virtual_address;
    generate &= f_mask;
    w = (int)(generate.to_ulong());

    *trips = std::make_tuple(s, p, w);
}
