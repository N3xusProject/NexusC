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


#include <parse.h>
#include <scanner.h>
#include <token.h>
#include <expr.h>
#include <stdio.h>
#include <stdint.h>

struct Token current_token;

#define SCAN \
    scan(&current_token);  \
    printf("%d\n", current_token.type);


static int64_t interpret_ast(struct ASTNode* ast)
{

    int64_t leftval, rightval;

    if (ast->left)
    {
        leftval = interpret_ast(ast->left);
    }

    if (ast->right)
    {
        rightval = interpret_ast(ast->right);
    }

    switch (ast->op)
    {
        case A_ADD:
            return leftval + rightval;
        case A_SUB:
            return leftval - rightval;
        case A_MUL:
            return leftval * rightval;
        case A_DIV:
            return leftval / rightval;
        case A_INTLIT:
            return ast->intval;
    }
}

void parse(void)
{
    SCAN;
    struct ASTNode* expr = binexpr();
    printf("RESULT: %ld\n", interpret_ast(expr));
}
