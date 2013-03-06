/* 
 * @File   list.h
 * @Author Derek Lane 0134101
 * @date 23 Feb 2013
 *
 * Doubly linked list template
 * based on ListP.h from Carrano (pp195-202)
 * and template list provided by Prof Stewart for Midterm Project
 * 
 */

#ifndef LIST_H
#define	LIST_H

# include <iostream>
# include <fstream>
# include <cstddef>
# include <new>

template <typename objType>
class List {

template <typename U> // output operator for class List
friend std::ostream &operator<< (std::ostream &, const List<U> &);

public:
   List();                       // default constructor
   List(const List &);        // copy constructor
   ~List();                   // destructor

   bool is_empty() const;        // is list empty?
   int getLength() const;    
   bool insert(int index, const objType& newItem); //return false if bad index
   bool remove(int index); //return false if bad index
   bool getObj(int index, objType& dataItem) const;
   //doesn’t allow for changes, returns false if bad index or NULL
   
   bool setObj(int index, objType& dataItem) const;
   //sets theObj in Node[index] to dataItem, returns false if bad index or NULL 

private:
    struct ListNode //a node on the list
    {
        objType theObj;
        ListNode *previous;
        ListNode *next;
    };
    
    int length; //number of items in list
   
    ListNode *head; //pointer to first node in list of items 
};
/******************************************************************
*   default constructor
*     set key initial data on object creation
*/

template <typename objType>
List<objType>::List()
{
    this->length = 0;
    head = NULL;
}

/******************************************************************
*   copy constructor
*     performs deep copy assumes T has a deep copy copy constructor
*/
template <typename objType>
List<objType>::List(const List &sourceList)
{
    objType *pObjType;
    length = 0;
    head = NULL;
    ListNode *toCopy;
    toCopy= sourceList.head;

    while (toCopy) //if aList is empty, this.head is already NULL
    {
       pObjType = new objType (toCopy->theObj); //assumes T has deep copy constructor
       this->insert(++this->length, pObjType);
       toCopy = toCopy->next;   //sets toCopy = NULL after last node    
    }
}
  
template <typename objType>
List<objType>::~List()
{
    while (head)
    {
        remove(1);
    }
    delete head;
    head = NULL;
}
 
/******************************************************************
*   is_empty
*     check to see if list is empty (NULL head?)
*/

template <typename objType>
bool List<objType>::is_empty() const
{
   return (head == NULL);
}

/**
 * Determines the length of the list
 * @pre None
 * @post None
 * @return number of items currently in the list
 */
template <typename objType>
int List<objType>::getLength() const
{
    return this->length;
}

/**
 * Inserts an item into the list at position index
 * @pre 
 * @param index
 * @param newItem
 * @throw ListIndexOutOfRangeException If index < 1 or index
 *          > getLength() +1
 * @throw ListException If newItem cannot be placed in the list
 *          because array is full
 */
template <typename objType>
bool List<objType>::insert(int index, const objType& newObj)
{
    ++this->length;
    ListNode *newPtr = new ListNode;
    newPtr->theObj = newObj;

    //attach new node to list
    if ((index < 1) || (index > length))
    {
        --this->length;
        return false;
    } else if (index == 1)
        {// insert node at beginning of list
            newPtr->previous = NULL;
            if (this->length == 1)
            {
                    newPtr->next = NULL;
            } else
            {
                newPtr->next = head->next;
                newPtr->next->previous = newPtr;
            }
            head = newPtr;
    } else
            {
            int findIndex = 1;
            newPtr->previous = head;
            while (++findIndex  <  index)
                {
                newPtr->previous  = newPtr->previous->next;
                }
            //insert new node after node to which prev points
            newPtr->next = newPtr->previous->next;
            newPtr->previous->next = newPtr;
            if (newPtr->next)
                {
                  newPtr->next->previous = newPtr;
                }
            }
    newPtr = NULL;
    return true;  
}

/**
 * Deletes an item from the list at a given position
 * @pre 1<= index <= getLength()
 * @post if 1 <= index <= getLength(), the item at position index in the
 *          list is deleted, other items are renumbered accordingly
 * @param index The list position to delete from
 * @return bool true if successful false if not
 */
template <typename objType>
bool List<objType>::remove(int index)
{ 
    ListNode *current;
    current = head;
    if ((index < 1) || (index > getLength()))
            {
                return false;
            } else
            {
                --length;
                //ListNode *current;
                //current = head;
                if (index == 1)
                {//delete the first node form the list
                    head = head->next;
                } else
                {
                    {
                    int findIndex = 1;
                    while (++findIndex  <=  index)
                        {
                        current  = current->next;
                        }
                    if(current->next)
                        {//if current is last node, ->next is null
                        current->next->previous = current->previous;
                        }
                    current->previous->next = current->next;
                    }
            

                }
            }
                // return node to system
                if (current)
                    {
                    current->next = NULL;
                    current->previous = NULL;
                    delete current;
                    current = NULL;
                    }    
    return true;
}

/**
 * Retrieve a list item by position
 * @pre 1 <= index <= getLength()
 * @post If 1 <= index <= getLength(), dataItem is the value of the
 *          desired item
 * @param index the list position to retrieve from
 * @param dataItem The ListItemType retrieved from the list
 * @return true if valid index
 */
template <typename objType>
bool List<objType>::getObj(int index, objType& theObj) const
{
    if ((index < 1) || (index > getLength()))
    {
        return false;
    } else
    {//get pointer to node, then data in node
        ListNode *current = head;
        int findIndex = 1;
        while (++findIndex  <=  index)
            {
            current  = current->next;
            }
        theObj = current->theObj;
    }
    return true;
}

/**
 * set theObj in Node[index] to the passed object
 * @pre 1 <= index <= getLength()
 * @post Node[index].theObj set to param theObj
 * @param index the list position to retrieve address of theObj
 * @param theObj is objType data item
 * @return true if valid index, false if index NULL if bad index
 */
template <typename objType>
bool List<objType>::setObj(int index, objType& theObj) const
{
    ListNode *current = head;
    bool retVal = false;
    if ((index < 1) || (index > getLength()))
    {
        std::cout <<"Bad index, no change to theObj" << std::endl;
    } else
    {//get pointer to node, then data in node
        int findIndex = 1;
        while (++findIndex  <=  index)
            {
            current  = current->next;
            }
        current->theObj = theObj;
        retVal = true;
    }
    return retVal;    
}


/******************************************************************
*   operator <<
*     overloaded insertion operator for object data output
*     (useful for debugging execution)
*/

template <typename T>
std::ostream &operator<< (std::ostream &output, const List<T> &my_list)
{
   typename List<T>::ListNode *current = my_list.head;

   while (current != NULL)
   { 
      output << current->theObj << "\n\n";
      current = current->next;
   }
   return output;
}

#endif	/* LIST_H */

