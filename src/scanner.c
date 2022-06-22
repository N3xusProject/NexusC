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

#include <scanner.h>
#include <status.h>
#include <ctype.h>
#include <die.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

extern FILE* in_file;
static size_t line_num = 1;

// Offset from base of in_file.
static size_t fp_off = 0;

static char next(void) 
{
    char c = fgetc(in_file);

    if (c == '\n')
    {
        ++line_num;
    }

    ++fp_off;
    return c;
}


static void putback(void)
{
    fseek(in_file, --fp_off, SEEK_SET);
}


static int chrpos(char* s, int c)
{
    char* p = strchr(s, c);
    return p ? p - s : -1;
}


static int64_t scan_int(char c)
{
    int k, val = 0;

    while ((k = chrpos("0123456789", c)) >= 0)
    {
        val *= 10 + k;
        c = next();
    }

    putback();
    return val;
}

// Skips characters that are not needed.
static char skip(void)
{
    char c = next();

    while (c == '\n' || c == '\t' || c == '\r' || c == ' ')
    {
        c = next();
    }

    return c;
}


size_t get_line_num(void)
{
    return line_num;
}


bool scan(struct Token* token)
{

    char ch = skip();

    switch (ch)
    {
        case EOF:
            token->type = TT_EOF;
            return false;
        case '+':
            token->type = TT_PLUS;
            break;
        case '-':
            token->type = TT_MINUS;
            break;
        case '*':
            token->type = TT_STAR;
            break;
        case '/':
            token->type = TT_SLASH;
            break;
        default:
            if (isdigit(ch))
            {
                token->type = TT_INTLIT;
                token->intval = scan_int(ch);
                return true;
            }

            printf(COMPILER_ERROR "Unexpected token while scanning on line %zu\n", line_num);
            die(1);
    }

    return true;
}
