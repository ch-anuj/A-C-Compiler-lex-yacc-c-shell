/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser_temp.y" /* yacc.c:337  */

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <unistd.h>
using namespace std;
extern int yylex();
extern int yylineno;

void yyerror (const char* s) {
  cout<<s<<"in line:"<<yylineno<<endl;
  exit(0);
}

// -----------------------------------
typedef struct A{
  char label[25];
  int id;
  struct A *next;
  struct B *right;
}A;
typedef struct B
{
  int id;
  struct B *next;
}B;
typedef struct C
{
	char varName[25];
	struct C *next;
}C;
typedef struct parameters
{
	int type;
	struct parameters *next;
}parameters;
typedef struct lst
{
	char var_name[25];
	char type[7];
	struct lst * next;
}lst;
typedef struct gst{
	char f_name[25];
	struct lst* f_tableHead;
	struct lst* f_tableTail;
	char ret_type[7];
	struct gst* next;
	int nParam;
	parameters* p_list;
}gst;
int searchResult,nodeId=1,temp1,temp2,temp3,temp4,temp5,temp6,temp7;
A* headA = NULL;
A* tailA = NULL;
C* headC = NULL;
C* tailC = NULL;
C* headC1 = NULL;
C* tailC1 = NULL;
gst *PtrGst=NULL;
lst *PtrLst=NULL;
gst* headGst=NULL;
gst* tailGst=NULL;
char functionType[7];
char currentFun[25];
char currType[10];
ofstream myfile;
ofstream tableFile;
void emit(char* s){
	cout<<"|";
	cout<<s;
	cout<<"|";


  myfile <<s<<" ";

}
int lookupGst(char* fooName,gst* headGst){
	//cout<<"entering lookupGst";
  gst* temp = headGst;
  while(temp!=NULL){
    if(strcmp(temp->f_name,fooName)==0){
    	PtrGst = temp;
    //	cout<<"exiting lookupGst";
    	return 1;
    }
    temp = temp->next;
  }
 // cout<<"exiting lookupGst";
  return 0;
}

void insert_gst(char* fooName,gst** tailGst,gst** headGst){
//	cout<<"entering 2 ";
	int k;
	k=lookupGst(fooName,*headGst);
	if(k==1){
		cout<<"error! (duplicate insertion tried)";
		return;
	}
  gst* temp = (gst*)malloc(sizeof(gst));
  temp->next = NULL;
  temp->f_tableHead=NULL;
  temp->f_tableTail=NULL;
  temp->p_list = NULL;
  strcpy(temp->f_name,fooName);
  if((*tailGst)!=NULL){
  	// gst table is not empty
    (*tailGst)->next = temp;
  }
  (*tailGst) = temp;
  if((*headGst)==NULL)(*headGst) = (*tailGst);
 // cout<<"exiting 2 ";
  return;
}
int lookupLst(char* varName,lst* headLst){
//	cout<<"entering 3 ";
  lst* temp = headLst;
  while(temp!=NULL){
    if(strcmp(temp->var_name,varName)==0){
    	PtrLst = temp;
    //	cout<<"exiting 3 ";
    	return 1;
    }
    temp = temp->next;
  }
  //cout<<"exiting 3 ";
  return 0;
}
void insert_lst(char* varName,lst** tailLst,lst** headLst){
//	cout<<"entering 4 ";
	int k;
	k=lookupLst(varName,*headLst);
	if(k==1){
		cout<<"error! (duplicate insertion tried)";
    exit(0);
		return;
	}
  lst* temp = (lst*)malloc(sizeof(lst));
  temp->next = NULL;
  strcpy(temp->var_name,varName);
  if((*tailLst)!=NULL){
  	// lst table is not empty
    (*tailLst)->next = temp;
  }
  (*tailLst) = temp;
  if((*headLst)==NULL)(*headLst) = (*tailLst);
 // cout<<"exiting 4 ";
  return;
}
void printlistP(parameters* headP){
  if(headP==NULL)return;
  parameters* temp = headP;
  while(temp!=NULL){
    cout<<temp->type<<" ";
    temp = temp->next;
  }
}

