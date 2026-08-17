
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decode.h"
#include "common.h"
#include "types.h"

/* Validate decode arguments */
Status read_and_validate_decode_args(char *argv[],
                                     DecodeInfo *decInfo)
{
    if (argv[2] == NULL)
    {
        printf("ERROR: Stego image file not provided\n");
        return e_failure;
    }

    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("ERROR: Invalid BMP file\n");
        return e_failure;
    }

    decInfo->stego_image_fname = argv[2];

    return e_success;
}

/* Open stego image */
Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image =
        fopen(decInfo->stego_image_fname, "rb");

    if (decInfo->fptr_stego_image == NULL)
    {
        printf("ERROR: Unable to open file %s\n",
               decInfo->stego_image_fname);
        return e_failure;
    }

    return e_success;
}

/* Decode one byte from 8 image bytes */
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;

    for (int i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= (image_buffer[i] & 1);
    }

    return data;
}

/* Decode integer from 32 image bytes */
int decode_size_from_lsb(char *image_buffer)
{
    int data = 0;

    for (int i = 0; i < 32; i++)
    {
        data <<= 1;
        data |= (image_buffer[i] & 1);
    }

    return data;
}

/* Decode and verify magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[8];
    char magic[3];

    for (int i = 0; i < 2; i++)
    {
        fread(buffer, 8, 1,
              decInfo->fptr_stego_image);

        magic[i] =
            decode_byte_from_lsb(buffer);
    }

    magic[2] = '\0';

    if (strcmp(magic, MAGIC_STRING) == 0)
    {
        return e_success;
    }

    return e_failure;
}

/* Decode extension size and extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 32, 1,
          decInfo->fptr_stego_image);

    decInfo->extn_size =
        decode_size_from_lsb(buffer);

    for (int i = 0; i < decInfo->extn_size; i++)
    {
        char temp[8];

        fread(temp, 8, 1,
              decInfo->fptr_stego_image);

        decInfo->extn_secret_file[i] =
            decode_byte_from_lsb(temp);
    }

    decInfo->extn_secret_file[
        decInfo->extn_size] = '\0';

    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer, 32, 1,
          decInfo->fptr_stego_image);

    decInfo->size_secret_file =
        decode_size_from_lsb(buffer);

    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char buffer[8];

    for (long i = 0;
         i < decInfo->size_secret_file;
         i++)
    {
        fread(buffer, 8, 1,
              decInfo->fptr_stego_image);

        char ch =
            decode_byte_from_lsb(buffer);

        fwrite(&ch, 1, 1,
               decInfo->fptr_output);
    }

    return e_success;
}

/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_failure)
    {
        return e_failure;
    }

    /* Skip BMP header */
    fseek(decInfo->fptr_stego_image,
          54,
          SEEK_SET);

    /* Verify magic string */
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: Magic String Mismatch\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Decode extension */
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Create output filename */
    strcpy(decInfo->secret_fname,
           "decoded");

    strcat(decInfo->secret_fname,
           decInfo->extn_secret_file);

    /* Open output file */
    decInfo->fptr_output =
        fopen(decInfo->secret_fname, "wb");

    if (decInfo->fptr_output == NULL)
    {
        printf("ERROR: Unable to create output file\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }

    /* Decode file size */
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    /* Decode file data */
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    fclose(decInfo->fptr_stego_image);
    fclose(decInfo->fptr_output);

    return e_success;
}

