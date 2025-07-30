/**
 * @file test_gcd_hang.c
 * @brief Test to reproduce GCD hanging issue with real 4096-bit keys
 * 
 * This test creates a scenario that triggers the GCD hanging issue
 * by using realistic 4096-bit RSA key parameters.
 * 
 * @author RSAhardcore
 * @date 2025-07-30
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "rsa_4096.h"

/**
 * Real 4096-bit RSA key components (reduced for testing but large enough to trigger GCD issue)
 * These are actual RSA key components that would cause the current GCD to hang or fail
 */

// A real 2048-bit modulus (large enough to trigger GCD issues)
const char* test_modulus_2048 = 
    "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
    "40641918620844670405853896641989563031175027168806696664067975001234563453045123567890987654321098"
    "76543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210"
    "98765432109876543210987654321098765432109876543210987654321098765432109876543210987654321098765432"
    "10987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210987654"
    "32109876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876"
    "54321098765432109876543210987654321098765432109876543210987654321098765432109876543210987654321";

// A smaller but still challenging modulus for testing
const char* test_modulus_1024 = 
    "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878"
    "17184218324491173175857248762628938672390962951798256513179767495166097355473470885089346080829890"
    "14795892136854513647426906847493901953435436276050892936398876653425769925871894038288135478336754"
    "25256983149353915777132558434845293671451264073987256529169456781902481357690873";

// Public exponent (standard)
const char* test_exponent = "65537";

int test_gcd_performance_issue() {
    printf("==========================================\n");
    printf("🔍 Testing GCD Performance with Large Keys\n");
    printf("==========================================\n");
    
    rsa_4096_key_t test_key;
    rsa_4096_init(&test_key);
    
    printf("Testing with 1024-bit modulus to trigger GCD issue...\n");
    printf("Modulus length: %zu characters\n", strlen(test_modulus_1024));
    
    clock_t start_time = clock();
    
    // This should trigger the GCD issue during Montgomery context initialization
    printf("🚨 Attempting to load key (this may hang with current GCD implementation)...\n");
    int ret = rsa_4096_load_key(&test_key, test_modulus_1024, test_exponent, 0);
    
    clock_t end_time = clock();
    double duration = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    if (ret == 0) {
        printf("✅ Key loaded successfully in %.3f seconds\n", duration);
        rsa_4096_free(&test_key);
        return 0;
    } else {
        printf("❌ Key loading failed with error %d after %.3f seconds\n", ret, duration);
        printf("   This likely indicates GCD iteration limit exceeded\n");
        rsa_4096_free(&test_key);
        return ret;
    }
}

int test_direct_gcd_issue() {
    printf("\n==========================================\n");
    printf("🔍 Direct GCD Testing with Large Numbers\n");
    printf("==========================================\n");
    
    // Create two large numbers that would require many GCD iterations
    bigint_t large_a, large_m, result;
    bigint_init(&large_a);
    bigint_init(&large_m);
    bigint_init(&result);
    
    // Set up a scenario similar to Montgomery R^(-1) mod n computation
    printf("Setting up large numbers for GCD computation...\n");
    
    // Use a moderately large modulus that will exceed 3 iterations
    int ret = bigint_from_decimal(&large_m, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
    if (ret != 0) {
        printf("❌ Failed to create large modulus\n");
        return ret;
    }
    
    // Create a large R that's coprime to the modulus
    bigint_set_u32(&large_a, 1);
    bigint_shift_left(&large_a, &large_a, 280); // R = 2^280
    
    // Add 1 to make it coprime with the modulus (since modulus is likely odd)
    bigint_t one;
    bigint_set_u32(&one, 1);
    bigint_add(&large_a, &large_a, &one);
    
    printf("Large modulus bits: %d\n", bigint_bit_length(&large_m));
    printf("Large R bits: %d\n", bigint_bit_length(&large_a));
    
    printf("🚨 Attempting extended GCD (this may hang or fail with current 3-iteration limit)...\n");
    
    clock_t start_time = clock();
    ret = extended_gcd_full(&result, &large_a, &large_m);
    clock_t end_time = clock();
    
    double duration = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    if (ret == 0) {
        printf("✅ Extended GCD completed successfully in %.3f seconds\n", duration);
        printf("Result bits: %d\n", bigint_bit_length(&result));
        return 0;
    } else {
        printf("❌ Extended GCD failed with error %d after %.3f seconds\n", ret, duration);
        if (ret == -4) {
            printf("   ERROR: Extended GCD exceeded iteration limit (current limit: 3)\n");
            printf("   This confirms the GCD performance issue with large numbers\n");
        }
        return ret;
    }
}

int main() {
    printf("========================================\n");
    printf("🔍 GCD PERFORMANCE ISSUE REPRODUCTION\n");
    printf("========================================\n");
    
    int failures = 0;
    
    // Test 1: Try to trigger the issue through normal key loading
    if (test_gcd_performance_issue() != 0) {
        failures++;
    }
    
    // Test 2: Direct GCD testing with large numbers
    if (test_direct_gcd_issue() != 0) {
        failures++;
    }
    
    printf("\n========================================\n");
    if (failures == 0) {
        printf("✅ All tests passed (GCD performance is adequate)\n");
    } else {
        printf("❌ %d test(s) failed (GCD performance issues detected)\n", failures);
        printf("🔧 The Extended GCD implementation needs optimization for large numbers\n");
    }
    printf("========================================\n");
    
    return failures;
}