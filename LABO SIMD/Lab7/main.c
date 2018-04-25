#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main()
{
    time_t start, end;
    float dt;


    int W=1024, H=1024;
    unsigned char *src;
    unsigned char *dst;
    src = (unsigned char *) malloc (W*H*sizeof(unsigned char));
    dst = (unsigned char *) malloc (W*H*sizeof(unsigned char));

    if (src == NULL || dst == NULL) {
        printf ("Out of memory!");
        exit (1);
    }

    FILE *fp1 = fopen("test.raw","rb");
    if (fp1 != NULL) {
        fread(src , sizeof(unsigned char), W*H, fp1);
        fclose(fp1);}
    else{
        printf("Cannot open specified file!");
        exit (1);
    }

    int i;

    unsigned char *ptrin = src;
    unsigned char *ptrout = dst;
    int ii = (W*H)/16;
    unsigned char tresh[16]={32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32};
    unsigned char* foo = malloc(sizeof(unsigned char) * 16);
    memset(foo, 128, 16);
    start = clock();
    __asm__(
        "mov %[ptrin], %%esi\n"
        "mov %[ii], %%ecx\n"
        "mov %[ptrout], %%edi\n"
        "mov %[tresh], %%eax\n"
        "mov %[foo], %%ebx\n"
        "movdqu (%%eax), %%xmm7\n"
        "movdqu (%%ebx), %%xmm1\n"
        "psubb %%xmm1, %%xmm7\n"
    "l1:\n"
        "movdqu (%%esi), %%xmm0\n"
        "psubb %%xmm1, %%xmm0\n"
        "PCMPGTB %%xmm7 , %%xmm0\n"
        "movdqu %%xmm0, (%%edi)\n"
        "add $16, %%edi\n"
        "add $16, %%esi\n"
        "sub $1, %%ecx\n"
    "jnz l1\n"

        :"=m" (ptrout) //outputs
        :[ptrin]"m" (ptrin), [ii]"m" (ii),[ptrout]"m" (ptrout),[tresh] "g" (tresh),[foo] "g" (foo): //inputs
        "esi", "ecx", "edi", "eax" //clobbers

    );

    end = clock();
    FILE *fp2 = fopen("testwrite.raw","wb");
    if (fp2 != NULL) {
        fwrite(dst , sizeof(unsigned char), W*H, fp2);
        fclose(fp2);}
    else {
        printf("Cannot open specified file2!");
        exit (1);
    }
    free(src);
    free(dst);

    dt = (end-start)/(float)(CLOCKS_PER_SEC);
    printf("time to execute %f",dt);
    return 0;
}
