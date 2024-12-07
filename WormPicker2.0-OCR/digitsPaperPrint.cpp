#include <iostream>
#include <vector>

int digitsPaperPrint() {
    // Use a fixed seed for reproducibility
    std::srand(8);

    // Vector to store the random numbers
    std::vector<int> randomNumbers;

    // Generate 20 random 6-digit numbers (5 digit number)
    for (int i = 0; i < 20; ++i) {
        int randomNumber = 10000 + std::rand() % 90000;
        randomNumbers.push_back(randomNumber);
    }

    // Print the generated numbers
    std::cout << "Generated 6-digit random numbers:\n";
    for (const int& number : randomNumbers) {
        std::cout << number << std::endl;
    }

    return 0;
}