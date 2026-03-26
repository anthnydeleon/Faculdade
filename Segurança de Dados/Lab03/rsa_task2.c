#include <stdio.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a)
{
    char *number_str = BN_bn2hex(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main()
{
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *n = BN_new();
    BIGNUM *e = BN_new();
    BIGNUM *d = BN_new();

    BIGNUM *M = BN_new();
    BIGNUM *C = BN_new();
    BIGNUM *M2 = BN_new();

    /* carregar chaves */

    BN_hex2bn(&n,"DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");

    BN_hex2bn(&e,"010001");

    BN_hex2bn(&d,"74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");

    /* mensagem em hexadecimal */

    BN_hex2bn(&M,"4375696461646f3a2070726f6772616d6120696e73656775726f21");

    printBN("Mensagem original =", M);

    /* CRIPTOGRAFAR */
    BN_mod_exp(C, M, e, n, ctx);

    printBN("Mensagem criptografada =", C);

    /* DESCRIPTOGRAFAR */
    BN_mod_exp(M2, C, d, n, ctx);

    printBN("Mensagem recuperada =", M2);

    return 0;
}

0BBB587F6B992817AC23F006FF49EFBD7070C0D0F7FE5A6005EE794C97CBB592

0BBB587F6B992817AC23F006FF49EFBD7070C0D0F7FE5A6005EE794C97CBB592