// Maximum number to verify in the sieve algorithm
#define LIMIT 100000000  

// Array that stores if a number is prime (up to LIMIT numbers)
// Using static allocation to ensure this code is strictly CPU-bound
static char is_prime[LIMIT];

/**
 * Function: sieve_of_eratosthenes
 * --------------------------------
 * Runs the Sieve of Eratosthenes algorithm to find prime numbers up to `limit`.
 * P.S.: In the is_prime array, 1 means it's a prime and 0 means it's not.
 * 
 * Parameters:
 *   limit - The maximum limit to verify if a number is prime.
 *
 * Algorithm:
 *   - Initializes all numbers as prime (1).
 *   - Intializes the 0 and 1 numbers as non-prime (0).
 *   - Iterates and marks multiples of primes as non-prime.
 */
void sieve_of_eratosthenes(int limit) {
    // First initializes the array assuming all numbers are prime
    for (int i = 0; i < limit; i++)
        is_prime[i] = 1;

    // Initializes 0 and 1 as non-prime
    is_prime[0] = is_prime[1] = 0;  

    // Do the iteration for every number until limit
    for (int i = 2; i * i < limit; i++) {
        if (is_prime[i]) {
            // if the current i is a prime, mark multiples of i as non-primes
            for (int j = i * i; j < limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }
}

/**
 * Function: main
 * --------------
 * Calls the sieve function to find prime numbers up to LIMIT.
 *
 * Returns:
 *   0 - executed with success.
 */
int main() {
    sieve_of_eratosthenes(LIMIT);
    return 0;
}
