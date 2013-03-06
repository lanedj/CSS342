
//----------------------------
/**@File main.cpp
 * @Author Derek Lane 0134101
 * @date 3 Mar 2013
 *
 * runs my 3D sphere emulation.  pretty ugly, but at least valgrind says I don't
 * have any leaks.
 * 
 */

# include <iostream>
# include <fstream>
# include <sstream>
# include <cmath>
# include "list.h"
# include "sphere.h"
# include "simulator.h"

int main ()
{
//instantiate the simulator 
    Simulator theSimulator;
    
//load the simulator      
    theSimulator.getData("sphere.txt");
//run the simulation
    double time = 0;
    double tick = .01;
    while (time < 1)
    {
        theSimulator.updateCoords();
        theSimulator.boundaryCheck();
        theSimulator.collisionCheck();
        theSimulator.prune();
        theSimulator.updateAccels();
        theSimulator.updateVels();
        theSimulator.clockTick();
        time = time + tick;
    }
        while (time < 6)
    {
        theSimulator.updateCoords();
        theSimulator.boundaryCheck();
        theSimulator.collisionCheck();
        theSimulator.prune();
        theSimulator.updateAccels();
        theSimulator.updateVels();
        theSimulator.clockTick();
        time = time + tick;
    }
        while (time < 17)
    {
        theSimulator.updateCoords();
        theSimulator.boundaryCheck();
        theSimulator.collisionCheck();
        theSimulator.prune();
        theSimulator.updateAccels();
        theSimulator.updateVels();
        theSimulator.clockTick();
        time = time + tick;
    }
           while (theSimulator.updateCoords())
    {
        theSimulator.boundaryCheck();
        theSimulator.collisionCheck();
        theSimulator.prune();
        theSimulator.updateAccels();
        theSimulator.updateVels();
        theSimulator.clockTick();
    }
//print the report  
    theSimulator.finalReport();
    
}



