/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "minipas.y" /* yacc.c:339  */

#include <stdio.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "strtab.h"
#include "lex.yy.c"
#include "stack.h"
#include "outputToC.h"
#include "tempType.h"

bucket* globalTable; 
bucket* localTable;
int isGlobal; //0 if it aint global else 1

int yyerror (char *msg) {
  showLine();
  fprintf (stderr, "%s (token=%s)\n", msg, yytext);
  exit(EXIT_FAILURE);
}

//Note: to help see the difference with our old code and new ouput code we used the /*cOutput*/ comment behind new code

/* typelist:
 * int 			265
 * real 		266
 * int[]		267
 * real[]		268 
 * void			-1
 */

tempType getType(char *strtabEntry){ //Returns the type of an variable, does also check if it exists in the scope

  int typeLocal = lookupSymbol(localTable, strtabEntry);
  struct tempType t;
  if(typeLocal == 0){ //linearSearch returns type and 0 if no type is found.
    if (lookupSymbol(globalTable, strtabEntry) == 0){
      printf("%s not declared or not accesible\n", strtabEntry);
      exit(-1);
    }
    t.type = lookupSymbol(globalTable, strtabEntry);
    return t;
  }
  t.type = typeLocal;
  return t; 
}

void checkDoubleDeclaration(char *strtabEntry, bucket* table){ //does what it says
    if (lookupSymbol(table, strtabEntry) == 0){
	return; //everything is fine not declared before
    } 
    printf("already declared, double declaration %s \n", strtabEntry);
    exit(-1);
}

void checkEqual(tempType t1, tempType t2){ //check type function/assignment (real/int is accepted)
    int type1 = t1.type;
    int type2 = t2.type;
    if(type1 == type2){
      return;
    }
    if((265 == type2|| type2 == 266) && (265 == type1 || type1 == 266)){
      return;
    }
    printf("type mismatch of %d with type %d\n", type1, type2);
    exit(-1);
}

void insertSymbolsFromStack(tempType t) {
	bucket* currentTable;
	  if (isGlobal) {
		  currentTable = globalTable;
	  } else {
		currentTable = localTable;
	  }
	while(!isEmpty()) {
		char* ident = pop();
		outputDeclaration(t, ident);//cOuput
		checkDoubleDeclaration(ident, currentTable);
		insertSymbol(currentTable, ident, t, 0);
	}
	  freeStack();
}


int* arguments;
int argSize, argIndex;

void initArguments(){
	argSize=10;
	argIndex=0;
	arguments = malloc(argSize*sizeof(int)); 
}

void addArg(tempType t) {
	int arg = t.type;
    if((argSize-1) == argIndex){
        int* temp = malloc(argSize*2*sizeof(int));    
        copy(temp, arguments);
        argSize *= 2;
        free(arguments);
        arguments = temp;    
    } 
    arguments[argIndex] = arg;
    argIndex++;
}

void insertSymbolsAndArguments(tempType t){
	  bucket* currentTable;
	  if (isGlobal) {
		currentTable = globalTable;
		 
	  } else {
		 currentTable = localTable;
	  }
	
	  while(!isEmpty()) {
		char* ident = pop();
		storeArgument(t, ident);//cOuput
		checkDoubleDeclaration(ident, currentTable);
		insertSymbol(currentTable, ident, t, 0);
		addArg(t);
	  }
	
	  freeStack();
}


#line 194 "minipas.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
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
   by #include "minipas.tab.h".  */
#ifndef YY_YY_MINIPAS_TAB_H_INCLUDED
# define YY_YY_MINIPAS_TAB_H_INCLUDED
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
    PROGRAM = 258,
    IDENTIFIER = 259,
    VAR = 260,
    ARRAY = 261,
    RANGE = 262,
    NUMBER = 263,
    OF = 264,
    INTEGER = 265,
    REAL = 266,
    FUNCTION = 267,
    PROCEDURE = 268,
    BEGINTOK = 269,
    ENDTOK = 270,
    ASSIGN = 271,
    IF = 272,
    THEN = 273,
    ELSE = 274,
    WHILE = 275,
    DO = 276,
    RELOP = 277,
    MULOP = 278,
    READLN = 279,
    WRITELN = 280
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 129 "minipas.y" /* yacc.c:355  */

  struct tempType tempType; //cOutput
  char *strtabptr;
  int label; //cOutput

