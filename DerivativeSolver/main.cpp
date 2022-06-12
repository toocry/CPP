
#include "Source.h"

int main()
{
	std::string line;

	while (getline(std::cin, line)) 
    {
        output(line);
	}
    
	return 0;
}