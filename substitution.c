#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//prompt for a cypher with 26 letters
//make a reference array with the correct alphabet order
//use loops and print the cypher

//input and output must be CASE INSENSITIVE
//must validate = no argv input/incomplete 26 letter input/must not have an integer/must not have repeated characters

int main(int argc, string argv[])
{
    int count = 0;
    string ref1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string ref2 = "abcdefghijklmnopqrstuvwxyz";

    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    for (int i = 0; i < strlen(argv[1]) ; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }
        count++;
    }
    if (count != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    for (int k = 0; k < strlen(argv[1]); k++)
    {
        for (int j = k + 1; j < strlen(argv[1]); j++)
        {
            if (argv[1][k] == argv[1][j])
            {
                printf("Key must not have repeated characters.\n");
                return 1;
            }
        }
    }

    string text = get_string("plaintext: ");
    printf("ciphertext: ");
    //I NEED TO MAKE IT PRINT THE SPACES,COMMAS, AND NUMBERS WHILE ALSO PRINING THE CIPHER
    //if the plaintext is small letter, ciphertext must also be small letter
    //need another cipher key but in small letters
    for (int l = 0; l < strlen(text); l++)
    {
        if (isalpha(text[l]) == 0)
        {
            printf("%c", text[l]);
        }
        else
        {
            for (int n = 0; n < strlen(ref1); n++)
            {
                if (text[l] == ref1[n])
                {
                    printf("%c", toupper(argv[1][n]));
                }
                else if (text[l] == ref2[n])
                {
                    printf("%c", tolower(argv[1][n]));
                }
            }
        }
    }
    printf("\n");
    return 0;
}
