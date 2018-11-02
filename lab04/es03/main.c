#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 200+1
#define M 30+1

char *cercaRegexp(char **src, char *regexp);
void casopunto(char s, char **t, int *f);
void casoparentesi(char s, char **t, int *f);
void casoslash(char s, char **t, int *f);

typedef enum {
    primo, secondo, terzo, err
} t_comandi;

/*
 * per lo switch di seleziona comando
 */
t_comandi leggiComando(char cmd)
{
    t_comandi c;
    char tabella[err] = {
            '.', '[', '\\', 'e'
    };

    c = primo;
    while (c<err && cmd != tabella[c])
        c++;

    return (c);
}

/*
 * passiamo il buffer di appoggio perchè
 * abbiamo un vettore di puntatore a stringhe
 * dove ogni stringa rappresenta una parola del testo
 */
void leggistringhe(char **v, char b[], char nome[])
{
    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if (fp == NULL)
    {
        printf("Errore parametri\n");
        exit(-1);
    }

    int i = 0;
    while (fscanf(fp, "%s", b) != EOF)
    {
        /*
         * copio indirizzo inizio stringa (vettore di caratteri)
         */
        *(v+i) = b;
        /*
         * il puntatore di inizio stringa di b supera la parola appena salvata
         */
        b = &b[strlen(*(v+i))+1]; // +1 per il terminatore di stringhe
        i++;
    }

    *(v+i) = NULL;
}

void stampa(char **s)
{
    int i;
    for (i = 0 ; *(s+i) != NULL; i++)
        printf("%s ", *(s+i));

}

int main(int argc, int *argv[]) {

    char *s[N];
    char buff[M*N];

    leggistringhe(s, buff, argv[1]);

    stampa(s);

    char c[M];
    printf("\nInserisci carattere da cercare: ");
    scanf("%s", c);

    printf("%s", cercaRegexp(s, c));

    return 0;
}

char *cercaRegexp(char **src, char *regexp)
{
    int i;
    int flag;
    char *r = regexp;

    /*
     * Si sarebbe potuto evitare l'utilizzo di tutti questi puntatori e
     * preferire un semplice utilizzo di vettori ma essendo l'esercizio
     * basato sulla pratica proprio dei puntatori si è deciso di svolgere
     * l'esercizio come mostrato nel seguente modo
     */

    for ( ; *src != NULL; src++) {
        /*
         * ricominciamo dall'inizio della nostra stringa
         */
        regexp = r;
        flag = 1;
        for (i = 0; *regexp != '\0' && flag && **src != '\0'; (*src)++, regexp++, i++) {
            if (!isalpha(*regexp)) {
                switch (leggiComando(*regexp)) {
                    case primo:
                        casopunto(**src, &regexp, &flag);
                        break;
                    case secondo:
                        casoparentesi(**src, &regexp, &flag);
                        break;
                    case terzo:
                        casoslash(**src, &regexp, &flag);
                        break;
                    case err:
                    default:
                        printf("Errore\n");
                        exit(-2);
                }
            } else if (*regexp != **src)
                flag = 0;
        }

        /*
         * se la parola corrisponde alla nostra ricerca e se abbiamo
         * analizzato tutti i caratteri delle nostre parola siamo per entrambe al terminatore di stringa
         */
        if (flag && *regexp == **src)
            return *(src)-i;
    }

    return NULL;

}

void casopunto(char s, char **t, int *f)
{
    /*
     * controlliamo che il nostro carattere sia alfabetico
     */
    !isalpha(s) ? (*f = 0) : *(t++);
}

void casoparentesi(char s, char **t, int *f)
{
    (*t)++;

    if (!isalpha(**t))
    {
        /*
        * caso [^ ] - carattere non contenuro tra parentesi
        */
        (*t)++;
        while (**t != ']')
        {
            if (**t == s) *f = 0;
            (*t)++;
        }
    } else  {
        /*
        * caso [ ] - carattere contenuro tra parentesi
        */
        *f = 0;
        while (**t != ']')
        {
            if (**t == s && *f == 0) *f = 1;
            (*t)++;
        }
    }
}

void casoslash(char s, char **t, int *f)
{
    (*t)++;
    if ((**t == 'A' && !isupper(s)) || (**t == 'a' && !islower(s)))
        *f = 0;
}