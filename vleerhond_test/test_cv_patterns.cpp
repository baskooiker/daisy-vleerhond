// #include <iostream>

#include "patterns/cv_patterns.h"

int main(int argc, char* argv[])
{
    // std::cout << "Hello fuggup..." << std::endl;

    using namespace Vleerhond;
    MorphingCvPattern pattern;
    
    pattern.setLength(8);
    // std::cout << pattern.toString() << std::endl;
    
    for (int step = 0; step < 10; step++)
    {
        // std::cout << pattern.getTarget().abPattern.toString() << std::endl;
        for (int i = 0; i < 1000; i++)
        {
            // std::cout << "Morph step " << i << std::endl;
            pattern.morph();
            // std::cout << pattern.toString() << std::endl;

            if (pattern.isMorphed())
            {
                // std::cout << "Morphed after " << i << " steps\n";
                pattern.randomizeTarget();
                break;
            }
        }
    }

    return 1;
}
