// Define the limit for the sieve algorithm (adjust for performance and memory constraints)
#define LIMIT 100000000  

// Statically allocated array to store primality status of numbers up to LIMIT
// Using static allocation avoids dynamic memory management overhead
static char is_prime[LIMIT];

/**
 * Function: sieve_of_eratosthenes
 * --------------------------------
 * Implements the Sieve of Eratosthenes algorithm to find all prime numbers up to `limit`.
 *
 * Parameters:
 *   limit - The upper bound up to which primes are calculated.
 *
 * Algorithm:
 *   - Initializes all numbers as prime (1).
 *   - Marks 0 and 1 as non-prime (0).
 *   - Iterates through numbers and marks multiples of primes as non-prime.
 *   - Counts the total number of primes found (though unused in this version).
 */
void sieve_of_eratosthenes(int limit) {
    // Step 1: Initialize the array, assuming all numbers are prime
    for (int i = 0; i < limit; i++)
        is_prime[i] = 1;

    // Step 2: Mark 0 and 1 as non-prime
    is_prime[0] = is_prime[1] = 0;  

    // Step 3: Perform sieve operation
    for (int i = 2; i * i < limit; i++) {  // Iterate only up to sqrt(limit) for efficiency
        if (is_prime[i]) {  // If i is still marked as prime
            for (int j = i * i; j < limit; j += i) {  // Mark multiples of i as non-prime
                is_prime[j] = 0;
            }
        }
    }

    // Step 4: Count the number of primes found (though not printed in this version)
    int prime_count = 0;
    for (int i = 2; i < limit; i++) {
        if (is_prime[i])
            prime_count++;
    }
}

/**
 * Function: main
 * --------------
 * Entry point of the program.
 * Calls the sieve function to compute prime numbers up to the defined LIMIT.
 *
 * Returns:
 *   0 - Indicates successful execution.
 */
int main() {
    sieve_of_eratosthenes(LIMIT);  // Compute primes up to LIMIT
    return 0;
}
