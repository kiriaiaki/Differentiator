#include "Dif.h"

#define DEBUG // включает верификаторы и создание logfile
//#define STOP_PROGRAMME // в случае выявления ошибок программа останавливается
//#define LIGHT_DUMP

#define d(NODE_PTR) Differentiation_Node (NODE_PTR, Number_Variable)
#define c(NODE_PTR) Copy_Tree (NODE_PTR)
#define L Node->Left
#define R Node->Right

#define ADD_(LEFT, RIGHT) Create_Node(Operation, ADD, LEFT, RIGHT)
#define SUB_(LEFT, RIGHT) Create_Node(Operation, SUB, LEFT, RIGHT)
#define MUL_(LEFT, RIGHT) Create_Node(Operation, MUL, LEFT, RIGHT)
#define DIV_(LEFT, RIGHT) Create_Node(Operation, DIV, LEFT, RIGHT)
#define POW_(LEFT, RIGHT) Create_Node(Operation, POW, LEFT, RIGHT)
#define LN_(LEFT)         Create_Node(Operation, LN,  LEFT, NULL)
#define SIN_(LEFT)        Create_Node(Operation, SIN, LEFT, NULL)
#define COS_(LEFT)        Create_Node(Operation, COS, LEFT, NULL)
#define SH_(LEFT)         Create_Node(Operation, SH,  LEFT, NULL)
#define CH_(LEFT)         Create_Node(Operation, CH,  LEFT, NULL)

#define MINUS_ONE_ Create_Node(Number, -1,  NULL, NULL)
#define ZERO_      Create_Node(Number, 0,   NULL, NULL)
#define HALF_      Create_Node(Number, 0.5, NULL, NULL)
#define ONE_       Create_Node(Number, 1,   NULL, NULL)
#define TWO_       Create_Node(Number, 2,   NULL, NULL)