void printGst(gst* headA){
	if(headA==NULL) return;
  cout<<"\n--printing gst----\n";
  gst* temp = headA;
  while(temp!=NULL){
    if(temp->p_list==NULL)
    cout<<temp->f_name<<" "<<temp->ret_type<<" "<<temp->nParam<<endl;
    else{
        cout<<temp->f_name<<" "<<temp->ret_type<<" "<<temp->nParam;
        cout<<"(";
        printlistP(temp->p_list);
        cout<<")";
        cout<<endl;
    }

    temp=temp->next;
  }
  cout<<"\n------\n";
}
void printLst(lst* headA){
	if(headA==NULL)return;
  cout<<"\n--printing lst----\n";
  lst* temp = headA;
  while(temp!=NULL){
    cout<<temp->type<<" "<<temp->var_name<<" "<<endl;
    temp=temp->next;
  }
  cout<<"\n------\n";
}
int setPtrGst(char* fooName){
//	cout<<"entering 5 ";
	lookupGst(fooName,headGst);
	//cout<<"exiting 5 ";
	if(PtrGst!=NULL)return 1;
	else return 0;
}
int setPtrLst(char* varName){
//	cout<<"entering 6 ";
	if(PtrGst==NULL){
		cout<<"error in setting ptrlst because ptrgst is null!";
		return 0;
	}
	lookupLst(varName,PtrGst->f_tableHead);
//	cout<<"exiting 6 ";
	if(PtrLst!=NULL)return 1;
	else return 0;
}
int setBothPtr(char* fooName,char* varName){
  int k;
  k=setPtrGst(fooName);
  if(k==0)return 0;
  else{
    k=setPtrLst(varName);
    if(k==0)return 0;
    else
      return 1;
  }
}
void printAllTables(gst* headGst){
//	cout<<"entering 7 ";
	gst* temp = headGst;
	printGst(headGst);
  	while(temp!=NULL){
    	printLst(temp->f_tableHead);
    temp=temp->next;
  }
//  cout<<"exiting 7 ";

}
void printGraph(A* headA){
  cout<<"strict graph G {\n";
  A* temp = headA;
  while(temp!=NULL){
    cout<<temp->id<<" [label=\""<<temp->label<<"\"];"<<endl;
    temp=temp->next;
  }
  temp = headA;
  B* helper;
  while(temp!=NULL){
    if(temp->right!=NULL){
      helper = temp->right;
      cout<<temp->id<<"--";
      while(helper!=NULL){
        if(helper->next!=NULL)
          cout<<helper->id<<", ";
        else
          cout<<helper->id<<" ";
        helper = helper->next;
      }
      cout<<";"<<endl;
    }
    temp = temp->next;
  }
  cout<<"\n}\n";
}
void addNodeC(char* label,C** tailC,C** headC){
//	cout<<"adding node "<<label<<" to C \n";

  C* temp = (C*)malloc(sizeof(C));
  temp->next = NULL;
  strcpy(temp->varName,label);
  if((*tailC)!=NULL){
    (*tailC)->next = temp;
    (*tailC) = (*tailC)->next;
  }
  else{
  	(*tailC) = temp;
  }


  if((*headC)==NULL)(*headC) = (*tailC);
//  cout<<"exiting 8 ";
  return ;
}
void flush(C** headC){
//	cout<<"entering 9 ";
	C* temp = *headC;
	C* temp1;
	if(*headC==NULL)return;
	char np[25];
	strcpy(np,(*headC)->varName);
	temp=temp->next;
	while(temp!=NULL){
		insert_lst(temp->varName,&(PtrGst->f_tableTail),&(PtrGst->f_tableHead));
		temp=temp->next;
	}
	 temp=(*headC)->next;
	  while(temp!=NULL){
	  	setPtrLst(temp->varName);
	  	strcpy(PtrLst->type,np);
      temp=temp->next;
	  }
	 while((*headC)!=NULL){
	 	temp1 = (*headC);
	 	(*headC)=(*headC)->next;
	 	free(temp1);
	 }
   *headC=NULL;
//	cout<<"exiting 9 ";
}
int findNode(int id,A* headA){
  A* temp = headA;
  while(temp!=NULL){
    if(temp->id==id)return 1;
    temp = temp->next;
  }
  return 0;
}
// makeChild to be called only if both parent and child are already created in the graph
void makeChild(int parent,int child,A* headA){
  A* p=headA;
  B* temp;
  if(p==NULL)return;
  int flagC = 0;
  while(p!=NULL){
    if(p->id==parent)break;
    p = p->next;
  }
  if(p==NULL)return;
  B* c = p->right;
  while(1){
    if(c!=NULL){
      if(c->next==NULL) break;
      else{
        c = c->next;
      }
    }
    else{
      flagC = 1;
      break;
    }
  }
  temp = (B*)malloc(sizeof(B));
  temp->next = NULL;
  temp->id = child;
  if(flagC==0){
    //there is at least one neighbour of the parent
    c->next = temp;
  }
  else{
    p->right = temp;
  }

}
void printlistC(C* headC){
	cout<<"entering printlistC\n";
	if(headC==NULL)return;
	C* temp = headC;
  while(temp!=NULL){
    cout<<temp->varName<<endl;
    temp = temp->next;
  }
  cout<<"\nexiting printlistC";
}

int length(C* head)
{
    int count = 0;
    C* current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}
parameters* allocate(int k) {
    if (k==0) return NULL;
    parameters* result = (parameters*)malloc(sizeof(parameters));
    result->type=7;
    result->next = allocate(k-1);
    return result;
}
void flush2(C** headC){
 // cout<<"\nentering flush2 ";
  C* temp = *headC;
  C* temp1;
  parameters* tempP;
  if(*headC==NULL)return;
  PtrGst->nParam=length(*headC);
  PtrGst->p_list=allocate(length(*headC));
  tempP=PtrGst->p_list;
  while(temp!=NULL){
    if(strcmp(temp->varName,strdup("int"))==0){
        tempP->type = 1;
    }
    if(strcmp(temp->varName,strdup("char"))==0){
        tempP->type = 2;
    }
    if(strcmp(temp->varName,strdup("void"))==0){
        tempP->type = 3;
    }
    temp=temp->next;
    tempP=tempP->next;
  }

   while((*headC)!=NULL){
    temp1 = (*headC);
    (*headC)=(*headC)->next;
    free(temp1);
   }
   *headC=NULL;
 // cout<<"\nexiting flush2 ";
}
// ----------------------------------
////////////////anuj functions///////////////////////

int tempCounter = 0;
int labelCounter = 0;
int funcCounter = 0;
char buffer[1000];

char* newTempGen(){
  char buffer[50];
  char* a = (char*)malloc(10*sizeof(char));
  sprintf(buffer, "t.%d", tempCounter);
  tempCounter= tempCounter + 1;
  strcpy(a,buffer);
  return a;
}

char* newLabelGen(){
  char buffer[50];
  char* a = (char*)malloc(10*sizeof(char));
  sprintf(buffer, "L~%d", labelCounter);
  labelCounter++;
  strcpy(a,buffer);
  return a;
}
int lookAllTable(char* name,gst* headGst){
  int a,b;
  gst* temp;
  a=lookupGst(name,headGst);
  if(a==1)return 1;
  else{
    temp = headGst;
    while(temp!=NULL){
      b=lookupLst(name,temp->f_tableHead);
      if(b==1)return 1;
      temp = temp->next;
    }
    return 0;
  }
}
void TableDump(gst* headGst){
  freopen("table.csv","w",stdout);
  gst* temp = headGst;
  printGst(headGst);
    while(temp!=NULL){
      printLst(temp->f_tableHead);
    temp=temp->next;
  }
//  cout<<"exiting 7 ";

}


#line 528 "y.tab.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    O_BRAC = 259,
    C_BRAC = 260,
    OPEN_BRAC = 261,
    CLOSE_BRAC = 262,
    MAIN = 263,
    THEN = 264,
    PERCENT = 265,
    ENDIF = 266,
    NUM = 267,
    CHARACTER = 268,
    SEMICOLON = 269,
    COLON = 270,
    DOT = 271,
    AMPERSAN = 272,
    PRECENT = 273,
    BACKSLASH = 274,
    ELSE = 275,
    SCAN = 276,
    PRINT = 277,
    INT = 278,
    CHAR = 279,
    FLOAT = 280,
    WHILE = 281,
    GOTO = 282,
    SWITCH = 283,
    VOID = 284,
    BREAK = 285,
    CASE = 286,
    CONTINUE = 287,
    RETURN = 288,
    COMMA = 289,
    LETTER = 290,
    NUMBER = 291,
    INTEGER = 292,
    FLOATING_INT = 293,
    IDENTIFIER = 294,
    Obrac = 295,
    Cbrac = 296,
    EQUALS = 297,
    NOT = 298,
    DO = 299,
    FOR = 300,
    EXIT = 301,
    OR = 302,
    EQEQ = 303,
    NEQ = 304,
    G_THAN = 305,
    L_THAN = 306,
    G_EQ = 307,
    L_EQ = 308,
    AND = 309,
    PLUS = 310,
    MINUS = 311,
    MULTIPLY = 312,
    DIVIDE = 313
  };
