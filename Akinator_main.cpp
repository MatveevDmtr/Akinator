
#define TX_USE_SPEAK
#include <TXLib.h>

#include <stdio.h>
#include <math.h>
#define LOGGING
#include "akinator.h"


//#include "Tree.h"


//#include "D:\\Programming\\C\\Ded_course_1_sem\\Stack\\stack.h"

#include "D:\\Programming\\C\\Ded_course_1_sem\\Processor_v4\\GetPoison.h"

#include "D:\\Programming\\C\\Ded_course_1_sem\\Processor_v4\\logging.h"

enum ERRCODES
{
     OK                 , // 0
     SEGFAULT           , // 1
     ZOMBIE             , // 2
     NULLPTR            , // 3
     SIZEPOISONED       , // 4
     NEGCAP             , // 5
     DEBUGINFOERROR     , // 6
     REALLOCERROR       , // 7
     STACKOVERFLOW      , // 8
     FREE_ERROR         , // 9
     CHAINERROR         , // 10
     CONSTR_ERROR         // 11
};

enum NUM_SONS
{
    LEFT  = 1,
    RIGHT = 2
};

enum BIN_ANSWERS
{
    YES = 1,
    NO  = 0
};

//enum REALLOC_MODES



int main()
{
    SpeakAndPrint("start\n");

    tree_t tree = StructTreeInit(tree);

    TreeCtor(&tree);

    log("Tree created\n");

    def_node* temp_ptr_wayd_d  = allocate_array(def_node, MAX_TREE_HIGHT);
    def_node* temp_ptr_wayd_c1 = allocate_array(def_node, MAX_TREE_HIGHT);
    def_node* temp_ptr_wayd_c2 = allocate_array(def_node, MAX_TREE_HIGHT); //make struct for 3

    Assert(temp_ptr_wayd_d == NULL);
    Assert(temp_ptr_wayd_c1 == NULL);
    Assert(temp_ptr_wayd_c2 == NULL);

    static queue_t way_down    = {.Ptr = temp_ptr_wayd_d,  .Head = 0, .Tail = 0};
    static queue_t way_down_c1 = {.Ptr = temp_ptr_wayd_c1, .Head = 0, .Tail = 0};
    static queue_t way_down_c2 = {.Ptr = temp_ptr_wayd_c2, .Head = 0, .Tail = 0};

    ScanDataBase(&tree);

    Assert(way_down.Ptr == NULL);

    MainMenu(&tree, &way_down, &way_down_c1, &way_down_c2);
    //free way down, akinator_Dtor

    HTMLDump(&tree, "EXAMPLE");
}

tree_t StructureTreeInit(const char* name,
                         const char* func,
                         const char* file,
                         int line)
{
    void*   Ptr  = NULL;
    size_t  Size = 0;

    tree_t temp_tree =
    {
        .Ptr               = (elem_s*) getPoison(Ptr),
        .Size              = getPoison(Size),
        .DeadInside        = 1
    };

    return temp_tree;
}

int TreeCtor(tree_t* tree)
{
    static size_t num_dump = 0;

    if (tree == NULL || tree == getPoison(tree))
    {
        LogError(SEGFAULT);

        return SEGFAULT;
    }

    if (!(tree->DeadInside))
    {
        LogError(CONSTR_ERROR);

        return CONSTR_ERROR;
    }

    void* temp_ptr = (elem_s*) calloc(1, sizeof(elem_s));

    if (temp_ptr == NULL)
    {
        LogError(REALLOCERROR);

        return REALLOCERROR;
    }

    tree->Ptr                = (elem_s*)(temp_ptr);

    strcpy(tree->Ptr->elem, POISONED_ELEM);
    tree->DeadInside         = 0;
    tree->Size               = 0;

    return 0;
}

int WriteNode(elem_s* node, FILE* db, size_t num_spaces)
{
    Assert(node == nullptr);
    Assert(db   == nullptr);

    log("start writing node: %s\n", node->elem);

    WriteSpaces(num_spaces, db);

    if (node->son1 && node->son2)
    {
        fprintf(db, "%c%s\n", QUEST_PREFIX, node->elem);

        num_spaces += TAB;

        log("try to write sons of %s\n", node->elem);

        WriteNode(node->son1, db, num_spaces);
        WriteNode(node->son2, db, num_spaces);
    }
    else
    {
        log("try to write leaf %s\n", node->elem);

        fprintf(db, "%c%s\n", LEAF_PREFIX, node->elem);
    }

    return 0;
}

