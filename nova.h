/* 
 * nova.h
 * 
 * This file creates a class nova that supports deep copying, move semantics, glows as well as queries for minimum glow and maximum glow,
 * this class will also internally recharge its lumen objects when more than half of lumen object in nova is inactive and also replace persistenly
 * unstable lumen objects if they reach beyond the unstable threshold count
 * 
 */

#ifndef NOVA_H
#define NOVA_H

#include "lumen.h"

/* Class Invariants:
    * 1) size, power and brightness should never be negative
    * 2) Unstable Threshold is always constant
    * 3) Allocated an array of lumen objects in the heap
    * 4) Support deep copying and move semantics
    * 5) get maximum and minimum glows only by query without state and property changes
    * 6) glow function that takes a specific number of lumens while throwing exception if out of bounds
    * 7) Destructor deallocated all the objects in the array and itself after.
    * 8) Internal recharge, recharges stable lumen objects when more than half lumen objects are inactive in nova
    * 9) Replacing a lumen resets it to original power 
    * 10) Support addition for both types, including
        a. standard addition
        b. mixed-mode addition
        c. ++
        d. short-cut assignment
*/

class Nova
{
public:
    Nova(int brightness, int size, int power, int numLumens, Lumen** lumensInject);
    Nova() = default;
    ~Nova();
    void glow(int numLumens);
    int getMinGlow();
    int getMaxGlow();

    Nova(const Nova& other); // Copy constructor
    Nova& operator=(const Nova& other); // Copy assignment operator

    Nova(Nova&& other); // Move constructor
    Nova& operator=(Nova&& other); // Move assignment operator

    Nova operator+(const Nova& other) const;
    Nova operator+(int value) const;
    Nova& operator+=(const Nova& other);
    Nova& operator++();
    Nova operator++(int);
    bool operator==(const Nova& other) const;
    bool operator!=(const Nova& other) const;
    bool operator>(const Nova& other) const;
    bool operator<(const Nova& other) const;
    Nova operator-(const Nova& other) const;
    Nova& operator-=(const Nova& other);
    Nova& operator--();
    Nova operator--(int);
   




private:
    Lumen** lumens; // Dynamic array of Lumen* pointers
    int numLumens; // Number of lumens in the array
    void internalRecharge();
    void replaceUnstableLumens();
    const int UNSTABLE_THRESHOLD = 24;
};

#endif