/* 
 * nova.cpp
 * Name: Robert Widjaja
 * Date: 4/14/2023
 * Revision History: 4/14/2023 -- finished documentation
 * 
 * Platform: Windows
 * 
 * This program implements a nova class which uses a dynamic array for lumen subobjects. 
 * This program demonstrates allocating and deallocation of heap memory through the use of pointers and
 * supports deep copying as well as move semantics. It also uses dependency injection for nova to acquire its lumen subobjects 
 * via Dependency Injection
 * 
 * ASSUMPTIONS:
 *  1) Constructor is implemented correctly.
 *  2) Deconstructor deallocated array of lumen pointers.
 *  3) glow(numLumens), getMinGlow() and getMaxGlow() functions work as intended from the class.
 *  4) each lumen object in the lumen array is constructed correctly.
 *  5) Brightness, power and size are non-negative.
 * 
*/

#include "nova.h"
#include <iostream>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
using namespace std;


// Pre-Condtion: brightness, size and power should not be negative, lumen subobjects are injected into Nova
// Post-Condition: first lumen is initialized along with the rest of the lumens for nova
Nova::Nova(int brightness, int size, int power, int numLumens, Lumen** lumensInject)
{
    if (power < 0 || brightness < 0 || size <= 0 || numLumens < 0)
    {
        throw std::invalid_argument("Values must be non-negative!");
    }
    this->numLumens = numLumens;

    lumens = lumensInject;

    // Create the first lumen object with specified brightness and size
    Lumen *firstLumen = new Lumen(brightness, size, power);
    lumens[0] = firstLumen;
    // Generate remaining lumen objects with random values
    for (int i = 1; i < numLumens; i++)
    {
        int b = (i * 123) % 600;
        int s = (i * 123) % 20;
        int p = (i * 123) % 400;

        Lumen *l = new Lumen(b, s, p);
        lumens[i] = l;
    }
}
// Private utility for copying
// Pre-condition: Assumes that 'other' is a valid 'Nova' object.
// Post-condition: Creates a new 'Nova' object with the same 'numLumens' and copied 'Lumen' objects.
Nova::Nova(const Nova& other)
{
    // Allocate memory for lumens array
    this->numLumens = other.numLumens;
    lumens = new Lumen*[numLumens];

    // Copy Lumen objects from 'other' to current object
    for (int i = 0; i < numLumens; i++)
    {
        lumens[i] = new Lumen(*other.lumens[i]);
    }
}

// Overloaded assignment operator
// Pre-condition: Assumes that 'other' is a valid 'Nova' object.
// Post-condition: Copies the 'numLumens' and 'Lumen' objects from 'other' to the current object.
Nova& Nova::operator=(const Nova& other)
{
    if (this == &other)
    {
        // If self-assignment, return current object
        return *this;
    }

    // Deallocate existing memory
    for (int i = 0; i < numLumens; i++)
    {
        delete lumens[i];
    }
    delete[] lumens;

    // Copy from other object
    this->numLumens = other.numLumens;
    lumens = new Lumen*[numLumens];
    for (int i = 0; i < numLumens; i++)
    {
        lumens[i] = new Lumen(*other.lumens[i]);
    }

    return *this;
}

// Move constructor supports efficient call by value
// Pre-condition: Assumes that 'other' is a valid 'Nova' object.
// Post-condition: Creates a new 'Nova' object by moving the 'numLumens' and 'Lumen' objects from 'other' to the current object.
Nova::Nova(Nova&& other)
{
    this->numLumens = other.numLumens;
    this->lumens = other.lumens;
    other.numLumens = 0;
    other.lumens = nullptr;
}

// Move assignment exchanges ownership
// Pre-condition: Assumes that 'other' is a valid 'Nova' object.
// Post-condition: Moves the 'numLumens' and 'Lumen' objects from 'other' to the current object.
Nova& Nova::operator=(Nova&& other)
{

    // Move from other object
    swap(numLumens, other.numLumens);

    swap(lumens, other.lumens);

    if (this == &other)
    {
        // If self-assignment, return current object
        return *this;
    }

    return *this;
}