#endif
/* Tokens.  */
#define IF 258
#define O_BRAC 259
#define C_BRAC 260
#define OPEN_BRAC 261
#define CLOSE_BRAC 262
#define MAIN 263
#define THEN 264
#define PERCENT 265
#define ENDIF 266
#define NUM 267
#define CHARACTER 268
#define SEMICOLON 269
#define COLON 270
#define DOT 271
#define AMPERSAN 272
#define PRECENT 273
#define BACKSLASH 274
#define ELSE 275
#define SCAN 276
#define PRINT 277
#define INT 278
#define CHAR 279
#define FLOAT 280
#define WHILE 281
#define GOTO 282
#define SWITCH 283
#define VOID 284
#define BREAK 285
#define CASE 286
#define CONTINUE 287
#define RETURN 288
#define COMMA 289
#define LETTER 290
#define NUMBER 291
#define INTEGER 292
#define FLOATING_INT 293
#define IDENTIFIER 294
#define Obrac 295
#define Cbrac 296
#define EQUALS 297
#define NOT 298
#define DO 299
#define FOR 300
#define EXIT 301
#define OR 302
#define EQEQ 303
#define NEQ 304
#define G_THAN 305
#define L_THAN 306
#define G_EQ 307
#define L_EQ 308
#define AND 309
#define PLUS 310
#define MINUS 311
#define MULTIPLY 312
#define DIVIDE 313

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 460 "parser_temp.y" /* yacc.c:352  */

  struct{
  char val[25];
  int nodeId;
  char ircode[10000];
  }value;

#line 695 "y.tab.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   216

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  44
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  176

#define YYUNDEFTOK  2
#define YYMAXUTOK   313

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   490,   490,   499,   509,   514,   521,   559,   573,   600,
     614,   622,   636,   645,   654,   666,   674,   684,   688,   693,
     700,   713,   723,   737,   744,   749,   756,   763,   771,   779,
     787,   801,   809,   816,   826,   837,   842,   849,   854,   858,
     862,   867,   872,   877,   881,   888,   900,   913,   921,   937,
     942,   951,   968,   980,   984,   990,   996,  1001,  1010,  1014,
    1018,  1023,  1028,  1036,  1041,  1050,  1061,  1073,  1084,  1097,
    1107,  1112,  1120,  1126,  1135,  1144,  1153,  1162,  1168,  1177,
    1192,  1198,  1207,  1216,  1225,  1237,  1243,  1252,  1264,  1270,
    1281,  1286,  1297
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "O_BRAC", "C_BRAC", "OPEN_BRAC",
  "CLOSE_BRAC", "MAIN", "THEN", "PERCENT", "ENDIF", "NUM", "CHARACTER",
  "SEMICOLON", "COLON", "DOT", "AMPERSAN", "PRECENT", "BACKSLASH", "ELSE",
  "SCAN", "PRINT", "INT", "CHAR", "FLOAT", "WHILE", "GOTO", "SWITCH",
  "VOID", "BREAK", "CASE", "CONTINUE", "RETURN", "COMMA", "LETTER",
  "NUMBER", "INTEGER", "FLOATING_INT", "IDENTIFIER", "Obrac", "Cbrac",
  "EQUALS", "NOT", "DO", "FOR", "EXIT", "OR", "EQEQ", "NEQ", "G_THAN",
  "L_THAN", "G_EQ", "L_EQ", "AND", "PLUS", "MINUS", "MULTIPLY", "DIVIDE",
  "$accept", "main_unit", "func_decls", "main_decl", "mainkeyword",
  "func_decl", "MainParamList", "fun_name", "fun_type_spec", "type_spec",
  "empty", "param_list", "param_decl", "Pdecl_spec", "decl_spec", "body",
  "compound_stmt", "decl_list", "declare", "init_decl_list", "declarators",
  "declarator", "stmt_list", "stmt", "print_stmt", "scan_stmt", "if_stmt",
  "func_call_stmt", "with_return", "without_return", "call_param_list",
  "empty_param_list", "non_empty_param_list", "jump_stmt", "expr_stmt",
  "iterate_stmt", "primary_expr", "mul_expr", "add_expr",
  "relational_expr", "equality_expr", "log_and_expr", "log_or_expr",
  "assgn_expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313
};
# endif

