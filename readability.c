#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int knowGrade(string text);

int main(void)
{
    int words = 1;
    int sentences = 0;
    string text = get_string("Text: ");

    if (knowGrade(text) < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (knowGrade(text) >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", knowGrade(text));
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}

int knowGrade(string text)
{
    int letters = count_letters(text);
    int sentences = count_sentences(text);
    int words = count_words(text);
    float L = (float)letters / (float)words * 100;
    float S = (float)sentences / (float)words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    index = round(index);
    return index;
}
