/**
 * @file debug_oddness.c
 * @brief Debug the oddness check issue
 */

#include <stdio.h>
#include <string.h>
#include "rsa_4096.h"

int main(void) {
    printf("🔍 Debugging oddness check issue\n\n");
    
    /* Test the exact modulus from our test */
    const char* test_hex = "C5ABF3E8B8FCBD4C8A8C7E2F9A1D3B5E7F8E9C0D1A3B4F7C8E6A2B5D9F1C4E8B";
    printf("Test hex: %s\n", test_hex);
    printf("Last character: %c\n", test_hex[strlen(test_hex)-1]);
    printf("Last char decimal: %d\n", test_hex[strlen(test_hex)-1] - (test_hex[strlen(test_hex)-1] <= '9' ? '0' : 'A' - 10));
    
    bigint_t n;
    bigint_init(&n);
    
    int ret = bigint_from_hex(&n, test_hex);
    printf("Hex parsing result: %d\n", ret);
    printf("Parsed number used words: %d\n", n.used);
    printf("First word: 0x%08X\n", n.words[0]);
    printf("Is odd (words[0] & 1): %d\n", n.words[0] & 1);
    
    /* Print first few hex digits back to verify parsing */
    char hex_back[128];
    bigint_to_hex(&n, hex_back, sizeof(hex_back));
    printf("Hex back: %.32s...\n", hex_back);
    
    return 0;
}