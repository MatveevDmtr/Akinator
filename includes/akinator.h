#include <stdio.h>
#include <stdlib.h>

#define USER_MODE      1
#define HASH_MODE      2
#define HARDDEBUG_MODE 3

#define Tree_MODE USER_MODE

#define StructTreeInit(name)                              \
        StructureTreeInit(#name,                          \
                           __PRETTY_FUNCTION__,           \
                           __FILE__,                      \
                           __LINE__);


#define allocate_array(type, num_elems)                             \
({                                                                  \
    type* location__ = (type*) calloc (num_elems, sizeof (type));   \
    log ("Type_calloc: %s\n", #type);                               \
    log ("Sizeof type: %d\n", sizeof (type*));                      \
    Assert(location__ == NULL);                                     \
    log("Calloc returned: %p\n", location__);                       \
    location__;                                                     \
})

// START DUMP  DSL

#define dumpline(...)  fprintf(dump_file, __VA_ARGS__);
#define dumphtml(...)  fprintf(html_file, __VA_ARGS__);
// FINISH DUMP DSL

#define MAX_LEN_ELEM 100// to insert in scanf




const char* DATABASE_PATH = "DataBases/dataBase.txt";
const char* DUMP_FILE_NAME = "GraphViz/dump.dot";
const char* PICNAME        = "GraphViz/graphCode";
const char* FILE_EXTENSION = ".png";
const char* HTML_FILE_NAME = "GraphViz/htmldump.html";


typedef unsigned long long bird_t;

typedef int elem_t;

typedef struct struct_elem
{
    char elem[MAX_LEN_ELEM] = "";

    struct struct_elem* son1;
    struct struct_elem* son2;
    struct struct_elem* parent;
} elem_s;


typedef struct struct_def_node
{
    elem_s* Node;
    size_t  Branch;
} def_node;

typedef struct struct_queue
{
    def_node* Ptr;
    size_t    Head;
    size_t    Tail;

} queue_t;

typedef struct tree_info
{
    elem_s*            Ptr;
    size_t             Size;

    bool               DeadInside;
} tree_t;

const size_t MAX_TREE_HIGHT = 50;

const size_t MAX_LEN_LINE = 40;

const char* POISONED_ELEM = "Xz";

const size_t MAX_LEN_PICNAME = 15;

const size_t MAX_LEN_CONSOLE_CMD = 200;

const size_t MAX_LEN_NUM_DUMP = 10;

const size_t TAB = 4;

const int PREV_FOR_FREE = -1;

const char QUEST_PREFIX = '?';
const char  LEAF_PREFIX = '#';

tree_t StructureTreeInit(const char* name,
                           const char* func,
                           const char* file,
                           int line);

int TreeCtor(tree_t* tree);

int MainMenu(tree_t* tree, queue_t* way_down_d, queue_t* way_down_c1, queue_t* way_down_c2);

int Guess(tree_t* tree, elem_s* node);

void SpeakAndPrint(const char* str...);

int GiveDefinition(tree_t* tree, queue_t* way_down);

int ReadNewNode(tree_t* tree, elem_s* node);

void WriteSpaces(int num_spaces, FILE* db);

int ScanNode(tree_t* tree, elem_s* node, elem_s* parent, FILE* db, size_t num_son);

int ScanDataBase(tree_t* tree);

char ReadPrefix(FILE* db);

int PrintQuality(queue_t* way_down, size_t i);

int WriteDataBase(elem_s* root);

int PrintDif(const char* name, queue_t* way_down, size_t i_dif);

int PrintComparison(tree_t* tree, queue_t* way_down1, queue_t* way_down2);

int FindNode(tree_t* tree, elem_s* node, const char* name, queue_t* way_down, size_t branch);

int PrintDefinition(tree_t* tree, queue_t* way_down, const char* name);

int InsertSon(tree_t* tree, elem_s* parent, char* ins_elem, const char* quest, size_t num_son);

void GraphTreeDump(const tree_t* tree, const char* picname);

int HTMLDump(const tree_t* tree, const char* occasion);

int LogCritError(int errcode, const char* func, int line);

int TreeRecalloc(tree_t* tree, size_t mode);

int YesOrNo();

void PrintBinQuest();
