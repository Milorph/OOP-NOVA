/*
 * P4.cpp
 * Name: Robert
 * Date: 5/19/2023
 * Revision History -- 5/19/2023 -> Finished Documentation
 *
 * Description:
 * This driver program tests the functionality of the Lumen and Nova classes by
 * performing various operations and verifying the expected results. It includes
 * tests for the overloaded operators in both classes, as well as testing move
 * semantics for the Nova class.
 *
 * Client Expectations:
 * - The client expects the Lumen and Nova classes to correctly implement the
 *   required functionality and behavior.
 * - The client expects the Lumen class to support operators such as assignment,
 *   addition, subtraction, increment, decrement, and comparison.
 * - The client expects the Nova class to support operators such as assignment,
 *   addition, subtraction, increment, decrement, comparison, and move semantics.
 * - The client expects all functions and operators to behave as specified in the
 *   documentation, ensuring correct results and maintaining object integrity.
 * - The client expects the driver program to output the results of the tests and
 *   verify that the classes function as expected.
 *
 */



#include "nova.h"
#include "lumen.h"
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <vector>

using namespace std;

void testLumenOperators()
{
    std::cout << "\nNOW TESTING LUMEN OPERATORS..." << std::endl;
    srand(time(NULL)); // Initialize random seed

    std::vector<std::unique_ptr<Lumen>> lumenVec;

    // Create 10 unique_ptr Lumens with arbitrary values
    for(int i = 0; i < 10; ++i) {
        int b = rand() % 100 + 1;
        int s = rand() % 100 + 1;
        int p = rand() % 100 + 1;
        lumenVec.push_back(std::make_unique<Lumen>(b, s, p));
    }

    // Test overloaded operators with unique_ptr
    *lumenVec[0] = *lumenVec[1];  // Assignment operator
    *lumenVec[2] = *lumenVec[3] + *lumenVec[4];  // Addition operator
    *lumenVec[1] = *lumenVec[2] - *lumenVec[3]; // Subtraction operator
    (*lumenVec[5])++;  // Increment operator
    (*lumenVec[6])--;  // Decrement operator
    *lumenVec[7] += *lumenVec[8];  // Addition assignment operator
    *lumenVec[9] -= *lumenVec[1];  // Subtraction assignment operator

    // Test results with glow() function
    for(std::vector<std::unique_ptr<Lumen>>::iterator lumen = lumenVec.begin(); lumen != lumenVec.end(); ++lumen) {
        std::cout << "Glow value: " << (*lumen)->glow() << std::endl;
        (*lumen)->reset();
    }

    std::vector<std::shared_ptr<Lumen>> sharedLumenVec;

    // Create 10 shared_ptr Lumens with arbitrary values
    for(int i = 0; i < 10; ++i) {
        int b = rand() % 100 + 1;
        int s = rand() % 100 + 1;
        int p = rand() % 100 + 1;
        sharedLumenVec.push_back(std::make_shared<Lumen>(b, s, p));
    }

    // Test overloaded operators with shared_ptr
    *sharedLumenVec[0] = *sharedLumenVec[1];  // Assignment operator
    *sharedLumenVec[2] = *sharedLumenVec[3] + *sharedLumenVec[4];  // Addition operator
    (*sharedLumenVec[5])++;  // Increment operator
    (*sharedLumenVec[6])--;  // Decrement operator
    *sharedLumenVec[7] += *sharedLumenVec[8];  // Addition assignment operator
    *sharedLumenVec[9] -= *sharedLumenVec[1];  // Subtraction assignment operator

    // Test results with glow() function
    for(std::vector<std::shared_ptr<Lumen>>::iterator lumen = sharedLumenVec.begin(); lumen != sharedLumenVec.end(); ++lumen) {
        std::cout << "Glow value: " << (*lumen)->glow() << std::endl;
        (*lumen)->reset();
    }
}

void testNovaOperators()
{
    srand(time(NULL)); // Initialize random seed
    std::cout << "\nNOW TESTING NOVA OPERATORS..." << std::endl;

    // Create 2 arrays of Lumen pointers for Nova
    Lumen** lumens1 = new Lumen*[12];
    Lumen** lumens2 = new Lumen*[12];
    for (int i = 0; i < 12; ++i) {
        lumens1[i] = new Lumen(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
        lumens2[i] = new Lumen(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
    }

    // Create 2 Nova instances for testing
    Nova nova1(100, 10, 30, 12, lumens1);
    Nova nova2(50, 5, 20, 12, lumens2);

    
    // Test overloaded operators
    nova1 = nova2;  // Assignment operator
    nova1 = nova1 + nova2;  // Addition operator
    nova2 = nova1 - nova2; // Subtraction operator
    nova1 = nova1 + 10;  // Addition operator with int
    ++nova1;  // Increment operator (prefix)
    nova1++;  // Increment operator (postfix)
    std::cout << "EQUALITY: " << (nova1 == nova2) << std::endl; // Equality operator
    std::cout << "INEQUALITY: " << (nova1 != nova2) << std::endl; // Inequality operator
    std::cout << "GREATER THAN: " << (nova1 > nova2) << std::endl; // Greater than operator
    std::cout << "LESS THAN: " << (nova1 < nova2) << std::endl; // Less than operator
    nova1 -= nova2;  // Subtraction assignment operator
    --nova1;  // Decrement operator (prefix)
    nova1--;  // Decrement operator (postfix)
    
    // Test results with glow() function
    std::cout << "Glow nova 1 max value: " << nova1.getMaxGlow() << " | Glow nova 1 min value:" << nova1.getMinGlow() << std::endl;
    
    std::cout << "Glow nova 2 max value: " << nova2.getMaxGlow() << " | Glow nova 2 min value:" << nova2.getMinGlow() << std::endl;
}

void testNovaMoveSemantics() {

    std::cout << "\nTesting MOVE SEMANTICS..." << std::endl;
    srand(time(NULL)); // Initialize random seed

    std::vector<std::unique_ptr<Nova>> novas;

    // Create 2 arrays of Lumen pointers for Nova
    Lumen** lumens1 = new Lumen*[12];
    Lumen** lumens2 = new Lumen*[12];
    for (int i = 0; i < 12; ++i) {
        lumens1[i] = new Lumen(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
        lumens2[i] = new Lumen(rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1);
    }

    // Create Nova instances and add them to the vector
    novas.push_back(std::make_unique<Nova>(100, 10, 30, 12, lumens1));
    novas.push_back(std::make_unique<Nova>(50, 5, 20, 12, lumens2));

    std::cout << "Before moving:\n";
    for (std::vector<std::unique_ptr<Nova>>::iterator nova = novas.begin(); nova != novas.end(); ++nova) {
        std::cout << "Glow value: " << (*nova)->getMaxGlow() << std::endl;
    }

    std::vector<std::unique_ptr<Nova>> movedNovas;
    movedNovas.push_back(std::move(novas[0])); // Move nova from novas to movedNovas

    // Clear novas[0] as it's now a dangling pointer
    novas.erase(novas.begin());

    std::cout << "After moving:\n";
    for (std::vector<std::unique_ptr<Nova>>::iterator nova = movedNovas.begin(); nova != movedNovas.end(); ++nova) {
        std::cout << "Glow value: " << (*nova)->getMaxGlow() << std::endl;
    }
    
}


int main(){
  testLumenOperators();
  testNovaOperators();
  testNovaMoveSemantics();
  
}