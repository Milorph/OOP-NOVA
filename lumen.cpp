/* 
 * Lumen.cpp
 * Name: Robert Widjaja
 * Date: 4/14/2023
 * Revision History: 4/4/2023 -- finished documentation, 4/14/2023 -- added recharge functionality, 
 * added glowQuery functionality, added getUnstableCount functionality, also made pre and post conditions more readable by putting
 * them outside of functions (CORRECTION FROM P1)
 * 
 * Platform: Windows
 * 
 * This program encapsulates the state of a Lumen object and allows the client to see only the glow value from the .glow() method.
 * It does not allow for the modification of internal properties such as power, brightness and size. The .glow() method updates the
 * Lumen object's internal state and returns the glow value. The .reset() method reverts each interal state back to its original state.
 * glowQuery() simulates the glow function but doesn't change the states and properties of the object.
 * recharge() simply makes power revert back to orginal and change states back to active
 * getUnstableCount() is used to get persistently unstable lumen objects to replace in nova class
 * 
 * ASSUMPTIONS:
 *  1) Constructor is implemented correctly.
 *  2) Random function randomizes the values evenly.
 *  3) glow() and reset() functions work as intended from the class.
 *  4) each object in the array is constructed correctly.
 *  5) Brightness, power and size are non-negative.
 * 
*/

#include "lumen.h"
#include <iostream>
#include <stdexcept>
using namespace std;


// Constructor
// Pre-Condition: If any values are negative, throw an exception printing to the client that values inputted must not be negative.
// Post-Condition: values for each properties are set. Lumen is initially active.
Lumen::Lumen(int b, int s, int p)
    : originalBrightness(b), originalPower(p), brightness(b), size(s), power(p), isActive(true), glowCount(0), unstableCount(0)
{
    if (p < 0 || b < 0 || s <= 0)
    {
        throw std::invalid_argument("Values must be non-negative!");
    }

    POWER_THRESHOLD = (int)(originalPower * 0.4 / size);
    STABLE_THRESHOLD = (int)(originalPower * 0.5 / size);
    DIMNESS_VALUE = (int)(originalPower * 0.21 - POWER_THRESHOLD * 0.01 - brightness * size * 0.001);
    maxReset = size*3;
}

Lumen::Lumen()
    : originalBrightness(0), originalPower(0), brightness(0), size(0), power(0), isActive(false), glowCount(0), unstableCount(0), POWER_THRESHOLD(0), STABLE_THRESHOLD(0), DIMNESS_VALUE(0), maxReset(0)
{
}


// Glow method
// Pre-Conditon: None
// Post-Condition: glow value is returned depending on the state of the lumen object
int Lumen::glow()
{
    glowCount++;
    int glowValue = calculateGlowValue();
    return glowValue;
}

// Reset method
// Pre-Condtion: None
// Post-Condition: Reverts the objects state to original if valid, else decrease brightness
bool Lumen::reset()
{
    if (glowCount >= RESET_THRESHOLD && power > 0)
    {
        if(resetCount >= maxReset) return false;
        resetOriginal();
        return true;
    }
    else
    {
        changeBrightness();
        return false;
    }
}

// Getter to get persistently unstable lumens
// Pre-Condition: None
// Post-Condtion: Returns a count of how many times lumen is unstable
int Lumen::getUnstableCount()
{
    return unstableCount;
}

// Simulates the glow but doesn't change the state
// Pre-Condtion: None
// Post-Condition: Returns glow value but doesn't change the state of the object
int Lumen::glowQuery() {
    
    int tempPower = power;
    bool tempIsActive = isActive;

    tempPower -= (int)(0.35 * tempPower);
    if (tempPower < POWER_THRESHOLD) tempIsActive = false;

    if (!tempIsActive) {
        return DIMNESS_VALUE;
    } else if (isStable()) {
        return brightness * size;
    } else {
        return getErraticPower();
    }
}

// Helper method for resetting to original state
// Pre-Condition: None
// Post-Condition: set objects state back to its original form
void Lumen::resetOriginal()
{
    resetCount += 1;
    brightness = originalBrightness;
    power = originalPower;
    isActive = true;
    glowCount = 0;
}

// Helper method for decreasing brightness after invalid reset
// Pre-Condition: None
// Post-Condition: Decrement brightness by 10% of its value
void Lumen::changeBrightness()
{
    brightness -= (int)(0.1 * brightness);
}

// Helper method for getting erratic power value
// Pre-Condition: None
// Post-Conditon: returns a random number associated with the power of the object using its power brightness and size
int Lumen::getErraticPower()
{
    return power * brightness / size;
}

// Helper method for checking stability
// Pre-Conditon: None
// Post-Condition: returns if object is stable or unstable by boolean value
bool Lumen::isStable()
{
    return power > STABLE_THRESHOLD && power > POWER_THRESHOLD; // Make it single line (CORRECTION FROM P1)

}

