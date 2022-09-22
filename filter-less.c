#include "helpers.h"
#include <math.h>

void swap(int *a, int *b);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //look and read the r.g.b values of all pixels 1by1 of the image array
    //get the average of the 3 values
    //round them to the nearest integer value so that it will be 1-255
    //the result should be swapped to the values of the r,g,b. all should be the same values for 1 pixel
    //use for loop to read/rewrite the values so that you can return the whole greyscaled image
    float sum = 0, avrg = 0;
    int result = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            avrg = sum / 3.0;
            result = round(avrg);
            image[i][j].rgbtBlue = result;
            image[i][j].rgbtGreen = result;
            image[i][j].rgbtRed = result;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //look and read the rgb values of each pixel inside the image array
    //convert the rgb values based on the formula given
    //cap the maximum integer value to 255 for the sepia rgb
    //use for loop to read and rewrite the rgb values
    float sepiared = 0, sepiagreen = 0, sepiablue = 0;
    int sepiaRed = 0, sepiaGreen = 0, sepiaBlue = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiared = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            sepiagreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;
            sepiablue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;

            sepiaRed = round(sepiared);
            sepiaGreen = round(sepiagreen);
            sepiaBlue = round(sepiablue);

            if (sepiaRed >= 255)
            {
                sepiaRed = 255;
                if (sepiaGreen >= 255)
                {
                    sepiaGreen = 255;
                    if (sepiaBlue >= 255)
                    {
                        sepiaBlue = 255;
                    }
                }
                else if (sepiaBlue >= 255)
                {
                    sepiaBlue = 255;
                }
            }
            else if (sepiaGreen >= 255)
            {
                sepiaGreen = 255;
            }
            else if (sepiaBlue >= 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //look and read all the rgb values of each pixel inside the image array
    //swap the rgb values of each pixel from left to right to its opposite right to left order
    //make a temp variable that will hold the first value as you swap them values
    int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //only look for half the row otherwise the swapped pixels will get swapped again and it will get back to the original places
            a = image[i][j].rgbtRed;
            b = image[i][width - (j + 1)].rgbtRed; //width means last row - j depending on what pixel you are looking for in order
            swap(&a, &b);
            image[i][j].rgbtRed = a;
            image[i][width - (j + 1)].rgbtRed = b;

            c = image[i][j].rgbtGreen;
            d = image[i][width - (j + 1)].rgbtGreen;
            swap(&c, &d);
            image[i][j].rgbtGreen = c;
            image[i][width - (j + 1)].rgbtGreen = d;

            e = image[i][j].rgbtBlue;
            f = image[i][width - (j + 1)].rgbtBlue;
            swap(&e, &f);
            image[i][j].rgbtBlue = e;
            image[i][width - (j + 1)].rgbtBlue = f;

        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make a copy of image array RGBTRIPLE copy[height][width];
    //read the pixels of copy array but write the blurred one in the image array
    RGBTRIPLE copy[height][width];
    float avr = 0;
    int avrg = 0;
    int i, j;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                avrg = copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i == 0 && j == width - 1)
            {
                avrg = copy[i][j - 1].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i + 1][j].rgbtRed;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i == height - 1 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i == height - 1 && j == width - 1)
            {
                avrg = copy[i - 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j - 1].rgbtRed + copy[i][j].rgbtRed;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;

            }
            else if (i == 0 && j > 0)
            {
                avrg = copy[i][j - 1].rgbtRed + copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i + 1][j].rgbtRed +
                       copy[i + 1][j + 1].rgbtRed;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i == height - 1 && j > 0)
            {
                avrg = copy[i - 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j - 1].rgbtRed +
                       copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i > 0 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j].rgbtRed +
                       copy[i + 1][j + 1].rgbtRed;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else if (i > 0 && j == width - 1)
            {
                avrg =  copy[i - 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i][j - 1].rgbtRed + copy[i][j].rgbtRed + copy[i + 1][j -
                        1].rgbtRed + copy[i + 1][j].rgbtRed;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
            else
            {
                avrg = copy[i - 1][j - 1].rgbtRed + copy[i - 1][j].rgbtRed + copy[i - 1][j + 1].rgbtRed + copy[i][j - 1].rgbtRed +
                       copy[i][j].rgbtRed + copy[i][j + 1].rgbtRed + copy[i + 1][j - 1].rgbtRed + copy[i + 1][j].rgbtRed + copy[i + 1][j + 1].rgbtRed;
                avr = avrg / 9.0;
                avrg = round(avr);
                image[i][j].rgbtRed = avrg;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                avrg = copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i == 0 && j == width - 1)
            {
                avrg = copy[i][j - 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i + 1][j].rgbtGreen;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i == height - 1 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i == height - 1 && j == width - 1)
            {
                avrg = copy[i - 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i][j].rgbtGreen;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;

            }
            else if (i == 0 && j > 0)
            {
                avrg = copy[i][j - 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i +
                        1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i == height - 1 && j > 0)
            {
                avrg = copy[i - 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j - 1].rgbtGreen +
                       copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i > 0 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i +
                        1][j].rgbtGreen + copy[i + 1][j + 1].rgbtGreen;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else if (i > 0 && j == width - 1)
            {
                avrg =  copy[i - 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i][j - 1].rgbtGreen + copy[i][j].rgbtGreen + copy[i + 1][j -
                        1].rgbtGreen + copy[i + 1][j].rgbtGreen;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
            else
            {
                avrg = copy[i - 1][j - 1].rgbtGreen + copy[i - 1][j].rgbtGreen + copy[i - 1][j + 1].rgbtGreen + copy[i][j - 1].rgbtGreen +
                       copy[i][j].rgbtGreen + copy[i][j + 1].rgbtGreen + copy[i + 1][j - 1].rgbtGreen + copy[i + 1][j].rgbtGreen + copy[i + 1][j +
                               1].rgbtGreen;
                avr = avrg / 9.0;
                avrg = round(avr);
                image[i][j].rgbtGreen = avrg;
            }
        }
    }
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (i == 0 && j == 0)
            {
                avrg = copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i == 0 && j == width - 1)
            {
                avrg = copy[i][j - 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i + 1][j].rgbtBlue;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i == height - 1 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i == height - 1 && j == width - 1)
            {
                avrg = copy[i - 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i][j - 1].rgbtBlue + copy[i][j].rgbtBlue;
                avr = avrg / 4.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;

            }
            else if (i == 0 && j > 0)
            {
                avrg = copy[i][j - 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i +
                        1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i == height - 1 && j > 0)
            {
                avrg = copy[i - 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j - 1].rgbtBlue +
                       copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i > 0 && j == 0)
            {
                avrg = copy[i - 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i +
                        1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else if (i > 0 && j == width - 1)
            {
                avrg =  copy[i - 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i][j - 1].rgbtBlue + copy[i][j].rgbtBlue + copy[i + 1][j -
                        1].rgbtBlue + copy[i + 1][j].rgbtBlue;
                avr = avrg / 6.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
            else
            {
                avrg = copy[i - 1][j - 1].rgbtBlue + copy[i - 1][j].rgbtBlue + copy[i - 1][j + 1].rgbtBlue + copy[i][j - 1].rgbtBlue +
                       copy[i][j].rgbtBlue + copy[i][j + 1].rgbtBlue + copy[i + 1][j - 1].rgbtBlue + copy[i + 1][j].rgbtBlue + copy[i + 1][j + 1].rgbtBlue;
                avr = avrg / 9.0;
                avrg = round(avr);
                image[i][j].rgbtBlue = avrg;
            }
        }
    }
    return;
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