int WriteDataBase(elem_s* root)
{
    log("Start writing db\n");

    Assert(root == nullptr);

    FILE* db = fopen("dataBase.txt", "w");

    if (db == NULL)
    {
        SpeakAndPrint("Can't open dataBase file\n");

        return -1;
    }

    WriteNode(root, db, 0);

    fclose(db);

    log("Finish writing db\n");

    return 0;
}

void WriteSpaces(int num_spaces, FILE* db)
{
    Assert(db == nullptr);

    while (num_spaces-- > 0)
    {
        fprintf(db, " ");
    }
}

int ScanDataBase(tree_t* tree)
{
    log("Start scanning db\n");

    Assert(tree->Ptr == nullptr);

    FILE* db = fopen("dataBase.txt", "r");

    if (db == NULL)
    {
        SpeakAndPrint("Can't open dataBase file\n");

        return -1;
    }

    ScanNode(tree, tree->Ptr, NULL, db, LEFT);

    fclose(db);

    log("Finish scanning db\n");

    return 0;
}

int ScanNode(tree_t* tree, elem_s* node, elem_s* parent, FILE* db, size_t num_son)
{
    Assert(db   == nullptr);

    char line[MAX_LEN_ELEM] = "";

    char sym = ReadPrefix(db);

    fscanf(db, "%s", line);

    strcpy(node->elem, line);

    node->parent = parent;

    tree->Size++;

    if (sym == QUEST_PREFIX)
    {
        node->son1 = allocate_array(elem_s, 1);
        node->son2 = allocate_array(elem_s, 1);

        ScanNode(tree, node->son1, node, db, LEFT);
        ScanNode(tree, node->son2, node, db, RIGHT);
    }

    return 0;
}

char ReadPrefix(FILE* db)
{
    char sym = 0;

    while (sym == 0 || sym == ' ' || sym == '\n' || sym == 9)
    {
        sym = fgetc(db);
    }
    log("prefix: %d\n", sym);

    return sym;
}

int Comparison(tree_t* tree, const char* name1, const char* name2,
               queue_t* way_down1, queue_t* way_down2)
{
    if (FindNode(tree, tree->Ptr, name1, way_down1, LEFT) ||
        FindNode(tree, tree->Ptr, name2, way_down2, LEFT))  //const ==NOT_FOUND
    {
        SpeakAndPrint("I have no information about this objects\n");

        return -1;
    };

    Assert(way_down1->Tail == 0);
    Assert(way_down2->Tail == 0);

    bool sim_found = false;

    size_t i_dif = 0;

    SpeakAndPrint("Similarities: ");

    for (size_t i = 0; i < way_down1->Tail; i++)
    {
        if (way_down1->Ptr[i    ].Node   == way_down2->Ptr[i].Node &&
            way_down1->Ptr[i + 1].Branch == way_down2->Ptr[i + 1].Branch)
        {
            PrintQuality(way_down1, i);

            if (!sim_found) sim_found = true;
        }
        else
        {
            i_dif = i;
            break;
        }
    }
    if     (!sim_found)     SpeakAndPrint("no similarities found.\n");
    else                    SpeakAndPrint("that\'s all.\n");

    SpeakAndPrint("However: \n");

    PrintDif(name1, way_down1, i_dif);
    PrintDif(name2, way_down2, i_dif);

    return 0;
}

int PrintDif(const char* name, queue_t* way_down, size_t i_dif)
{
    SpeakAndPrint("%s: ", name);

    for (size_t i = i_dif; i < way_down->Tail - 1; i++)
    {
        PrintQuality(way_down, i);
    }
    SpeakAndPrint("that\'s all.\n");

    return 0;
}

