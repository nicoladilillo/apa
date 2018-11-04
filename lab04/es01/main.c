#include <stdio.h>

int gcd(int a, int b)
{
    /*
     * stabilisce valore massimo e minimo
     */
    if (a < b) {
        a += b;
        b = a-b;
        a -=b;
    }

    /*
     * se a e b sono uguali abbiamo trovato
     * il massimo comun divisore
     */
    if(a == b)
        return a;

    if (a%2 == 0 && b%2 == 0)
        return 2*gcd(a/2, b/2);
    else if (a%2 != 0 && b%2 == 0)
        return gcd(a, b/2);
    else if (a%2 == 0 && b%2 != 0)
        return gcd(a/2, b);
    else
        return gcd((a-b)/2, b);

}

int main() {

    int a = 5, b = 10, n;
    printf("Inserire numeri a e b: ");
    scanf("%d %d", &a, &b);

    n = gcd(a, b);
    printf("M.C.D: %d", n);

    return 0;
}