//Pre-Condition: None
//Post-Condition: Deallocates/free the memory allocated for lumen array
Nova::~Nova() 
{
    // Deallocate memory for lumens array and each Lumen object
    for (int i = 0; i < this->numLumens; i++) {
        delete this->lumens[i]; // Delete each Lumen object
    }
    delete[] this->lumens; // Delete the array of pointers
}

// Pre-Condition: Throw exception when number of Lumens to glow is greater than the amount nova has or negative number
// Post-Condtion: Glows specified amount of lumens 
void Nova::glow(int numLumenGlow)
{
    if(numLumenGlow > numLumens || numLumenGlow < 0)
    {
        throw std::invalid_argument("Number of lumens to glow exceeds size or below 0");
    }

    replaceUnstableLumens();
    internalRecharge();
    for (int i = 0; i < numLumenGlow; i++)
    {
        lumens[i]->glow();
        int glowVal = lumens[i]->glow();
    }
}

// Get the minimum glow value across all Lumen subobjects
// Pre-condition: None
// Post-condition: Gets the query of the minimum glow without changing states.
int Nova::getMinGlow()
{
    int minGlow = INT_MAX; // Initialize to maximum possible int value
    for (int i = 0; i < numLumens; i++)
    {
        int glow = lumens[i]->glowQuery(); //No state change in Lumen Object, no reduction in power, size or brightness too
        if (glow < minGlow)
        {
            minGlow = glow;
        }
    }
    return minGlow;
}

// Get the maximum glow value across all Lumen subobjects
// Pre-condition: None
// Post-condition: Gets the query of the maximum glow without changing states.
int Nova::getMaxGlow()
{
    int maxGlow = INT_MIN; // Initialize to minimum posssible int value
    for (int i = 0; i < numLumens; i++)
    {
        int glow = lumens[i]->glowQuery(); //No state change in Lumen Object, no reduction in power, size or brightness too  
        if (glow > maxGlow)
        {
            maxGlow = glow;
        }
    }
    return maxGlow;
}

// Helper method to check if half of lumens is inactive, then internally recharge
// Pre-condition: None
// Post-Condition: recharges when more than half of the lumens are inactive.
void Nova::internalRecharge()
{

    int inactiveCount = 0;
    int rechargeCount = 0;
    for (int i = 0; i < numLumens; i++)
    {
        if (!lumens[i]->getActive())
        {
            inactiveCount++;
        }
    }
    

    // Recharge lumen subobjects when more than half are inactive
    if (inactiveCount > numLumens / 2)
    {
        for (int i = 0; i < numLumens; i++)
        {
            if (lumens[i]->isStable())
            {
                lumens[i]->recharge();
                rechargeCount++;
            }
        }
    }
}

