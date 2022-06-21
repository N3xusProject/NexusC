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

#include <AST.h>
#include <stdlib.h>
#include <stddef.h>

// Important for when we are deallocating resources.
static struct ASTNode** nodes_allocd = NULL;
static size_t allocd_nodes_idx = 0;

struct ASTNode* mkastnode(struct ASTNode* left, struct ASTNode* right, AST_OP op, int64_t intval)
{
    // Ensure nodes_allocd is not NULL, 
    // otherwise allocate memory for it.
    if (nodes_allocd == NULL)
    {
        nodes_allocd = malloc(sizeof(struct ASTnode*));
    }

    struct ASTNode* new = malloc(sizeof(struct ASTNode));
    new->op = op;
    new->left = left;
    new->right = right;
    new->intval = intval;
    
    // Add new node to allocd list.
    nodes_allocd[allocd_nodes_idx++] = new;

    // Realloc nodes_allocd list.
    nodes_allocd = realloc(nodes_allocd, sizeof(struct ASTNode*) * (allocd_nodes_idx + 1));

    return new;
}

struct ASTNode* mkastleaf(AST_OP op, int64_t intval)
{
    return mkastnode(NULL, NULL, op, intval);
}

