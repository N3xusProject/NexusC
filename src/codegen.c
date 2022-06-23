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

#include <codegen.h>
#include <stdio.h>
#include <stddef.h>

FILE* out_file = NULL;


static void genprologue(void)
{
    fputs(
            "global main\n"
            "default rel\n"
            "extern printf\n"
            "section .rodata\n"
            "intfmt: db \"%d\", 0xA, 0\n"
            "section .text\n"
            "print_int:\n"
            "\tpush rbp\n"
            "\tmov rbp, rsp\n"
            "\tsub rsp, 16\n"
            "\tmov esi, edi\n"
            "\tlea rdi, [rel intfmt]\n"
            "\txor eax, eax\n"
            "\tcall printf\n"
            "\tleave\n"
            "\tret\n"
            "\nmain:\n"
            "\tpush rbp\n"
            "\tmov rbp, rsp\n"
            "\tsub rsp, 16\n",
            out_file);
}


static void genepilogue(void) 
{
    fputs(
            "\tmov eax, 0\n"
            "\tleave\n"
            "\tret\n", out_file);
}


static int64_t interpret_ast(struct ASTNode* ast)
{

    int64_t leftval, rightval;

    if (ast->right)
    {
        rightval = interpret_ast(ast->right);
    }

    if (ast->left)
    {
        leftval = interpret_ast(ast->left);
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

    return 1;
}


void gencode(struct ASTNode* ast)
{
    out_file = fopen("/tmp/nexcc_out.s", "w");
    genprologue();
    genepilogue();
    fclose(out_file);
}
