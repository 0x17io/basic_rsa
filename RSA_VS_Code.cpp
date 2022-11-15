#include <iostream>
#include <random>
#include <math.h>
#include <boost/random.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/multiprecision/random.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/uniform_int.hpp>

using namespace boost::multiprecision;
namespace mp = boost::multiprecision;
using namespace boost::random;
boost::random::mt19937 rng;

int1024_t gcd(int1024_t a, int1024_t b) {
    // Calculate GCD as needed
    while (a != b) {
        if (a > b) {
            a = a - b;
        }
        else {
            b = b - a;
        }
    }
    return a;
}

int1024_t inverse(int1024_t a, int1024_t m) {
    // Note that effectively, we are doing GCD algorithm in reverse.
    // Using our textbook page 61 for help and the Ext Euclidean Algo
    // from wiki we construct below algorithm.

    int1024_t t = 0; int1024_t r = m; int1024_t n_t = 1; int1024_t n_r = a;

    while (n_r != 0) {
        int1024_t q = r / n_r;
        int1024_t prev_t = n_t;
        n_t = t - q * prev_t;
        t = prev_t;

        int1024_t prev_r = n_r;
        n_r = r - q * prev_r;
        r = prev_r;

    }

    if (r > 1) {
        // No inverse exists.
        return -1;
    }
    if (t < 0) {
        t = t + m;
    }

    return t;
}

bool randomized_prime_check(int p) {
    // Use Fermat's Theorem to preform primality check.
    if (p <= 0) {
        // Making sure p > 0
        std::cout << "Invalid number" << std::endl;
        return false;
    }
    else {
        // Got idea from boost 

        boost::random::uniform_int_distribution<> rando(1, p - 1);
        int1024_t a = rando(rng);

        int1024_t powered_num= mp::powm((int1024_t) a, ((int1024_t) p - 1) / 2, (int1024_t) p); 

        if (gcd((int1024_t) a, (int1024_t) p) != 1) {
            return false;
        }
        if (powered_num != (int1024_t) 1 || powered_num != (int1024_t) -1) {
            return false;
        }
    }
    
    return true;
}

bool k_randomized_prime_check(int number_to_check, int amount_to_check) {
    // Check repeat prime number check. Use amount_to_check=20 for an
    // error rate < 0.000000953674...

    for (int k = 0; k < amount_to_check; k++) {
        bool prime_check = randomized_prime_check(number_to_check);
        //std::cout << prime_check << std::endl;
        if (!prime_check) {
            return false;
        }
        
    }
    return true;
}

int main() {
    // Showcasing the main part of the RSA Algorithm.
    // Note that we are using the boost library which allows for large number operations,
    // which are useful especially mpow (mod pow).

    // Step 1. Generate two large primes
    // Note that we can run the following cript to get primes, but this is time consuming.
    // We hardcode primes for quickness.
    /*
    for (int i = 3; i < 20; i++) {
        if (k_randomized_prime_check(i, 100)) {
            std::cout << i << std::endl;
        }


    }*/
    int1024_t p = 877;
    int1024_t q = 883;

    // Step 2. Generate public key.
    int1024_t n = p * q;

    // Step 3. Find e value to be used for encryption
    int1024_t e = 7;
    int1024_t phi_function_value = (p - 1) * (q - 1);

    // Step 4. Ensure coprimeness
    while (e < phi_function_value) {
        if (gcd(e, phi_function_value) == 1) {
            // found our desired e value
            break;
        }
        else {
            // Try again.
            e++;
        }
    }

    // Step 5. Compute d (for decryption).
    int1024_t d_inverse_num = inverse(e, phi_function_value);

    // Testing Encryption
    int1024_t message = 4353478332;
    int1024_t encrypted_message = mp::powm(message, e, n);
    std::cout << "encrypt: " << message << ">>" << encrypted_message << std::endl;

    // Testing Decryption
    int1024_t decrypted_message = mp::powm(encrypted_message, d_inverse_num, n);
    std::cout << "decypt: " << encrypted_message << ">>" << decrypted_message << std::endl;


    return 0;
}
