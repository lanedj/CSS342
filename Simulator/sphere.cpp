//----------------------------
/**@File Sphere.cpp
 * @Author Derek Lane 0134101
 * @date 6 Feb 2013
 *
 * update of Sphere class to include dx, dy, dz and sphereNumber, for use
 * in a linked list of spheres.
 * 
 */

#include <iostream>
#include <cmath>

#include "sphere.h"

/**
*Default constructor
*@pre none
*@post Sphere created with active = true, color = black and all doubles = zero
*/ 
Sphere::Sphere()// no param constructor, x,y,z to zero, radius = 1
{
    this->setSphereNumber(0);
    this->setActiveStatus(true);
    this->setTimeOfDeath(0);
    this->setDemise("Not Dead Yet");
    this->setSphereColor("black");
    this->setRadius(0);
    this->setMass(0);
    this->setCoords(0,0,0);
    this->setVels(0,0,0);
    this->setAccels(0,0,0);
}

/**
*Copy Constructor
*@pre sphereToCopy is properly formatted and not NULL
*@post this is deep copy of sphereToCopy 
*@param sphereToCopy address of Sphere Type
*/
Sphere::Sphere (const Sphere *sphereToCopy) //copy constructor
{
    this->setSphereNumber(sphereToCopy->getSphereNumber());
    this->setActiveStatus(sphereToCopy->getActiveStatus());
    this->setSphereColor(sphereToCopy->getSphereColor());
    this->setDemise(sphereToCopy->getDemise());
    this->setTimeOfDeath(sphereToCopy->getTimeOfDeath());
    this->setRadius(sphereToCopy->getRadius());
    this->setMass(sphereToCopy->getMass());
    this->setCoords(sphereToCopy->getX(),
                    sphereToCopy->getY(),
                    sphereToCopy->getZ());
    this->setVels(sphereToCopy->getdX(),
                  sphereToCopy->getdY(),
                  sphereToCopy->getdZ());
    this->setAccels(sphereToCopy->getaX(),
                    sphereToCopy->getaY(),
                    sphereToCopy->getaZ());
}

/**overload "=" to deep copy
 * @pre sphereToCopy is properly formatted and non-NULL
 * @post this is deep copy of sphereToCopy
 * @param Sphere sphereToCopy
 * @return *retSphere
 */
Sphere& Sphere::operator=(const Sphere& sphereToCopy)
{
    this->setSphereNumber(sphereToCopy.getSphereNumber());
    this->setActiveStatus(sphereToCopy.getActiveStatus());
    this->setSphereColor(sphereToCopy.getSphereColor());
    this->setDemise(sphereToCopy.getDemise());
    this->setTimeOfDeath(sphereToCopy.getTimeOfDeath()); 
    this->setRadius(sphereToCopy.getRadius());
    this->setMass(sphereToCopy.getMass());
    this->setCoords(sphereToCopy.getX(),
                    sphereToCopy.getY(),
                    sphereToCopy.getZ());
    this->setVels(sphereToCopy.getdX(),
                  sphereToCopy.getdY(),
                  sphereToCopy.getdZ());
    this->setAccels(sphereToCopy.getaX(),
                    sphereToCopy.getaY(),
                    sphereToCopy.getaZ());
return *this;
};

//overload comparison operators

/**overload "==" to compare sphere volumes
 * DOES NOT COMPARE LOCATION
 * @pre sphereToCopy is properly formatted and non-NULL
 * @param Sphere checkSphere
 * @return boolean
 */
bool Sphere::operator==(const Sphere& checkSphere) const
{
    return this->getMass() == checkSphere.getMass();
}

/** overloads < to return true if this.getMass < checkSphere.getMass
 * @pre this and checkSphere are properly formatted and non-NULL
 * @post none
 * @param Sphere checkSphere
 * @return true or false
 */
bool Sphere::operator<(const Sphere& checkSphere) const
{
    return this->getMass() < checkSphere.getMass();
}

/** overloads > to return true if this.getMass > checkSphere.getMass
 * @pre this and checkSphere are properly formatted and non-NULL
 * @post none
 * @param Sphere checkSphere
 * @return true or false
 */
bool Sphere::operator>(const Sphere& checkSphere) const
{
    return this->getMass() > checkSphere.getMass();
}

/** overloads << to print Spheres in form "TBD"
 * @pre none
 * @post none
 * @param output to ostream
 * @param s sphere to print
 * @return returns "(x,y,z) radius r\n"
 */
std::ostream& operator<<(std::ostream &output, const Sphere &s) {
      output << "SphereNumber " << s.getSphereNumber() << "\n"
             << "ActiveStatus " << s.getActiveStatus() << "\n"
             << "Time Of Death " << s.getTimeOfDeath() << "\n"
             << "Demise type " <<s.getDemise() << "\n"
             << "SphereColor  " << s.getSphereColorString() << "\n"
             << "Radius " << s.getRadius() << "\n"
             << "Mass " << s.getMass() << "\n"
             << "Coords  x " << s.getX()
             << "  y " << s.getY()
             << "  z " << s.getZ() << "\n"
             << "Vels   dx " << s.getdX()
             << " dy " << s.getdY()
             << " dz " << s.getdZ() << "\n"
             << "Accels ax " << s.getaX()
             << " ay " << s.getaY()
             << " az " << s.getaZ();
      return output;
}        

