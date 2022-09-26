#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

//first 3 bytes are 0xff 0xd8 0xff 0xe-somthing at the beginning of a 512 byte block
    //512 bytes

    //open and look at the memory card
    //open a new jpg and copy the jpg from the memory card and then close after
    //use FILE *memory = fopen(card.raw,"r") to open the memory card
    //use uint8_t buffer[512] to store 512 bytes ==because uint8_t stores 8 bits that is 1 byte exactly
    //use fread(buffer, 1(how many elements to read), 512(size of elements), memory(file to read))
    //use for loop
        //if buffer[0] == 0xff
            //if buffer[1] == 0xd8
                //if buffer[2] == 0xff
                    //if (buffer[3] & 0xf0) == 0xe0
                        //then it is a start of a new jpeg
                        //if no open jpeg
                            //else close opened jpeg
    //filenames: ###.jpg, starting at 000.jpg
    //use sprintf(filename,"%03i.jpg", 1); //create a string called filename to store this.. make sure it has enough memory
    //use FILE *img = fopen(filename, "w")
    //use fwrite (buffer,1,512,img)
    //close all opened files

const int BYTES = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    char filenames[8] = "000.jpg";
    int i = 0, j = 0;
    FILE *memory = fopen(argv[1], "r");
    FILE *img;

    uint8_t buffer[BYTES];

    while (fread(buffer, 1, BYTES, memory) == BYTES)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (i == 0)
            {
                sprintf(filenames, "%03i.jpg", j);
                img = fopen(filenames, "w");
                fwrite(buffer, 1, BYTES, img);
                i++;
            }
            else
            {
                fclose(img);
                sprintf(filenames, "%03i.jpg", j);
                img = fopen(filenames, "w");
                fwrite(buffer, 1, BYTES, img);
            }
            j++;
        }
        else
        {
            if (i > 0)
            {
                fwrite(buffer, 1, BYTES, img);
            }
        }
    }
    fclose(img);
    fclose(memory);


}
