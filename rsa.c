#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Find the Greatest Common Divisor between two numbers
int
gcd(int num1, int num2)
{
    int temp;
    while (num2 > 0)
    {
        temp = num1 % num2;
        num1 = num2;
        num2 = temp;
    }
    return num1;
}

// d = (1/e) mod n
int
mod_inverse(int e, int phi)
{
    int inv, u1, u3, v1, v3, t1, t3, q;
    u1 = 1;
    u3 = e;
    v1 = 0;
    v3 = phi;
    while (v3 != 0)
    {
        q  = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 - q * v1;
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
    }
    if (u3 != 1)
        return 0; // No inverse exists
    if (u1 < 0)
        u1 += phi;
    return u1;
}

// Check if the input number is a prime number or not
bool
check_prime(int num)
{
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
    }
    return true;
}

// Check if input e is valid
bool
check_e(int e, int phi)
{
    return gcd(e, phi) == 1 && e > 1 && e < phi;
}

// The Modular Exponentiation Algorithm
int
MEA(int base, int exp, int mod)
{
    int result = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// RSA encode function for numeric data
void
rsa_encode(int *data, int length, int e, int n)
{
    for (int i = 0; i < length; ++i)
    {
        data[i] = MEA(data[i], e, n);
    }
}

// RSA decode function for numeric data
void
rsa_decode(int *data, int length, int d, int n)
{
    for (int i = 0; i < length; ++i)
    {
        data[i] = MEA(data[i], d, n);
    }
}

// Ez conversion
void
str_to_int(char *str, int **out, int *length)
{
    *length = strlen(str);

    if (*out == NULL)
        *out = malloc(sizeof(int) * (*length));

    for (int i = 0; i < *length; i++)
        (*out)[i] = str[i];
}

int
main()
{
    int p = 61;    // Prime 1
    int q = 53;    // Prime 2
    int n = p * q; // n = p * q
    int e = 17;    // Public exponent
    int d;         // d = (1/e) mod (phi)
    int phi = (p - 1) * (q - 1);

    if (check_prime(p) && check_prime(q))
    {
        if (check_e(e, phi))
        {
            d = mod_inverse(e, phi);
        }
        else
        {
            puts("Invalid 'e'");
            return 0;
        }
    }
    else
    {
        puts("Invalid 'p' and/or 'q' (not prime)");
        return 0;
    }

    char str[]  = "Hello, world!";
    int *msg    = NULL;
    int  length = 0;

    str_to_int(str, &msg, &length);

    printf("Original msg: ");
    for (int i = 0; i < length; ++i)
        printf("%c", msg[i]);
    printf("\n");

    rsa_encode(msg, length, e, n);

    printf("Encoded msg: ");
    for (int i = 0; i < length; ++i)
        printf("%d ", msg[i]);
    printf("\n");

    rsa_decode(msg, length, d, n);

    printf("Decoded msg: ");
    for (int i = 0; i < length; ++i)
        printf("%c", msg[i]);
    printf("\n");

    free(msg);

    return 0;
}
