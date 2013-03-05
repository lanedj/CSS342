
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

//retrieves data from the file, places it in sphere, updates the int to length
//of the list
void getData (std::string, List<Sphere> &);

//prints out the list with formatting
void finalReport (List<Sphere> &);

//updates the x, y, z coordinates based on velocity
//first int is index of firstlive sphere
//second int is index of first dead sphere
//double is tick
void updateCoords (List<Sphere> &, int, const double);

//updates each acceleration
//int is first dead sphere
//first double is tick
//second double is gravity
//const double is current time
void updateAccels (List<Sphere> &, int, double, double, const double);

//checks sphere to see if it has impacted boundary, returns true and sets
//active status to boundary if impact occurs
bool boundaryCheck (Sphere &, const double);

//checks for collsion between spheres
//double is current time
bool collisionCheck (Sphere &, Sphere &, double);

//checks each sphere, if it is inactive moves to end of list, if active
//and not black, updates velocity based on new accelerations
void sweep (List<Sphere> &, int &, double);

    int MAX_COORD = 1000;
    int MIN_COORD = 0;

int main ()
{
    List<Sphere> sphereList;//create the list
    double currentTime = 0;
    double tick = .01;
    double G = 10;
    int firstDead = 0;
    std::string sourceFile = "sphere.txt";

    getData(sourceFile, sphereList);
    firstDead = sphereList.getLength() + 1;
    
    while (firstDead > 2)   
    { 
        updateCoords (sphereList, firstDead-1, tick);
        currentTime = currentTime + tick;
        updateAccels (sphereList, firstDead, tick, G, currentTime);
        sweep(sphereList, firstDead, tick);

     }

    finalReport(sphereList);
    
    
}

void getData (std::string inFile, List<Sphere> &theList)
{
    int length = 1;
    
    Sphere tempSphere; //to pass data from file into list of Spheres
    
    //temp variables for loading spheres
    int sphereNumber; //identifier for sphere
    std::string sphereColor;//enum type def'd in appconsts.h
    double radius;	// sphere radius
    double mass; //sphere mass (equals radius unless black hole
    double x, y, z; //spatial coords
    double dx, dy, dz; //velocities in secs
    
    //open the file for reading
    std::ifstream theFile(inFile.c_str());
    
    //setup the blackhole
    theFile >> x >> y >> z >> mass;
    tempSphere.setSphereNumber(length);
    tempSphere.setMass(mass);
    tempSphere.setCoords(x, y, z);
    theList.insert(length, tempSphere); //calls insert

   
    //setup the rest of the spheres
        while (theFile.good())
    {
        length++;
        theFile >> sphereColor
                >> x >> y >> z 
                >> radius 
                >> dx >> dy >> dz;
        if (theFile.fail()) // bad input or other error, does not read eof bit
        {
        break; // or return/throw exception/destroy all evidence
        } 
        tempSphere.setSphereNumber(length);
        tempSphere.setSphereColor(sphereColor);
        tempSphere.setMass(radius);
        tempSphere.setRadius(radius);
        tempSphere.setCoords(x, y, z);
        tempSphere.setVels(dx, dy, dz);
        theList.insert(length, tempSphere); //calls insert
    }
    theFile.close();
}
void finalReport(List<Sphere> &theList)
{
    std::cout << "Sphere Elimination Records" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "Index    Color    Times (s)   Event Type"<< std::endl;
    std::cout << "-----    -----    ---------   ----------" << std::endl;
    for (int i = 2; i<= theList.getLength(); i++)
    {
        Sphere tempSphere;
        theList.retrieve(i,tempSphere);
        std::cout << "  " <<  tempSphere.getSphereNumber()-1
                  << "\t " <<  tempSphere.getSphereColor()
                  << "\t     " << tempSphere.getDemise()
                  << std::endl;
    }
    std::cout << "***end of run" << std::endl;
    
}

