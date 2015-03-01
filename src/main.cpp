#include "driver.h"
void runDriver(bool hasArgument, std::string commands);

int main(int argc, char* argv[])
{
    if (argc == 2) {
        std::string incoming(argv[1]);
        runDriver(true, incoming);
    } else if (argc == 1) {
        std::string incoming;
        runDriver(false, incoming);
    } else {
        std::cerr << "too many arguments used" << std::endl;
        return -1;
    }
    return 0;
}


void runDriver(bool hasArgument, std::string commands) {
    std::string rawInput, buf, o;
    Driver *driver = new Driver();
    vecstr in;

    if (hasArgument) { 
        std::ofstream out("11233529.txt");
        std::ifstream incoming(commands);
        o += "init ";
        if (!incoming && !out) 
            std::cout << "error opening argument file, resuming default behaviour" << std::endl;
        else {
            while (std::getline(incoming, rawInput)) {
                const char *c = rawInput.c_str();
                if (rawInput == "\n" || rawInput == "\r\n" || 
                        rawInput == "" || rawInput == "\r" || c[0] == '#') { 
                    continue;
                }
                std::stringstream ss(rawInput);
                while (ss >> buf) { 
                    in.push_back(buf);
                }
                o += driver->interface(&in) + " ";
            }
            out << o;
            std::cout << o << std::endl;
            incoming.close();
            out.close();
        }
    }

   /*
   while(true) {
        std::getline(std::cin, rawInput);
        std::stringstream ss(rawInput);
        while (ss >> buf) 
               in.push_back(buf);
        std::cout << driver->interface(&in) << std::endl;
        if(driver->getQuit()) {
            break;
        }
   }*/

   delete driver;

}

















































