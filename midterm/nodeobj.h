/******************************************************************
*   nodeobj.h
*     module for 'NodeObj' class info/declarations
*
*   (note that identifiers are made as intentionally generic as
*   possible to aid in re-use)
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <fstream>
#include "appconsts.h"
#include "sphere.h"

using namespace std;


/******************************************************************
*   class NodeObj declaration
*/

class NodeObj {
   // overload insertion op for 'NodeObj's & make it our buddy
   friend ostream &operator<< (ostream &, const NodeObj &);

public:
   NodeObj();   // default constructor
   NodeObj(const NodeObj *); //copy constructor
   ~NodeObj();  //destructor

   bool set_data(ifstream &);   // set node data from input file
   void set_mark(EventType);
   void reset_alive(void);
   int get_index(void);
   EventType get_mark(void);
   bool is_alive(void);
   int get_radius(void);
   void get_new_position(const double, 
      double &, double &, double &);
   bool collision_test(const double, NodeObj *);
   bool boundary_test(const double);

private:
   Sphere *pS;    // node data defined in sphere.h

   int index;        // list index = order read & created
   EventType markedForDeath; // marked flag indicates event type
   bool alive;       // existence/viability flag
};

#endif