int PrintQuality(queue_t* way_down, size_t i) //flag for ','
{
    if (way_down->Ptr[i + 1].Branch == LEFT)
    {
        SpeakAndPrint("%s, "    , way_down->Ptr[i].Node->elem);
    }
    else
    {
        SpeakAndPrint("not %s, ", way_down->Ptr[i].Node->elem);
    }

    return 0;
}

int MainMenu(tree_t* tree, queue_t* way_down_d, queue_t* way_down_c1, queue_t* way_down_c2)
{
    char line[MAX_LEN_LINE] = "";

    char sym = 0;

    while (true)
    {
        SpeakAndPrint("Type \'e\' to exit, \'g\' to guess,"
                      "\'d\' to see the definition of an object,"
                      "\'c\' to compare objects\n"); //define not speak

        char sym = 0;

        while (sym != 'e' && sym != 'g' && sym != 'd' && sym != 'c')
        {
            sym = getchar();
        }

        switch(sym)
        {
            case 'g':
                Guess(tree, tree->Ptr);
                break;

            case 'd':
                GiveDefinition(tree, way_down_d);
                break;

            case 'c':
                PrintComparison(tree, way_down_c1, way_down_c2);
                break;

            case 'e':
                SpeakAndPrint("Goodbye!\n");

                WriteDataBase(tree->Ptr);

                return 0; // break from cycle

            default:    //show db
                SpeakAndPrint("WHATAFUCK, u must type one of 3 symbols\n");
        }
    }
}

int GiveDefinition(tree_t* tree, queue_t* way_down)
{
    char curr_elem[MAX_LEN_ELEM + 1] = "";

    SpeakAndPrint("What object do you want to know about?\n");

    scanf("%100s", curr_elem);

    PrintDefinition(tree, way_down, (const char*)curr_elem);

    return 0;
}

int PrintComparison(tree_t* tree, queue_t* way_down1, queue_t* way_down2)
{
    char object1[MAX_LEN_ELEM] = "";
    char object2[MAX_LEN_ELEM] = "";
    log("objects made\n");

    SpeakAndPrint("Insert the first  object for comparison:\n");
    scanf("%s", object1);
    SpeakAndPrint("Insert the second object for comparison:\n");
    scanf("%s", object2);

    Comparison(tree, object1, object2, way_down1, way_down2);

    return 0;
}

int YesOrNo()
{
    char sym = 0;

    while (sym != 'y' && sym != 'n')
    {
        sym = getchar();
    }

    if (sym == 'y') return YES;

    return NO;
}

int Guess(tree_t* tree, elem_s* node)
{
    bool is_leaf = false;

    if (node->son1 == NULL && node->son2 == NULL)
    {
        is_leaf = true;

        SpeakAndPrint("Let me guess... Is it %s?\n", node->elem);
    }
    else
    {
        SpeakAndPrint("%s?\n", node->elem);
    }
    PrintBinQuest();

    switch(YesOrNo())
    {
        case YES:
            if   (is_leaf) SpeakAndPrint("I was really sure!\n");
            else Guess(tree, node->son1);
            break;

        case NO:
            if (is_leaf)
            {
                ReadNewNode(tree, node);
            }
            else
            {
                Guess(tree, node->son2);
            }
            break;
        default:
            SpeakAndPrint("TYPE \'y\' OR \'n\' NEXT TIME!\n");
    }

    return 0;
}

void PrintBinQuest()
{
    SpeakAndPrint("Type \'y\' for yes and \'n\' for no.\n");
}

int ReadNewNode(tree_t* tree, elem_s* node)
{
    static char curr_elem[MAX_LEN_ELEM + 1] = "";
    static char  question[MAX_LEN_ELEM + 1] = "";

    SpeakAndPrint("And who did you mean?\n");
    scanf("%100s", curr_elem);

    SpeakAndPrint("What is the difference between %s and %s?\n", node->elem, curr_elem);
    scanf("%100s", question);

    SpeakAndPrint("Is %s %s or not %s?\n", curr_elem, question, question);
    PrintBinQuest();

    switch(YesOrNo())
    {
        case YES:
            InsertSon(tree, node, curr_elem, (const char*)question, LEFT);
            break;

        case NO:
            InsertSon(tree, node, curr_elem, (const char*)question, RIGHT);
            break;

        default:
            SpeakAndPrint("TYPE y OR n NEXT TIME!\n");
    }

    SpeakAndPrint("Now I've remembered!\n");

    return 0;
}

