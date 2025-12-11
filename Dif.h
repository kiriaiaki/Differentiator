#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>

struct node_k
{
    unsigned long Verification;
    int Type;
    double Value;
    node_k* Left;
    node_k* Right;
    node_k* Daddy;
};

struct tree_k
{
    size_t Size;
    node_k* Root;
};

enum Name_Tree_Error
{
    Not_Error_Tree  =  0,
    Not_Struct_Tree =  1,
    Not_Root        =  2,
    Bad_Root        =  3,
    Bad_Type        =  4,
    Bad_Operation   =  5,
    Bad_Variable    =  6,
    Bad_Left        =  7,
    Bad_Right       =  8,
    Bad_Size        =  9,
    Quantity_Error  = 10
};

const char* Array_Name_Tree_Error[Quantity_Error] = {
    "NOT ERROR TREE",
    "!!!NOT STRUCT TREE!!!",
    "!!!NOT ROOT!!!",
    "!!!BAD ROOT!!!",
    "!!!BAD TYPE!!!",
    "!!!BAD OPERATION!!!",
    "!!!BAD VARIABLE!!!",
    "!!!BAD LEFT!!!",
    "!!!BAD RIGHT!!!",
    "!!!BAD SIZE!!!"
};

enum Type_Arguments
{
    Operation =  0,
    Number    =  1,
    Variable  =  2,
    Quantity_Types = 3
};

const char* Name_Types_Arguments[Quantity_Types] = {
    "Operation",
    "Number"   ,
    "Variable"
};

struct oper_k
{
    const char* Name_For_Dump;
    int Number_Operation;
    const char* Name_For_User;
};

enum Operation
{
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,

    POW = 4,
    LN  = 5,

    SIN =  6,
    COS =  7,
    TG  =  8,
    CTG =  9,

    ARCSIN = 10,
    ARCCOS = 11,
    ARCTG  = 12,
    ARCCTG = 13,

    SH  = 14,
    CH  = 15,
    TH  = 16,
    CTH = 17,

    PI  = 18,
    EXP = 19,

    Quantity_Operations = 20
};

const oper_k Name_Operation[Quantity_Operations] = {
    {"ADD", ADD, "+"},
    {"SUB", SUB, "-"},
    {"MUL", MUL, "*"},
    {"DIV", DIV, "/"},

    {"POW", POW, "^" },
    {"LN" , LN , "ln"},

    {"SIN", SIN, "sin"},
    {"COS", COS, "cos"},
    {"TG" , TG , "tg" },
    {"CTG", CTG, "ctg"},

    {"ARCSIN", ARCSIN, "arcsin"},
    {"ARCCOS", ARCCOS, "arccos"},
    {"ARCTG" , ARCTG , "arctg" },
    {"ARCCTG", ARCCTG, "arcctg"},

    {"SH" , SH , "sh" },
    {"CH" , CH , "ch" },
    {"TH" , TH , "th" },
    {"CTH", CTH, "cth"},

    {"PI" , PI , "pi"},
    {"EXP", EXP, "e" }
};

enum Type_Degree
{
    Not_Variable     = -1,
    Only_Base        =  0,
    Only_Degree      =  1,
    Base_And_Degree  =  2,

};

struct variable_k
{
    char* Name_Variable;
    int Number_Variable;
};

variable_k* Array_Variable   = NULL;
size_t Size_Array_Variable   =    0;
int Variable_Differentiation =   -1;

const int There_Are_Errors =           -2902;
const int Canary           =            2108;
const char Name_Log[]      = "Log_File.html";
const char Equation[]      =  "Equation.txt";
const double Minimum_Contrast_Ratio    =   6;

int Print_Node (const node_k* const Node);

int Tree_Ctor (tree_k* const Tree);
int Tree_Dtor (tree_k* const Tree);

int Tree_Error         (const tree_k* const Tree);
int Check_Verification (const node_k* Node, size_t* const Counter_Element);

int Tree_Dump          (const tree_k* const Tree, const char* const Name_Function);
int Naming_File        (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph     (const tree_k* const Tree, FILE* const file_graph);
int Dump_Node          (const node_k* const Node, FILE* const file_graph);
int Dump_For_Html      (const tree_k* const Tree, const char* const Name_File, const char* const Name_Function);

int Delete_Subtree  (node_k* Node, size_t* const Counter_Delete);
size_t Size_Subtree (const node_k* const Node);

node_k* Get_General            (char** Str, const char* const Start_Str);
node_k* Get_For_Addition       (char** Str, const char* const Start_Str);
node_k* Get_For_Multiplication (char** Str, const char* const Start_Str);
node_k* Get_For_Power          (char** Str, const char* const Start_Str);
node_k* Get_Bracket            (char** Str, const char* const Start_Str);
node_k* Get_Text               (char** Str, const char* const Start_Str);
node_k* Get_Function           (char** Str, const char* const Start_Str);
node_k* Get_Double             (char** Str, const char* const Start_Str);
int Get_Number                 (char** Str, const char* const Start_Str);

int Table_Name_Function (const char* const Str);
int Table_Name_Variable (const char* const Str);
int Skip_Spaces         (char** Str);
int Read_Diff_Variable  (char** Str);

tree_k* Differentiation_Tree (const tree_k* const Tree, const int Number_Variable);
node_k* Differentiation_Node (const node_k* const Node, const int Number_Variable);
node_k* Copy_Tree            (const node_k* const Node);
node_k* Create_Node          (const int Type, const double Value, node_k* const Left, node_k* const Right);
int Find_Variable            (const node_k* const Node, const int Number_Variable);
int Mini_Find_Variable       (const node_k* const Node, const int Number_Variable);

int Start_Logfile           ();
int Print_Separator_In_Log  (const size_t Len_Separator, FILE* file_html);
int Reverse_Str             (char* const Str);
char* itoa_k                (int Number, char* const Str);
unsigned int Generate_Color (const void *Ptr, unsigned int Color_Text);
unsigned int Hash_Pointer   (const void* const ptr);
double Calculate_Contrast   (unsigned int Color, unsigned int Color_Text);
int Copy_File_In_Buffer     (char** const Buffer);
int Clean_Stdin             ();
char* Read_Answer           ();
int getline_k               (char** const Str);
int Delete_Slash_N          (char* const Str);
int Count_Digits            (int Number);
