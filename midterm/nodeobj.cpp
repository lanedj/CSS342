/******************************************************************
*   nodeobj.cpp
*     module for NodeObj class definitions
*/

#include <cmath>
#include "nodeobj.h"


/******************************************************************
*   NodeObj (default constructor)
*     create node & initialize key member values
*/
NodeObj::NodeObj()
{
   alive = true;
   markedForDeath = NOT_YET;
}  

/******************************************************************
*   NodeObj (copy constructor)
*     create node & copy values from toCopy
*/
NodeObj::NodeObj(const NodeObj *toCopy) //copy constructor
{
    this->pS = new Sphere;
    this->alive = toCopy->alive;
    this->index = toCopy->index;
    this->markedForDeath = toCopy->markedForDeath;
    this->pS->x = toCopy->pS->x;
    this->pS->y = toCopy->pS->y;
    this->pS->z = toCopy->pS->z;
    this->pS->r = toCopy->pS->r;
    this->pS->dx = toCopy->pS->dx;
    this->pS->dy = toCopy->pS->dy;
    this->pS->dz = toCopy->pS->dz;
}


/******************************************************************
*   destructor
*     delete the sphere object and set point to NULL
*/
NodeObj::~NodeObj()
{
    delete pS;
    pS = NULL;
}

/******************************************************************
*   set_data
*     set initial node data using file input
*/

bool NodeObj::set_data(ifstream &fin)
{
   static int readOrder = 0;

   pS = new Sphere;  // alloc space for sphere data
   
   // read sphere data from file
   fin >> pS->x >> pS->y >> pS->z;
   fin >> pS->r;
   fin >> pS->dx >> pS->dy >> pS->dz;
   
   index = ++readOrder;  // order of node in list

   return true;
}


/******************************************************************
*   set_mark
*     mark sphere for subsequent elimination
*/

void NodeObj::set_mark(EventType type)
{
   markedForDeath = type;
}


/******************************************************************
*   reset_alive
*     flag sphere for immediate elimination
*/

void NodeObj::reset_alive(void)
{
   alive = false;
}


/******************************************************************
*   get_index
*     return list index
*/

int NodeObj::get_index(void)
{
   return index;
}


/******************************************************************
*   get_mark
*     return mark of death flag
*/

EventType NodeObj::get_mark(void)
{
   return markedForDeath;
}


/******************************************************************
*   is_alive
*     return existence/viability flag
*/

bool NodeObj::is_alive(void)
{
   return alive;
}


/******************************************************************
*  get_radius
*     return radius value of sphere
*/

int NodeObj::get_radius(void)
{
   return (pS->r);
}


/******************************************************************
*  get_new_position
*     update the position of the sphere based on current time 't'
*     (note that the sphere is not actually being repositioned)
*/

void NodeObj::get_new_position(const double t, 
   double &newX, double &newY, double &newZ)
{
   newX = static_cast <double> (pS->x) + 
      (static_cast <double> (pS->dx) * t);
   newY = static_cast <double> (pS->y) + 
      (static_cast <double> (pS->dy) * t);
   newZ = static_cast <double> (pS->z) + 
      (static_cast <double> (pS->dz) * t);

/***  only use following in extreme cases  :D
#ifdef DEBUG
   cout << "\n\tindex= " << index << 
      "\tx= " << newX << "\ty= " << newY << "\tz= " << newZ;
#endif
***/
}


/******************************************************************
*  collision_test
*     calcuate distance from current reference sphere to test
*     (target) sphere: compute center-to-center distance and 
*     subtract radii; if result <= 0 then impact occurred
*/

bool NodeObj::collision_test(const double t, NodeObj *pNode)
{
   Sphere *pS2 = pNode->pS;
   double xPosn1, yPosn1, zPosn1;
   double xPosn2, yPosn2, zPosn2;

   // preempt check if either sphere is already dead
   if (!alive || !pNode->is_alive())
      return false;

   get_new_position(t, xPosn1, yPosn1, zPosn1);
   pNode->get_new_position(t, xPosn2, yPosn2, zPosn2);

   double delX = xPosn1 - xPosn2;
   double delY = yPosn1 - yPosn2;
   double delZ = zPosn1 - zPosn2;

   if (
      (sqrt(pow(delX, 2.) + pow(delY, 2.) + pow(delZ, 2.))
      <= static_cast <double> (pS->r + pS2->r)))
   {
#ifdef DEBUG
   cout << "\n*** collision between " << index << " and " << 
      pNode->index << 
      "\n    position1= " <<
      xPosn1 << "  "  << yPosn1 << "  " << zPosn1 <<
      "\n    position2= " <<
      xPosn2 << "  "  << yPosn2 << "  " << zPosn2 <<
      "\n    time= " << t << "\n";
#endif
      // mark smallest radius as a COLLISION event
      (pS->r < pS2->r) ? 
         markedForDeath = COLLISION :
         pNode->markedForDeath = COLLISION;
      return true;
   }
   return false;
}


/******************************************************************
*  boundary_test
*     calcuate distance from current sphere to all bounding walls:
*     compute center-to-center distance and subtract radii;
*     if result <= 0 for any wall then impact occurred
*/

bool NodeObj::boundary_test(const double t)
{
   double xPosn, yPosn, zPosn;

   // preempt check if already dead
   if (!alive)
      return false;

   get_new_position(t, xPosn, yPosn, zPosn);

   // check against minimum boundary walls
   if (
      ((xPosn - pS->r) <= MIN_COORD) ||
      ((yPosn - pS->r) <= MIN_COORD) ||
      ((zPosn - pS->r) <= MIN_COORD))
   {
#ifdef DEBUG
   cout << "\n*** boundary for " << index <<
      "\n    position= " <<
      xPosn << "  "  << yPosn << "  " << zPosn <<
      "\n    time= " << t << "\n";
#endif
      markedForDeath = BOUNDARY;
      return true;
   }
   
   // check against maximum boundary walls
   if (
      ((xPosn + pS->r) >= MAX_COORD) ||
      ((yPosn + pS->r) >= MAX_COORD) ||
      ((zPosn + pS->r) >= MAX_COORD))
   {
#ifdef DEBUG
   cout << "\n*** boundary for " << index <<
      "\n    position= " <<
      xPosn << "  "  << yPosn << "  " << zPosn <<
      "\n    time= " << t << "\n";
#endif
      markedForDeath = BOUNDARY;
      return true;
   }
   return false;
}


/******************************************************************
*   operator <<
*     object's insertion operator: display important stuff
*     (very useful for debugging)
*/

ostream &operator<< (ostream &output, const NodeObj &n)
{
   char s1[][4] = {"no", "yes"};
   char s2[][10] = {"NOT_YET", "COLLISION", "BOUNDARY"};
   
   output << "\nindex = " << n.index << "   radius = " <<
      (n.pS)->r << "   alive = " << s1[n.alive] << 
      "   marked = " << s2[n.markedForDeath];

   return output;
}