int main ()
{
    #ifdef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    tree_k Tree = {};

    if (Tree_Ctor (&Tree) == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return 0;
    }

    tree_k* Tree_1 = Differentiation_Tree (&Tree, Variable_Differentiation);

    Print_Node (Tree.Root);
    printf ("\n");

    Tree_Dtor (&Tree);
    Tree_Dtor (Tree_1);
    #ifndef DEBUG
        if (Start_Logfile () == There_Are_Errors)
        {
            printf ("%s:%d: Error in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }

        if (Tree_Dump (&Tree, "After all function") == There_Are_Errors)
        {
            printf ("%s:%d: Error dump in %s", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    free (Array_Variable);
    return 0;
}

int Print_Node (const node_k* const Node)
{
    switch (Node->Type)
    {
        case Number:
            break;
        case Operation:
            switch (int(Node->Value))
            {
                case ADD:
                    printf ("( ");
                    break;
                case SUB:
                    printf ("( ");
                    break;
            }
            break;
        case Variable:
            break;
    }

    if (Node->Left != NULL)
    {
        Print_Node (Node->Left);
    }

    switch (Node->Type)
    {
        case Number:
            printf ("%g ", Node->Value);
            break;
        case Operation:
            printf ("%s ", Name_Operation[int(Node->Value)].Name_For_User);
            break;
        case Variable:
            printf ("%s ", Array_Variable[int(Node->Value)].Name_Variable);
            break;
    }

    if (Node->Right != NULL)
    {
        Print_Node (Node->Right);
    }

    switch (Node->Type)
    {
        case Number:
            break;
        case Operation:
            switch (int(Node->Value))
            {
                case ADD:
                    printf (") ");
                    break;
                case SUB:
                    printf (") ");
                    break;
            }
            break;
        case Variable:
            break;
    }

    return 0;
}

int Tree_Ctor (tree_k* const Tree)
{
    char* Buffer = NULL;
    if (Copy_File_In_Buffer (&Buffer) == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }
    char* Copy_Buffer = Buffer;

    Tree->Root = Get_General (&Buffer, Copy_Buffer);
    if (Tree->Root == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Tree->Size = Size_Subtree (Tree->Root);
    if (Tree->Size == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Variable_Differentiation = Read_Diff_Variable (&Buffer);

    free (Copy_Buffer);

    #ifdef DEBUG
        if (Tree_Dump (Tree, "Tree_Ctor_From_Base (Tree, Buffer)") == There_Are_Errors)
        {
            printf ("%s:%d Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    #endif // DEBUG

    return 0;
}

int Tree_Dtor (tree_k* const Tree)
{
    #ifdef DEBUG
        if (Tree_Error (Tree) != Not_Error_Tree)
        {
            if (Tree_Dump (Tree, "Tree_Dtor (Tree)") == There_Are_Errors)
            {
                printf ("%s:%d Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }

            printf ("%s:%d Error start in %s\n", __FILE__, __LINE__, __FUNCTION__);
        }
    #endif // DEBUG

    size_t Counter_Delete = 0;

    if (Tree->Root != NULL && Tree->Root->Verification == ((uintptr_t) Tree->Root ^ Canary))
    {
        if (Delete_Subtree (Tree->Root, &Counter_Delete) == There_Are_Errors)
        {
            printf ("%s:%d Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        }
    }

    Tree->Size = 0;

    free (Tree->Root);
    Tree->Root = NULL;

    return 0;
}


int Tree_Error         (const tree_k* const Tree)
{
    if (Tree == NULL)
    {
        printf ("%s:%d: Error %s in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Not_Struct_Tree], __FUNCTION__);
        return Not_Struct_Tree;
    }

    if (Tree->Root == NULL)
    {
        printf ("%s:%d: Error %s in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Not_Root], __FUNCTION__);
        return Not_Root;
    }

    if (Tree->Root->Verification != ((uintptr_t) Tree->Root ^ Canary))
    {
        printf ("%s:%d: Error %s in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Root], __FUNCTION__);
        return Bad_Root;
    }

    size_t Counter_Element = 0;
    int Er = Check_Verification (Tree->Root, &Counter_Element);
    if (Er != Not_Error_Tree)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return Er;
    }

    if (Counter_Element != Tree->Size)
    {
        printf ("%s:%d: Error %s in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Size], __FUNCTION__);
        return Bad_Size;
    }

    return Not_Error_Tree;
}

int Check_Verification (const node_k* Node, size_t* const Counter_Element)
{
    if (Node->Type < 0 || Node->Type >= Quantity_Types)
    {
        printf ("%s:%d: Error %s (node %p) in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Type], Node, __FUNCTION__);
        return Bad_Type;
    }

    if (Node->Type == Operation)
    {
        if (Node->Value < 0 || Node->Value >= Quantity_Operations)
        {
            printf ("%s:%d: Error %s (node %p) in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Operation], Node, __FUNCTION__);
            return Bad_Operation;
        }
    }

    if (Node->Type == Variable)
    {
        if (Node->Value < 0 || Node->Value >= Size_Array_Variable)
        {
            printf ("%s:%d: Error %s (node %p) in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Variable], Node, __FUNCTION__);
            return Bad_Variable;
        }
    }

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification != ((uintptr_t) (Node->Left) ^ Canary))
        {
            printf ("%s:%d: Error %s (node %p) in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Left], Node, __FUNCTION__);
            return Bad_Left;
        }

        else
        {
            int A = Check_Verification (Node->Left, Counter_Element);
            if (A != Not_Error_Tree)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return A;
            }
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification != ((uintptr_t) (Node->Right) ^ Canary) )
        {
            printf ("%s:%d: Error %s (node %p) in %s\n", __FILE__, __LINE__, Array_Name_Tree_Error[Bad_Right], Node, __FUNCTION__);
            return Bad_Right;
        }

        else
        {
            int B = Check_Verification (Node->Right, Counter_Element);
            if (B != Not_Error_Tree)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return B;
            }
        }
    }

    (*Counter_Element)++;
    return Not_Error_Tree;
}


int Tree_Dump          (const tree_k* const Tree, const char* const Name_Function)
{
    char Name_File_Graph[30] = "File_txt/File_";

    Naming_File (Name_File_Graph);

    FILE* file_graph = fopen (Name_File_Graph, "w");
    if (file_graph == NULL)
    {
        printf ("%s:%d Error open file for graph in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Dump_For_Graph (Tree, file_graph);

    fclose (file_graph);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File_Graph);

    system (Command);

    if (Dump_For_Html (Tree, Name_File_Graph, Name_Function) != Not_Error_Tree)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    return 0;
}

int Naming_File        (char* const Name_File)
{
    static int Counter = 1;

    char Temp_Str[11];
    itoa_k (Counter, Temp_Str);

    strcat(Name_File, Temp_Str);
    strcat(Name_File, ".txt");

    Counter++;
    return 0;
}

int Naming_Command_Dot (char* const Name_Command, char* const Name_File)
{
    strcat(Name_Command, Name_File);
    strcat(Name_Command, " -T png -o ");

    Name_File[5] = 'p';
    Name_File[6] = 'n';
    Name_File[7] = 'g';
    size_t Len = strlen (Name_File);
    Name_File[Len - 3] = 'p';
    Name_File[Len - 2] = 'n';
    Name_File[Len - 1] = 'g';
    strcat(Name_Command, Name_File);

    return 0;
}

int Dump_For_Graph     (const tree_k* const Tree, FILE* const file_graph)
{
    fprintf (file_graph, "digraph G\n"
                   "{\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file_graph, "\n");

    #ifndef LIGHT_DUMP
        fprintf (file_graph, "    root [shape = invhouse, label = \"root = %p\", style = \"filled\", fillcolor = \"#%06x\"];\n", Tree->Root, Generate_Color (Tree->Root, 0x000000));
        fprintf (file_graph, "    root -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Tree->Root, Generate_Color (Tree->Root, 0x000000));
        fprintf (file_graph, "\n");
    #endif // LIGHT_DUMP

    if (Tree->Root != NULL && Tree->Root->Verification == ((uintptr_t) Tree->Root ^ Canary))
    {
        Dump_Node (Tree->Root, file_graph);
    }

    fprintf (file_graph, "}\n");
    return 0;
}

int Dump_Node          (const node_k* const Node, FILE* const file_graph)
{
    #ifndef LIGHT_DUMP
        if (Node->Type == Operation)
        {
            fprintf (file_graph, "    node_%lx [shape = Mrecord, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\" BGCOLOR = \"lightgrey\">\n"
                                "        <TR> <TD COLSPAN = \"2\" BGCOLOR = \"#%06x\"> <b>ptr</b>   = %p  </TD> </TR>\n"
                                "        <TR> <TD COLSPAN = \"2\">          <b>ver</b>   = %lx </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"lightgreen\"> <b>type</b>  = %s </TD>\n"
                                "             <TD BGCOLOR = \"pink\">       <b>value</b> = %s </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"#%06x\">      <b>left</b>  = %p </TD>\n"
                                "             <TD BGCOLOR = \"#%06x\">      <b>right</b> = %p </TD> </TR>\n"
                                "        </TABLE>>, style = \"filled\", fillcolor = \"lightgrey\"];\n\n", (uintptr_t) Node, Generate_Color (Node, 0x000000), Node, Node->Verification, Name_Types_Arguments[Node->Type], Name_Operation[int (Node->Value)].Name_For_Dump, Generate_Color (Node->Left, 0x000000), Node->Left, Generate_Color (Node->Right, 0x000000), Node->Right);
        }

        else if (Node->Type == Number)
        {
                fprintf (file_graph, "    node_%lx [shape = Mrecord, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\" BGCOLOR = \"lightgrey\">\n"
                                "        <TR> <TD COLSPAN = \"2\" BGCOLOR = \"#%06x\"> <b>ptr</b>   = %p  </TD> </TR>\n"
                                "        <TR> <TD COLSPAN = \"2\">          <b>ver</b>   = %lx </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"lightgreen\"> <b>type</b>  = %s </TD>\n"
                                "             <TD BGCOLOR = \"pink\">       <b>value</b> = %g </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"#%06x\">      <b>left</b>  = %p </TD>\n"
                                "             <TD BGCOLOR = \"#%06x\">      <b>right</b> = %p </TD> </TR>\n"
                                "        </TABLE>>, style = \"filled\", fillcolor = \"lightgrey\"];\n\n", (uintptr_t) Node, Generate_Color (Node, 0x000000), Node, Node->Verification, Name_Types_Arguments[Node->Type], Node->Value, Generate_Color (Node->Left, 0x000000), Node->Left, Generate_Color (Node->Right, 0x000000), Node->Right);
        }

        else if (Node->Type == Variable)
        {
                fprintf (file_graph, "    node_%lx [shape = Mrecord, label = <<TABLE BORDER = \"0\" CELLBORDER = \"1\" CELLSPACING = \"0\" CELLPADDING = \"4\" BGCOLOR = \"lightgrey\">\n"
                                "        <TR> <TD COLSPAN = \"2\" BGCOLOR = \"#%06x\"> <b>ptr</b>   = %p  </TD> </TR>\n"
                                "        <TR> <TD COLSPAN = \"2\">          <b>ver</b>   = %lx </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"lightgreen\"> <b>type</b>  = %s </TD>\n"
                                "             <TD BGCOLOR = \"pink\">       <b>value</b> = %s </TD> </TR>\n"
                                "        <TR> <TD BGCOLOR = \"#%06x\">      <b>left</b>  = %p </TD>\n"
                                "             <TD BGCOLOR = \"#%06x\">      <b>right</b> = %p </TD> </TR>\n"
                                "        </TABLE>>, style = \"filled\", fillcolor = \"lightgrey\"];\n\n", (uintptr_t) Node, Generate_Color (Node, 0x000000), Node, Node->Verification, Name_Types_Arguments[Node->Type], Array_Variable[int (Node->Value)].Name_Variable, Generate_Color (Node->Left, 0x000000), Node->Left, Generate_Color (Node->Right, 0x000000), Node->Right);
        }
    #endif // LIGHT_DUMP

    #ifdef LIGHT_DUMP
        if (Node->Type == Operation)
        {
            fprintf (file_graph, "    node_%lx [shape = ellipse, label = \"%s\", style = \"filled\", fillcolor = \"#FFD70080\"];\n\n", (uintptr_t) Node, Name_Operation[int(Node->Value)].Name_For_Dump);
        }

        else if (Node->Type == Number)
        {
            fprintf (file_graph, "    node_%lx [shape = Mrecord, label = \"%g\", style = \"filled\", fillcolor = \"lightblue\"];\n\n", (uintptr_t) Node, Node->Value);
        }

        else if (Node->Type == Variable)
        {
            fprintf (file_graph, "    node_%lx [shape = record, label = \"%s\", style = \"filled\", fillcolor = \"lightgreen\"];\n\n", (uintptr_t) Node, Array_Variable[int(Node->Value)].Name_Variable);
        }
    #endif // LIGHT_DUMP

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            #ifndef LIGHT_DUMP
                fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left, Generate_Color (Node->Left, 0x000000));
                Dump_Node (Node->Left, file_graph);
            #endif // LIGHT_DUMP

            #ifdef LIGHT_DUMP
                if (Node->Left->Type == Operation)
                {
                    fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#FFD70080\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
                }

                else if (Node->Left->Type == Number)
                {
                    fprintf (file_graph, "    node_%lx -> node_%lx [color = \"lightblue\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
                }

                else if (Node->Left->Type == Variable)
                {
                   fprintf (file_graph, "    node_%lx -> node_%lx [color = \"lightgreen\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
                }

                Dump_Node (Node->Left, file_graph);
            #endif // LIGHT_DUMP
        }

        else
        {
            fprintf (file_graph, "    node_%lx -> node_%lx [label = \"Невозможный указатель right\"];\n", (uintptr_t) Node, (uintptr_t) Node->Left);
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            #ifndef LIGHT_DUMP
                fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#%06x\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right, Generate_Color (Node->Right, 0x000000));
                Dump_Node (Node->Right, file_graph);
            #endif // LIGHT_DUMP

            #ifdef LIGHT_DUMP
                if (Node->Right->Type == Operation)
                {
                    fprintf (file_graph, "    node_%lx -> node_%lx [color = \"#FFD70080\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
                }

                else if (Node->Right->Type == Number)
                {
                    fprintf (file_graph, "    node_%lx -> node_%lx [color = \"lightblue\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
                }

                else if (Node->Right->Type == Variable)
                {
                   fprintf (file_graph, "    node_%lx -> node_%lx [color = \"lightgreen\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
                }

                Dump_Node (Node->Right, file_graph);
            #endif // LIGHT_DUMP
        }

        else
        {
            fprintf (file_graph, "    node_%lx -> node_%lx [label = \"Невозможный указатель right\"];\n", (uintptr_t) Node, (uintptr_t) Node->Right);
        }
    }

    return 0;
}

int Dump_For_Html      (const tree_k* const Tree, const char* const Name_File_Graph, const char* const Name_Function)
{
    FILE* file = fopen (Name_Log, "a");
    if (file == NULL)
    {
        printf ("%s:%d Error open logfile in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    int Number_Error = Tree_Error (Tree);
    if (Number_Error != 0)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        fprintf (file, "                          <b>%s</b>\n", Array_Name_Tree_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", Tree->Size);
    fprintf (file, "ROOT = %p\n", Tree->Root);
    fprintf (file, "\n");

    fprintf (file, "<img src = \"%s\">\n", Name_File_Graph);
    fprintf (file, "\n");

    Print_Separator_In_Log (200, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
    return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}


int Delete_Subtree  (node_k* Node, size_t* const Counter_Delete)
{
    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            Delete_Subtree (Node->Left, Counter_Delete);
            (*Counter_Delete)++;

            free (Node->Left);
            Node->Left = NULL;
        }

        else
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary))
        {
            Delete_Subtree (Node->Right, Counter_Delete);
            (*Counter_Delete)++;

            free (Node->Right);
            Node->Right = NULL;
        }

        else
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    return 0;
}

size_t Size_Subtree (const node_k* const Node)
{
    size_t Counter_Element = 1;

    if (Node->Left != NULL)
    {
        if (Node->Left->Verification == ((uintptr_t) (Node->Left) ^ Canary))
        {
            size_t A = Size_Subtree (Node->Left);
            if (A == There_Are_Errors)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }

            Counter_Element += A;
        }

        else
        {
            printf ("%s:%d: Error bad connection in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    if (Node->Right != NULL)
    {
        if (Node->Right->Verification == ((uintptr_t) (Node->Right) ^ Canary) )
        {
            size_t B = Size_Subtree (Node->Right);
            if (B == There_Are_Errors)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return There_Are_Errors;
            }

            Counter_Element += B;
        }

        else
        {
            printf ("%s:%d: Error bad connection in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    return Counter_Element;
}


node_k* Get_General            (char** Str, const char* const Start_Str)
{
    node_k* Root_Node = Get_For_Addition (Str, Start_Str);
    if (Root_Node == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    if (**Str != '\n')
    {
        printf ("%s:1:%ld: ERROR bad symbol\n", Equation, *Str - Start_Str);
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    (*Str)++;

    return Root_Node;
}

node_k* Get_For_Addition       (char** Str, const char* const Start_Str)
{
    node_k* Node = Get_For_Multiplication (Str, Start_Str);
    if (Node == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    while (**Str == '+' || **Str == '-')
    {
        node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
        if (New_Node == NULL)
        {
            printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
        New_Node->Left = Node;
        Node = New_Node;

        int Symbol = **Str;

        (*Str)++;

        New_Node->Right = Get_For_Multiplication (Str, Start_Str);
        if (New_Node->Right == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        if (Symbol == '+')
        {
            New_Node->Type = Operation;
            New_Node->Value = ADD;
        }

        else
        {
            New_Node->Type = Operation;
            New_Node->Value = SUB;
        }
    }

    return Node;
}

node_k* Get_For_Multiplication (char** Str, const char* const Start_Str)
{
    node_k* Node = Get_For_Power (Str, Start_Str);
    if (Node == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    while (**Str == '*' || **Str == '/')
    {
        node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
        if (New_Node == NULL)
        {
            printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
        New_Node->Left = Node;
        Node = New_Node;

        int Symbol = **Str;

        (*Str)++;

        New_Node->Right = Get_For_Power (Str, Start_Str);
        if (New_Node->Right == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        if (Symbol == '*')
        {
            New_Node->Type = Operation;
            New_Node->Value = MUL;
        }

        else
        {
            New_Node->Type = Operation;
            New_Node->Value = DIV;
        }
    }

    return Node;
}

node_k* Get_For_Power          (char** Str, const char* const Start_Str)
{
    node_k* Node = Get_Bracket (Str, Start_Str);
    if (Node == NULL)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    while (**Str == '^')
    {
        node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
        if (New_Node == NULL)
        {
            printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
        New_Node->Left = Node;
        Node = New_Node;

        int Symbol = **Str;

        (*Str)++;

        New_Node->Right = Get_Bracket (Str, Start_Str);
        if (New_Node->Right == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        New_Node->Type = Operation;
        New_Node->Value = POW;
    }

    return Node;
}

node_k* Get_Bracket            (char** Str, const char* const Start_Str)
{
    Skip_Spaces (Str);

    node_k* Node = NULL;

    if (**Str == '(')
    {
        (*Str)++;
        Skip_Spaces (Str);

        if (**Str == '-')
        {
            (*Str)++;

            Node = (node_k*) calloc (1, sizeof (node_k));
            if (Node == NULL)
            {
                printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
                printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return NULL;
            }
            Node->Verification = (uintptr_t) (Node) ^ Canary;
            Node->Type = Operation;
            Node->Value = MUL;

            node_k* Right_Node = (node_k*) calloc (1, sizeof (node_k));
            if (Right_Node == NULL)
            {
                printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
                printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return NULL;
            }
            Right_Node->Verification = (uintptr_t) (Right_Node) ^ Canary;
            Right_Node->Type = Number;
            Right_Node->Value = -1;
            Right_Node->Left = NULL;
            Right_Node->Right = NULL;

            Node->Right = Right_Node;

            Node->Left = Get_Bracket (Str, Start_Str);
            if (Node->Left == NULL)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return NULL;
            }
        }

        else
        {
            Node = Get_For_Addition (Str, Start_Str);
            if (Node == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
        }

        if (**Str == ')')
        {
            (*Str)++;
            Skip_Spaces (Str);
            return Node;
        }

        else
        {
            printf ("%s:1:%ld: ERROR not close bracket\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
    }

    else
    {
        node_k* Node = Get_Double (Str, Start_Str);

        if (Node == NULL)
        {
            Node = Get_Text (Str, Start_Str);

            if (Node == NULL)
            {
                printf ("%s:1:%ld: ERROR bad symbol\n", Equation, *Str - Start_Str);
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return NULL;
            }
        }

        return Node;
    }
}

node_k* Get_Text               (char** Str, const char* const Start_Str)
{
    Skip_Spaces (Str);

    int Symbol = 0;
    int Interation = 0;
    char String[100];

    while (('a' <= **Str && **Str <= 'z') || ('A' <= **Str && **Str <= 'Z') || **Str == '_')
    {
        Symbol = **Str;

        String[Interation] = Symbol;

        (*Str)++;
        Interation++;

        if (Interation == 99)
        {
            printf ("%s:1:%ld: ERROR very long word\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
    }

    if (Interation == 0)
    {
        return NULL;
    }

    else
    {
        String[Interation] = '\0';

        node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
        if (New_Node == NULL)
        {
            printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
        New_Node->Left = NULL;
        New_Node->Right = NULL;

        Skip_Spaces (Str);

        int A = Table_Name_Function (String);

        if (A == -1)
        {
            New_Node->Type = Variable;
            New_Node->Value = Table_Name_Variable (String);
            if (New_Node->Value == There_Are_Errors)
            {
                printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                return NULL;
            }
        }

        else
        {
            if (A == PI)
            {
                New_Node->Type = Number;

                New_Node->Value = M_PI;
            }

            else if (A == EXP)
            {
                New_Node->Type = Number;

                New_Node->Value = M_E;
            }

            else
            {
                New_Node->Type = Operation;

                New_Node->Left = Get_Function (Str, Start_Str);
                if (New_Node->Left == NULL)
                {
                    printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
                    return NULL;
                }

                New_Node->Value = A;
            }
        }

        return New_Node;
    }
}

node_k* Get_Function           (char** Str, const char* const Start_Str)
{
    Skip_Spaces (Str);

    if (**Str == '(')
    {
        (*Str)++;

        Skip_Spaces (Str);

        node_k* Node = Get_For_Addition (Str, Start_Str);
        if (Node == NULL)
        {
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        if (**Str == ')')
        {
            (*Str)++;
            Skip_Spaces (Str);

            return Node;
        }

        else
        {
            printf ("%s:1:%ld: ERROR not close bracket\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
    }

    else
    {
        node_k* Node = Get_Double (Str, Start_Str);
        if (Node == NULL)
        {
            printf ("%s:1:%ld: ERROR bad symbol\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        return Node;
    }
}

node_k* Get_Double             (char** Str, const char* const Start_Str)
{
    Skip_Spaces (Str);

    double Value = 0;
    double Value_2 = 0;

    int Integer_Part = Get_Number (Str, Start_Str);
    if (Integer_Part == There_Are_Errors)
    {
        return NULL;
    }

    Value = Integer_Part;

    if (**Str == '.')
    {
        (*Str)++;

        int Fractional_Part = Get_Number (Str, Start_Str);
        if (Fractional_Part == There_Are_Errors)
        {
            printf ("%s:1:%ld: ERROR bad fractional part\n", Equation, *Str - Start_Str);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }

        double A = pow (10, (Count_Digits (Fractional_Part)));

        Value_2 = double (Fractional_Part) / A;
    }

    Value += Value_2;

    node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
    if (New_Node == NULL)
    {
        printf ("%s:1:%ld: ERROR\n", Equation, *Str - Start_Str);
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
    New_Node->Type = Number;
    New_Node->Value = Value;
    New_Node->Left = NULL;
    New_Node->Right = NULL;

    Skip_Spaces (Str);

    return New_Node;
}

int Get_Number                 (char** Str, const char* const Start_Str)
{
    int Value = 0;
    int Execution_Flag = 0;

    while ('0' <= **Str && **Str <= '9')
    {
        Value = Value * 10 + (**Str - '0');
        (*Str)++;
        Execution_Flag = 1;
    }

    if (Execution_Flag == 1)
    {
        return Value;
    }

    else
    {
        return There_Are_Errors;
    }
}


int Table_Name_Function (const char* const Str)
{
    for (int i = 0; i < Quantity_Operations; i++)
    {
        if (strcmp (Str, Name_Operation[i].Name_For_User) == 0)
        {
            return Name_Operation[i].Number_Operation;
        }
    }

    return -1;
}

int Table_Name_Variable (const char* const Str)
{
    for (int i = 0; i < Size_Array_Variable; i++)
    {
        if (strcmp (Str, Array_Variable[i].Name_Variable) == 0)
        {
            return Array_Variable[i].Number_Variable;
        }
    }

    Size_Array_Variable++;
    Array_Variable = (variable_k*) realloc (Array_Variable, Size_Array_Variable * sizeof (variable_k));
    if (Array_Variable == NULL)
    {
        printf ("%s:%d: Error reallocation memory for array variable in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Array_Variable[Size_Array_Variable - 1].Name_Variable = strdup (Str);
    if (Array_Variable[Size_Array_Variable - 1].Name_Variable == NULL)
    {
        printf ("%s:%d: Error allocation memory for name variable in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    Array_Variable[Size_Array_Variable - 1].Number_Variable = Size_Array_Variable - 1;

    return Size_Array_Variable - 1;
}

int Skip_Spaces         (char** Str)
{
    while (**Str == ' ')
    {
        (*Str)++;
    }

    return 0;
}

int Read_Diff_Variable  (char** Str)
{
    Skip_Spaces (Str);

    int Symbol = 0;
    int Interation = 0;
    char String[100];

    while (('a' <= **Str && **Str <= 'z') || ('A' <= **Str && **Str <= 'Z') || **Str == '_')
    {
        Symbol = **Str;

        String[Interation] = Symbol;

        (*Str)++;
        Interation++;

        if (Interation == 99)
        {
            printf ("%s:2: ERROR very long word\n", Equation);
            printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return There_Are_Errors;
        }
    }

    if (Interation == 0)
    {
        return 0;
    }

    else
    {
        String[Interation] = '\0';

        Skip_Spaces (Str);

        return Table_Name_Variable (String);
    }
}


tree_k* Differentiation_Tree (const tree_k* const Tree, const int Number_Variable)
{
    #ifdef DEBUG
        char Name_Func[52];
        snprintf (Name_Func, sizeof (Name_Func), "Differentiation_Tree (Tree, %d)", Number_Variable);

        if (Tree_Error (Tree) != Not_Error_Tree)
        {
            if (Tree_Dump (Tree, Name_Func) == There_Are_Errors)
            {
                printf ("%s:%d: Error dump in %s\n", __FILE__, __LINE__, Name_Func);
                return NULL;
            }

            printf ("%s:%d: Error start in %s\n", __FILE__, __LINE__, __FUNCTION__);
            #ifdef STOP_PROGRAMME
                return NULL;
            #endif // STOP_PROGRAMME
        }
    #endif // DEBUG

    tree_k* New_Tree = (tree_k*) calloc (1, sizeof (tree_k));
    if (New_Tree == NULL)
    {
        printf ("%s:%d: Error allocation memory for tree in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    if (Number_Variable < 0 || Number_Variable >= Size_Array_Variable)
    {
        printf ("%s:%d: Error bad argument Number_Variable in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    New_Tree->Root = Differentiation_Node (Tree->Root, Number_Variable);

    New_Tree->Size = Size_Subtree (New_Tree->Root);
    if (New_Tree->Size == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    #ifdef DEBUG
        if (Tree_Dump (New_Tree, Name_Func) == There_Are_Errors)
        {
            printf ("%s:%d: Error dump in %s\n", __FILE__, __LINE__, __FUNCTION__);
            return NULL;
        }
    #endif // DEBUG

    return New_Tree;
}

node_k* Differentiation_Node (const node_k* const Node, const int Number_Variable)
{
    int When_Variable = 0;

    switch (Node->Type)
    {
        case Number:
            return ZERO_;

        case Variable:

            if (int (Node->Value) != Number_Variable)
            {
                return ZERO_;
            }

            return ONE_;

        case Operation:

            switch (int (Node->Value))
            {
                case ADD:
                    return ADD_ (d(L), d(R));

                case SUB:
                    return SUB_ (d(L), d(R));

                case MUL:
                    return ADD_ (MUL_ (d(L), c(R)), MUL_ (c(L), d(R)));

                case DIV:
                    return DIV_ (SUB_ (MUL_ (d(L), c(R)), MUL_ (c(L), d(R))), POW_ (c(R), TWO_));

                case POW:

                    When_Variable = Find_Variable (Node, Number_Variable);

                    switch (When_Variable)
                    {
                        case Only_Base:
                            return MUL_ (MUL_ (c(R), d(L)),  POW_ (c(L), SUB_ (c(R), ONE_)));

                        case Only_Degree:
                            return MUL_ (POW_ (c(L), c(R)), MUL_ (d(R), LN_ (c(L))));

                        case Base_And_Degree:
                            return MUL_ (POW_ (c(L), c(R)), ADD_ (MUL_ (d(R), LN_ (c(L))),  DIV_ (MUL_ (c(R), d(L)), c(L))));

                        default:
                            return ZERO_;
                    }

                case LN:
                    return DIV_ (d(L), c(L));

                case SIN:
                    return MUL_ (COS_ (c(L)), d(L));

                case COS:
                    return MUL_ (MINUS_ONE_, MUL_ (SIN_ (c(L)), d(L)));

                case TG:
                    return DIV_ (d(L), POW_ (COS_ (c(L)), TWO_));

                case CTG:
                    return DIV_ (MUL_ (MINUS_ONE_, d(L)), POW_ (SIN_ (c(L)), TWO_));

                case ARCSIN:
                    return DIV_ (d(L), POW_ (SUB_ (ONE_, POW_ (c(L), TWO_)), HALF_));

                case ARCCOS:
                    return MUL_ (MINUS_ONE_, DIV_ (d(L), POW_ (SUB_ (ONE_, POW_ (c(L), TWO_)), HALF_)));

                case ARCTG:
                    return DIV_ (d(L), ADD_ (ONE_, POW_ (c(L), TWO_)));

                case ARCCTG:
                    return MUL_(MINUS_ONE_, DIV_(d(L), ADD_(ONE_, POW_(c(L), TWO_))));

                case SH:
                    return MUL_ (CH_ (c(L)), d(L));

                case CH:
                    return MUL_ (SH_ (c(L)), d(L));;

                case TH:
                    return DIV_ (d(L), POW_ (CH_ (c(L)), TWO_));

                case CTH:
                    return MUL_ (MINUS_ONE_, DIV_ (d(L), POW_ (SH_ (c(L)), TWO_)));
            }
    }

    return NULL;
}

node_k* Copy_Tree            (const node_k* const Node)
{
    node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
    if (New_Node == NULL)
    {
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    New_Node->Type = Node->Type;
    New_Node->Value = Node->Value;
    New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;

    if (Node->Left != NULL)
    {
        New_Node->Left = Copy_Tree (Node->Left);
    }
    else
    {
        New_Node->Left = NULL;
    }


    if (Node->Right != NULL)
    {
        New_Node->Right = Copy_Tree (Node->Right);
    }
    else
    {
        New_Node->Right = NULL;
    }

    return New_Node;
}

node_k* Create_Node          (const int Type, const double Value, node_k* const Left, node_k* const Right)
{
    node_k* New_Node = (node_k*) calloc (1, sizeof (node_k));
    if (New_Node == NULL)
    {
        printf ("%s:%d: Error allocation memory for node in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    New_Node->Type = Type;
    New_Node->Value = Value;

    New_Node->Verification = (uintptr_t) (New_Node) ^ Canary;
    New_Node->Left = Left;
    New_Node->Right = Right;

    return New_Node;
}

int Find_Variable            (const node_k* const Node, const int Number_Variable)
{
    int Left_Status  = Mini_Find_Variable (Node->Left, Number_Variable);
    int Right_Status = Mini_Find_Variable (Node->Right, Number_Variable);

    if (Left_Status == 1 && Right_Status == 1)
    {
        return Base_And_Degree;
    }

    else if (Left_Status == 1 && Right_Status == 0)
    {
        return Only_Base;
    }

    else if (Left_Status == 0 && Right_Status == 1)
    {
        return Only_Degree;
    }

    else
    {
        return Not_Variable;
    }
}

int Mini_Find_Variable       (const node_k* const Node, const int Number_Variable)
{
    if (Node->Type == Variable && Node->Value == Number_Variable)
    {
        return 1;
    }

    if (Node->Left != NULL)
    {
        int A = Mini_Find_Variable (Node->Left, Number_Variable);

        if (A == 1)
        {
            return 1;
        }
    }

    if (Node->Right != NULL)
    {
        int B = Mini_Find_Variable (Node->Right, Number_Variable);

        if (B == 1)
        {
            return 1;
        }
    }

    return 0;
}


int Start_Logfile           ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("%s:%d: Error open logfile in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (200, file_html);

    fclose (file_html);

    return 0;
}

int Print_Separator_In_Log  (const size_t Len_Separator, FILE* file_html)
{
    for (size_t i = 0; i < Len_Separator / 2; i++)
    {
        fprintf (file_html, "+=");
    }

    fprintf (file_html, "\n\n");

    return 0;
}

int Reverse_Str             (char* const Str)
{
    size_t i = 0;
    size_t j = strlen (Str) - 1;

    while (i < j)
    {
        char Symbol = Str[i];
        Str[i] = Str[j];
        Str[j] = Symbol;
        i++;
        j--;
    }

    return 0;
}

char* itoa_k                (int Number, char* const Str)
{
    size_t i = 0;

    if (Number == 0)
    {
        Str[i] = '0';
        i++;

        Str[i] = '\0';

        return Str;
    }

    while (Number != 0)
    {
        int Rem = Number % 10;

        Str[i] = char (Rem) + '0';
        i++;

        Number = Number / 10;
    }

    Str[i] = '\0';

    Reverse_Str (Str);

    return Str;
}

unsigned int Generate_Color (const void *Ptr, unsigned int Color_Text)
{
    if (Ptr == NULL)
    {
        return 0xD3D3D3;
    }

    unsigned int Color = Hash_Pointer (Ptr) & 0xFFFFFF;

    if (Calculate_Contrast (Color, Color_Text) < Minimum_Contrast_Ratio)
    {
        unsigned int Reverse_Color = (~Color) & 0xFFFFFF;
        return Reverse_Color;
    }

    return Color;
}

unsigned int Hash_Pointer   (const void* const Ptr)
{
    uintptr_t Ptr_Value = (uintptr_t) Ptr;

    unsigned int Hash = 5381;

    unsigned char *Byte_Ptr = (unsigned char *) &Ptr_Value;
    size_t Pointer_Size = sizeof(Ptr_Value);

    for (size_t i = 0; i < Pointer_Size; i++)
    {
        Hash = ((Hash << 7) + Hash) + Byte_Ptr[i];
    }

    return Hash;
}

double Calculate_Contrast   (unsigned int Color, unsigned int Color_Text)
{
    double r_1 = ((Color >> 16) & 0xFF) / 255.0;
    double g_1 = ((Color >> 8) & 0xFF) / 255.0;
    double b_1 = (Color & 0xFF) / 255.0;
    double luminance_1 = 0.2126 * r_1 + 0.7152 * g_1 + 0.0722 * b_1;

    double r_2 = ((Color_Text >> 16) & 0xFF) / 255.0;
    double g_2 = ((Color_Text >> 8) & 0xFF) / 255.0;
    double b_2 = (Color_Text & 0xFF) / 255.0;
    double luminance_2 = 0.2126 * r_2 + 0.7152 * g_2 + 0.0722 * b_2;

    return (fmax(luminance_1, luminance_2) + 0.05) / (fmin(luminance_1, luminance_2) + 0.05);
}

int Copy_File_In_Buffer     (char** const Buffer)
{
    struct stat Data_File = {};

    if (stat (Equation, &Data_File) != 0)
    {
        printf ("%s:%d: Error stat in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    off_t Size_File  = Data_File.st_size;

    int File_Equation = open (Equation, O_RDONLY);
    if (File_Equation == -1)
    {
        printf ("%s:%d: Error open file with base data in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    *Buffer = (char*) calloc (size_t (Size_File + 1), sizeof (char));
    if (*Buffer == NULL)
    {
        printf ("%s:%d: Error allocation memory for buffer in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    if (read (File_Equation, *Buffer, size_t (Size_File)) < Size_File)
    {
        printf ("%s:%d: Error read file base data in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    close (File_Equation);

    return 0;
}

int Clean_Stdin             ()
{
    printf ("\033[H\033[J");
    fflush(stdout);

    return 0;
}

char* Read_Answer           ()
{
    char* Answer_User = NULL;

    if (getline_k (&Answer_User) == There_Are_Errors)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return NULL;
    }

    return Answer_User;
}

int getline_k               (char** const Str)
{
    size_t Len = 0;

    if (getline (Str, &Len, stdin) == -1)
    {
        printf ("%s:%d: Error getline in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    if (Delete_Slash_N (*Str) == There_Are_Errors)
    {
        printf ("%s:%d: Error in %s\n", __FILE__, __LINE__, __FUNCTION__);
        return There_Are_Errors;
    }

    return 0;
}

int Delete_Slash_N          (char* const Str)
{
    size_t Len = strlen (Str);

    if (Len > 0 && Str[Len - 1] == '\n')
    {
        Str[Len - 1] = '\0';
        return 0;
    }

    else
    {
        printf ("%s:%d: Error delete in string\n", __FILE__, __LINE__);
        return There_Are_Errors;
    }
}

int Count_Digits            (int Number)
{
    int Count = 0;

    if (Number == 0)
    {
        return 1;
    }

    if (Number < 0)
    {
        Number = Number * (-1);
    }

    while (Number != 0)
    {
        Number /= 10;
        Count++;
    }

    return Count;
}