/**
*Mutator: Set demise to string
*@pre this exists
*@post this.demise = the given string
*@param newDemise
*/
void Sphere::setDemise(std::string newDemise)
{
    this->demise = newDemise;
}

/**
*Mutator: Set timeOfDeath to a double
*@pre this exists
*@post this.timeOfDeath = the given double
*@param newTimeOfDeath
*/
void Sphere::setTimeOfDeath (double newTimeOfDeath)
{
    this->timeOfDeath = newTimeOfDeath;
}

/**
*Mutator: Set center using three doubles
*@pre this exists
*@post this.center x,y,z set to new x,y,z values
*@param newX saved as this.x
*@param newY saved as this.y
*@param newZ saved as this.z
*/
void Sphere::setCoords(double newX, double newY, double newZ)
{
    this->x = newX;
    this->y = newY;
    this->z = newZ;
}

/**
*Mutator: Set velocities using three doubles
*@pre this exists
*@post this dx,dy,dz set to new dx,dy,dz values
*@param newdX saved as this.dx
*@param newdY saved as this.dy
*@param newdZ saved as this.dz
*/
void Sphere::setVels(double newdX, double newdY, double newdZ)
{
    this->dx = newdX;
    this->dy = newdY;
    this->dz = newdZ;
}

/**
*Mutator: Set accelerations using three doubles
*@pre this exists
*@post this ax,ay,az set to new ax,ay,az values
*@param newaX saved as this.ax
*@param newaY saved as this.ay
*@param newaZ saved as this.az
*/
void Sphere::setAccels(double newaX, double newaY, double newaZ)
{
    this->ax = newaX;
    this->ay = newaY;
    this->az = newaZ;
}
        
/**
*Mutator: Set radius
*@pre this exists
*@post this.radius set to newRadius
*@param double newRadius to replace current this.radius 
 */
void Sphere::setRadius(double newRadius)
{
    radius = newRadius;
}

        
/**
*Mutator: Set mass
*@pre this exists
*@post this.mass set to newMass
*@param double newMass to replace current this.mass 
 */
void Sphere::setMass(double newMass)
{
    mass = newMass;
}

/**
*Mutator: Set sphereNumber
*@pre this exists
*@post this.radius set to newSphereNumber
*@param double newSphereNumber to replace current this.sphereNumber 
 */
void Sphere::setSphereNumber (int newSphereNumber)
        {
    this->sphereNumber = newSphereNumber;      
        } 

/**
*Mutator: Set sphereColor
*@pre this exists
*@post this.radius set to newSphereColor
*@param double newSphereNumber to replace current this.sphereColor 
 */
void Sphere::setSphereColor (std::string newSphereColor)
        {
    this->sphereColor = newSphereColor;      
        } 

/**
*Mutator: Set active
*@pre this exists
*@post this.active set to newActiveStatus
*@param bool active to replace current this.sphereActiveStatus 
 */
void Sphere::setActiveStatus (bool newActiveStatus)
        {
    this->active = newActiveStatus;      
        } 

/**
*Mutator: Get demise  string
*@pre this exists
*@return this.Demise
*/
std::string Sphere::getDemise()const
{
    return this->demise;
}

/**
*Mutator: Get timeOfDeath double
*@pre this exists
*@return this.timeOfDeath
*/
double Sphere::getTimeOfDeath ()const
{
    return this->timeOfDeath;
}

/**
*Accesor: get radius
*@pre this exists
*@post none
*@return double this.radius  
 */
double Sphere::getRadius() const
{
    return radius;
}

/**
*Accesor: get mass
*@pre this exists
*@post none
*@return double this.mass  
 */
double Sphere::getMass() const
{
    return mass;
}

/**
*Accesor: get x
*@pre this exists
*@post none
*@return double this.x  
 */
 double Sphere::getX () const //returns x value of center
        {
    return this->x;
        }

 /**
*Accesor: get y
*@pre this exists
*@post none
*@return double this.y  
 */
double Sphere::getY () const //returns y value of center
        {
    return this->y;
        }
        
/**
*Accesor: get z
*@pre this exists
*@post none
*@return double this.z  
 */
double Sphere::getZ () const //returns y value of center
        {
    return this->z;
        }
 
/**
*Accesor: get dx
*@pre this exists
*@post none
*@return double this.dx  
 */
 double Sphere::getdX () const //returns x value of center
        {
    return this->dx;
        }

 /**
*Accesor: get dy
*@pre this exists
*@post none
*@return double this.dy  
 */
double Sphere::getdY () const 
        {
    return this->dy;
        }
        
/**
*Accesor: get dz
*@pre this exists
*@post none
*@return double this.dz  
 */
double Sphere::getdZ () const 
        {
    return this->dz;
        }


