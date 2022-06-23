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


#include <expr.h>
#include <scanner.h>
#include <token.h>
#include <AST.h>
#include <die.h>
#include <stdio.h>
#include <status.h>


extern struct Token current_token;


static struct ASTNode* primary(void)
{
    struct ASTNode* n = NULL;

    if (current_token.type == TT_INTLIT)
    {
        n = mkastleaf(A_INTLIT, current_token.intval);
        scan(&current_token);
        return n;
    }
    else
    {
        printf(COMPILER_ERROR "Expected integer literal near line %zu\n", get_line_num());
        die(1);
    }

    return NULL;
}


static AST_OP arithop(TOKEN_TYPE token)
{
    switch (token)
    {
        case TT_INTLIT:
            return A_INTLIT;
        case TT_PLUS:
            return A_ADD;
        case TT_MINUS:
            return A_SUB;
        case TT_STAR:
            return A_MUL;
        case TT_SLASH:
            return A_DIV;
        default:
            printf(COMPILER_ERROR "Unexpected token caught in __nexcc_internal_%s(), line %zu\n", __func__, get_line_num());
            die(1);
    }

    return 1;
}



struct ASTNode* binexpr(void)
{
    struct ASTNode* left = NULL;
    struct ASTNode* right = NULL;

    left = primary();

    if (current_token.type == TT_EOF)
    {
        return left;
    }

    AST_OP root_operator = arithop(current_token.type);
    scan(&current_token);

    right = binexpr();

    return mkastnode(left, right, root_operator, current_token.intval);
}