// Pre-condition: None
// Post-Condition: replace simply resets the lumen objects to its orginal form when its been unstable for 10 times when glow is called
void Nova::replaceUnstableLumens()
{
    for(int i = 0; i < numLumens; i++)
    {
        if(lumens[i]->getUnstableCount() > UNSTABLE_THRESHOLD)
        {
            lumens[i]->reset();
        }
    }
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns a new 'Nova' object with 'Lumen' objects that are the result of addition between the current and 'other' objects' lumens
Nova Nova::operator+(const Nova& other) const {
    Nova result(*this);
    for(int i = 0; i < numLumens; ++i) {
        *result.lumens[i] = *this->lumens[i] + *other.lumens[i];
    }
    return result;
}

// Pre-Condition: None
// Post-Condition: Returns a new 'Nova' object with 'Lumen' objects that are the result of adding the specified value to each lumen in the current object
Nova Nova::operator+(int value) const {
    Nova result(*this);
    for(int i = 0; i < result.numLumens; i++)
    {
        result.lumens + value;
    }
    return result;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Adds the 'Lumen' objects of 'other' to the current object's lumenss
Nova& Nova::operator+=(const Nova& other) {
    for(int i = 0; i < numLumens; ++i) {
        *this->lumens[i] += *other.lumens[i];
    }
    return *this;
}

// Pre-Condition: None
// Post-Condition: Increments the brightness, size, and power of each lumen in the current object
Nova& Nova::operator++() {
    for(int i = 0; i < numLumens; ++i) {
        ++(*this->lumens[i]);
    }
    return *this;
}

// Pre-Condition: None
// Post-Condition: Increments the brightness, size, and power of each lumen in the current object (postfix)
Nova Nova::operator++(int) {
    Nova copy(*this);
    ++(*this);
    return copy;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns true if the current object is equal to 'other', false otherwise
bool Nova::operator==(const Nova& other) const {
    if (this->numLumens != other.numLumens) return false;
    for(int i = 0; i < numLumens; ++i) {
        if (*this->lumens[i] != *other.lumens[i]) return false;
    }
    return true;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns true if the current object is not equal to 'other', false otherwise
bool Nova::operator!=(const Nova& other) const {
    return !(*this == other);
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns true if the current object is greater than 'other', false otherwise
bool Nova::operator>(const Nova& other) const {
    if (this->numLumens != other.numLumens) return this->numLumens > other.numLumens;
    for(int i = 0; i < numLumens; ++i) {
        if (*this->lumens[i] != *other.lumens[i]) return *this->lumens[i] > *other.lumens[i];
    }
    return false;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns true if the current object is less than 'other', false otherwise
bool Nova::operator<(const Nova& other) const {
    if (this->numLumens != other.numLumens) return this->numLumens < other.numLumens;
    for(int i = 0; i < numLumens; ++i) {
        if (*this->lumens[i] != *other.lumens[i]) return *this->lumens[i] < *other.lumens[i];
    }
    return false;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Returns a new 'Nova' object with 'Lumen' objects that are the result of subtraction between the current and 'other' objects' lumens
Nova Nova::operator-(const Nova& other) const {
    Nova result(*this);
    for(int i = 0; i < numLumens; ++i) {
        if((*this->lumens[i] > *other.lumens[i]) )
        {
            *result.lumens[i] = *this->lumens[i] - *other.lumens[i];
        }
    }
    return result;
}

// Pre-Condition: Assumes that 'other' is a valid 'Nova' object
// Post-Condition: Subtracts the 'Lumen' objects of 'other' from the current object's lumens
Nova& Nova::operator-=(const Nova& other) {
    for(int i = 0; i < numLumens; ++i) {
        *this->lumens[i] -= *other.lumens[i];
    }
    return *this;
}

// Pre-Condition: None
// Post-Condition: Decrements the brightness, size, and power of each lumen in the current object
Nova& Nova::operator--() {
    for(int i = 0; i < numLumens; ++i) {
        --(*this->lumens[i]);
    }
    return *this;
}

// Pre-Condition: None
// Post-Condition: Decrements the brightness, size, and power of each lumen in the current object (postfix)
Nova Nova::operator--(int) {
    Nova copy(*this);
    --(*this);
    return copy;
}



/*
 * IMPLEMENTATION INVARIANTS: 
 * 
 * Constructor throws exceptions if brightness, size or power is negative
 * If glow(numLumens) parameter goes out of bounds, throw an exception
 * Supports Deep Copying as well as define move semantics
 * Internal recharge is called every glow to check if more than half of lumen objects is inactive.
 * Replacing is simply resetting the lumen object back to its original, by implementing a unstable count
 * in lumen and using an unstable threshold in nova.
 * Generate numbers for the remaining lumen objects by a pattern
 * getMinGlow() and getMaxGlow() only gets the query but does not change the state and properties of lumen object.
 * Support addition for both types, including
    a. standard addition
    b. mixed-mode addition
    c. ++
    d. short-cut assignment
 */