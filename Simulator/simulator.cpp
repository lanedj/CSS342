
//----------------------------
/**@File main.cpp
 * @Author Derek Lane 0134101
 * @date 3 Mar 2013
 *
 * runs my 3D sphere emulation.  pretty ugly, but at least valgrind says I don't
 * have any leaks.
 * 
 * I chose to play around with using single list, probably should have spent
 * my time on other things.
 * 
 */
# include <iostream>
# include <fstream>
# include <sstream>
# include <cmath>
# include "list.h"
# include "sphere.h"
# include "simulator.h"

/**
 * Default constructor
 * @post Simulator class created with an empty sphereList, currentTime = 0
 *       and firstDead = 0
 */
 Simulator::Simulator()
 {
     this->currentTime = 0;
     this->firstDead = 0;
 }
    
/**
 * Populates the simulator with blackhole and spheres from provided datafile
 * @pre param string is name of properly formatted file in correct location
 * @post this.theList populated with black hole and spheres
 * @return false if failed to open file or if it was empty
 */
bool Simulator::getData (std::string inFile)
{
    int index = 1;
    bool retVal = true;
    Sphere tempSphere; //to pass data from file into list of Spheres
    
    //temp variables for loading spheres
    std::string sphereColor;//enum type def'd in appconsts.h
    double radius;	// sphere radius
    double mass; //sphere mass (equals radius unless black hole
    double x, y, z; //spatial coords
    double dx, dy, dz; //velocities in secs
    
    //open the file for reading
    std::ifstream theFile(inFile.c_str());
    
    //setup the blackhole
    theFile >> x >> y >> z >> mass;
    tempSphere.setSphereNumber(index);
    tempSphere.setMass(mass);
    tempSphere.setCoords(x, y, z);
    this->sphereList.insert(index, tempSphere); //calls insert

   
    //setup the rest of the spheres
        while (theFile.good())
    {
        index++;
        theFile >> sphereColor
                >> x >> y >> z 
                >> radius 
                >> dx >> dy >> dz;
        if (theFile.fail()) // bad input or other error, does not read eof bit
            {
            firstDead = sphereList.getLength()+1;
            theFile.close();
            return retVal = false; // or return/throw exception/destroy all evidence
            } 
        tempSphere.setSphereNumber(index);
        tempSphere.setSphereColor(sphereColor);
        tempSphere.setMass(radius);
        tempSphere.setRadius(radius);
        tempSphere.setCoords(x, y, z);
        tempSphere.setVels(dx, dy, dz);
        this->sphereList.insert(index, tempSphere); //calls insert
    }
    firstDead = sphereList.getLength()+1;
    theFile.close();
    return retVal;
}

/**
 * prints final report of all collisions, in order (does not print blackhole)
 * @pre spheres in the list (other than black hole
 * @post output to console
 */
void Simulator::finalReport()
{
    std::cout << "Sphere Elimination Records" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Index    Color    Times (s)   Event Type"<< std::endl;
    std::cout << "-----    -----    ---------   ----------" << std::endl;
    for (int i = 2; i<= this->sphereList.getLength(); i++)
    {
        Sphere tempSphere;
        this->sphereList.getObj(i, tempSphere);
        std::cout << "  " <<  tempSphere.getSphereNumber()-1
                  << "\t " <<  tempSphere.getSphereColor()
                  << "\t      " << (int) tempSphere.getTimeOfDeath()
                  << " \t" << tempSphere.getDemise()
                  << std::endl;
    }
    std::cout << "***end of run" << std::endl;
       
}

/**
 * Updates the coordinates of all active spheres in the list with the
 * based on their current velocities dx, dy, dz
 * @pre active spheres in the list
 * @post dx, dy, dz of all active spheres updated
 * @return false if no active spheres otherwise true
 */
bool Simulator::updateCoords()
{
    double tempX, tempY, tempZ;
    int limit;
    bool retVal = true;
    
    //check for empty list or all spheres dead
    if ((sphereList.getLength() < 1)||(this->firstDead == 2))
    {
        return retVal = false;
    }
    
    //if there's a dead sphere, set that as limit, if no dead sphere,
    //length is limit
    limit = this->getForLimit();

    for (int i = 2; i < limit; i++) //skip the blackhole
    {
        Sphere tempSphere;
        //get the sphere from index[i]
        sphereList.getObj(i, tempSphere);
        tempX = tempSphere.getX();
        tempY = tempSphere.getY();
        tempZ = tempSphere.getZ();
        

        //update x,y,z based on velocity
        tempX = tempX +(tick*tempSphere.getdX());
        tempY = tempY +(tick*tempSphere.getdY());
        tempZ = tempZ +(tick*tempSphere.getdZ());
        //update based on accelerations
        tempX = tempX +(tempSphere.getaX()*tick*tick)/2;
        tempY = tempY +(tempSphere.getaY()*tick*tick)/2;
        tempZ = tempZ +(tempSphere.getaZ()*tick*tick)/2;
        tempSphere.setCoords(tempX, tempY, tempZ);
        
        //copy the tempsphere over the orginal sphere at index[i]
        sphereList.setObj(i, tempSphere);
    }
    return retVal;
}

