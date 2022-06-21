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
#include <stdio.h>
#include <stddef.h>

extern FILE* in_file;
static size_t line_num = 1;

static char next(void) {
    char c = fgetc(in_file);

    if (c == '\n')
    {
        ++line_num;
    }

    return c;
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
    }

    return true;
}
