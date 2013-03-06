/* 
 * File:   simulator.h
 * Author: Derek
 *
 * Created on March 4, 2013, 9:30 PM
 */

#ifndef SIMULATOR_H
#define	SIMULATOR_H

# include <iostream>
# include <fstream>
# include <sstream>
# include <cmath>
# include "list.h"
# include "sphere.h"

class Simulator
{
public:

/**
 * Default constructor
 * @post Simulator class created with an empty sphereList, currentTime = 0
 *       and firstDead = 0
 */
 Simulator();
    
/**
 * Populates the simulator with blackhole and spheres from provided datafile
 * @pre param string is name of properly formatted file in correct location
 * @post this.theList populated with black hole and spheres
 * @return false if failed to open file or if it was empty
 */
bool getData (std::string);

/**
 * prints final report of all collisions, in order (does not print blackhole)
 * @pre spheres in the list (other than black hole
 * @post output to console
 */
void finalReport ();

/**
 * Updates the coordinates of all active spheres in the list with the
 * based on their current velocities dx, dy, dz
 * @pre active spheres in the list
 * @post dx, dy, dz of all active spheres updated
 * @return false if no active spheres otherwise true
 */
bool updateCoords ();

/**
 * Updates the accelerations of all active spheres in the list with the
 * aggregate accelerations based on all other bodies in the space
 * @pre active spheres in the list
 * @post ax, ay, az of all active spheres updated
 * @return false if no active spheres otherwise true
 */
bool updateAccels ();

/**
 * check for collision between a spheres and the boundaries
 * collision occurs if distance from center to boundary is <= sphere radius
 * @pre active spheres in sphereList
 * @post spheres that hit the boundary marked as dead
 */
void boundaryCheck ();

/**
 * check for collision between two spheres at current time
 * collision occurs if distance between centers <= sum of the two radii
 * @pre active spheres in sphere list
 * @post any spheres that lost collisions have active set false
 *       and demise type set to either collision or blackhole
 * @return true if any spheres were checked
 */
bool collisionCheck ();

/**
 * moves any inactive spheres to the end of the list
 * @pre active spheres in the list
 * @post all inactive spheres to end of list
 *       this.firstDead updated to new position
 * @return false if no active spheres otherwise true
 */
bool prune ();

/**
 * Updates the velocities of all active spheres in the list
 * @pre active spheres in the list
 * @post dx, dy, dz of all active spheres updated based on their
 *       aggregate ax, ay, az
 * @post ax, ay, az of all spheres reset to zero
 * @return false if no active spheres otherwise true
 */
bool updateVels();

/**
 * adds one tick to current time.
 * @post currentTime updated by one tick
 */
void clockTick ();

static const int MAX_COORD = 1000;
static const int MIN_COORD = 0;
static const double G = 10;
static const double tick = .001;

private:
    
    List<Sphere> sphereList;//create the list
    double currentTime;
    int firstDead;

/**
 * compare the length of the list to the location of lastDead and return
 * the limit that will make sure all active sphere are accessed
 * @pre active spheres in the list
 * @return int to use as limit of for loop
 */
int getForLimit();    
    
    
};
#endif	/* SIMULATOR_H */
