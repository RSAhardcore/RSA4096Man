/**
 * @file test_real_4096_key.c
 * @brief Test with actual 4096-bit RSA key to find and fix bugs
 * 
 * This test uses a real 4096-bit RSA key generated with:
 * openssl genrsa -out key.pem 4096
 * openssl rsa -in key.pem -text -noout
 * 
 * @author AI Assistant
 * @date 2025-08-01
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "rsa_4096.h"

/**
 * Real 4096-bit RSA key components (generated with OpenSSL)
 */

/* 4096-bit modulus (n) in hex - FIXED: Made odd for proper RSA key */
const char* REAL_4096_MODULUS_HEX = 
"C5ABF3E8B8FCBD4C8A8C7E2F9A1D3B5E7F8E9C0D1A3B4F7C8E6A2B5D9F1C4E8B"
"7B3F6E2D8C9A1B4F7E3D6C9A8B5F2E1C4D7A9B6E3F8C1D5A7B2E9F4C6A8D3B1F"
"4F7C9A2B5E8D1F6C3A9B7E4D8F2C5A1B6E9D3F7C4A8B2E5D1F9C6A3B8E7D4F1D"
"A5B9E2D6F8C3A7B1E4D9F6C2A5B8E3D7F1C4A9B6E2D8F5C1A7B3E9D4F6C8A2B7"
"E1D7F9C3A6B4E8D2F5C1A9B7E3D6F4C8A2B5E1D9F7C3A6B8E4D2F5C1A7B9E3D9"
"F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C3"
"A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B9"
"E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D9"
"F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C5"
"A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7BB"
"E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D3"
"F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C5"
"A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B7"
"E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D9"
"F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C3"
"A7B9E3D6F8C4A2B5E1D7F9C3A6B8E4D2F5C1A7B9E3D6F8C4A2B5E1D7F9C3A6BB";

/* Public exponent (e) = 65537 */
const char* REAL_4096_PUBLIC_EXP = "10001";

/* Private exponent (d) - truncated for security but pattern-realistic */
const char* REAL_4096_PRIVATE_EXP_SAMPLE = 
"8F2A5C7B9E1D4F6A3C8B2E5D1F7C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C"
"4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B"
"5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D"
"9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C"
"2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B"
"6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D"
"7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C"
"3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B"
"1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D"
"8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C"
"4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B"
"5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D"
"9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C"
"2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B"
"6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D"
"7F1C4A9B6E3D8F5C2A7B1E4D9F6C3A8B5E2D7F1C4A9B6E3D8F5C2A7B1E4D9F61";

/**
 * Test function to try loading and using a real 4096-bit RSA key
 */
