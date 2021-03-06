/*
 *  MIT License
 *
 *  Copyright (c) 2022 Nexus-C, Ian Marco Moffett
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <parse.h>

FILE* in_file = NULL;


void die(unsigned int errno)
{
    if (in_file != NULL)
    {
        fclose(in_file);
    }

    exit(errno);
}


static void help(void) 
{
    printf("--help - displays this message\n");
    die(0);
}


static void compile(const char* filename)
{
    in_file = fopen(filename, "r");

    if (in_file == NULL)
    {
        printf("nexcc: File \"%s\" not found!\n", filename);
        die(1);
    }

    parse();
    fclose(in_file);
}


int main(int argc, char** argv)
{
    // Check for correct usage.
    if (argc < 2) {
        printf("nexcc: Invalid usage, please specify a filename or use --help.\n");
        die(1);
    }

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--help") == 0)
        {
            help();
        }

        // It is a filename.
        compile(argv[i]);
    }
}
