#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encode.h"
#include "common.h"
#include "types.h"

/* Check operation type */
OperationType check_operation_type(char *argv[])
{
    if (argv[1] == NULL)
        return e_unsupported;

    if (strcmp(argv[1], "-e") == 0)
        return e_encode;

    if (strcmp(argv[1], "-d") == 0)
        return e_decode;

    return e_unsupported;
}

/* Validate arguments */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if (argv[2] == NULL)
    {
        printf("BMP file not passed\n");
        return e_failure;
    }

    if (strstr(argv[2], ".bmp") == NULL)
    {
        printf("Invalid BMP file\n");
        return e_failure;
    }

    encInfo->src_image_fname = argv[2];

    if (argv[3] == NULL)
    {
        printf("Secret file missing\n");
        return e_failure;
    }

    if (strchr(argv[3], '.') == NULL)
    {
        printf("Invalid secret file\n");
        return e_failure;
    }

    encInfo->secret_fname = argv[3];

    if (argv[4] == NULL)
        encInfo->stego_image_fname = "stego.bmp";
    else
        encInfo->stego_image_fname = argv[4];

    return e_success;
}

/* Open files */
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");

    if (encInfo->fptr_src_image == NULL)
        return e_failure;

    encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");

    if (encInfo->fptr_secret == NULL)
        return e_failure;

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");

    if (encInfo->fptr_stego_image == NULL)
        return e_failure;

    return e_success;
}

/* Get file size */
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);

    return size;
}

/* Get bmp image size */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    fseek(fptr_image, 0, SEEK_END);
    uint size = ftell(fptr_image);
    rewind(fptr_image);

    return size;
}

/* Check capacity */
Status check_capacity(EncodeInfo *encInfo)
{
    strcpy(encInfo->extn_secret_file,
           strchr(encInfo->secret_fname, '.'));

    encInfo->extn_size =
        strlen(encInfo->extn_secret_file);

    encInfo->size_secret_file =
        get_file_size(encInfo->fptr_secret);

    uint image_size =
        get_image_size_for_bmp(encInfo->fptr_src_image);

    int required_size =
        54 +
        (strlen(MAGIC_STRING) * 8) +
        32 +
        (encInfo->extn_size * 8) +
        32 +
        (encInfo->size_secret_file * 8);

    if (required_size <= image_size)
        return e_success;

    return e_failure;
}

/* Copy BMP header */
Status copy_bmp_header(FILE *fptr_src_image,
                       FILE *fptr_dest_image)
{
    char header[54];

    fread(header, 54, 1, fptr_src_image);
    fwrite(header, 54, 1, fptr_dest_image);

    return e_success;
}

/* Encode byte into 8 bytes */
Status encode_byte_to_lsb(char data,
                          char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) |
            ((data >> (7 - i)) & 1);
    }

    return e_success;
}

/* Encode 32-bit integer */
Status encode_size_to_lsb(int data,
                          char *image_buffer)
{
    for (int i = 0; i < 32; i++)
    {
        image_buffer[i] =
            (image_buffer[i] & 0xFE) |
            ((data >> (31 - i)) & 1);
    }

    return e_success;
}

/* Encode magic string */
Status encode_magic_string(const char *magic_string,
                           EncodeInfo *encInfo)
{
    for (int i = 0; magic_string[i] != '\0'; i++)
    {
        char buffer[8];

        fread(buffer, 8, 1,
              encInfo->fptr_src_image);

        encode_byte_to_lsb(magic_string[i],
                           buffer);

        fwrite(buffer, 8, 1,
               encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Encode extension size */
Status encode_extn_file_size(int extn_size,
                             EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer, 32, 1,
          encInfo->fptr_src_image);

    encode_size_to_lsb(extn_size,
                       buffer);

    fwrite(buffer, 32, 1,
           encInfo->fptr_stego_image);

    return e_success;
}

/* Encode extension */
Status encode_secret_file_extn(
    const char *file_extn,
    EncodeInfo *encInfo)
{
    for (int i = 0; file_extn[i] != '\0'; i++)
    {
        char buffer[8];

        fread(buffer, 8, 1,
              encInfo->fptr_src_image);

        encode_byte_to_lsb(file_extn[i],
                           buffer);

        fwrite(buffer, 8, 1,
               encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(
    long file_size,
    EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer, 32, 1,
          encInfo->fptr_src_image);

    encode_size_to_lsb(file_size,
                       buffer);

    fwrite(buffer, 32, 1,
           encInfo->fptr_stego_image);

    return e_success;
}

/* Encode secret file data */
Status encode_secret_file_data(
    EncodeInfo *encInfo)
{
    char ch;

    while (fread(&ch, 1, 1,
                  encInfo->fptr_secret) == 1)
    {
        char buffer[8];

        fread(buffer, 8, 1,
              encInfo->fptr_src_image);

        encode_byte_to_lsb(ch,
                           buffer);

        fwrite(buffer, 8, 1,
               encInfo->fptr_stego_image);
    }

    return e_success;
}

/* Copy remaining image */
Status copy_remaining_img_data(
    FILE *fptr_src,
    FILE *fptr_dest)
{
    char ch;

    while (fread(&ch, 1, 1,
                  fptr_src) == 1)
    {
        fwrite(&ch, 1, 1,
               fptr_dest);
    }

    return e_success;
}

/* Main encoding flow */
Status do_encoding(EncodeInfo *encInfo)
{
    if (open_files(encInfo) == e_failure)
        return e_failure;

    if (check_capacity(encInfo) == e_failure)
        return e_failure;

    copy_bmp_header(
        encInfo->fptr_src_image,
        encInfo->fptr_stego_image);

    encode_magic_string(
        MAGIC_STRING,
        encInfo);

    encode_extn_file_size(
        encInfo->extn_size,
        encInfo);

    encode_secret_file_extn(
        encInfo->extn_secret_file,
        encInfo);

    encode_secret_file_size(
        encInfo->size_secret_file,
        encInfo);

    encode_secret_file_data(
        encInfo);

    copy_remaining_img_data(
        encInfo->fptr_src_image,
        encInfo->fptr_stego_image);

    return e_success;
}