#define YYPACT_NINF -95

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-95)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -11,   -95,   -95,   -95,    17,   -11,   -95,   -95,    22,   -95,
     -95,   -95,   -95,   -95,    21,    68,   -95,   124,    71,   -95,
     -95,    77,   -95,   -95,    50,   -95,    58,    93,    93,   124,
     -95,   -95,    13,   -95,   -95,   -95,   -95,   113,   -95,   -95,
     117,   132,   137,   129,   142,   120,     5,   160,   148,   143,
     -95,    58,   -95,    57,   -95,   105,   -95,   -95,   -95,   -95,
     -95,   144,   150,   -95,   -95,   -95,   153,    29,   123,   131,
      29,   -95,   -95,    29,   -95,   -95,   -95,   -95,    -3,   -95,
      49,    70,    45,   114,    30,    79,    36,   147,    56,   -95,
     173,   154,   -95,   -95,   -95,   139,   -95,   -95,   -95,   -95,
     -95,     3,   184,   186,     9,    18,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,    29,    29,   -95,    29,
     -95,    33,   -95,   -95,   -95,   190,   149,   191,   155,    29,
     -95,    58,   -95,    93,   187,   188,    93,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,    73,    73,    73,    70,    70,    45,
     114,   -95,    79,    79,    29,    29,    39,   -95,   178,   -95,
     -95,   -95,   -95,    64,    19,    23,    93,   -95,   189,   160,
     195,   -95,   -95,   -95,   160,   -95
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    12,    13,    14,     0,     0,     3,     4,     0,     1,
       2,     5,     7,    11,     0,     0,    17,    17,     0,    15,
      16,     0,    21,    10,     9,    18,     0,     0,     0,     0,
      34,    20,     0,     6,    23,     8,    19,     0,    24,    63,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      22,     0,    37,     0,    28,     0,    35,    43,    44,    42,
      40,     0,     0,    41,    38,    39,     0,     0,     0,     0,
       0,    59,    58,     0,    70,    71,    60,    69,    72,    77,
      80,    85,    88,    90,     0,    17,     0,     0,     0,    61,
       0,    31,    32,    26,    29,     0,    25,    36,    49,    50,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    62,     0,
      55,     0,    53,    54,    56,    69,    92,     0,     0,     0,
      30,     0,    27,     0,     0,     0,     0,    76,    75,    73,
      74,    81,    78,    79,    82,    84,    83,    86,    87,    89,
      91,    52,     0,    17,     0,     0,     0,    33,    47,    46,
      45,    65,    57,     0,     0,     0,     0,    51,     0,     0,
       0,    48,    66,    67,     0,    68
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -95,   -95,   202,   -95,   203,   -95,   -95,   -95,   -22,
     138,   -95,   180,   -95,   -95,   -26,   -27,   -95,   157,   -95,
     -95,   -46,   158,   -44,   -95,   -95,   -95,   -95,   -95,   -95,
      59,   -95,   -95,   -95,   125,   -95,   -79,    69,     2,   -94,
      97,    96,   -41,    51
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    14,     7,    21,    15,     8,    22,
     120,    24,    25,    26,    51,    33,    52,    53,    54,    90,
      91,    31,    55,    56,    57,    58,    59,    60,    61,    62,
     121,   122,   123,    63,    64,    65,    78,    79,    80,    81,
      82,    83,   126,    66
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      34,    34,    35,    87,    84,    92,   124,   106,   133,    85,
      50,    97,     1,     2,   136,   141,    37,     9,     3,    32,
      38,   147,   148,   137,   168,    16,   101,    39,   169,   104,
      12,    50,   105,    73,    40,    41,    19,    20,   151,    42,
      73,    74,    75,    43,   118,    44,    45,    86,    74,    75,
     119,    97,    46,   165,   107,   108,   119,    47,    48,    49,
      37,    13,   128,    32,    93,   119,   119,   152,    77,   167,
      39,    39,    17,   162,   124,   125,    27,   119,    40,    41,
      19,    20,    28,    42,    29,   157,   119,    43,   156,    44,
      45,    74,    75,   115,   116,   128,    46,    30,   152,    32,
     109,    47,    48,    49,   110,   111,    34,   158,    37,    34,
     161,    32,    96,   164,   144,   145,   146,    67,    77,    39,
     112,    68,   113,   114,    73,   173,    40,    41,   110,   111,
     175,    42,    74,    75,    76,    43,    69,    44,    45,    34,
     171,    70,    37,    71,    46,    32,   132,    19,    20,    47,
      48,    49,    88,    39,    18,    23,    72,    89,    98,    77,
      40,    41,   102,    37,    99,    42,    32,   100,   117,    43,
     103,    44,    45,   127,    39,   138,   139,   140,    46,   142,
     143,    40,    41,    47,    48,    49,    42,   130,   131,   134,
      43,   135,    44,    45,   153,   154,   119,   155,   166,    46,
     174,   159,   160,   172,    47,    48,    49,    10,    11,    36,
      94,    95,   163,   129,   149,   150,   170
};

static const yytype_uint8 yycheck[] =
{
      27,    28,    28,    47,    45,    51,    85,    10,     5,     4,
      32,    55,    23,    24,     5,   109,     3,     0,    29,     6,
       7,   115,   116,     5,     5,     4,    67,    14,     5,    70,
       8,    53,    73,     4,    21,    22,    23,    24,     5,    26,
       4,    12,    13,    30,    14,    32,    33,    42,    12,    13,
      47,    95,    39,    14,    57,    58,    47,    44,    45,    46,
       3,    39,    39,     6,     7,    47,    47,    34,    39,     5,
      14,    14,     4,   152,   153,    39,     5,    47,    21,    22,
      23,    24,     5,    26,    34,   131,    47,    30,   129,    32,
      33,    12,    13,    48,    49,    39,    39,    39,    34,     6,
      51,    44,    45,    46,    55,    56,   133,   133,     3,   136,
     136,     6,     7,   154,   112,   113,   114,     4,    39,    14,
      50,     4,    52,    53,     4,   169,    21,    22,    55,    56,
     174,    26,    12,    13,    14,    30,     4,    32,    33,   166,
     166,     4,     3,    14,    39,     6,     7,    23,    24,    44,
      45,    46,     4,    14,    16,    17,    14,    14,    14,    39,
      21,    22,    39,     3,    14,    26,     6,    14,    54,    30,
      39,    32,    33,    26,    14,   106,   107,   108,    39,   110,
     111,    21,    22,    44,    45,    46,    26,    14,    34,     5,
      30,     5,    32,    33,     4,     4,    47,    42,    20,    39,
       5,    14,    14,    14,    44,    45,    46,     5,     5,    29,
      53,    53,   153,    88,   117,   119,   165
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    23,    24,    29,    60,    61,    62,    64,    67,     0,
      62,    64,     8,    39,    63,    66,     4,     4,    69,    23,
      24,    65,    68,    69,    70,    71,    72,     5,     5,    34,
      39,    80,     6,    74,    75,    74,    71,     3,     7,    14,
      21,    22,    26,    30,    32,    33,    39,    44,    45,    46,
      68,    73,    75,    76,    77,    81,    82,    83,    84,    85,
      86,    87,    88,    92,    93,    94,   102,     4,     4,     4,
       4,    14,    14,     4,    12,    13,    14,    39,    95,    96,
      97,    98,    99,   100,   101,     4,    42,    82,     4,    14,
      78,    79,    80,     7,    77,    81,     7,    82,    14,    14,
      14,   101,    39,    39,   101,   101,    10,    57,    58,    51,
      55,    56,    50,    52,    53,    48,    49,    54,    14,    47,
      69,    89,    90,    91,    95,    39,   101,    26,    39,    93,
      14,    34,     7,     5,     5,     5,     5,     5,    96,    96,
      96,    98,    96,    96,    97,    97,    97,    98,    98,    99,
     100,     5,    34,     4,     4,    42,   101,    80,    74,    14,
      14,    74,    95,    89,   101,    14,    20,     5,     5,     5,
     102,    74,    14,    82,     5,    82
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    59,    60,    60,    61,    61,    62,    63,    64,    65,
      65,    66,    67,    67,    67,    68,    68,    69,    70,    70,
      71,    72,    73,    74,    75,    75,    75,    75,    76,    76,
      77,    78,    79,    79,    80,    81,    81,    82,    82,    82,
      82,    82,    82,    82,    82,    83,    84,    85,    85,    86,
      86,    87,    88,    89,    89,    90,    91,    91,    92,    92,
      92,    92,    92,    93,    93,    94,    94,    94,    94,    95,
      95,    95,    96,    96,    96,    96,    96,    97,    97,    97,
      98,    98,    98,    98,    98,    99,    99,    99,   100,   100,
     101,   101,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     2,     6,     1,     6,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     3,
       2,     1,     1,     1,     2,     3,     3,     4,     1,     2,
       3,     1,     1,     3,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     5,     5,     7,     2,
       2,     6,     4,     1,     1,     1,     1,     3,     2,     2,
       2,     2,     3,     1,     2,     5,     7,     7,     8,     1,
       1,     1,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 490 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"1 ";
                  cout<<"succesfully parsed(1)";
                  printAllTables(headGst);
                  strcpy((yyval.value).ircode,(yyvsp[-1].value).ircode);
                  sprintf((yyval.value).ircode, "%s\n%s", (yyvsp[-1].value).ircode, (yyvsp[0].value).ircode);
                  emit((yyval.value).ircode);

      }
