#include <openipLL/Transform2Generator.h>

namespace openip
{
    int getTransformNameAndParameters(std::string& input, std::string& name, vector<float>& parameters)
    {
        float p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;

        int f= input.find_first_of(' ');
	
	if ( f == -1 || f == int(input.size()) )
	{
	  name= string("");
	  parameters.resize(0);
	  return 0;
	}

        name= input.substr(0,f);

        parameters.clear();
	
	//printf(".%d", input.size()); fflush(stdout);
        int number= sscanf(input.c_str() + f, "%f%f%f%f%f%f%f%f%f%f%f%f%f%f", &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10, &p11, &p12, &p13, &p14);
	//printf(";%d", number); fflush(stdout);

        if ( number >= 1 )
            parameters.push_back(p1);
        if ( number >= 2 )
            parameters.push_back(p2);
        if ( number >= 3 )
            parameters.push_back(p3);
        if ( number >= 4 )
            parameters.push_back(p4);
        if ( number >= 5 )
            parameters.push_back(p5);
        if ( number >= 6 )
            parameters.push_back(p6);
        if ( number >= 7 )
            parameters.push_back(p7);
        if ( number >= 8 )
            parameters.push_back(p8);
        if ( number >= 9 )
            parameters.push_back(p9);
        if ( number >= 10 )
            parameters.push_back(p10);
        if ( number >= 11 )
            parameters.push_back(p11);
        if ( number >= 12 )
            parameters.push_back(p12);
        if ( number >= 13 )
            parameters.push_back(p13);
        if ( number >= 14 )
            parameters.push_back(p14);

        return 0;
    }
}
