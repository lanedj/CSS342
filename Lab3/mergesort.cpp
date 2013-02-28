/******************************************************************
*   @file mergesort.cpp
 *  @author Derek Lane
 *  @author from code provided by Dr Stewart
 *  @date 021913
 * 
*   code for mergesort implementation of integer arrays
 * 
 *  no changes were made to 'main' all other code updated
 * 
*   note: since the user sends "size" of the array to the
 *  mergesort function instead of the actual number for the last index
 *  the mergesort function has been re-written as wrapper for 
 *  mergesortPrivate in order to decrement the input from size to the final
 *  index of the array. automatically decrementing it every time made the code
 *  all kinds of wonkey.
 * 
 *  Valgrind results (interpretation follows below):
    [lanedj@uw1-320-21 newtest]$ valgrind ./a.out
    ==4184== Memcheck, a memory error detector
    ==4184== Copyright (C) 2002-2009, and GNU GPL'd, by Julian Seward et al.
    ==4184== Using Valgrind-3.5.0 and LibVEX; rerun with -h for copyright info
    ==4184== Command: ./a.out
    ==4184==
    0  1  2  3  4  5
    ==4184==
    ==4184== HEAP SUMMARY:
    ==4184==     in use at exit: 0 bytes in 0 blocks
    ==4184==   total heap usage: 25 allocs, 25 frees, 8,884 bytes allocated
    ==4184==
    ==4184== All heap blocks were freed -- no leaks are possible
    ==4184==
    ==4184== For counts of detected and suppressed errors, rerun with: -v
    ==4184== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 15 from 8)
 *  
 *  These valgrind results mean that my program made 25 allocations and freed 
 *  all 25 of them, thus no memory leaks are present.
*/

#include <iostream>
#include <fstream>

using namespace std;



/******************************************************************
*   read
*     read array data file
*/

void read(int **arr, int *size)
{
   int inVal, *tmp;
   ifstream fin("input.txt");
    while(fin.good()) // will catch the eof
    {
      fin >> inVal; // read a value
      if (fin.fail()) // bad input or other error, does not read eof bit
        {
        break; // or return/throw exception/destroy all evidence
        }     
      tmp = new int [++(*size)]; // enlarge temp array
      tmp[*size-1] = inVal;   // store new value @ end of temp
      if (*size > 1) // copy array -> temp only if size > 1
          {
             for (int i = 0; i < *size - 1; ++i)
                tmp[i] = (*arr)[i];  // load temp with current array
             delete [] *arr;   // throw away current data store
          }
      *arr = new int [*size]; // create expanded data store
      for (int i = 0; i < *size; ++i)
         (*arr)[i] = tmp[i];  // copy temp contents into array
      delete [] tmp; // toss temp version of data
       }
} 

/******************************************************************
*   write
*     write resulting array data
*/

void write(int *arr, int first, int size)
{
   for (int i = first; i < size; ++i)
   {
      cout << arr[i] << "  ";
   }
   cout << endl;
}


/******************************************************************
*  merge
*  merges two sorted subarrays so that result is sorted least to greatest
 * 
 * @param &subjArray is the address of the full array to be sorted
 * @param firstIndex is the first entry in the current sub-array
 * @param lastIndex is the final entry in the current sub-array
 * 
 * @pre subjArray exists
 *      subjArray[firstIndex] to subjArray[midpoint] sorted least->greatest
 *      subjArray[midpoint+1] to subjArray[lastIndex] sorted least->greatest
 * @post subjArray[firstIndex] to subjArray[lastIndex] sorted least->greatest
 *     
*/

void merge(int *subjArray, int firstIndex, int lastIndex)
{
    int indexPrime; //track writing point in tempArray
    int index1, index2; //track current candidate in left and right arrays
    int midPoint;//find and store midpoint of arrays to combine
    int *tempArray; //becomes pointer to first record in array of ints
    
    //set indices to their start locations
    indexPrime = 0;
    //adding 1 to the equation for midPoints deals with the problem of arrays
    //with only 2 records. 
    midPoint = (((firstIndex + lastIndex)/2))+1;
    index1 = firstIndex;
    index2 = midPoint;
    
    //create tempArray equal to size of current subArray
    tempArray = new int [lastIndex-firstIndex+1]; //create temporary array 
 
    while (indexPrime <= lastIndex)
    {       
        //if true there are items to sort in left and right arrays
        if ((index1 < midPoint) && (index2 <= lastIndex))
            {
           if (subjArray[index1] < subjArray[index2])//left is less
                {
                 tempArray[indexPrime] = subjArray[index1];
                 index1++;
                } else //right is less
                {
                 tempArray[indexPrime] = subjArray[index2];
                 index2++;
                }
        }else if (index1 < midPoint) //if true there are only items in left
            {
           tempArray[indexPrime] = subjArray[index1];
           index1++;
        } else if (index2 <= lastIndex)//remaining in right only
               {
                tempArray[indexPrime] = subjArray[index2];
                index2++;
                }
      indexPrime++; //increment insert point to next record
    }
    
    //tempArray is now a sorted version of subjArray so
    //reset indexPrime to 0 and index1 to firstIndex
    indexPrime = 0;
    index1 = firstIndex;
    
    //loop to write recored from sorted tempArray over records subjArray
    while (index1 <= lastIndex)
    {
        subjArray[index1++] = tempArray[indexPrime++];
    }
    delete [] tempArray;
} 

 
 /******************************************************************
*  mergesortPrivate
 * this method is inside a wrapper because the user initiates the sort
 * with 'size' instead of lastIndex (which is size-1) but decrementing
 * 'size' in each of the recursive calls causes issues. 
 * 
 * Recursive function, finds midpoint of given span in an array then calls
 * mergesortPrivate on first to midpoint and midpont + 1 to last
 * 
 * Basecase: first == last which means array is single record
 * 
 * @param &subjArray is the address of the full array to be sorted
 * @param firstIndex is the first entry in the current sub-array
 * @param lastIndex  is last entry in sub-array
 * 
 * @pre subjArray exists, firstIndex <= lastIndex
 * @post if baseCase calls merge and starts moving back up recursion stack
 *       subjArray records firstIndex through lastIndex will be sorted least
 *       to greatest
 * @post if not baseCase splits subjArray and call mergesort on each half
 *         
*/

void mergesortPrivate(int *subjArray, int firstIndex, int lastIndex)
{
   if (firstIndex < lastIndex) //if not basecase of firstIndex = lastIndex
   {
       int midpoint = ((firstIndex + lastIndex)/2);
       
       //left side of array
       mergesortPrivate (subjArray, firstIndex, midpoint);
       
       //right side of array
       mergesortPrivate (subjArray, midpoint+1, lastIndex);
   }
   //recombine the sorted left and right sub arrays
   merge(subjArray,firstIndex,lastIndex);
 }

/******************************************************************
*  mergesort
 * 
 * This is a wrapper that calls mergesortPrivate because the user wants
 * to enter size instead of the final index, but we only want to decrement
 * size once not every time the method recurses.
 * */

void mergesort(int *subjArray, int firstIndex, int size)
{
  mergesortPrivate(subjArray, firstIndex, size-1);
}


/******************************************************************
*   main
*     read input; call mergesort to sort it out
*/

int main()
{
   int size = 0, *arr;

   // input data
   read(&arr, &size);

   // sorting
   mergesort(arr, 0, size);
   
   // output
   write(arr, 0, size);
   
   // cleanup
   delete [] arr;
}