/**
*Accesor: get ax
*@pre this exists
*@post none
*@return double this.ax  
 */
 double Sphere::getaX () const //returns x value of center
        {
    return this->ax;
        }

 /**
*Accesor: get ay
*@pre this exists
*@post none
*@return double this.ay  
 */
double Sphere::getaY () const //returns y value of center
        {
    return this->ay;
        }
        
/**
*Accesor: get az
*@pre this exists
*@post none
*@return double this.az  
 */
double Sphere::getaZ () const //returns y value of center
        {
    return this->az;
        }

/**
*Accesor: get sphereNumber
*@pre this exists
*@post none
*@return double this.sphereNumber  
 */
int Sphere::getSphereNumber () const 
        {
    return this->sphereNumber;
        }

/**
*Accesor: get sphereColor
*@pre this exists
*@post none
*@return Color this.sphereColor  
 */
std::string Sphere::getSphereColor () const 
        {
    return this->sphereColor;
        }

/**
*Mutator: get sphereColorString
*@pre this exists
*@post none
*@return string version of this.sphereColor 
 */
std::string Sphere::getSphereColorString() const
        {
    /*std::string colorString;
    if (this->sphereColor == BLACK)
    {
        colorString = "BLACK";
    } else if (this->sphereColor == WHITE)
    {
        colorString = "WHITE";
    } else if (this->sphereColor == RED)
    {
        colorString = "RED";
    } else if (this->sphereColor == GREEN)
    {
        colorString = "GREEN";
    } else if (this->sphereColor == BLUE)
    {
        colorString = "BLUE";
    } else if (this->sphereColor == YELLOW)
    {
        colorString = "YELLOW";
    }*/
    return this->sphereColor;
        }


/**
*Accesor: get active
*@pre this exists
*@post none
*@return bool this.active  
 */
bool Sphere::getActiveStatus () const 
        {
    return this->active;
        }

/**
* Returns distance between surface of this and Sphere thatSphere
* @Pre Sphere param is valid non-null Sphere
* @Post none
* @param Sphere thatSphere
* @return distance from Sphere surface to Sphere surface as double
*/
double Sphere::distanceTo (const Sphere* thatSphere)
{
   double a = 0;
   double b = 0;
   double c = 0;
   double dSquare = 0;
   double rawDistance = 0;
   double totalRadius = 0;
   double finalDistance = 0;
   a = (this->getX()) - (thatSphere->getX());
   b = (this->getY()) - (thatSphere->getY());
   c = (this->getZ()) - (thatSphere->getZ());
   dSquare = (a*a)+(b*b)+(c*c);
   rawDistance = sqrt(dSquare);
   totalRadius = this->getRadius() + thatSphere->getRadius();
   finalDistance = rawDistance - totalRadius;    
   return finalDistance;
}

/*
int main () // to test methods
    {
       ThreeDPoint pointTwo (2,2,2);
       Sphere noParamSphere;
       Sphere allIntSphere(1,1,1,1);
       Sphere pointAndRadSphere(&pointTwo, 2);
       Sphere copySphere(&allIntSphere);
       Sphere copy2;
       copy2 = allIntSphere;
       
       std::cout << "pointTwo is " << pointTwo << std::endl;
       std::cout << "noParamSphere is " << noParamSphere << std::endl;
       std::cout << "allIntSphere is " << allIntSphere << std::endl; 
       std::cout << "copy2 is " << copy2 << std::endl;
       std::cout << "copy2 == allInt: " << (copy2 == allIntSphere) << std::endl;
       std::cout << "copy2 < allInt: " << (copy2 < allIntSphere) << std::endl;
       std::cout << "copy2 > allInt: " << (copy2 > allIntSphere) << std::endl;
       allIntSphere.setRadius(10);
       std::cout << "copy2 == allInt: " << (copy2 == allIntSphere) << std::endl;
       std::cout << "copy2 < allInt: " << (copy2 < allIntSphere) << std::endl;
       std::cout << "copy2 > allInt: " << (copy2 > allIntSphere) << std::endl;
       std::cout << "pointAndRadSphere is " << pointAndRadSphere << std::endl;
       std::cout << "copySphere is " << copySphere << std::endl;
       
       copySphere.setX(4);
       copySphere.setY(5);
       copySphere.setZ(6);
       copySphere.setRadius(7);
       
       std::cout << copySphere.getX() << "," << copySphere.getY() <<","
               << copySphere.getZ() << " radius " << copySphere.getRadius()
               << std::endl;
       
       copySphere.setCenter(&pointTwo);
       std::cout << "copySphere is " << copySphere << std::endl;
       
       copySphere.setCenter(101,-101,-101);
       copySphere.setRadius(11);
       std::cout << "copySphere is " << copySphere << std::endl;
       
       std::cout << "copySphere Surface Area: " << copySphere.getSurfaceArea()
               << std::endl;
       std::cout << "copySphere Volume: " << copySphere.getVolume()
               << std::endl;
       std::cout<< "dist allInt to copySphere: " << 
                   allIntSphere.distanceTo(&copySphere) << std::endl;
                    
       
    }
*/

