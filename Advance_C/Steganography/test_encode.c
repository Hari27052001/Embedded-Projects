
#include <stdio.h>

#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if (argc < 2)
    {
        printf("Usage:\n");
        printf("./a.out -e <source.bmp> <secret_file> [output.bmp]\n");
        printf("./a.out -d <stego.bmp>\n");
        return 1;
    }

    OperationType op_type = check_operation_type(argv);

    switch(op_type)
    {
        case e_encode:

            printf("INFO: Selected Encoding\n");

            if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
            {
                printf("ERROR: Invalid encoding arguments\n");
                return 1;
            }

            printf("INFO: Encoding Started...\n");

            if (do_encoding(&encInfo) == e_failure)
            {
                printf("ERROR: Encoding Failed\n");
                return 1;
            }

            printf("INFO: Encoding Successful\n");
            break;

        case e_decode:

            printf("INFO: Selected Decoding\n");

            if (read_and_validate_decode_args(argv, &decInfo) == e_failure)
            {
                printf("ERROR: Invalid decoding arguments\n");
                return 1;
            }

            printf("INFO: Decoding Started...\n");

            if (do_decoding(&decInfo) == e_failure)
            {
                printf("ERROR: Decoding Failed\n");
                return 1;
            }

            printf("INFO: Decoding Successful\n");
            break;

        default:

            printf("ERROR: Unsupported Operation\n");
            printf("Usage:\n");
            printf("./stego -e <source.bmp> <secret_file> [output.bmp]\n");
            printf("./stego -d <stego.bmp>\n");
            return 1;
    }

    return 0;
}
