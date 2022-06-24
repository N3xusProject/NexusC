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

#define NOREG -1
#define MAX_REGS 4


typedef int8_t REG_COUNT;
FILE* out_file = NULL;

static const char* const REGLIST[MAX_REGS] = {"r8", "r9", "r10", "r11"};

// A bitmap for registers.
// 1 taken, 0 free.
static REG_COUNT reg_bmp = 0;

// Allocates a register.
static REG_COUNT alloc_reg(void)
{
    for (REG_COUNT i = 0; i < MAX_REGS; ++i)
    {
        if (!(reg_bmp & (1 << i)))
        {
            reg_bmp |= (1 << i);
            return i;
        }
    }

    return NOREG;
}

// De-allocates a register.
static void free_reg(REG_COUNT reg)
{
    reg_bmp &= ~(1 << reg);
}


static void free_all_regs(void)
{
    reg_bmp = 0;
}


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
            "\tmov rsi, rdi\n"
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


// Loads a register with a value
// and returns that value.
static REG_COUNT load_reg(int64_t value)
{
    REG_COUNT r = alloc_reg();
    fprintf(out_file, "\tmov %s, %ld\n", REGLIST[r], value);
    return r;
}


// Adds two registers together and returns
// the register that holds the result.
static REG_COUNT add_regs(REG_COUNT r1, REG_COUNT r2)
{
    fprintf(out_file, "\tadd %s, %s\n", REGLIST[r2], REGLIST[r1]);
    free_reg(r1);
    return r2;
}


static REG_COUNT mul_regs(REG_COUNT r1, REG_COUNT r2)
{
    fprintf(out_file, "\timul, %s, %s\n", REGLIST[r2], REGLIST[r1]);
    free_reg(r2);
    return r1;
}

static REG_COUNT sub_regs(REG_COUNT r1, REG_COUNT r2)
{
    fprintf(out_file, "\tsub %s, %s\n", REGLIST[r1], REGLIST[r2]);
    free_reg(r1);
    return r2;
}

static REG_COUNT div_regs(REG_COUNT r1, REG_COUNT r2)
{
    fprintf(out_file, "\tmov rax, %s\n", REGLIST[r1]);
    fprintf(out_file, "\tcqo\n");
    fprintf(out_file, "\tidiv %s\n", REGLIST[r2]);
    fprintf(out_file, "\tmov %s, rax\n", REGLIST[r1]);
    free_reg(r2);
    return r1;
}

static REG_COUNT reg_load(int64_t v) 
{
    REG_COUNT r = alloc_reg();
    fprintf(out_file, "\tmov %s, %ld\n", REGLIST[r], v);
    return r;
}

static void print_int(REG_COUNT reg)
{
    fprintf(out_file, "\tmov rdi, %s\n", REGLIST[reg]);
    fprintf(out_file, "\tcall print_int\n");
}


static int64_t interpret_ast(struct ASTNode* ast)
{

    int64_t leftreg, rightreg;

    if (ast->right)
    {
        rightreg = interpret_ast(ast->right);
    }

    if (ast->left)
    {
        leftreg = interpret_ast(ast->left);
    }

    while (1);

    switch (ast->op)
    {
        case A_ADD:
            return add_regs(leftreg, rightreg);
        case A_SUB:
            return sub_regs(leftreg, rightreg);
        case A_MUL:
            return mul_regs(leftreg, rightreg);
        case A_DIV:
            return mul_regs(leftreg, rightreg);
        case A_INTLIT:
            return reg_load(ast->intval);
    }

    return 1;
}


void gencode(struct ASTNode* ast)
{
    out_file = fopen("/tmp/nexcc_out.s", "w");
    genprologue();
    interpret_ast(ast);
    // print_int(interpret_ast(ast));
    genepilogue();
    fclose(out_file);
}