#line 1938 "y.tab.c" /* yacc.c:1652  */
    break;

  case 3:
#line 499 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"71 ";
                  cout<<"succesfully parsed(2)";
                  printAllTables(headGst);
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
                  emit((yyval.value).ircode);

      }
#line 1951 "y.tab.c" /* yacc.c:1652  */
    break;

  case 4:
#line 509 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"43 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

      }
#line 1961 "y.tab.c" /* yacc.c:1652  */
    break;

  case 5:
#line 514 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"45 ";
                  sprintf((yyval.value).ircode,"%s\n%s", (yyvsp[-1].value).ircode, (yyvsp[0].value).ircode);

      }
#line 1971 "y.tab.c" /* yacc.c:1652  */
    break;

  case 6:
#line 522 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"3 ";
                  funcCounter++;
                  sprintf((yyval.value).ircode, "@%s:\n%s", (yyvsp[-4].value).ircode,(yyvsp[0].value).ircode);
                  // emit($$.ircode);

      }
#line 1983 "y.tab.c" /* yacc.c:1652  */
    break;

  case 7:
#line 559 "parser_temp.y" /* yacc.c:1652  */
    {
				cout<<"210 ";
				 insert_gst((yyvsp[0].value).val,&tailGst,&headGst);
         strcpy(currentFun,(yyvsp[0].value).val);
         if(setPtrGst((yyvsp[0].value).val)==1){
             strcpy(PtrGst->ret_type,functionType);
         //ircode
         strcpy((yyval.value).ircode,(yyvsp[0].value).val);
         }

		}
#line 1999 "y.tab.c" /* yacc.c:1652  */
    break;

  case 8:
#line 573 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"47 ";
                  //ircode
                  funcCounter++;
                  sprintf((yyval.value).ircode, "@%s:\n%s\n%s", (yyvsp[-4].value).ircode, (yyvsp[-2].value).ircode, (yyvsp[0].value).ircode);
                  // emit($$.ircode);


      }
#line 2013 "y.tab.c" /* yacc.c:1652  */
    break;

  case 9:
#line 600 "parser_temp.y" /* yacc.c:1652  */
    {
					cout<<"228 ";
					// emit(strdup(":"));
                  setPtrGst(currentFun);
                   cout<<"here is headC list--";
                   printlistC(headC);
                   cout<<"--";

                  flush2(&headC);
                  //ircode
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);


        }
#line 2032 "y.tab.c" /* yacc.c:1652  */
    break;

  case 10:
#line 614 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"69 ";
                  setPtrGst(currentFun);
                  PtrGst->nParam=0;

        }
#line 2043 "y.tab.c" /* yacc.c:1652  */
    break;

  case 11:
#line 622 "parser_temp.y" /* yacc.c:1652  */
    {
				cout<<"218 ";
        insert_gst((yyvsp[0].value).val,&tailGst,&headGst);

        strcpy(currentFun,(yyvsp[0].value).val);
         if(setPtrGst((yyvsp[0].value).val)==1){
             strcpy(PtrGst->ret_type,functionType);
         }
         // emit($1.val);
         //ircode
         strcpy((yyval.value).ircode,(yyvsp[0].value).val);
		}
#line 2060 "y.tab.c" /* yacc.c:1652  */
    break;

  case 12:
#line 636 "parser_temp.y" /* yacc.c:1652  */
    {
				strcpy(functionType,(yyvsp[0].value).val);

				cout<<"212 ";
				//denoted by number 1
        //ircode
        strcpy((yyval.value).ircode,(yyvsp[0].value).val);

		}
#line 2074 "y.tab.c" /* yacc.c:1652  */
    break;

  case 13:
#line 645 "parser_temp.y" /* yacc.c:1652  */
    {

				cout<<"214 ";
        strcpy(functionType,(yyvsp[0].value).val);
				//denoted by number 2
        //ircode
        strcpy((yyval.value).ircode,(yyvsp[0].value).val);

		}
#line 2088 "y.tab.c" /* yacc.c:1652  */
    break;

  case 14:
#line 654 "parser_temp.y" /* yacc.c:1652  */
    {

				cout<<"216 ";
        strcpy(functionType,(yyvsp[0].value).val);
				//denoted by number 3
        //ircode
        strcpy((yyval.value).ircode,(yyvsp[0].value).val);

		}
#line 2102 "y.tab.c" /* yacc.c:1652  */
    break;

  case 15:
#line 666 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"6 ";
                  strcpy((yyval.value).val,(yyvsp[0].value).val);
                 // cout<<"---"<<$$.val<<"--";
                 //ircode
                // strcpy($$.ircode,$1.val);

      }
#line 2115 "y.tab.c" /* yacc.c:1652  */
    break;

  case 16:
#line 674 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"15 ";
                 	strcpy((yyval.value).val,(yyvsp[0].value).val);
                  //ircode
                 // strcpy($$.ircode,$1.val);

      }
#line 2127 "y.tab.c" /* yacc.c:1652  */
    break;

  case 17:
#line 684 "parser_temp.y" /* yacc.c:1652  */
    {;}
#line 2133 "y.tab.c" /* yacc.c:1652  */
    break;

  case 18:
#line 688 "parser_temp.y" /* yacc.c:1652  */
    {
      	cout<<"224 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

      }
#line 2143 "y.tab.c" /* yacc.c:1652  */
    break;

  case 19:
#line 693 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"51 ";
                  sprintf((yyval.value).ircode,"%s\n%s", (yyvsp[-2].value).ircode, (yyvsp[0].value).ircode);
      }
#line 2152 "y.tab.c" /* yacc.c:1652  */
    break;

  case 20:
#line 700 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"53 ";

                 addNodeC((yyvsp[-1].value).val,&tailC1,&headC1);
                 addNodeC((yyvsp[0].value).val,&tailC1,&headC1);
                 setPtrGst(currentFun);
                 flush(&headC1);
                 // emit($2.val);
                 sprintf((yyval.value).ircode,"%s %s", (yyvsp[-1].value).ircode, (yyvsp[0].value).ircode);

      }
