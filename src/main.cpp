#include "driver.h"

void withoutTLB(std::string input1, std::string input2);
void withTLB(std::string input1, std::string input2);

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "too few arguments, use -h for help" << std::endl;
    } else if (argc > 4) {
        std::cout << "too many arguments, use -h for help" << std::endl;
    } else {
        if (argc == 4) {
            std::string input1(argv[1]);
            if (input1 != "-t") 
                std::cout << "invalid flag" << std::endl;

            std::string input2(argv[2]);
            std::string input3(argv[3]);

            // call the appropriate function
            withoutTLB(input2, input3);
        } else {
            std::string input1(argv[1]);
            std::string input2(argv[2]);
            withoutTLB(input1, input2);
        }    
    }
    return 0;
}

void withoutTLB(std::string input1, std::string input2) {
    std::string raw_input, buf, o;
    std::ifstream proc_pm(input1);
    std::ifstream proc_va(input2);
    std::ofstream out("112335291.txt");
    Driver *driver = new Driver();
    
    if(proc_pm.is_open()) {
        std::cout << "fuck" << std::endl;
    }
    // We know that input1.txt will only be two lines long and so we can process each one separately
    std::getline(proc_pm, raw_input);
    driver->initSegementTable(raw_input);

    std::getline(proc_pm, raw_input);
    driver->initPageTable(raw_input);

    std::getline(proc_va, raw_input);
    out << driver->processVirtualAddresses(raw_input, false);
    
    // close all the files that were opened.
    proc_pm.close();
    proc_va.close();
    out.close();
}






























