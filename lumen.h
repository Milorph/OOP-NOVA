/* 
 * Lumen.h
 * 
 * This file creates a class Lumen, that has properties such as power, brightness and size.
 * This program demonstrates the use of encapsulation to hide internal state and provides methods to interact with those states,
 * it simply encapsulates state and behavior to provide better resuability.
 * 
 */
#ifndef LUMEN_H
#define LUMEN_H

/* Class Invariants:
    * 1) size, power and brightness should never be negative
    * 2) Thresholds are always positive
    * 3) Thresholds are associated with size
    * 4) Dimness is asscoiated with brightness
    * 5) orignalBrightness and originalPower stores whats initally intitalized
    * 6) getters are used for unit testing only to check state transitions.
    * 7) glowcount keeps track of the counter for glow requests
    * 8) unstable count keeps track of the counter of how many times glow is unstable
    * 9) glowQuery simulates glow but doesn't change the states
    * 10) Reset Threshold is 5
    * 11) Support addition for both types, including
        a. standard addition
        b. mixed-mode addition
        c. ++
        d. short-cut assignment
*/

class Lumen {
    int originalBrightness;
    int originalPower;
    int brightness;
    int size;
    int power;
    int glowCount;
    int unstableCount;
    bool isActive;
    const int RESET_THRESHOLD = 5;
    int maxReset;
    int resetCount;
    int POWER_THRESHOLD;
    int STABLE_THRESHOLD;
    int DIMNESS_VALUE;

public:
    // Constructor
    Lumen(int b, int s, int p);
    Lumen();
    // Method prototypes
    int glow();
    bool reset();
    int getUnstableCount();
    int glowQuery();
    bool getActive();
    void recharge();
    bool isStable();

    // Overloaded operators
    Lumen& operator=(const Lumen& other);
    Lumen operator+(const Lumen& other) const;
    Lumen operator+(int value) const;

    Lumen& operator+=(const Lumen& other);
    Lumen& operator++();
    Lumen operator++(int);
    bool operator==(const Lumen& other) const;
    bool operator!=(const Lumen& other) const;
    bool operator>(const Lumen& other) const;
    bool operator<(const Lumen& other) const;
    Lumen operator-(const Lumen& other) const;
    Lumen& operator-=(const Lumen& other);
    Lumen& operator--();
    Lumen operator--(int);


private:
    // Helper methods
    void resetOriginal();
    void changeBrightness();
    int getErraticPower();
    int calculateGlowValue();
};

#endif // LUMEN_H