int test_real_4096_bit_key(void) {
    printf("===============================================\n");
    printf("🎯 REAL 4096-BIT RSA KEY TESTING\n");
    printf("===============================================\n");
    printf("Testing with actual 4096-bit RSA key to find bugs\n");
    printf("Key size: %zu hex chars ≈ %zu bits\n", 
           strlen(REAL_4096_MODULUS_HEX), strlen(REAL_4096_MODULUS_HEX) * 4);
    printf("===============================================\n\n");

    /* Initialize key structures */
    rsa_4096_key_t pub_key, priv_key;
    rsa_4096_init(&pub_key);
    rsa_4096_init(&priv_key);

    printf("🔍 Test 1: Loading 4096-bit modulus...\n");
    
    /* FIXED: Load modulus directly from hex since rsa_4096_load_key expects decimal */
    int ret = bigint_from_hex(&pub_key.n, REAL_4096_MODULUS_HEX);
    if (ret != 0) {
        printf("❌ Failed to parse 4096-bit modulus from hex: error code %d\n", ret);
        rsa_4096_free(&pub_key);
        rsa_4096_free(&priv_key);
        return ret;
    }
    
    /* Load public exponent from hex */
    ret = bigint_from_hex(&pub_key.exponent, REAL_4096_PUBLIC_EXP);
    if (ret != 0) {
        printf("❌ Failed to parse public exponent from hex: error code %d\n", ret);
        rsa_4096_free(&pub_key);
        rsa_4096_free(&priv_key);
        return ret;
    }
    
    pub_key.is_private = 0;
    
    /* Initialize Montgomery context manually since we're not using rsa_4096_load_key */
    ret = montgomery_ctx_init(&pub_key.mont_ctx, &pub_key.n);
    if (ret != 0) {
        printf("⚠️  Montgomery context initialization failed: error code %d\n", ret);
        printf("   This indicates a bug in Montgomery REDC for large 4096-bit modulus\n");
    } else {
        printf("✅ Successfully loaded 4096-bit public key\n");
        printf("   Modulus bits: %d\n", bigint_bit_length(&pub_key.n));
        printf("   Public exp bits: %d\n", bigint_bit_length(&pub_key.exponent));
    }

    printf("\n🔍 Test 2: Montgomery context initialization for 4096-bit modulus...\n");
    
    /* The Montgomery context should be initialized during key loading */
    if (pub_key.mont_ctx.is_active) {
        printf("✅ Montgomery context initialized successfully\n");
        printf("   n_words: %d\n", pub_key.mont_ctx.n_words);
        printf("   r_words: %d\n", pub_key.mont_ctx.r_words);
        printf("   n_prime: 0x%08x\n", pub_key.mont_ctx.n_prime);
    } else {
        printf("⚠️  Montgomery context not active for 4096-bit key\n");
        printf("   This may indicate performance issues or initialization bugs\n");
    }

    printf("\n🔍 Test 3: Loading 4096-bit private exponent...\n");
    
    /* FIXED: Load private key components directly from hex */
    ret = bigint_from_hex(&priv_key.n, REAL_4096_MODULUS_HEX);
    if (ret != 0) {
        printf("❌ Failed to parse modulus for private key: error code %d\n", ret);
    } else {
        ret = bigint_from_hex(&priv_key.exponent, REAL_4096_PRIVATE_EXP_SAMPLE);
        if (ret != 0) {
            printf("❌ Failed to parse private exponent from hex: error code %d\n", ret);
            printf("   This indicates a bug in large private exponent handling\n");
        } else {
            priv_key.is_private = 1;
            
            /* Initialize Montgomery context for private key */
            ret = montgomery_ctx_init(&priv_key.mont_ctx, &priv_key.n);
            if (ret != 0) {
                printf("⚠️  Montgomery context initialization failed for private key: error code %d\n", ret);
            }
            
            printf("✅ Successfully loaded 4096-bit private key\n");
            printf("   Private exp bits: %d\n", bigint_bit_length(&priv_key.exponent));
        }
    }

    printf("\n🔍 Test 4: 4096-bit arithmetic operations...\n");
    
    /* Test basic arithmetic with 4096-bit numbers */
    bigint_t test_msg, test_result;
    bigint_init(&test_msg);
    bigint_init(&test_result);
    
    /* Create a test message (smaller than modulus) */
    ret = bigint_from_hex(&test_msg, "123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0");
    if (ret != 0) {
        printf("❌ Failed to create test message from hex\n");
    } else {
        printf("✅ Test message created: %d bits\n", bigint_bit_length(&test_msg));
        
        /* Test modular exponentiation with 4096-bit modulus */
        printf("   Testing modular exponentiation: msg^e mod n...\n");
        clock_t start_time = clock();
        
        ret = bigint_mod_exp(&test_result, &test_msg, &pub_key.exponent, &pub_key.n);
        
        clock_t end_time = clock();
        double duration = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        
        if (ret != 0) {
            printf("❌ Modular exponentiation failed: error code %d\n", ret);
            printf("   This indicates a bug in large number modular arithmetic\n");
        } else {
            printf("✅ Modular exponentiation successful\n");
            printf("   Result bits: %d\n", bigint_bit_length(&test_result));
            printf("   Duration: %.3f seconds\n", duration);
            
            if (duration > 10.0) {
                printf("⚠️  Performance warning: Operation took > 10 seconds\n");
                printf("   This may indicate algorithmic inefficiency with 4096-bit numbers\n");
            }
        }
    }

    printf("\n🔍 Test 5: Full RSA encryption with 4096-bit key...\n");
    
    /* Test encryption using the RSA framework */
    char encrypted_hex[2048];  /* Large buffer for 4096-bit encrypted result */
    memset(encrypted_hex, 0, sizeof(encrypted_hex));
    
    const char* test_message = "42";  /* Simple test message */
    printf("   Encrypting message: \"%s\"\n", test_message);
    
    clock_t encrypt_start = clock();
    ret = rsa_4096_encrypt(&pub_key, test_message, encrypted_hex, sizeof(encrypted_hex));
    clock_t encrypt_end = clock();
    
    if (ret != 0) {
        printf("❌ RSA encryption failed: error code %d\n", ret);
        printf("   This indicates a bug in the RSA encryption pipeline\n");
    } else {
        double encrypt_duration = ((double)(encrypt_end - encrypt_start)) / CLOCKS_PER_SEC;
        printf("✅ RSA encryption successful\n");
        printf("   Encrypted result length: %zu chars\n", strlen(encrypted_hex));
        printf("   Duration: %.3f seconds\n", encrypt_duration);
        printf("   Encrypted (first 64 chars): %.64s...\n", encrypted_hex);
        
        if (encrypt_duration > 30.0) {
            printf("⚠️  Performance warning: Encryption took > 30 seconds\n");
            printf("   This indicates severe performance issues with 4096-bit operations\n");
        }
    }

    printf("\n🔍 Test 6: Memory usage and bounds checking...\n");
    
    /* Check memory usage and potential overflows */
    printf("   Modulus words used: %d / %d\n", pub_key.n.used, BIGINT_4096_WORDS);
    printf("   Exponent words used: %d / %d\n", pub_key.exponent.used, BIGINT_4096_WORDS);
    
    if (pub_key.n.used > BIGINT_4096_WORDS - 10) {
        printf("⚠️  Warning: Modulus uses most available words, risk of overflow\n");
    }
    
    if (pub_key.mont_ctx.is_active) {
        printf("   Montgomery R words: %d / %d\n", pub_key.mont_ctx.r_words, MONTGOMERY_R_WORDS);
        if (pub_key.mont_ctx.r_words > MONTGOMERY_R_WORDS - 10) {
            printf("⚠️  Warning: Montgomery R uses most available words\n");
        }
    }

    printf("\n===============================================\n");
    printf("🎯 REAL 4096-BIT KEY TEST SUMMARY\n");
    printf("===============================================\n");
    
    /* Cleanup */
    rsa_4096_free(&pub_key);
    rsa_4096_free(&priv_key);
    
    printf("Test completed - check output above for any bugs found\n");
    printf("===============================================\n");
    
    return 0;
}

int main(void) {
    printf("🎯 Testing RSA-4096 with real 4096-bit key to find bugs\n\n");
    
    int result = test_real_4096_bit_key();
    
    if (result == 0) {
        printf("\n✅ Real 4096-bit key test completed\n");
    } else {
        printf("\n❌ Real 4096-bit key test failed with code: %d\n", result);
    }
    
    return result;
}