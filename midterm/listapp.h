/******************************************************************
*   listapp.h
*     declarations/definitions for a simple linked list, uses
*     'Node' as linked list node object
*
*   (plz note that identifiers are intentionally as generic as
*   possible to aid in re-use)
*
*   *** bad, bad code... mixed-mode header file includes both
*        declarations and implementation   :/
*        ->fix with "separate compilation" method using 'extern's
*/

#ifndef LISTAPP_H
#define LISTAPP_H

#include <iostream>
#include <fstream>
#include "appconsts.h"
#include "display.h"

using namespace std;


/******************************************************************
*   class List declaration
*     ADT List: ordered collection of items
*
*     criteria:  
*        - control of operators, printing, and handling of T
*            information is left to the T class
*        - head points to the list base node (so if the list is
*            empty, head is NULL)
*        - add_node allocates memory for a Node; data passed in
*            is the responsibility of the client
*     (Note that this definition is not a full-featured class)
*/

template <typename T>
class List {

template <typename U> // output operator for class List
friend ostream &operator<< (ostream &, const List<U> &);

public:
   List();                       // default constructor
   List(const List &);        // copy constructor
   ~List();                   // destructor

   bool is_empty() const;        // is list empty?
   bool is_exhausted() const;    // all nodes dead?
   bool add_node(T *);           // add a node to list
   void build_list(ifstream &);  // build a list from file data

   // event checking methods
   void proximity_chk(const double, T *, T *);
   void boundary_chk(const double, T *);
   void event_check(const double);
   void record_event(const double);

private:
   struct Node             // node in a linked list
   {
      T *pData;            // ptr to actual data/operations for T
      Node *next;          // ptr to next node in list
   };
   Node *head;             // ptr to base node in list
};


/******************************************************************
*   default constructor
*     set key initial data on object creation
*/

template <typename T>
List<T>::List()
{
   head = NULL;
}

/******************************************************************
*   copy constructor
*     performs deep copy assumes T has a deep copy copy constructor
*/
template <typename T>
List<T>::List(const List &aList)
{
    T *pT;
    head = NULL;
    Node *toCopy;
    toCopy= aList.head;

    while (toCopy) //if aList is empty, this.head is already NULL
    {
        pT = new T (toCopy->pData); //assumes T has deep copy constructor
       this->add_node(pT);
       toCopy = toCopy->next;   //sets toCopy = NULL after last node    
    }
}

/******************************************************************
*   destructor
*   starts at head and deletes all nodes in the list, including head
*/

template <typename T>
List<T>::~List()
{
    Node *current;
    Node *previous;
    current = head;
    head = NULL; //first time through the while loop will delete head because
                 //first value of current is the old head address
    //execute while until all nodes deleted
    while (current != NULL)
    {
        if (current->next != NULL)
        {//if there are at least two remaining nodes, delete the first one
            previous = current;
            current = current->next;
            previous->next = NULL;
            delete previous->pData;
            previous->pData = NULL;
            delete previous;
            previous = NULL; //make sure we don't have a wild pointer at end
        } else
        {//if there is only one remaining node, delete it
            delete current->pData;
            current->pData = NULL;
            delete current;
            current = NULL;
        }
    }
}

/******************************************************************
*   is_empty
*     check to see if list is empty (NULL head?)
*/

template <typename T>
bool List<T>::is_empty() const
{
   return (head == NULL);
}


/******************************************************************
*   is_exhausted
*     check to see if list is exhausted (i.e., no more nodes alive)
*/

template <typename T>
bool List<T>::is_exhausted() const
{
   Node *currNode = head;   // starting posn in list

   // examine status of each node in list
   while (currNode != NULL)
   {
      if (currNode->pData->is_alive())
         return false;

      currNode = currNode->next;  // next node      
   }
   return true;  // no living nodes remaining
}


/******************************************************************
*   add_node
*     add an item to end of the list
*/

template <typename T>
bool List<T>::add_node(T *pT)
{
   Node *pNode = new Node;

   if (pNode == NULL)  // out of memory (not likely)
      return false;
   pNode->pData = pT;  // link the node to the passed data

   // if list is empty create first node
   if (is_empty())
   {
      pNode->next = head;                           
      head = pNode;
   }
   // otherwise find end of list
   else
   {
      Node *current = head->next;   // current posn in list
      Node *previous = head;     // maintain contact w. last node

      // walk to end of list
      while (current != NULL)
      {
         previous = current;  // proceed to next node
         current = current->next;
      }

      // add new node & link to it
      pNode->next = current; 
      previous->next = pNode; 
   }
   return true;
}