#line 2168 "y.tab.c" /* yacc.c:1652  */
    break;

  case 21:
#line 713 "parser_temp.y" /* yacc.c:1652  */
    {
          				strcpy((yyval.value).val,(yyvsp[0].value).val);
          				addNodeC((yyvsp[0].value).val,&tailC,&headC);
          				cout<<"226 ";
                  sprintf((yyval.value).ircode,"param %s", (yyvsp[0].value).ircode);

		}
#line 2180 "y.tab.c" /* yacc.c:1652  */
    break;

  case 22:
#line 723 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<<"55 ";
                addNodeC((yyvsp[0].value).val,&tailC,&headC);
                strcpy((yyval.value).val,(yyvsp[0].value).val);
                strcpy(currType,(yyval.value).val);
                //ircode
               // strcpy($$.ircode,$1.ircode);


      }
#line 2195 "y.tab.c" /* yacc.c:1652  */
    break;

  case 23:
#line 737 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"4 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
      }
#line 2204 "y.tab.c" /* yacc.c:1652  */
    break;

  case 24:
#line 744 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"5 ";

      }
#line 2213 "y.tab.c" /* yacc.c:1652  */
    break;

  case 25:
#line 749 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"17 ";
                  strcpy((yyval.value).ircode,(yyvsp[-1].value).ircode);


      }
#line 2224 "y.tab.c" /* yacc.c:1652  */
    break;

  case 26:
#line 756 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"85 ";
                  sprintf((yyval.value).ircode, "%s", (yyvsp[-1].value).ircode);



      }
#line 2236 "y.tab.c" /* yacc.c:1652  */
    break;

  case 27:
#line 763 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"87 ";
                  sprintf((yyval.value).ircode, "%s\n%s", (yyvsp[-2].value).ircode, (yyvsp[-1].value).ircode);


      }
#line 2247 "y.tab.c" /* yacc.c:1652  */
    break;

  case 28:
#line 771 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"73 ";
                  setPtrGst(currentFun);
                  printLst(PtrGst->f_tableHead);
                  //ircode
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

      }
#line 2260 "y.tab.c" /* yacc.c:1652  */
    break;

  case 29:
#line 779 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"75 ";
                  sprintf((yyval.value).ircode, "%s\n%s", (yyvsp[-1].value).ircode, (yyvsp[0].value).ircode);


      }
#line 2271 "y.tab.c" /* yacc.c:1652  */
    break;

  case 30:
#line 787 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"77 ";
                  if(setPtrGst(currentFun)==1){
                    flush(&headC);

                  }
                //  printAllTables(headGst);
                //ircode
                sprintf((yyval.value).ircode, "%s %s",(yyvsp[-2].value).ircode, (yyvsp[-1].value).ircode);


      }
#line 2288 "y.tab.c" /* yacc.c:1652  */
    break;

  case 31:
#line 801 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<< "79 ";
                sprintf((yyval.value).ircode, "%s", (yyvsp[0].value).ircode);


      }
#line 2299 "y.tab.c" /* yacc.c:1652  */
    break;

  case 32:
#line 809 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<< "81 ";
                addNodeC((yyvsp[0].value).val,&tailC,&headC);
                //ircode
                sprintf((yyval.value).ircode, "%s %s",currType,(yyvsp[0].value).ircode);

      }
#line 2311 "y.tab.c" /* yacc.c:1652  */
    break;

  case 33:
#line 816 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<<"83 ";
                addNodeC((yyvsp[0].value).val,&tailC,&headC);
                //ircode
                sprintf((yyval.value).ircode, "%s\n%s %s\n",(yyvsp[-2].value).ircode, currType, (yyvsp[0].value).ircode);

      }
#line 2323 "y.tab.c" /* yacc.c:1652  */
    break;

  case 34:
#line 826 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<<"57 ";
                sprintf(buffer, "%s.%d", (yyvsp[0].value).val,funcCounter);
                strcpy((yyval.value).val,buffer);
                //  cout<<"--"<<$$.val<<"--";
                //ircode
                strcpy((yyval.value).ircode,buffer);

      }
#line 2337 "y.tab.c" /* yacc.c:1652  */
    break;

  case 35:
#line 837 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"19 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

                  }
#line 2347 "y.tab.c" /* yacc.c:1652  */
    break;

  case 36:
#line 842 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"21 ";
                  sprintf((yyval.value).ircode, "%s\n%s",(yyvsp[-1].value).ircode,(yyvsp[0].value).ircode);

                  }
#line 2357 "y.tab.c" /* yacc.c:1652  */
    break;

  case 37:
#line 849 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"23 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

                  }
#line 2367 "y.tab.c" /* yacc.c:1652  */
    break;

  case 38:
#line 854 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"25 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
                  }
#line 2376 "y.tab.c" /* yacc.c:1652  */
    break;

  case 39:
#line 858 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"27 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
                  }
#line 2385 "y.tab.c" /* yacc.c:1652  */
    break;

  case 40:
#line 862 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"91 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

         }
#line 2395 "y.tab.c" /* yacc.c:1652  */
    break;

  case 41:
#line 867 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"59 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

                  }
#line 2405 "y.tab.c" /* yacc.c:1652  */
    break;

  case 42:
#line 872 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"68 ";
                  strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);

                  }
#line 2415 "y.tab.c" /* yacc.c:1652  */
    break;

  case 43:
#line 877 "parser_temp.y" /* yacc.c:1652  */
    {
        			cout<<"202 ";
              strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
      }
#line 2424 "y.tab.c" /* yacc.c:1652  */
    break;

  case 44:
#line 881 "parser_temp.y" /* yacc.c:1652  */
    {
        			cout<<"204 ";
              strcpy((yyval.value).ircode,(yyvsp[0].value).ircode);
      }
#line 2433 "y.tab.c" /* yacc.c:1652  */
    break;

  case 45:
#line 888 "parser_temp.y" /* yacc.c:1652  */
    {
                cout<<"206 ";
                sprintf((yyval.value).ircode, "prnt %s.%d", (yyvsp[-2].value).val,funcCounter);
                sprintf(buffer, "%s.%d", (yyvsp[-2].value).val, funcCounter);
                if(lookAllTable(buffer,headGst)==0){
                  cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                  exit(0);
                }

      }
#line 2448 "y.tab.c" /* yacc.c:1652  */
    break;

  case 46:
#line 900 "parser_temp.y" /* yacc.c:1652  */
    {
              cout<<"208 ";
              sprintf((yyval.value).ircode, "scan %s.%d", (yyvsp[-2].value).val,funcCounter);
              sprintf(buffer, "%s.%d", (yyvsp[-2].value).val, funcCounter);
              if(lookAllTable(buffer,headGst)==0){
                cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                exit(0);
              }

      }
#line 2463 "y.tab.c" /* yacc.c:1652  */
    break;

  case 47:
#line 913 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"72";
                  char *f ;
                  f = newLabelGen();
                  sprintf((yyval.value).ircode, "%s\nif neq %s 1 goto %s\n%s\n%s:", (yyvsp[-2].value).ircode, (yyvsp[-2].value).val, f, (yyvsp[0].value).ircode,f);
                  // emit($$.ircode);

      }
#line 2476 "y.tab.c" /* yacc.c:1652  */
    break;

  case 48:
#line 921 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"74";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();

                  sprintf((yyval.value).ircode, "%s\nif neq %s 1 goto %s", (yyvsp[-4].value).ircode, (yyvsp[-4].value).val, f);
                  sprintf((yyval.value).ircode, "%s\n%s\ngoto %s",(yyval.value).ircode,(yyvsp[-2].value).ircode,t);
                  sprintf((yyval.value).ircode, "%s\n%s:", (yyval.value).ircode,f);
                  sprintf((yyval.value).ircode, "%s\n%s\n%s:",(yyval.value).ircode,(yyvsp[0].value).ircode,t);
                  // emit($$.ircode);

      }
#line 2494 "y.tab.c" /* yacc.c:1652  */
    break;

  case 49:
#line 937 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"95 ";
                  strcpy((yyval.value).ircode, (yyvsp[-1].value).ircode);
                  strcpy((yyval.value).val, (yyvsp[-1].value).val);
      }
#line 2504 "y.tab.c" /* yacc.c:1652  */
    break;

  case 50:
#line 942 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"95 ";
                  strcpy((yyval.value).ircode, (yyvsp[-1].value).ircode);
                  strcpy((yyval.value).val, (yyvsp[-1].value).val);

      }
#line 2515 "y.tab.c" /* yacc.c:1652  */
    break;

  case 51:
#line 951 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"99 ";
                  sprintf(buffer, "%s.%d", (yyvsp[-5].value).val, funcCounter);
                  sprintf((yyval.value).ircode, "%s %s call %s %s", buffer, (yyvsp[-4].value).val, (yyvsp[-3].value).val, (yyvsp[-1].value).ircode);
                  if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }
                  if(lookAllTable((yyvsp[-3].value).val,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
#line 2534 "y.tab.c" /* yacc.c:1652  */
    break;

  case 52:
#line 968 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"99 ";
                  sprintf((yyval.value).ircode, "call %s %s", (yyvsp[-3].value).val, (yyvsp[-1].value).ircode);
                  if(lookAllTable((yyvsp[-3].value).val,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
#line 2548 "y.tab.c" /* yacc.c:1652  */
    break;

  case 53:
#line 980 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"101 ";

      }
#line 2557 "y.tab.c" /* yacc.c:1652  */
    break;

  case 54:
#line 984 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"103 ";
                  sprintf((yyval.value).ircode, "%s",(yyvsp[0].value).ircode);
      }
#line 2566 "y.tab.c" /* yacc.c:1652  */
    break;

  case 55:
#line 990 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"107 ";
      }
#line 2574 "y.tab.c" /* yacc.c:1652  */
    break;

  case 56:
#line 996 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"105 ";
                  sprintf((yyval.value).ircode, "%s",(yyvsp[0].value).val);
            }
#line 2583 "y.tab.c" /* yacc.c:1652  */
    break;

  case 57:
#line 1002 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"107 ";
                  sprintf((yyval.value).ircode, "%s %s",(yyvsp[-2].value).ircode, (yyvsp[0].value).val);

        }
#line 2593 "y.tab.c" /* yacc.c:1652  */
    break;

  case 58:
#line 1010 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"61 ";

                  }
#line 2602 "y.tab.c" /* yacc.c:1652  */
    break;

  case 59:
#line 1014 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"63 ";

                  }
#line 2611 "y.tab.c" /* yacc.c:1652  */
    break;

  case 60:
#line 1018 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"65 ";
                  strcpy((yyval.value).ircode,"return\n");
                  // emit($$.ircode);
                  }
#line 2621 "y.tab.c" /* yacc.c:1652  */
    break;

  case 61:
#line 1023 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"655 ";
                  strcpy((yyval.value).ircode,"exit\n");
                  // emit($$.ircode);
                  }
#line 2631 "y.tab.c" /* yacc.c:1652  */
    break;

  case 62:
#line 1028 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"65 ";
                  sprintf((yyval.value).ircode,"%s\nreturn %s",(yyvsp[-1].value).ircode,(yyvsp[-1].value).val);
                  // emit($$.ircode);
                  }
#line 2641 "y.tab.c" /* yacc.c:1652  */
    break;

  case 63:
#line 1036 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"29 ";


      }
#line 2651 "y.tab.c" /* yacc.c:1652  */
    break;

  case 64:
#line 1041 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"31 ";
                  strcpy((yyval.value).ircode, (yyvsp[-1].value).ircode);
                  strcpy((yyval.value).val, (yyvsp[-1].value).val);

                }
#line 2662 "y.tab.c" /* yacc.c:1652  */
    break;

  case 65:
#line 1050 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"35 ";
                  char *f, *t;
                  t = newLabelGen();
                  f = newLabelGen();
                  sprintf((yyval.value).ircode, "%s:\n%s\nif neq %s 1 goto %s",t, (yyvsp[-2].value).ircode, (yyvsp[-2].value).val, f);
                  sprintf((yyval.value).ircode, "%s\n%s",(yyval.value).ircode,(yyvsp[0].value).ircode);
                  sprintf((yyval.value).ircode, "%s\ngoto %s\n%s:", (yyval.value).ircode,t,f);
                  // emit($$.ircode);

      }
#line 2678 "y.tab.c" /* yacc.c:1652  */
    break;

  case 66:
#line 1061 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"37 ";
                  char *f, *t;
                  t = newLabelGen();
                  f = newLabelGen();
                  sprintf((yyval.value).ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",(yyvsp[-5].value).ircode, t,(yyvsp[-2].value).ircode, (yyvsp[-2].value).val, f);
                  sprintf((yyval.value).ircode, "%s\n%s\ngoto %s",(yyval.value).ircode,(yyvsp[-5].value).ircode,t);
                  sprintf((yyval.value).ircode, "%s\n%s:", (yyval.value).ircode,f);
                  // emit($$.ircode);


      }