void SpeakAndPrint(const char* str...)
{
    Assert(str == nullptr);

    va_list args = {0};
    va_start(args, str);

    char message[MAX_LEN_ELEM] = "";

    vsprintf(message, str, args);

    printf("%s", message);

    char cmd[MAX_LEN_CONSOLE_CMD] = "";

    sprintf(cmd, ".\\eSpeak\\command_line\\espeak.exe \"%s\"", message);
    system(cmd);
}


int InsertSon(tree_t* tree, elem_s* parent, char* ins_elem, const char* quest, size_t num_son)
{
    //TreeVerify(tree);

    log("start insert\n");

    elem_s* temp_ptr1 = (elem_s*) calloc(1, sizeof(elem_s));
    elem_s* temp_ptr2 = (elem_s*) calloc(1, sizeof(elem_s));

    if (temp_ptr1 == NULL || temp_ptr2 == NULL)
    {
        LogError(REALLOCERROR);

        return REALLOCERROR;
    }

    parent->son1 = (elem_s*) temp_ptr1;
    parent->son2 = (elem_s*) temp_ptr2;

    strcpy(parent->son1->elem, parent->elem);
    strcpy(parent->son2->elem, ins_elem);
    strcpy(parent->elem, quest);

    parent->son1->parent = parent;
    parent->son2->parent = parent;

    if (num_son == 1)
    {
        elem_s* temp_son = parent->son1;
        parent->son1     = parent->son2;
        parent->son2     = temp_son;
    }

    tree->Size++;

    return 0;
}

int FindNode(tree_t* tree, elem_s* node, const char* name, queue_t* way_down, size_t branch)
{
    static bool if_found = false;

    if_found = false;

    Assert(way_down->Ptr == NULL);

    log("node: %s\n", node->elem);

    way_down->Ptr[way_down->Tail++] = {node, branch};

    log("added to wd: %p\n", way_down->Ptr[way_down->Tail - 1].Node);

    log("wd.tail=%d\n", way_down->Tail);

    for (size_t i = 0; i < way_down->Tail; i++)
    {
        log("question in FindNode: %s\n", way_down->Ptr[i].Node->elem);
        log("branch in FindNode: %d\n", way_down->Ptr[i].Branch);
    }

    if (if_found) return 0;

    if (!strcmp(name, node->elem))
    {
        log("node found\n");

        if_found = true;

        return 0;
    }

    if (node->son1)
    {
        FindNode(tree, node->son1, name, way_down, LEFT);
    }

    if (if_found) return 0;

    if (node->son2)
    {
        FindNode(tree, node->son2, name, way_down, RIGHT);
    }

    if (if_found)   return 0;

    way_down->Tail--;

    return -1;
}

int PrintDefinition(tree_t* tree, queue_t* way_down, const char* name)
{
    if (FindNode(tree, tree->Ptr, name, way_down, LEFT))
    {
        SpeakAndPrint("I have no information about this object\n");

        return -1;
    }

    Assert(way_down->Tail == 0);

    for (size_t i = 0; i < way_down->Tail - 1; i++)
    {
        if (i != way_down->Tail - 2)
        {
            log("str: %s, branch: %d\n", way_down->Ptr[i].Node->elem,
                                         way_down->Ptr[i + 1].Branch);

            PrintQuality(way_down, i);
        }
        else
        {
            if (way_down->Ptr[i + 1].Branch == LEFT) //to print_quality
                    SpeakAndPrint("%s"    , way_down->Ptr[i].Node->elem);
            else
                    SpeakAndPrint("not %s", way_down->Ptr[i].Node->elem);
        }
    }

    SpeakAndPrint(" - this is %s.\n", way_down->Ptr[way_down->Tail - 1].Node->elem);

    for (size_t i = 0; i < way_down->Tail; i++)
    {
        way_down->Ptr[i] = {NULL, 0};
    }
    way_down->Tail = 0;   //make func for free

    return 0;
}