/**
 * Updates the accelerations of all active spheres in the list with the
 * aggregate accelerations based on all other bodies in the space
 * @pre active spheres in the list
 * @post ax, ay, az of all active spheres updated
 * @return false if no active spheres otherwise true
 */
bool Simulator::updateAccels ()
{
    //x,y,z coords for finding distances
    double baseX=0, baseY=0, baseZ=0;
    double compX=0, compY=0, compZ=0;
    
    //for holding acceleration components    
    double baseaX=0, baseaY=0, baseaZ=0;
    double compaX=0, compaY=0, compaZ=0;
    
    //for holding distances in each direction
    double distX=0, distY=0, distZ=0;
    double threeDDist=0;//for the 3D distance
    
    //to hold absolute value of distances
    double absDistX=0, absDistY=0, absDistZ=0;
    
    //denominator for the acceleratiosn equation
    double denom;
    
    //masses of the two spheres
    double baseMass=0, compMass=0;

    int limit = this->getForLimit(); //choose between length and firstDead
    
    for (int i = 1; i < limit; i++)
        {
        //get the baseSphere and load its temp vars
        Sphere baseSphere;
        sphereList.getObj(i,baseSphere);
        baseX = baseSphere.getX();
        baseY = baseSphere.getY();
        baseZ = baseSphere.getZ();
        baseMass = baseSphere.getMass();
              
        for (int j = i + 1; j < limit; j++)
            {
                //get the compSphere and load its temp vars
                Sphere compSphere;
                sphereList.getObj(j,compSphere);
                compX = compSphere.getX();
                compY = compSphere.getY();
                compZ = compSphere.getZ();
                compMass = compSphere.getMass();
                                   
                if (baseSphere.getSphereColor()!="black")
                {
                    distX = compX - baseX;
                    distY = compY - baseY;
                    distZ = compZ - baseZ;

                    threeDDist = sqrt((distX*distX)+(distY*distY)+(distZ*distZ));
                    denom = pow(threeDDist,3);
                    
                    if (denom>0)
                    {
                        baseaX = baseSphere.getaX() + (distX*G*compMass/denom);
                        baseaY = baseSphere.getaY() + (distY*G*compMass/denom);
                        baseaZ = baseSphere.getaZ() + (distZ*G*compMass/denom);
                    }
                    baseSphere.setAccels(baseaX, baseaY, baseaZ);

                    //update the original baseSphere based on interaction
                    //with compSphere
                    sphereList.setObj(i,baseSphere);
                }  

                if (compSphere.getSphereColor()!="black")
                {
                    distX = baseX - compX;
                    distY = baseY - compY;
                    distZ = baseZ - compZ;
                                        
                    threeDDist = sqrt((distX*distX)+(distY*distY)+(distZ*distZ));
                    denom = pow(threeDDist,3);
                    
                    if (denom>0)
                    {
                        compaX = compSphere.getaX() + (distX*G*baseMass/denom);
                        compaY = compSphere.getaY() + (distY*G*baseMass/denom);
                        compaZ = compSphere.getaZ() + (distZ*G*baseMass/denom);
                    }
                    compSphere.setAccels(compaX, compaY, compaZ);

                    //update the original baseSphere based on interaction
                    //with compSphere
                    sphereList.setObj(j,compSphere);


                }
          
         }
    }
}
 
/**
 * check for collision between a spheres and the boundaries
 * collision occurs if distance from center to boundary is <= sphere radius
 * @pre active spheres in sphereList
 * @post spheres that hit the boundary marked as dead
 */
void Simulator::boundaryCheck()
{
    int limit;
    //if there's a dead sphere, set that as limit, if no dead sphere,
    //length is limit
    limit = this->getForLimit();
    
    for(int index = 2; index < limit; index++) //skip the blackhole
    {
    //check to see if the surface of the sphere has
    //made contact with the boundary of the space
    //first check for greater than max
    Sphere toCheck;
    sphereList.getObj(index, toCheck);

    if ((((toCheck.getX()+toCheck.getRadius()>=MAX_COORD)
            ||(toCheck.getY()+toCheck.getRadius()>=MAX_COORD)
            ||(toCheck.getZ()+toCheck.getRadius()>=MAX_COORD)))
        ||//then check for less than 0
       (((toCheck.getX()-toCheck.getRadius()<=MIN_COORD)
            ||(toCheck.getY()-toCheck.getRadius()<=MIN_COORD)
            ||(toCheck.getZ()-toCheck.getRadius()<=MIN_COORD))))     
        {    
        //set active to false
        toCheck.setActiveStatus(false);
        toCheck.setTimeOfDeath(currentTime);
        toCheck.setDemise("Boundary");
        sphereList.setObj(index,toCheck);
        }
    }
}

/**
 * check for collision between two spheres at current time
 * collision occurs if distance between centers <= sum of the two radii
 * @pre active spheres in sphere list
 * @post any spheres that lost collisions have active set false
 *       and demise type set to either collision or blackhole
 * @return true if any spheres were checked
 */
