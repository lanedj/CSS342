//----------------------------
/**@File Sphere.h
 * @author Derek Lane 0134101
 * @date 6 Feb 2013
 * From Timothy Stewart code provided at: 
 *  http://courses.washington.edu/css342/timots/Code/circle/circle4.cpp
 * 
 * Sphere class removes all previous sphere inheritance and makes it all
 * organic
 * 
 * all constants come from appconsts.h
 * 
 * << overloaded to print as
 * = overloaded to make deep copy
 * ==, <, and > overloaded to compare volume of spheres
 * 
 * method distanceTo returns distance between surfaces of two spheres
 * 
 * PI set as double constant at 3.14159
 * 
 */

#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include <string>

/**
 *Sphere class with variables for motion in 3D space
 */
class Sphere
{

public:
    
/**
*Default constructor
*@pre none
*@post Sphere created with active = true, color = black and all doubles = zero
*/    
Sphere();

/**
*Copy Constructor
*@pre sphereToCopy is properly formatted and not NULL
*@post this is deep copy of sphereToCopy 
*@param sphereToCopy address of Sphere Type
*/
Sphere(const Sphere* sphereToCopy); //copy constructor

/**overload "=" to deep copy
 * @pre sphereToCopy is properly formatted and non-NULL
 * @post this is deep copy of sphereToCopy
 * @param Sphere sphereToCopy
 * @return *retSphere
 */
Sphere& operator=(const Sphere& sphereToCopy);

//overload comparison operators

/**overloads == to return true if getMass are equal, else false
 * @pre this and checkSphere are properly formatted and non-NULL
 * @post none
 * @param Sphere checkSphere
 * @return true or false
 */
bool operator==(const Sphere& checkSphere) const;

/** overloads < to return true if this.getMass < checkSphere.getMass
 * @pre this and checkSphere are properly formatted and non-NULL
 * @post none
 * @param Sphere checkSphere
 * @return true or false
 */
 bool operator<(const Sphere& checkSphere) const;

/** overloads > to return true if this.getMass > checkSphere.getMass
 * @pre this and checkSphere are properly formatted and non-NULL
 * @post none
 * @param Sphere checkSphere
 * @return true or false
 */
bool operator>(const Sphere& checkSphere) const;

/** overloads << to print Spheres in form "TBD"
 * @pre none
 * @post none
 * @param output to ostream
 * @param s sphere to print
 * @return returns "TBD"
 */
friend std::ostream& operator<<(std::ostream& output, const Sphere& s);

/**
*Mutator: Set demise to string
*@pre this exists
*@post this.demise = the given string
*@param newDemise
*/
void setDemise(std::string newDemise);

/**
*Mutator: Set timeOfDeath to a double
*@pre this exists
*@post this.timeOfDeath = the given double
*@param newTimeOfDeath
*/
void setTimeOfDeath (double newTimeOfDeath);

/**
*Mutator: Set center using three doubles
*@pre this exists
*@post this.center x,y,z set to new x,y,z values
*@param newX saved as this.x
*@param newY saved as this.y
*@param newZ saved as this.z
*/
void setCoords(double newX, double newY, double newZ);

/**
*Mutator: Set velocities using three doubles
*@pre this exists
*@post this dx,dy,dz set to new dx,dy,dz values
*@param newdX saved as this.dx
*@param newdY saved as this.dy
*@param newdZ saved as this.dz
*/
void setVels(double newdX, double newdY, double newdZ);

/**
*Mutator: Set accelerations using three doubles
*@pre this exists
*@post this ax,ay,az set to new ax,ay,az values
*@param newaX saved as this.ax
*@param newaY saved as this.ay
*@param newaZ saved as this.az
*/
void setAccels(double newaX, double newaY, double newaZ);

/**
*Mutator: Set radius
*@pre this exists
*@post this.radius set to newRadius
*@param double newRadius to replace current this.radius 
 */
void setRadius(double newRadius);

/**
*Mutator: Set mass
*@pre this exists
*@post this.mass set to newMass
*@param double newMass to replace current this.mass 
 */
void setMass(double newMass);


/**
*Mutator: Set sphereNumber
*@pre this exists
*@post this.sphereNumber set to newSphereNumber
*@param double newSphereNumber to replace current this.sphereNumber 
 */
void setSphereNumber (int newSphereNumber);

/**
*Mutator: Set sphereColor
*@pre this exists
*@post this.sphereNumber set to newSphereColor
*@param double newSphereNumber to replace current this.sphereColor 
 */
void setSphereColor (std::string newSphereColor);

/**
*Mutator: Set active
*@pre this exists
*@post this.active set to newActiveStatus
*@param bool active to replace current this.sphereActiveStatus 
 */
void setActiveStatus (bool newActiveStatus);

/**
*Mutator: Get demise  string
*@pre this exists
*@return this.Demise
*/
std::string getDemise()const;

/**
*Mutator: Get timeOfDeath double
*@pre this exists
*@return this.timeOfDeath
*/
double getTimeOfDeath ()const;

/**
*Accesor: get radius
*@pre this exists
*@post none
*@return double this.radius  
 */
double getRadius() const;

/**
*Accesor: get mass
*@pre this exists
*@post none
*@return double this.mass  
 */
double getMass() const;

/**
*Accesor: get x
*@pre this exists
*@post none
*@return double this.x  
 */
double getX () const; //returns x value of center

/**
*Accesor: get y
*@pre this exists
*@post none
*@return double this.y  
 */
double getY () const; //returns y value of center

/**
*Accesor: get z
*@pre this exists
*@post none
*@return double this.z  
 */
double getZ () const; //returns z value of center

/**
*Accesor: get dx
*@pre this exists
*@post none
*@return double this.dx  
 */
double getdX () const; //returns x value of center

/**
*Accesor: get dy
*@pre this exists
*@post none
*@return double this.dy  
 */
double getdY () const; //returns y value of center

/**
*Accesor: get dz
*@pre this exists
*@post none
*@return double this.dz  
 */
double getdZ () const; //returns z value of center

/**
*Accesor: get ax
*@pre this exists
*@post none
*@return double this.ax  
 */
double getaX () const; //returns x value of center

/**
*Accesor: get ay
*@pre this exists
*@post none
*@return double this.ay  
 */
double getaY () const; //returns y value of center

/**
*Accesor: get az
*@pre this exists
*@post none
*@return double this.az  
 */
double getaZ () const; //returns z value of center

/**
*Accesor: get sphereNumber
*@pre this exists
*@post none
*@return double this.sphereNumber  
 */
int getSphereNumber () const;

/**
*Mutator: get sphereColor
*@pre this exists
*@post none
*@return Color enum this.sphereColor 
 */
std::string getSphereColor () const;

/**
*Mutator: get sphereColorString
*@pre this exists
*@post none
*@return string version of this.sphereColor 
 */
std::string getSphereColorString () const;
  
/**
*Mutator: get active status
*@pre this exists
*@post none
*@return bool this.active 
 */
bool getActiveStatus () const;

/**
* Returns distance between surface of this and Sphere thatSphere
* @Pre Sphere param is valid non-null Sphere
* @Post none
* @param Sphere thatSphere
* @return distance from Sphere surface to Sphere surface as double
*/
double distanceTo (const Sphere* thatSphere);

private:
    
    int sphereNumber; //identifier for sphere
    bool active; //set to false if sphere loses collision
    std::string sphereColor; //enum type def'd in appconsts.h
    std::string demise; //how was it destroyed? default "not dead yet"
    double radius;	// sphere radius
    double mass; //sphere mass (equals radius unless black hole
    double x, y, z; //spatial coords
    double dx, dy, dz; //velocities in secs
    double ax, ay, az; //accelerations in secs/secs
    double timeOfDeath; //holds time of destruction of Sphere
};

#endif
