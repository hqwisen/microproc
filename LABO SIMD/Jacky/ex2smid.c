#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[]){


    FILE *image_raw, *output;
    time_t start, end;
    float dt;
    start = clock();

    unsigned char buffer[4];
    unsigned char out[4];
    unsigned char zero = 0x00;
    unsigned char max = 0xFF;
    unsigned char threshold = 0x20;
    image_raw = fopen("test.raw", "rb");
    output = fopen("output.raw","wb+");
    int bytes_read;

    while ((bytes_read = fread(buffer,1, sizeof(buffer), image_raw)) > 0){
        int i;
        for (i = 0; i < bytes_read; i++){
            if (buffer[i] <= threshold){
                out[i] = zero;
            }
            else{
                out[i] = max;
            }
        }
        fwrite(out,1,bytes_read,output);
    }


    fclose(image_raw);
    fclose(output);
    end = clock();
    dt = (end-start)/(float)(CLOCKS_PER_SEC);
    printf("%f\n",dt);

    return 0;
}
