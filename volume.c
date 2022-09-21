// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    uint8_t header[HEADER_SIZE]; //uint8_t (name) [HEADER_SIZE] = means the variable has 44 bytes. wha we needed exactly

    fread(header, HEADER_SIZE, 1, input); //read the bytes inside input file that is fopened, store it in header
    fwrite(header, HEADER_SIZE, 1, output); //write all the bytes inside the header into the fopened output file

    // TODO: Read samples from input file and write updated data to output file
    int16_t buffer; //create a variable that will store the sample bytes

    while (fread(&buffer, sizeof(buffer), 1, input)) //while there are still bytes to read inside the input file, the while loop will keep looping, reading since it is fread
    {                                               //use the size of int16_t because it stores 2bytes, what we exactly need for sample bytes
        buffer = buffer * factor; //multiply the bytes inside the buffer by the factor entered by the user and it will result in a different volume in WAV file
        fwrite(&buffer, sizeof(buffer), 1, output); //write the multiplied byte that is inside the buffer onto the output WAV file
    }

    // Close files
    fclose(input);      //always close all file that are opened
    fclose(output);
}