void updateCoords(List<Sphere> &theList, int limit, double tick)
{
    double tempX, tempY, tempZ;
    for (int i = 2; i <= limit; i++)
    {
        Sphere *tempSphere = theList.access(i);
        tempX = tempSphere->getX();
        tempY = tempSphere->getY();
        tempZ = tempSphere->getZ();
        tempX = (tempX +(tick*tempSphere->getdX()))+(.5*(tempSphere->getaX())*tick*tick);
        tempY = (tempY +(tick*tempSphere->getdY()))+(.5*(tempSphere->getaY())*tick*tick);
        tempZ = (tempZ +(tick*tempSphere->getdZ()))+(.5*(tempSphere->getaZ())*tick*tick);
        tempSphere->setCoords(tempX, tempY, tempZ);

    }
}
void updateAccels (List<Sphere> &theList, int limit, 
                   double tick, double gravity, const double currentTime)
{
        double baseX=0, baseY=0, baseZ=0;
        double compX=0, compY=0, compZ=0;    
        double baseaX=0, baseaY=0, baseaZ=0;
        double compaX=0, compaY=0, compaZ=0;
        double distX=0, distY=0, distZ=0;
        double absDistX=0, absDistY=0, absDistZ=0;
        double baseMass=0, compMass=0;
    for (int i = 1; i < limit; i++)
        {
        Sphere *baseSphere = theList.access(i);
        baseX = baseSphere->getX();
        baseY = baseSphere->getY();
        baseZ = baseSphere->getZ();
        baseMass = baseSphere->getMass();
              
        for (int j = i + 1; j < limit; j++)
            {
            Sphere *compSphere = theList.access(j);
            compX = compSphere->getX();
            compY = compSphere->getY();
            compZ = compSphere->getZ();
            compMass = compSphere->getMass();
                        
            collisionCheck(*baseSphere, *compSphere, currentTime);
            
            if (baseSphere->getActiveStatus())
            {
                boundaryCheck(*baseSphere, currentTime);
                
                if (baseSphere->getSphereColor()!="black")
                {
                distX = compX - baseX;
                distY = compY - baseY;
                distZ = compZ - baseZ;
                absDistX = sqrt(distX*distX);
                absDistY = sqrt(distY*distY);
                absDistZ = sqrt(distZ*distZ);
                double denomX = pow(absDistX,3);
                double denomY = pow(absDistY,3);
                double denomZ = pow(absDistZ,3);
                
                if (absDistX > .1)
                    {
                        baseaX = baseSphere->getaX() + (distX*gravity*compMass/denomX);
                    }
                if (absDistY > .1)
                    {
                        baseaY = baseSphere->getaY() + (distY*gravity*compMass/denomY);
                    }
                if (absDistZ > .1)
                    {
                    baseaZ = baseSphere->getaZ() + (distZ*gravity*compMass/denomZ);
                    }
                baseSphere->setAccels(baseaX, baseaY, baseaZ);
                }
            }
            
 
            if (compSphere->getActiveStatus())
            {
              boundaryCheck(*compSphere, currentTime);
              if (compSphere->getSphereColor()!="black")
              {
                distX = baseX - compX;
                distY = baseY - compY;
                distZ = baseZ - compZ;
                absDistX = sqrt(distX*distX);
                absDistY = sqrt(distY*distY);
                absDistZ = sqrt(distZ*distZ);
                if (absDistX > .1)
                {
                    compaX = compSphere->getaX() + (distX*gravity*((baseMass)/(pow(absDistX,3))));
               }
                if (absDistY > .1)
                {
                    compaY = compSphere->getaY() + (distY*gravity*((baseMass)/(pow(absDistY,3))));
                }
                if (absDistZ > .1)
                {
                    compaZ = compSphere->getaZ() + (distZ*gravity*((baseMass)/(pow(absDistZ,3))));
                }
                compSphere->setAccels(compaX, compaY, compaZ);
                }
            }
         }
    }
}
bool boundaryCheck(Sphere  &toCheck, const double currentTime)
{
    //check to see if the surface of the sphere has
    //made contact with the boundary of the space
    //first check for greater than max
    //**false if boundary broken
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
        std::ostringstream timeReceive;
        timeReceive << (int) currentTime;
        std::string tempDemise = timeReceive.str();
        tempDemise = tempDemise + "\t\tBoundary";
        toCheck.setDemise(tempDemise);
        }
    return toCheck.getActiveStatus();
}

bool collisionCheck(Sphere &baseSphere, Sphere &compSphere, double currentTime)
{

    if ((baseSphere.distanceTo(&compSphere))<=0)
    {
        std::cout << "collisions check: sphere " << baseSphere.getSphereNumber()
          << " to " << compSphere.getSphereNumber() << " distance: "
          << (baseSphere.distanceTo(&compSphere)) << std::endl;
        if ((baseSphere < compSphere) && (baseSphere.getSphereColor()!="black"))
        {
            //set Active to false
            baseSphere.setActiveStatus(false);
            if (compSphere.getRadius()> 0)
            {
            //set Demise
            std::ostringstream timeReceive;
            timeReceive << (int) currentTime;
            std::string tempDemise = timeReceive.str();
            tempDemise += "\t\tCollision";
            baseSphere.setDemise(tempDemise);  
            } else 
            {
            //set Demise
            std::ostringstream timeReceive;
            timeReceive << (int) currentTime;
            std::string tempDemise = timeReceive.str();
            tempDemise += "\t\tBlack Hole";
            baseSphere.setDemise(tempDemise);
            }
        } else if ((compSphere < baseSphere) && (compSphere.getSphereColor()!="black"))
        {
            //set Active to false
            compSphere.setActiveStatus(false);
            if (baseSphere.getRadius()> 0)
            {
            //set Demise
            std::ostringstream timeReceive;
            timeReceive << (int) currentTime;
            std::string tempDemise = timeReceive.str();
            tempDemise += "\t\tCollision";
            compSphere.setDemise(tempDemise);  
            } else 
            {
            //set Demise
            std::ostringstream timeReceive;
            timeReceive << (int) currentTime;
            std::string tempDemise = timeReceive.str();
            tempDemise += "\t\tBlack Hole";
            compSphere.setDemise(tempDemise);
            }
        }
    }
}

void sweep (List<Sphere> &theList, int & firstDead, double tick)
{
    int j = 2;
    int length = theList.getLength();
    for(int i = 2; i < firstDead; i++)
        {
        Sphere tempSphere;
        theList.retrieve(j, tempSphere);
  /*      std::cout << "Sweep Sphere " << tempSphere.getSphereNumber() << " "
                  << tempSphere.getActiveStatus() << std::endl; */
            if (!tempSphere.getActiveStatus())
            {
 /*               std::cout << "Insert Sphere " << tempSphere.getSphereNumber()
                              << " at " << length << ". Listlength = " 
                              << theList.getLength() << std::endl; */
                theList.insert(length+1, tempSphere);
                theList.remove(j);
                firstDead--;
            } else {
                j++;
                if (tempSphere.getSphereColor()!="black")
                    {
                    Sphere *spherePointer = theList.access(i);
                    double tempdX = spherePointer->getdX();
                    double tempdY = spherePointer->getdY();
                    double tempdZ = spherePointer->getdZ();

                    tempdX = tempdX + (spherePointer->getaX() * tick);
                    tempdY = tempdY + (spherePointer->getaY() * tick);
                    tempdZ = tempdZ + (spherePointer->getaZ() * tick);

                    spherePointer->setVels(tempdX, tempdY, tempdZ);
                    spherePointer->setAccels(0,0,0);
                    }
            }
            
        }
}