/*
elem_t DeleteElem(tree_t* tree, size_t i_del)
{
    log("start delete\n");

    TreeVerify(tree);

    elem_t del_elem = tree->Ptr[i_del].elem;
    size_t del_next = tree->Ptr[i_del].next;
    size_t del_prev = tree->Ptr[i_del].prev;
    size_t old_free = tree->Free;

    tree->Free = i_del;

    tree->Ptr[i_del].elem = POISONED_ELEM;
    tree->Ptr[i_del].next = old_free;
    tree->Ptr[i_del].prev = PREV_FOR_FREE;

    tree->Ptr[del_prev].next = del_next;
    tree->Ptr[del_next].prev = del_prev;

    tree->Size--;

    TreeVerify(tree);

    return del_elem;
}
*/

/*
int TreeVerify(tree_t* tree)
{
    if (tree == NULL)
    {
        LogError(SEGFAULT);
        return SEGFAULT;
    }
    if (tree->DeadInside == 1)
    {
        LogError(ZOMBIE);
        return ZOMBIE;
    }
    if (tree->Ptr == NULL)
    {
        LogError(NULLPTR);
        return NULLPTR;
    }
    if (tree->Size < 0)                  LogError(SIZEPOISONED);
    if (tree->Capacity < MIN_LEN_Tree)   LogError(NEGCAP);
    if (tree->Capacity < tree->Size)      LogError(STACKOVERFLOW);
    if (tree->Free > tree->Capacity - 1)  LogError(FREE_ERROR);

    size_t i_next = 0;

    for (size_t i = 0; i < tree->Capacity - 1; i++)
    {
        i_next = tree->Ptr[i].next;

        if (tree->Ptr[i].prev != PREV_FOR_FREE)
        {
            if (tree->Ptr[i_next].prev != i)
            {
                LogError(CHAINERROR);

                log("Elements with phys addresses %d and %d have invalid connection\n", i, i_next);
            }
        }
        else
        {
            if (tree->Ptr[i_next].prev != PREV_FOR_FREE)
            {
                LogError(CHAINERROR);

                log("Free element with phys address %d points not at a free element %d\n", i, i_next);
            }
        }
    }

    return 0;
}

*/

int LogCritError(int errcode, const char* func, int line)
{
    switch (errcode)
    {
        case OK:
            //print_log(FRAMED, "Everything OK: Stack is all right and can be used");
            break;

        case SEGFAULT:
            print_crit_errors("SEGMENTATION FAULT: Invalid Pointer to Structure of Stack", func, line);
            break;

        case ZOMBIE:
            print_crit_errors("DEADINSIDE ERROR: Stack doesn't exist", func, line);
            break;

        case NULLPTR:
            print_crit_errors("POINTER ERROR: Stack Pointer (pointer to buffer) is NULL", func, line);
            break;

        case SIZEPOISONED:
            print_crit_errors("SIZE ERROR: Stack Size is poisoned", func, line);
            break;

        case NEGCAP:
            print_crit_errors("CAPACITY ERROR: Stack Capacity has a Negative Value", func, line);
            break;

        case STACKOVERFLOW:
            print_crit_errors("STACK OVERFLOW ERROR: Size of Stack is bigger than its Capacity", func, line);
            break;

        case FREE_ERROR:
            print_crit_errors("FREE ERROR: Index of free unit is invalid", func, line);
            break;

        case CHAINERROR:
            print_crit_errors("CHAIN ERROR: Invalid Next-Prev Connection between elements", func, line);
            break;

        case CONSTR_ERROR:
            print_crit_errors("CONSTRUCTION ERROR: Trying to construct an existing Tree", func, line);
            break;

        default:
            print_crit_errors("DECODE ERROR: Unexpected Error Code", func, line);
            return 1;
    }

    return 0;
}


