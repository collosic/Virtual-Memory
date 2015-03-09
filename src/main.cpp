#include "driver.h"

void runVirtualMemory(std::string input1, std::string input2, bool tlb);

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
            runVirtualMemory(input2, input3, true);
        } else {
            std::string input1(argv[1]);
            std::string input2(argv[2]);
            runVirtualMemory(input1, input2, false);
        }    
    }
    return 0;
}

void runVirtualMemory(std::string input1, std::string input2, bool tlb) {
    std::string raw_input, buf, o;
    std::ifstream proc_pm(input1);
    std::ifstream proc_va(input2);
    std::string output_name = tlb ? "11233529_2.txt" : "11233529_1.txt";
    std::ofstream out(output_name);
    Driver *driver = new Driver();
    
    // We know that input1.txt will only be two lines long and so we can process each one separately
    std::getline(proc_pm, raw_input);
    driver->initSegementTable(raw_input);

    std::getline(proc_pm, raw_input);
    driver->initPageTable(raw_input);

    std::getline(proc_va, raw_input);
    out << driver->processVirtualAddresses(raw_input, tlb);
    
    // close all the files that were opened.
    delete driver;
    proc_pm.close();
    proc_va.close();
    out.close();
}






