#line 266 "minipas.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MINIPAS_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 281 "minipas.tab.c" /* yacc.c:358  */

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
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
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
#  define YYSIZE_T unsigned int
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

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   116

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,    27,     2,    34,    30,    35,    29,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    31,    28,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    32,     2,    33,     2,     2,     2,     2,     2,     2,
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
      25
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   146,   146,   148,   146,   153,   158,   164,   168,   171,
     172,   175,   176,   179,   180,   183,   190,   190,   201,   201,
     213,   214,   217,   219,   223,   226,   227,   230,   231,   234,
     239,   240,   242,   246,   241,   255,   254,   266,   269,   272,
     275,   278,   279,   282,   283,   284,   284,   285,   288,   289,
     292,   300,   303,   304,   307,   308,   309,   315,   323,   324,
     327,   328,   336,   338,   339,   347,   348
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PROGRAM", "IDENTIFIER", "VAR", "ARRAY",
  "RANGE", "NUMBER", "OF", "INTEGER", "REAL", "FUNCTION", "PROCEDURE",
  "BEGINTOK", "ENDTOK", "ASSIGN", "IF", "THEN", "ELSE", "WHILE", "DO",
  "RELOP", "MULOP", "READLN", "WRITELN", "'('", "')'", "';'", "'.'", "','",
  "':'", "'['", "']'", "'+'", "'-'", "$accept", "program", "$@1", "$@2",
  "identlist", "declarations", "type", "standardtype", "subprogdecls",
  "subprogdecl", "subprogheading", "$@3", "$@4", "arguments", "paramlist",
  "compoundstatement", "optionalstatements", "statementlist", "statement",
  "$@5", "$@6", "$@7", "ifrule", "thenrule", "whilerule", "dorule",
  "variable", "procstatement", "$@8", "exprlist", "boolexpression",
  "relationop", "expression", "simpleexpr", "sign", "term", "multop",
  "factor", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    40,    41,    59,    46,
      44,    58,    91,    93,    43,    45
};
# endif

#define YYPACT_NINF -92

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-92)))

#define YYTABLE_NINF -42

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      12,    24,    22,     4,   -92,    40,   -92,    -7,    54,    56,
     -92,   -92,   -92,    45,    40,    53,    37,   -92,   -92,    57,
      55,   -92,     6,    77,    80,    29,    58,   -92,    33,    59,
     -92,   -92,    60,   -92,    64,    64,    -8,   -92,   -92,    66,
     -92,   -92,    70,    67,   -92,     1,     1,    78,   -92,   -92,
     -92,    85,   -92,    40,    68,    69,     1,     1,     1,    72,
     -92,    29,    74,   -92,     1,   -92,   -92,   -92,    83,    -9,
       3,    63,   -92,    75,     1,    95,    39,    46,    65,   -92,
      18,   -92,    71,    28,     1,   -92,     1,    76,   -92,   -92,
     -92,     3,     3,     1,    63,   -92,     3,   -92,   -92,   -92,
      97,     6,   -92,    40,    79,   -92,     1,   -92,   -92,    32,
      34,   -92,    29,    63,    63,    43,   -92,    29,    73,   -92,
      49,   -92,   -92,   -92,   -92,   -92,   -92,    99,     6,    90,
      65,   -92,    29,   -92,   -92
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     5,     0,     0,     0,
       2,     6,     8,    14,     0,     3,     0,    16,    18,     0,
       0,     8,     0,     0,     0,    26,     0,    13,     0,     0,
      11,    12,     0,     9,    21,    21,    43,    37,    39,     0,
      45,    31,     0,    25,    27,     0,     0,     0,    30,     4,
      15,     0,     7,     0,     0,     0,     0,     0,     0,     0,
      24,     0,    63,    65,     0,    58,    59,    53,     0,    52,
       0,    54,    60,     0,     0,     0,     0,     0,     0,    19,
       0,    48,     0,     0,     0,    28,     0,     0,    38,    32,
      51,     0,     0,     0,    55,    62,     0,    40,    35,    29,
       0,     0,    20,     0,     0,    44,     0,    42,    47,     0,
       0,    66,     0,    56,    57,    50,    61,     0,     0,    22,
       0,    17,    49,    46,    64,    33,    36,     0,     0,     0,
       0,    23,     0,    10,    34
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -92,   -92,   -92,   -92,   -14,    89,   -91,   -74,   -92,   -92,
     -92,   -92,   -92,    81,   -92,    23,   -92,   -92,   -60,   -92,
     -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,   -52,
     -92,   -92,   -43,    19,   -92,   -51,   -92,    15
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    12,    19,     7,    13,    32,    33,    15,    20,
      21,    23,    24,    54,    77,    41,    42,    43,    44,   112,
     129,   117,    45,    89,    46,    98,    47,    48,    59,    80,
      67,    93,    81,    69,    70,    71,    96,    72
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      16,    85,    68,    73,   104,    62,    83,    62,   -41,    63,
     119,    63,    29,    90,    82,     1,    30,    31,    56,    94,
       8,    87,     4,     9,    57,    91,    92,    64,     3,    64,
       5,    99,   109,    36,   110,    65,    66,   131,    14,    76,
     113,   114,    26,    25,     6,   105,    37,    25,   106,    38,
      14,    50,   125,    39,    40,   108,   133,   126,   106,   123,
      11,   124,   106,   122,   106,    17,    18,     9,    22,     9,
     101,    25,   134,   102,   103,    30,    31,    91,    92,     9,
     128,    34,    10,    27,    35,    60,    95,    49,    52,   120,
      53,    51,    58,    75,    74,    61,    97,    79,    84,    78,
      86,    88,   100,   111,   107,   118,   127,   121,   130,   132,
      28,   116,   115,     0,     0,     0,    55
};

