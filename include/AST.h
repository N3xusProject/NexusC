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

#ifndef AST_H
#define AST_H

#include <stdint.h>


typedef enum
{
    A_INTLIT,
    A_ADD,
    A_SUB,
    A_MUL,
    A_DIV
} AST_OP;


struct ASTNode
{
    AST_OP op;
    struct ASTNode* left;
    struct ASTNode* right;
    int64_t intval;
};


struct ASTNode* mkastnode(struct ASTNode* left, struct ASTNode* right, AST_OP op, int64_t intval);
struct ASTNode* mkastleaf(AST_OP op, int64_t intval);


#endif