// Helper method for calculating glow value
/* 
    * This method is used to decompose the glow() method for resusablity.
    * This calculate the glow value based on its current state and changes
    * the states of the object if it goes below a threshold.
*/
// Pre-Condition: None
// Post-condition: returns a glow value associated with state of lumen object
int Lumen::calculateGlowValue()
{
    power -= (int)(0.35 * power);
    if(power < POWER_THRESHOLD) isActive= false;

    if(!isActive)
    {
        unstableCount++;
        return DIMNESS_VALUE;
    }

    else if (isStable())
    {
        return brightness * size;
    }
    else
    {
        unstableCount++;
        return getErraticPower();
    }
}

bool Lumen::getActive()
{
    return isActive;
}

// Pre-Condition: None
// Post-Condition: Multiplies power by 6
void Lumen::recharge()
{
    if(isStable()){
        power = originalPower;
        if(power > POWER_THRESHOLD)
        {
            isActive = true;
        }
    }
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Copies the properties of 'other' to the current lumen object
Lumen& Lumen::operator=(const Lumen& other) {
    if(this != &other) {
        brightness = other.brightness;
        size = other.size;
        power = other.power;
    }
    return *this;
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns a new 'Lumen' object with the sum of the properties of the current and 'other' lumen objects
Lumen Lumen::operator+(const Lumen& other) const 
{
     return Lumen(brightness + other.brightness, size + other.size, power + other.power); 

}

// Pre-Condition: None
// Post-Condition: Returns a new 'Lumen' object with the sum of the current lumen object's properties and the specified value
Lumen Lumen::operator+(int value) const {
    return Lumen(brightness + value, size, power + value);
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Adds the properties of 'other' to the current lumen object
Lumen& Lumen::operator+=(const Lumen& other) {
    brightness += other.brightness;
    size += other.size;
    power += other.power;
    return *this;
}

// Pre-Condition: None
// Post-Condition: Increments the brightness, size, and power of the current lumen object
Lumen& Lumen::operator++() {
    ++brightness;
    ++size;
    ++power;
    return *this;
}

// Pre-Condition: None
// Post-Condition: Increments the brightness, size, and power of the current lumen object (postfix)
Lumen Lumen::operator++(int) {
    Lumen copy(*this);
    ++(*this);
    return copy;
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns true if the current lumen object is equal to 'other', false otherwise
bool Lumen::operator==(const Lumen& other) const 
{ 
    return brightness == other.brightness && size == other.size && power == other.power; 
    
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns true if the current lumen object is not equal to 'other', false otherwise
bool Lumen::operator!=(const Lumen& other) const 
{ 
    return !(*this == other); 

}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns true if the current lumen object is greater than 'other', false otherwise
bool Lumen::operator>(const Lumen& other) const 
{ 
    return brightness > other.brightness && size > other.size && power > other.power; 

}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns true if the current lumen object is less than 'other', false otherwise
bool Lumen::operator<(const Lumen& other) const {
     return brightness < other.brightness && size < other.size && power < other.power; 
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Returns a new 'Lumen' object with the difference of the properties of the current and 'other' lumen objects
Lumen Lumen::operator-(const Lumen& other) const 
{ 
    int newBrightness = brightness - other.brightness > 0 ? brightness - other.brightness : 0;
    int newSize = size - other.size > 0 ? size - other.size : 0;
    int newPower = power - other.power > 0 ? power - other.power : 0;

    return Lumen(newBrightness, newSize, newPower); 
}

// Pre-Condition: Assumes that 'other' is a valid 'Lumen' object
// Post-Condition: Subtracts the properties of 'other' from the current lumen object
Lumen& Lumen::operator-=(const Lumen& other) {
    brightness -= other.brightness;
    size -= other.size;
    power -= other.power;
    return *this;
}

// Pre-Condition: None
// Post-Condition: Decrements the brightness, size, and power of the current lumen object
Lumen& Lumen::operator--() {
    --brightness;
    --size;
    --power;
    return *this;
}

// Pre-Condition: None
// Post-Condition: Decrements the brightness, size, and power of the current lumen object (postfix)
Lumen Lumen::operator--(int) {
    Lumen copy(*this);
    --(*this);
    return copy;
}

/*
 * IMPLEMENTATION INVARIANTS: 
 * 
 * Thresholds take into account brightness, size and power
 * Power below power threshold cause state transition to inactive
 * Power above stable threshold is active and stable
 * Recharge changes power back to its original or max power
 * Calculate the glow value in seperate function and call it in the glow() function
 * Reset only works if glow has been called at least 5 times
 * glowQuery() doesn't change state and just simulates glow()
 * Support addition for both types, including
    a. standard addition
    b. mixed-mode addition
    c. ++
    d. short-cut assignment
 */