int HTMLDump(const tree_t* tree, const char* occasion)
{
    Assert(tree == NULL);

    log("tree pointer in HTMLDump: %p\n", tree);

    static size_t num_dump;  //init in TreeCtor

    char curr_picname[MAX_LEN_PICNAME] = "";

    char str_num_dump[MAX_LEN_NUM_DUMP] = "";

    itoa(num_dump, str_num_dump, 10);

    strcpy(curr_picname, PICNAME);
    strcat(curr_picname, str_num_dump);
    strcat(curr_picname, FILE_EXTENSION);

    log("picture name: %s\n", curr_picname);
    //picname made

    GraphTreeDump(tree, curr_picname);

    char html_piccmd[MAX_LEN_CONSOLE_CMD] = "<img src=\"";

    strcat(html_piccmd, curr_picname);
    strcat(html_piccmd, "\" alt=\"ERROR: Picture Not Found\">");

    char* r_mode = "a";
    if (num_dump == 0)          r_mode = "w";

    FILE* html_file = fopen(HTML_FILE_NAME, r_mode);

    if (html_file == NULL)
    {
        print_log(FRAMED, "FILE ERROR: can't open HTML file");
    }

    dumphtml("<pre>\n");
    dumphtml("<b>\n");
    dumphtml("<font size=6>");
    dumphtml("Tree dump (");
    dumphtml("%d", num_dump);
    dumphtml(")\n");
    dumphtml("<i>");
    dumphtml("<font size=4>");
    dumphtml("\nOcassion for DUMP: %s\n\n", occasion);
    dumphtml("%s\n\n", html_piccmd);
    dumphtml("<\pre>\n");

    num_dump++;

    fclose(html_file);
}

/*
int TreeRecalloc(Tree_t* tree, size_t mode)
{
    switch(mode)
    {
        case SORTING:
        {
            elem_s* temp_ptr = allocate_array(elem_s, tree->Capacity * 2);

            if (temp_ptr == NULL)
            {
                print_log(FRAMED, "CALLOC ERROR: Can't find memory for the Tree");

                return -1;
            }
            *temp_ptr = *(tree->Ptr);

            tree->Capacity *= 2;

            temp_ptr[0] = tree->Ptr[0];

            temp_ptr[0].next = 1;
            temp_ptr[0].prev = tree->Size;

            size_t i_next = tree->Ptr[0].next;

            for (size_t i = 0; i < tree->Size; i++)
            {
                log("i_next = %d\n", i_next);

                temp_ptr[i + 1] = tree->Ptr[i_next];

                temp_ptr[i + 1].next = i + 2;

                temp_ptr[i + 1].prev = i;

                i_next = tree->Ptr[i_next].next;
            }

            tree->Ptr = temp_ptr;

            tree->Ptr[tree->Size].next = 0;
            tree->Ptr[tree->Size].prev = tree->Size - 1;

            HTMLDump(tree, "after copy");

            for (size_t i = tree->Size + 1; i < tree->Capacity; i++)
            {
                tree->Ptr[i].next = i + 1;

                tree->Ptr[i].prev = -1;

                tree->Ptr[i].elem = POISONED_ELEM;
            }

            tree->Free = tree->Size + 1;

            break;
        }
        case 2:
        {
            elem_s* temp_ptr = (elem_s*)realloc(tree->Ptr, tree->Capacity * 2);
            if (temp_ptr == NULL)
            {
                print_log(FRAMED, "RECALLOC ERROR: Can't find memory for the Tree");

                return -1;
            }

            tree->Ptr = temp_ptr;
            break;
        }
    }

    return 0;
}
*/

void DrawNode(elem_s* node, FILE* dump_file, const char* branch_label)
{
    if (node->son1 != NULL || node->son1 != NULL)
    {
        dumpline("struct%p [\nlabel = \"{<data>elem: %s%c|<parent>parent: %p|<son1>son1: %p|<son2>son2: %p}\", style = \"filled\", color = \"black\", fillcolor = \"aquamarine\" \n];\n", node, node->elem, '?', node->parent, node->son1, node->son2);
    }
    else
    {
        dumpline("struct%p [\nlabel = \"{<data>elem: %s|<parent>parent: %p|<son1>son1: %p|<son2>son2: %p}\", style = \"filled\", color = \"black\", fillcolor = \"aquamarine\" \n];\n", node, node->elem, node->parent, node->son1, node->son2);
    }
    if (node->parent != NULL)
    {
        dumpline("struct%p -> struct%p [xlabel = \"%s\" weight=900 constraint=true color=red];\n", node->parent, node, branch_label);
    }
    if (node->son1 != NULL)
    {
        DrawNode(node->son1, dump_file, "yes");
    }
    if (node->son2 != NULL)
    {
        DrawNode(node->son2, dump_file, "no");
    }
}