/******************************************************************
*   build_list
*     add new items to the list until input is exhausted
*/

template <typename T>
void List<T>::build_list(ifstream &fin)
{
   T *pT;
   bool readSuccess;    // successful read of object data
   bool storeSuccess;   // successful node addition

   while (true)
   {
      storeSuccess = false;
      pT = new T;
      readSuccess = pT->set_data(fin); // fill the T object

      if (fin.eof())
      {
         delete pT;
         break;
      }

      // insert object data into the list
      if (readSuccess)
         storeSuccess = add_node(pT);
      else   // something bad happened during node setup
      {
         delete pT;
         fatal_err(BAD_SET_DATA);
      }
      if (!storeSuccess)   // something bad happened during store
         fatal_err(BAD_ADD_NODE);
   }
}


/******************************************************************
*   proximity_chk
*     compare positions of 2 spheres to see if they overlap; if so
*     then mark
*/

template <typename T>
void List<T>::proximity_chk(const double t, T *pT1, T *pT2)
{
   // check co-incidence of given spheres
   pT1->collision_test(t, pT2);
}


/******************************************************************
*   boundary_chk
*     compare position of sphere to walls to see if any overlap;
*     if so then mark
*/

template <typename T>
void List<T>::boundary_chk(const double t, T *pT)
{
   // check sphere contact with boundary plane
   pT->boundary_test(t);
}


/******************************************************************
*   event_check
*     walk through list and check if any events have occurred
*/

template <typename T>
void List<T>::event_check(const double t)
{
   Node *currNode = head;   // starting posn in list

   // examine relation of each node in list against all others
   while (currNode->next != NULL)
   {
      Node *testNode = currNode->next;

      while (testNode != NULL)
      {
         // compare updated positions of ref v. test
         proximity_chk(t, currNode->pData, testNode->pData);
         
         testNode = testNode->next;
      }
      currNode = currNode->next;  // next reference
   }

   currNode = head;   // starting posn in list

   // examine relation of each node in list against boundaries
   while (currNode != NULL)
   {
      boundary_chk(t, currNode->pData);
      
      currNode = currNode->next;  // next node      
   }
}


/******************************************************************
*  record_event
*     display the event record for the recently deceased sphere
*/

template <typename T>
void List<T>::record_event(const double t)
{
   int intTime = static_cast <int> (t);
   Node *currNode = head;   // start at the beginning
   Node *prevNode = currNode;

   // examine status flags of each node in list
   while (currNode != NULL)
   {
      if (((currNode->pData)->get_mark() != NOT_YET) &&
         ((currNode->pData)->is_alive()))
      {
         // these local vars used due to uncertainty about side
         //   effects of method calls inside overloaded insertion
         int index = (currNode->pData)->get_index();
         EventType type = (currNode->pData)->get_mark();
         
         cout << "      " << index << "        " << 
            intTime << "        ";
         if (type == COLLISION)
            cout << "COLLISION\n";
         else
            cout << "BOUNDARY\n";
         
 //        (currNode->pData)->reset_alive(); replaced by actual removal of node
         
         //special case for removing the head node
         if (currNode == head)
         {
             prevNode = head;
             head = head->next; //moves head to head->next
             if (head) // if !head then list only has 1 node
             {
             currNode = head->next;
             } else
             {
                 currNode = head; //avoids setting currNode = NULL->next
             }
             //next four lines release memory and advance prevNode
             //!!!note!!! in this case prevNode is deleted
             delete prevNode->pData;
             prevNode->pData = NULL;
             delete prevNode;
             prevNode = currNode; //make sure we don't have a wild pointer
         } else
         {//if the list has at least 2 nodes
          //!!!note!!! in this case currNode is deleted
         prevNode->next = currNode->next;
         delete currNode->pData;
         currNode->pData = NULL;
         delete currNode;
         currNode = prevNode; //taming a possible wild pointer
         }

      }
      if (currNode) //makes sure we don't try to access NULL->next
      {
      currNode = currNode->next;  // now currNode is either one node ahead of
                                  //prevNode or currNode == NULL
      }   
   }
}


/******************************************************************
*   operator <<
*     overloaded insertion operator for object data output
*     (useful for debugging execution)
*/

template <typename T>
ostream &operator<< (ostream &output, const List<T> &my_list)
{
   typename List<T>::Node *current = my_list.head;

   while (current != NULL)
   { 
      output << *current->pData;
      current = current->next;
   }
   return output;
}

#endif