#line 2695 "y.tab.c" /* yacc.c:1652  */
    break;

  case 67:
#line 1073 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"39 ";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();
                  sprintf((yyval.value).ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",(yyvsp[-4].value).ircode, t,(yyvsp[-3].value).ircode, (yyvsp[-3].value).val, f);
                  sprintf((yyval.value).ircode, "%s\n%s",(yyval.value).ircode,(yyvsp[0].value).ircode);
                  sprintf((yyval.value).ircode, "%s\ngoto %s\n%s:", (yyval.value).ircode,t,f);


      }
#line 2711 "y.tab.c" /* yacc.c:1652  */
    break;

  case 68:
#line 1084 "parser_temp.y" /* yacc.c:1652  */
    {
                  cout<<"41 ";
                  char *f, *t;
                  f = newLabelGen();
                  t = newLabelGen();
                  sprintf((yyval.value).ircode, "%s\n%s:\n%s\nif neq %s 1 goto %s",(yyvsp[-5].value).ircode, t,(yyvsp[-4].value).ircode, (yyvsp[-4].value).val, f);
                  sprintf((yyval.value).ircode, "%s\n%s\n%s",(yyval.value).ircode,(yyvsp[0].value).ircode,(yyvsp[-2].value).ircode);
                  sprintf((yyval.value).ircode, "%s\ngoto %s\n%s:", (yyval.value).ircode,t,f);
                  // emit($$.ircode);
      }
#line 2726 "y.tab.c" /* yacc.c:1652  */
    break;

  case 69:
#line 1097 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"56 ";
                    sprintf(buffer, "%s.%d", (yyvsp[0].value).val, funcCounter);
                    strcpy((yyval.value).val, buffer);
                    if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                  }

      }
#line 2741 "y.tab.c" /* yacc.c:1652  */
    break;

  case 70:
#line 1107 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"58 ";
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2751 "y.tab.c" /* yacc.c:1652  */
    break;

  case 71:
#line 1112 "parser_temp.y" /* yacc.c:1652  */
    {
        		        cout<<"208 ";
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2761 "y.tab.c" /* yacc.c:1652  */
    break;

  case 72:
#line 1120 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"220 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2772 "y.tab.c" /* yacc.c:1652  */
    break;

  case 73:
#line 1126 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"42 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s * %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2786 "y.tab.c" /* yacc.c:1652  */
    break;

  case 74:
#line 1135 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"44 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s / %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2800 "y.tab.c" /* yacc.c:1652  */
    break;

  case 75:
#line 1144 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"46 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s %% %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2814 "y.tab.c" /* yacc.c:1652  */
    break;

  case 76:
#line 1153 "parser_temp.y" /* yacc.c:1652  */
    {
            				cout<<"201 ";
                    strcpy((yyval.value).ircode, (yyvsp[-1].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[-1].value).val);

      				}
#line 2825 "y.tab.c" /* yacc.c:1652  */
    break;

  case 77:
#line 1162 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"48 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2836 "y.tab.c" /* yacc.c:1652  */
    break;

  case 78:
#line 1168 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"50 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s + %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2850 "y.tab.c" /* yacc.c:1652  */
    break;

  case 79:
#line 1177 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"52 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s - %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2864 "y.tab.c" /* yacc.c:1652  */
    break;

  case 80:
#line 1192 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"32 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2875 "y.tab.c" /* yacc.c:1652  */
    break;

  case 81:
#line 1198 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"34 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s lt %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2889 "y.tab.c" /* yacc.c:1652  */
    break;

  case 82:
#line 1207 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"36";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s gt %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2903 "y.tab.c" /* yacc.c:1652  */
    break;

  case 83:
#line 1216 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"38 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s leq %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2917 "y.tab.c" /* yacc.c:1652  */
    break;

  case 84:
#line 1225 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"40 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s geq %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2931 "y.tab.c" /* yacc.c:1652  */
    break;

  case 85:
#line 1237 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"30 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2942 "y.tab.c" /* yacc.c:1652  */
    break;

  case 86:
#line 1243 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"26 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s eqeq %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2956 "y.tab.c" /* yacc.c:1652  */
    break;

  case 87:
#line 1252 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"28 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s neq %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);

      }
#line 2970 "y.tab.c" /* yacc.c:1652  */
    break;

  case 88:
#line 1264 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"22 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);

      }
#line 2981 "y.tab.c" /* yacc.c:1652  */
    break;

  case 89:
#line 1270 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"24 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s and %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);
      }
#line 2994 "y.tab.c" /* yacc.c:1652  */
    break;

  case 90:
#line 1281 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"18 ";
                    strcpy((yyval.value).ircode, (yyvsp[0].value).ircode);
                    strcpy((yyval.value).val, (yyvsp[0].value).val);
      }
#line 3004 "y.tab.c" /* yacc.c:1652  */
    break;

  case 91:
#line 1286 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"20 ";
                    char *t;
                    t = newTempGen();
                    sprintf(buffer, "%s = %s or %s", t, (yyvsp[-2].value).val, (yyvsp[0].value).val);
                    sprintf((yyval.value).ircode, "%s\n%s\n%s", (yyvsp[-2].value).ircode,(yyvsp[0].value).ircode, buffer);
                    strcpy((yyval.value).val, t);
      }
#line 3017 "y.tab.c" /* yacc.c:1652  */
    break;

  case 92:
#line 1297 "parser_temp.y" /* yacc.c:1652  */
    {
                    cout<<"200 ";
                    sprintf(buffer, "%s.%d", (yyvsp[-2].value).val, funcCounter);
                    sprintf((yyval.value).ircode, "%s\n%s = %s",(yyvsp[0].value).ircode,buffer, (yyvsp[0].value).val);
                    if(lookAllTable(buffer,headGst)==0){
                      cout<<"error! in line"<<yylineno<<"(variable/function is used without declaring!)";
                      exit(0);
                    }
                    // emit($$.ircode);
      }
#line 3032 "y.tab.c" /* yacc.c:1652  */
    break;


#line 3036 "y.tab.c" /* yacc.c:1652  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1314 "parser_temp.y" /* yacc.c:1918  */





int main (int argc, char* argv[]){
if(argc != 2) {
        cerr << "give argument as <output_file> " << endl;
        return 1;
        }
string outfile(argv[1]);
	//myfile.open("out.ir");
	myfile.open (outfile);
	yyparse();
	cout<<endl;
	TableDump(headGst);
	return 0;

	//./out vipin.ir<input_fibo.c
	//use make file to get out
}