bool Simulator::collisionCheck()
{
    int limit;
    Sphere baseSphere;
    Sphere compSphere;
    //if there's a dead sphere, set that as limit, if no dead sphere,
    //length is limit
    limit = this->getForLimit();
    
    for(int index = 1; index < limit; index++)
    {
       sphereList.getObj(index, baseSphere); 
    //check to see if the surface of the sphere has
    //made contact with the boundary of the space
    //first check for greater than max
       for (int secondIndex = index + 1; secondIndex < limit; secondIndex++)
       {
           sphereList.getObj(secondIndex, compSphere);
            if ((baseSphere.distanceTo(&compSphere))<=0)
            {
                if ((baseSphere < compSphere) 
                        && (baseSphere.getSphereColor()!="black"))
                {
                    //set Active to false
                    baseSphere.setActiveStatus(false);
                    baseSphere.setTimeOfDeath(currentTime);
                    if (compSphere.getRadius()> 0)
                    {
                    //set Demise
                    baseSphere.setDemise("Collision");  
                    } else 
                    {
                    //set Demise
                    baseSphere.setDemise("Black Hole");
                    }
                    //baseSphere status and demise were changed so update
                    sphereList.setObj(index, baseSphere);
                } else if ((compSphere < baseSphere) 
                        && (compSphere.getSphereColor()!="black"))
                {
                    //set Active to false
                    compSphere.setActiveStatus(false);
                    compSphere.setTimeOfDeath(currentTime);
                    if (baseSphere.getRadius()> 0)
                    {
                    //set Demise
                    compSphere.setDemise("Collision");  
                    } else 
                    {
                    //set Demise
                    compSphere.setDemise("Black Hole");
                    }
                    //compSphere status and demise were changed so update
                    sphereList.setObj(secondIndex, compSphere);
                }
            }
       }
}
}


/**
 * moves any inactive spheres to the end of the list
 * @pre active spheres in the list
 * @post all inactive spheres to end of list
 *       this.firstDead updated to new position
 * @return false if no active spheres otherwise true
 */
bool Simulator::prune()
{
    bool retVal = true;
    int limit = 0;
    //check for empty list or all spheres dead
    if ((sphereList.getLength() < 1)||(this->firstDead == 2))
    {
        return retVal = false;
    }
    
    //if there's a dead sphere, set that as limit, if no dead sphere,
    //length is limit
    limit = this->getForLimit();

    for(int i = 2; i < limit;) //skip the black hole, and prune while there
                                   // are still active spheres
        {
        Sphere tempSphere;
        sphereList.getObj(i, tempSphere);
            if (!tempSphere.getActiveStatus())
            {
                //insert dead copy at end of list
                sphereList.insert(sphereList.getLength()+1, tempSphere);
                sphereList.remove(i); // remove original dead sphere
                firstDead--; //one less active so firstDead is one earlier
                limit = firstDead; //lower limit since less active to check
            } else
            {
                i++;
            }
        }
    return retVal;
}

/**
 * Updates the velocities of all active spheres in the list
 * @pre active spheres in the list
 * @post dx, dy, dz of all active spheres updated based on their
 *       aggregate ax, ay, az
 * @post ax, ay, az of all spheres reset to zero
 * @return false if no active spheres otherwise true
 */
bool Simulator::updateVels ()
{
    int limit = 0;
    bool retVal = true;
    Sphere tempSphere;
    //check for empty list or all spheres dead
    if ((sphereList.getLength() < 1)||(this->firstDead == 2))
    {
        return retVal = false;
    }
    
    //if there's a dead sphere, set that as limit, if no dead sphere,
    //length is limit
    limit = this->getForLimit();

    for(int i = 2; i < limit; i++) //skip the black hole, and prune while there
                                   // are still active spheres
        {
        sphereList.getObj(i,tempSphere);            
       
        if (tempSphere.getSphereColor()!="black")
            {
            double tempdX = tempSphere.getdX();
            double tempdY = tempSphere.getdY();
            double tempdZ = tempSphere.getdZ();

            tempdX = tempdX + (tempSphere.getaX() * tick);
            tempdY = tempdY + (tempSphere.getaY() * tick);
            tempdZ = tempdZ + (tempSphere.getaZ() * tick);

            tempSphere.setVels(tempdX, tempdY, tempdZ);
            tempSphere.setAccels(0,0,0);
            //changes were made to tempSphere so update it
            sphereList.setObj(i,tempSphere);
        
            }
    }
    return retVal;
}


/**
 * adds one tick to current time.
 * @post currentTime updated by one tick
 */
void Simulator::clockTick ()
{
    currentTime = currentTime + tick;
}


/**
 * compare the length of the list to the location of lastDead and return
 * the limit that will make sure all active sphere are accessed
 * @pre active spheres in the list
 * @return int to use as limit of for loop
 */
int Simulator::getForLimit()
    //if there's a dead sphere, return that as limit, if no dead sphere,
    //length+1 is limit
{
    int retVal = this->firstDead;
    return retVal;

    
}