void GraphTreeDump(const tree_t* tree, const char* picname)
{
    log("tree pointer: %p\n", tree);

    FILE* dump_file = fopen(DUMP_FILE_NAME, "w");

    if (dump_file == NULL)
    {
        print_log(FRAMED, "FILE ERROR: Cant't open file\n");
    }
    dumpline("digraph {\n");
    dumpline("graph [dpi = 100]");
    dumpline("rankdir = TB;\n");
    //dumpline("splines = \"ortho\";\n");
    dumpline("node [ shape=record ];\n");

    dumpline("Tree [label = \"Tree|size: %d\", style = \"filled\", rankdir = TB, fillcolor = \"indigo\", fontcolor = \"yellow\"];\n", tree->Size);

    DrawNode(tree->Ptr, dump_file, "root");
    //dumpline("Tree -> struct0 [style=\"invis\" weight = 1000];\n");
    /*for (int i = 0; i < tree->Size; i++)
    {
        dumpline("index%d[label = \"index: %d\", style = \"filled\" fillcolor = \"coral\"];\n", i, i);

        if (i > 0)
            dumpline("index%d -> index%d [style = \"invis\" weight = 200];\n", i - 1, i);
    }

    dumpline("INDEX [style = \"filled\", fillcolor = \"lightslateblue\"]\n");
    dumpline("INDEX -> index0[style = \"invis\" weight = 900]\n");
    */

    /*
    for (int i = 0; i < tree->Capacity; i++)
    {
        int elem = tree->Ptr[i].elem;
        int next = tree->Ptr[i].next;
        int prev = tree->Ptr[i].prev;

        char* color = NULL;

        if (i == 0)
        {
            dumpline("struct%d [\nlabel = \"<data>elem: nil|<next>next: %d|<prev>prev: %d\", style = \"filled\", fillcolor = \"cyan\" \n];\n", i, next, prev);
            dumpline("struct%d -> struct%d [weight = 0] [ color=red  ]\n", i, tree->Ptr[i].next);
            continue;
        }
        if (prev == PREV_FOR_FREE)
            color = "lightgrey";
        else
            color = "aquamarine";


        dumpline("struct%d [\nlabel = \"<data>elem: %d|<next>next: %d|<prev>prev: %d\", style = \"filled\", color = \"black\", fillcolor = \"%s\" \n];\n", i, elem, next, prev, color);
        dumpline("struct%d -> struct%d [dir=none weight=900 style=\"invis\" constraint=true];\n", i - 1, i);

        if (i < tree->Capacity - 1)
        {
            dumpline("struct%d -> struct%d [weight = 0, constraint=false, color=red  ]\n", i, tree->Ptr[i].next);
        }
    }
    /*for (int i = 1; i < tree->Capacity; i++)
    {
        //dumpline("struct%d:<prev> -> struct%d:<next> [weight = 2] [color=green]\n", i, tree->Ptr[i].prev);
    }*/
/*
    dumpline("structFree [\nlabel = \"free: %d\", style = \"filled\", fillcolor = \"green\"]\n", tree->Free);
    dumpline("structFree -> struct%d [weight = 0] [color = darkgoldenrod2]\n", tree->Free);
    */
    dumpline("}");

    fclose(dump_file);

    static size_t num_dump;  //init in TreeCtor

    log("start making console cmd\n");

    char console_cmd[MAX_LEN_CONSOLE_CMD] = "";
    strcpy(console_cmd, "dot -T png -o ");
    strcat(console_cmd, picname);
    strcat(console_cmd, " ");
    strcat(console_cmd, DUMP_FILE_NAME);

    log("console cmd: %s\n", console_cmd);

    printf("%d\n", system("cd D:"));
    printf("%d\n", system("cd D:\\Programming\\C\\Ded_course_1_sem\\Akinator"));
    printf("%d\n", system(console_cmd));
}