static const yytype_int16 yycheck[] =
{
      14,    61,    45,    46,    78,     4,    58,     4,    16,     8,
     101,     8,     6,    22,    57,     3,    10,    11,    26,    70,
      27,    64,     0,    30,    32,    34,    35,    26,     4,    26,
      26,    74,    84,     4,    86,    34,    35,   128,     5,    53,
      91,    92,    19,    14,     4,    27,    17,    14,    30,    20,
       5,    28,   112,    24,    25,    27,   130,   117,    30,    27,
       4,    27,    30,   106,    30,    12,    13,    30,    31,    30,
      31,    14,   132,    27,    28,    10,    11,    34,    35,    30,
      31,     4,    28,    28,     4,    15,    23,    29,    28,   103,
      26,    32,    26,     8,    16,    28,    21,    28,    26,    31,
      26,    18,     7,    27,    33,     8,    33,    28,     9,    19,
      21,    96,    93,    -1,    -1,    -1,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    37,     4,     0,    26,     4,    40,    27,    30,
      28,     4,    38,    41,     5,    44,    40,    12,    13,    39,
      45,    46,    31,    47,    48,    14,    51,    28,    41,     6,
      10,    11,    42,    43,     4,     4,     4,    17,    20,    24,
      25,    51,    52,    53,    54,    58,    60,    62,    63,    29,
      51,    32,    28,    26,    49,    49,    26,    32,    26,    64,
      15,    28,     4,     8,    26,    34,    35,    66,    68,    69,
      70,    71,    73,    68,    16,     8,    40,    50,    31,    28,
      65,    68,    68,    65,    26,    54,    26,    68,    18,    59,
      22,    34,    35,    67,    71,    23,    72,    21,    61,    68,
       7,    31,    27,    28,    43,    27,    30,    33,    27,    65,
      65,    27,    55,    71,    71,    69,    73,    57,     8,    42,
      40,    28,    68,    27,    27,    54,    54,    33,    31,    56,
       9,    42,    19,    43,    54
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    36,    38,    39,    37,    40,    40,    41,    41,    42,
      42,    43,    43,    44,    44,    45,    47,    46,    48,    46,
      49,    49,    50,    50,    51,    52,    52,    53,    53,    54,
      54,    54,    55,    56,    54,    57,    54,    58,    59,    60,
      61,    62,    62,    63,    63,    64,    63,    63,    65,    65,
      66,    67,    68,    68,    69,    69,    69,    69,    70,    70,
      71,    71,    72,    73,    73,    73,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,    12,     1,     3,     6,     0,     1,
       8,     1,     1,     3,     0,     3,     0,     7,     0,     5,
       3,     0,     3,     5,     3,     1,     0,     1,     3,     3,
       1,     1,     0,     0,     8,     0,     5,     1,     1,     1,
       1,     1,     4,     1,     4,     0,     5,     4,     1,     3,
       3,     1,     1,     1,     1,     2,     3,     3,     1,     1,
       1,     3,     1,     1,     4,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
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
  unsigned long int yylno = yyrline[yyrule];
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
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
            /* Fall through.  */
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

  return yystpcpy (yyres, yystr) - yyres;
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
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
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
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

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
| yyreduce -- Do a reduction.  |
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
#line 146 "minipas.y" /* yacc.c:1646  */
    {freeStack();}
#line 1447 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 148 "minipas.y" /* yacc.c:1646  */
    {outputMain();}
#line 1453 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 150 "minipas.y" /* yacc.c:1646  */
    {outputEndMain();}
#line 1459 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 154 "minipas.y" /* yacc.c:1646  */
    { 
						initStack();
						push((yyvsp[0].strtabptr));
					}
#line 1468 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 159 "minipas.y" /* yacc.c:1646  */
    {
					   push((yyvsp[0].strtabptr));
					}
#line 1476 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 165 "minipas.y" /* yacc.c:1646  */
    {
						insertSymbolsFromStack((yyvsp[-1].tempType));
					}
#line 1484 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 172 "minipas.y" /* yacc.c:1646  */
    {struct tempType t = (yyvsp[0].tempType); t.type + 2; (yyval.tempType) = t;}
#line 1490 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 184 "minipas.y" /* yacc.c:1646  */
    {
				outputString("return "); outputString((yyvsp[-2].strtabptr)); outputEnd(); 
				outputString("}\n");
			}
#line 1499 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 190 "minipas.y" /* yacc.c:1646  */
    {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments(); initStoredArguments();/*cOutput*/}
#line 1505 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 191 "minipas.y" /* yacc.c:1646  */
    {
							outputFunctionName((yyvsp[-1].tempType), (yyvsp[-4].strtabptr)); //cOuput
							checkDoubleDeclaration((yyvsp[-4].strtabptr), localTable); 
							insertSymbol(localTable, (yyvsp[-4].strtabptr), (yyvsp[-1].tempType), 0); 
							checkDoubleDeclaration((yyvsp[-4].strtabptr), globalTable); 
							insertSymbol(globalTable, (yyvsp[-4].strtabptr), (yyvsp[-1].tempType), 1); 
							addArguments(globalTable,(yyvsp[-4].strtabptr), arguments, argIndex); 
							free(arguments);
							(yyval.strtabptr) = (yyvsp[-4].strtabptr);
						}
#line 1520 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 201 "minipas.y" /* yacc.c:1646  */
    {isGlobal = 0; free(localTable); localTable = initSymbolTable(); initArguments(); initStoredArguments();/*cOutput*/}
#line 1526 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 202 "minipas.y" /* yacc.c:1646  */
    {
							struct tempType t; t.type = -1; //cOuput
							outputFunctionName(t, (yyvsp[-2].strtabptr)); //cOuput
							checkDoubleDeclaration((yyvsp[-2].strtabptr), globalTable); 
							insertSymbol(globalTable, (yyvsp[-2].strtabptr), t, 1); 
							addArguments(globalTable,(yyvsp[-2].strtabptr), arguments, argIndex); 
							free(arguments);
							(yyval.strtabptr) = "";
						}
#line 1540 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 218 "minipas.y" /* yacc.c:1646  */
    {insertSymbolsAndArguments((yyvsp[0].tempType));}
#line 1546 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 220 "minipas.y" /* yacc.c:1646  */
    {insertSymbolsAndArguments((yyvsp[0].tempType));}
#line 1552 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 235 "minipas.y" /* yacc.c:1646  */
    {
				checkEqual(getType((yyvsp[-2].strtabptr)),(yyvsp[0].tempType));
				outputString((yyvsp[-2].strtabptr)); outputString(" = "); outputOldTemp((yyvsp[0].tempType).temp); outputEnd(); //cOuput
			}
#line 1561 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 242 "minipas.y" /* yacc.c:1646  */
    {
				outputString("if(!"); outputOldTemp((yyvsp[-1].tempType).temp); outputString(")"); outputString("goto "); outputLabel((yyvsp[-2].label)); outputEnd(); //cOuput
			}
#line 1569 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 246 "minipas.y" /* yacc.c:1646  */
    {
				outputString("goto "); outputLabel((yyvsp[-2].label)); outputEnd(); //cOuput
				outputLabel((yyvsp[-4].label)); outputString(": "); outputEnd(); //cOuput
			}
#line 1578 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 251 "minipas.y" /* yacc.c:1646  */
    {
				outputLabel((yyvsp[-5].label)); outputString(": "); outputEnd(); //cOuput
			}
#line 1586 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 255 "minipas.y" /* yacc.c:1646  */
    {
				outputLabel((yyvsp[-2].label)); outputString(": "); outputEnd(); //cOutput
				outputString("if(!"); outputOldTemp((yyvsp[-1].tempType).temp); outputString(")"); outputString("goto "); outputLabel((yyvsp[0].label)); outputEnd(); //cOutput
			}
#line 1595 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 260 "minipas.y" /* yacc.c:1646  */
    {
				outputString("goto "); outputLabel((yyvsp[-4].label)); outputEnd(); //cOutput
				outputLabel((yyvsp[-2].label)); outputString(": "); outputEnd(); //cOutput
			}
#line 1604 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 266 "minipas.y" /* yacc.c:1646  */
    {(yyval.label) = getLabel();}
#line 1610 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 269 "minipas.y" /* yacc.c:1646  */
    {(yyval.label) = getLabel();}
#line 1616 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 272 "minipas.y" /* yacc.c:1646  */
    {(yyval.label) = getLabel();}
#line 1622 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 275 "minipas.y" /* yacc.c:1646  */
    {(yyval.label) = getLabel();}
#line 1628 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 283 "minipas.y" /* yacc.c:1646  */
    {getType((yyvsp[-3].strtabptr)); if(!isFunction(globalTable, (yyvsp[-3].strtabptr))) {printf("%s is not a function\n", (yyvsp[-3].strtabptr)); exit(-1);} checkArguments(globalTable, (yyvsp[-3].strtabptr), arguments, argIndex); free(arguments);}
#line 1634 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 284 "minipas.y" /* yacc.c:1646  */
    {outputString("printf(");}
#line 1640 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 284 "minipas.y" /* yacc.c:1646  */
    {free(arguments); outputString(")"); outputEnd();}
#line 1646 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 285 "minipas.y" /* yacc.c:1646  */
    {free(arguments);}
#line 1652 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 288 "minipas.y" /* yacc.c:1646  */
    {initArguments(); addArg((yyvsp[0].tempType)); initTempList(); storeToTempList();/*cOutput*/}
#line 1658 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 289 "minipas.y" /* yacc.c:1646  */
    {addArg((yyvsp[0].tempType)); storeToTempList();/*cOutput*/}
#line 1664 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 293 "minipas.y" /* yacc.c:1646  */
    {
							checkEqual((yyvsp[-2].tempType),(yyvsp[0].tempType)); 
							outputTempValue((yyvsp[-2].tempType)); outputOldTemp((yyvsp[-2].tempType).temp); outputString((yyvsp[-1].strtabptr)); outputOldTemp((yyvsp[0].tempType).temp); outputEnd(); //cOuput 
							struct tempType t = (yyvsp[-2].tempType); t.temp = getLastTemp(); (yyval.tempType) = t; //cOuput
						}
#line 1674 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 300 "minipas.y" /* yacc.c:1646  */
    {char *tmp = malloc((strlen(yytext) + 1) * sizeof(char)); strcpy(tmp, yytext); (yyval.strtabptr) = tmp;}
#line 1680 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 308 "minipas.y" /* yacc.c:1646  */
    {outputTempValue((yyvsp[0].tempType)); outputString((yyvsp[-1].strtabptr)); outputLastTemp(); outputEnd(); struct tempType t = (yyvsp[0].tempType); t.temp = getLastTemp(); (yyval.tempType) = t;}
#line 1686 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 310 "minipas.y" /* yacc.c:1646  */
    {
							checkEqual((yyvsp[-2].tempType),(yyvsp[0].tempType)); 
							outputTempValue((yyvsp[-2].tempType)); outputOldTemp((yyvsp[-2].tempType).temp); outputString(" + "); outputOldTemp((yyvsp[0].tempType).temp); outputEnd(); //cOuput
							struct tempType t = (yyvsp[-2].tempType); t.temp = getLastTemp(); (yyval.tempType) = t; //cOuput
						}
#line 1696 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 316 "minipas.y" /* yacc.c:1646  */
    {
							checkEqual((yyvsp[-2].tempType),(yyvsp[0].tempType)); 
							outputTempValue((yyvsp[-2].tempType)); outputOldTemp((yyvsp[-2].tempType).temp); outputString(" - "); outputOldTemp((yyvsp[0].tempType).temp); outputEnd(); //cOuput
							struct tempType t = (yyvsp[-2].tempType); t.temp = getLastTemp(); (yyval.tempType) = t; //cOuput
						}
#line 1706 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 323 "minipas.y" /* yacc.c:1646  */
    {(yyval.strtabptr) = "+";}
#line 1712 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 324 "minipas.y" /* yacc.c:1646  */
    {(yyval.strtabptr) = "-";}
#line 1718 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 329 "minipas.y" /* yacc.c:1646  */
    {
							checkEqual((yyvsp[-2].tempType),(yyvsp[0].tempType)); 
							outputTempValue((yyvsp[-2].tempType)); outputOldTemp((yyvsp[-2].tempType).temp); outputMulop((yyvsp[-1].strtabptr)); outputOldTemp((yyvsp[0].tempType).temp); outputEnd(); //cOuput
							struct tempType t = (yyvsp[-2].tempType); t.temp = getLastTemp(); (yyval.tempType) = t; //cOuput
						}
#line 1728 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 336 "minipas.y" /* yacc.c:1646  */
    {(yyval.strtabptr)=yytext;}
#line 1734 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 338 "minipas.y" /* yacc.c:1646  */
    {outputTempValue(getType((yyvsp[0].strtabptr))); outputString((yyvsp[0].strtabptr)); outputEnd(); struct tempType t; t = getType((yyvsp[0].strtabptr)); t.temp = getLastTemp(); (yyval.tempType) = t;}
#line 1740 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 340 "minipas.y" /* yacc.c:1646  */
    {
							if(!isFunction(globalTable, (yyvsp[-3].strtabptr))) {printf("%s is not a function\n", (yyvsp[-3].strtabptr)); exit(-1);}
							checkArguments(globalTable, (yyvsp[-3].strtabptr), arguments, argIndex);
							free(arguments);
							outputTempValue(getType((yyvsp[-3].strtabptr))); outputString((yyvsp[-3].strtabptr)); outputTempList(); outputEnd(); //cOutput
							struct tempType t; t = getType((yyvsp[-3].strtabptr)); t.temp = getLastTemp(); (yyval.tempType) = t; //cOuput
						}
#line 1752 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 347 "minipas.y" /* yacc.c:1646  */
    {outputTempValue((yyvsp[0].tempType)); outputString(yytext); outputEnd(); struct tempType t = (yyvsp[0].tempType); t.temp = getLastTemp(); (yyval.tempType) = t;}
#line 1758 "minipas.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 348 "minipas.y" /* yacc.c:1646  */
    {outputTempValue((yyvsp[-1].tempType)); outputLastTemp(); outputEnd(); struct tempType t = (yyvsp[-1].tempType); t.temp = getLastTemp(); (yyval.tempType) = t;}
#line 1764 "minipas.tab.c" /* yacc.c:1646  */
    break;


#line 1768 "minipas.tab.c" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
#line 351 "minipas.y" /* yacc.c:1906  */


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <pasfile>\n", argv[0]);
    return EXIT_FAILURE;
  }
  
  initializeOutput();
  initializeStringTable(argv[1]);
  initLexer(argv[1]);
  isGlobal = 1;
  globalTable = initSymbolTable();
  yyparse();  
  freeStringTable();
  finalizeLexer();
  freeSymbolTable(globalTable);
  freeSymbolTable(localTable);
  
  
  return EXIT_SUCCESS;
}
