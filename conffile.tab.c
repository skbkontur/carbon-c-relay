/* A Bison parser, made by GNU Bison 3.5.  */

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
#define YYBISON_VERSION "3.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         ROUTER_YYSTYPE
#define YYLTYPE         ROUTER_YYLTYPE
/* Substitute the variable and function names.  */
#define yyparse         router_yyparse
#define yylex           router_yylex
#define yyerror         router_yyerror
#define yydebug         router_yydebug
#define yynerrs         router_yynerrs

/* First part of user prologue.  */
#line 1 "conffile.y"

#include "allocator.h"
#include "conffile.h"
#include "conffile.tab.h"
#include "aggregator.h"
#include "receptor.h"
#include "config.h"

int router_yylex(ROUTER_YYSTYPE *, ROUTER_YYLTYPE *, void *, router *, allocator *, allocator *);

#line 88 "conffile.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
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
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_ROUTER_YY_CONFFILE_TAB_H_INCLUDED
# define YY_ROUTER_YY_CONFFILE_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef ROUTER_YYDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define ROUTER_YYDEBUG 1
#  else
#   define ROUTER_YYDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define ROUTER_YYDEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined ROUTER_YYDEBUG */
#if ROUTER_YYDEBUG
extern int router_yydebug;
#endif
/* "%code requires" blocks.  */
#line 11 "conffile.y"

struct _clust {
	enum clusttype t;
	int ival;
};
struct _clhost {
	char *ip;
	unsigned short port;
	char *inst;
	int proto;
	con_type type;
	con_trnsp trnsp;
	void *saddr;
	void *hint;
	struct _clhost *next;
};
struct _cluster_options {
	int server_connections;
	int ttl;
	int threshold_start;
	int threshold_end;
};
struct _maexpr {
	route *r;
	char drop;
	struct _maexpr *next;
};
struct _agcomp {
	enum _aggr_compute_type ctype;
	unsigned char pctl;
	char *metric;
	struct _agcomp *next;
};
struct _lsnr {
	con_type type;
	struct _rcptr_trsp *transport;
	struct _rcptr *rcptr;
};
struct _rcptr {
	con_proto ctype;
	char *ip;
	unsigned short port;
	void *saddr;
	struct _rcptr *next;
};
struct _rcptr_trsp {
	con_trnsp mode;
	char *pemcert;
};

#line 190 "conffile.tab.c"

/* Token type.  */
#ifndef ROUTER_YYTOKENTYPE
# define ROUTER_YYTOKENTYPE
  enum router_yytokentype
  {
    crCLUSTER = 258,
    crFORWARD = 259,
    crANY_OF = 260,
    crFAILOVER = 261,
    crLB = 262,
    crCARBON_CH = 263,
    crFNV1A_CH = 264,
    crJUMP_FNV1A_CH = 265,
    crFILE = 266,
    crIP = 267,
    crREPLICATION = 268,
    crDYNAMIC = 269,
    crPROTO = 270,
    crUSEALL = 271,
    crUDP = 272,
    crTCP = 273,
    crTTL = 274,
    crCONNECTIONS = 275,
    crTHRESHOLD_START = 276,
    crTHRESHOLD_END = 277,
    crMATCH = 278,
    crVALIDATE = 279,
    crELSE = 280,
    crLOG = 281,
    crDROP = 282,
    crROUTE = 283,
    crUSING = 284,
    crSEND = 285,
    crTO = 286,
    crBLACKHOLE = 287,
    crSTOP = 288,
    crREWRITE = 289,
    crINTO = 290,
    crAGGREGATE = 291,
    crEVERY = 292,
    crSECONDS = 293,
    crEXPIRE = 294,
    crAFTER = 295,
    crTIMESTAMP = 296,
    crAT = 297,
    crSTART = 298,
    crMIDDLE = 299,
    crEND = 300,
    crOF = 301,
    crBUCKET = 302,
    crCOMPUTE = 303,
    crSUM = 304,
    crCOUNT = 305,
    crMAX = 306,
    crMIN = 307,
    crAVERAGE = 308,
    crMEDIAN = 309,
    crVARIANCE = 310,
    crSTDDEV = 311,
    crPERCENTILE = 312,
    crWRITE = 313,
    crSTATISTICS = 314,
    crSUBMIT = 315,
    crRESET = 316,
    crCOUNTERS = 317,
    crINTERVAL = 318,
    crPREFIX = 319,
    crWITH = 320,
    crLISTEN = 321,
    crTYPE = 322,
    crLINEMODE = 323,
    crSYSLOGMODE = 324,
    crTRANSPORT = 325,
    crPLAIN = 326,
    crGZIP = 327,
    crLZ4 = 328,
    crSNAPPY = 329,
    crSSL = 330,
    crUNIX = 331,
    crINCLUDE = 332,
    crCOMMENT = 333,
    crSTRING = 334,
    crUNEXPECTED = 335,
    crINTVAL = 336
  };
#endif

/* Value type.  */
#if ! defined ROUTER_YYSTYPE && ! defined ROUTER_YYSTYPE_IS_DECLARED
union ROUTER_YYSTYPE
{

  /* crCOMMENT  */
  char * crCOMMENT;
  /* crSTRING  */
  char * crSTRING;
  /* crUNEXPECTED  */
  char * crUNEXPECTED;
  /* cluster_opt_instance  */
  char * cluster_opt_instance;
  /* match_opt_route  */
  char * match_opt_route;
  /* statistics_opt_prefix  */
  char * statistics_opt_prefix;
  /* cluster  */
  cluster * cluster;
  /* statistics_opt_counters  */
  col_mode statistics_opt_counters;
  /* cluster_opt_proto  */
  con_proto cluster_opt_proto;
  /* rcptr_proto  */
  con_proto rcptr_proto;
  /* cluster_opt_transport  */
  con_trnsp cluster_opt_transport;
  /* cluster_transport_trans  */
  con_trnsp cluster_transport_trans;
  /* cluster_transport_opt_ssl  */
  con_trnsp cluster_transport_opt_ssl;
  /* cluster_opt_type  */
  con_type cluster_opt_type;
  /* match_opt_send_to  */
  destinations * match_opt_send_to;
  /* match_send_to  */
  destinations * match_send_to;
  /* match_dsts  */
  destinations * match_dsts;
  /* match_dsts2  */
  destinations * match_dsts2;
  /* match_opt_dst  */
  destinations * match_opt_dst;
  /* match_dst  */
  destinations * match_dst;
  /* aggregate_opt_send_to  */
  destinations * aggregate_opt_send_to;
  /* aggregate_opt_timestamp  */
  enum _aggr_timestamp aggregate_opt_timestamp;
  /* aggregate_ts_when  */
  enum _aggr_timestamp aggregate_ts_when;
  /* cluster_useall  */
  enum clusttype cluster_useall;
  /* cluster_ch  */
  enum clusttype cluster_ch;
  /* crPERCENTILE  */
  int crPERCENTILE;
  /* crINTVAL  */
  int crINTVAL;
  /* cluster_opt_useall  */
  int cluster_opt_useall;
  /* cluster_opt_repl  */
  int cluster_opt_repl;
  /* cluster_opt_dynamic  */
  int cluster_opt_dynamic;
  /* connections  */
  int connections;
  /* threshold_start  */
  int threshold_start;
  /* threshold_end  */
  int threshold_end;
  /* ttl  */
  int ttl;
  /* match_log_or_drop  */
  int match_log_or_drop;
  /* match_opt_stop  */
  int match_opt_stop;
  /* statistics_opt_interval  */
  int statistics_opt_interval;
  /* aggregate_comp_type  */
  struct _agcomp aggregate_comp_type;
  /* aggregate_computes  */
  struct _agcomp * aggregate_computes;
  /* aggregate_opt_compute  */
  struct _agcomp * aggregate_opt_compute;
  /* aggregate_compute  */
  struct _agcomp * aggregate_compute;
  /* cluster_paths  */
  struct _clhost * cluster_paths;
  /* cluster_opt_path  */
  struct _clhost * cluster_opt_path;
  /* cluster_path  */
  struct _clhost * cluster_path;
  /* cluster_hosts  */
  struct _clhost * cluster_hosts;
  /* cluster_opt_host  */
  struct _clhost * cluster_opt_host;
  /* cluster_host  */
  struct _clhost * cluster_host;
  /* cluster_type  */
  struct _clust cluster_type;
  /* cluster_file  */
  struct _clust cluster_file;
  /* listener  */
  struct _lsnr * listener;
  /* match_exprs  */
  struct _maexpr * match_exprs;
  /* match_exprs2  */
  struct _maexpr * match_exprs2;
  /* match_opt_expr  */
  struct _maexpr * match_opt_expr;
  /* match_expr  */
  struct _maexpr * match_expr;
  /* match_opt_validate  */
  struct _maexpr * match_opt_validate;
  /* receptors  */
  struct _rcptr * receptors;
  /* opt_receptor  */
  struct _rcptr * opt_receptor;
  /* receptor  */
  struct _rcptr * receptor;
  /* transport_opt_ssl  */
  struct _rcptr_trsp * transport_opt_ssl;
  /* transport_mode_trans  */
  struct _rcptr_trsp * transport_mode_trans;
  /* transport_mode  */
  struct _rcptr_trsp * transport_mode;
#line 406 "conffile.tab.c"

};
typedef union ROUTER_YYSTYPE ROUTER_YYSTYPE;
# define ROUTER_YYSTYPE_IS_TRIVIAL 1
# define ROUTER_YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined ROUTER_YYLTYPE && ! defined ROUTER_YYLTYPE_IS_DECLARED
typedef struct ROUTER_YYLTYPE ROUTER_YYLTYPE;
struct ROUTER_YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define ROUTER_YYLTYPE_IS_DECLARED 1
# define ROUTER_YYLTYPE_IS_TRIVIAL 1
#endif



int router_yyparse (void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc);

#endif /* !YY_ROUTER_YY_CONFFILE_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
         || (defined ROUTER_YYLTYPE_IS_TRIVIAL && ROUTER_YYLTYPE_IS_TRIVIAL \
             && defined ROUTER_YYSTYPE_IS_TRIVIAL && ROUTER_YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   155

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  137
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  208

#define YYUNDEFTOK  2
#define YYMAXUTOK   336


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    84,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    82,
       2,    83,     2,     2,     2,     2,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81
};

#if ROUTER_YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   134,   134,   137,   138,   141,   144,   145,   146,   147,
     148,   149,   150,   151,   155,   228,   271,   273,   277,   278,
     279,   280,   283,   284,   287,   288,   289,   292,   293,   296,
     297,   300,   301,   304,   305,   308,   309,   312,   313,   316,
     317,   320,   322,   323,   325,   345,   347,   348,   350,   372,
     373,   374,   384,   385,   386,   389,   390,   391,   394,   395,
     405,   406,   416,   426,   437,   438,   453,   505,   517,   520,
     522,   523,   526,   545,   546,   565,   566,   569,   570,   573,
     574,   577,   580,   590,   593,   595,   596,   599,   615,   616,
     621,   663,   751,   752,   757,   758,   759,   762,   766,   767,
     769,   784,   785,   786,   787,   788,   789,   790,   800,   801,
     804,   805,   810,   825,   850,   851,   862,   863,   866,   867,
     872,   890,   915,   918,   938,   948,   965,   982,  1002,  1011,
    1022,  1025,  1026,  1029,  1066,  1088,  1089,  1094
};
#endif

#if ROUTER_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "crCLUSTER", "crFORWARD", "crANY_OF",
  "crFAILOVER", "crLB", "crCARBON_CH", "crFNV1A_CH", "crJUMP_FNV1A_CH",
  "crFILE", "crIP", "crREPLICATION", "crDYNAMIC", "crPROTO", "crUSEALL",
  "crUDP", "crTCP", "crTTL", "crCONNECTIONS", "crTHRESHOLD_START",
  "crTHRESHOLD_END", "crMATCH", "crVALIDATE", "crELSE", "crLOG", "crDROP",
  "crROUTE", "crUSING", "crSEND", "crTO", "crBLACKHOLE", "crSTOP",
  "crREWRITE", "crINTO", "crAGGREGATE", "crEVERY", "crSECONDS", "crEXPIRE",
  "crAFTER", "crTIMESTAMP", "crAT", "crSTART", "crMIDDLE", "crEND", "crOF",
  "crBUCKET", "crCOMPUTE", "crSUM", "crCOUNT", "crMAX", "crMIN",
  "crAVERAGE", "crMEDIAN", "crVARIANCE", "crSTDDEV", "crPERCENTILE",
  "crWRITE", "crSTATISTICS", "crSUBMIT", "crRESET", "crCOUNTERS",
  "crINTERVAL", "crPREFIX", "crWITH", "crLISTEN", "crTYPE", "crLINEMODE",
  "crSYSLOGMODE", "crTRANSPORT", "crPLAIN", "crGZIP", "crLZ4", "crSNAPPY",
  "crSSL", "crUNIX", "crINCLUDE", "crCOMMENT", "crSTRING", "crUNEXPECTED",
  "crINTVAL", "';'", "'='", "'*'", "$accept", "stmts", "opt_stmt", "stmt",
  "command", "cluster", "cluster_type", "cluster_useall",
  "cluster_opt_useall", "cluster_ch", "cluster_opt_repl",
  "cluster_opt_dynamic", "connections", "threshold_start", "threshold_end",
  "ttl", "cluster_file", "cluster_paths", "cluster_opt_path",
  "cluster_path", "cluster_hosts", "cluster_opt_host", "cluster_host",
  "cluster_opt_instance", "cluster_opt_proto", "cluster_opt_type",
  "cluster_opt_transport", "cluster_transport_trans",
  "cluster_transport_opt_ssl", "match", "match_exprs", "match_exprs2",
  "match_opt_expr", "match_expr", "match_opt_validate",
  "match_log_or_drop", "match_opt_route", "match_opt_send_to",
  "match_send_to", "match_dsts", "match_dsts2", "match_opt_dst",
  "match_dst", "match_opt_stop", "rewrite", "aggregate",
  "aggregate_opt_timestamp", "aggregate_ts_when", "aggregate_computes",
  "aggregate_opt_compute", "aggregate_compute", "aggregate_comp_type",
  "aggregate_opt_send_to", "send", "statistics", "statistics_opt_interval",
  "statistics_opt_counters", "statistics_opt_prefix", "listen", "listener",
  "transport_opt_ssl", "transport_mode_trans", "transport_mode",
  "receptors", "opt_receptor", "receptor", "rcptr_proto", "include", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,    59,    61,    42
};
# endif

#define YYPACT_NINF (-97)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -2,   -69,   -57,   -29,   -46,   -36,   -10,   -11,   -21,    59,
     -97,    -2,   -22,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,    31,   -97,   -97,    38,   -97,   -36,    32,    30,    29,
      33,     6,     1,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,   -97,   -97,   -97,    56,    51,    57,    61,    51,
      -7,    48,   -97,   -97,   -28,    -1,    -4,     2,    17,    16,
      11,   -97,     3,    63,   -97,   -97,     4,    72,    63,    62,
      60,    58,   -97,   -97,    65,   -97,    12,   -97,    52,    54,
      53,    34,    58,   -65,    19,    21,   -97,    14,    75,   -97,
     -97,   -97,    75,    22,    23,    70,    65,   -97,   -97,   -97,
     -97,   -97,    64,   -97,    41,    26,   -97,    65,   -97,   -97,
     -97,   -97,    27,   -97,    92,   -97,    21,   -97,    28,    86,
      86,   -97,   -97,   -97,   -97,   -28,   -97,    71,   -97,   -97,
     -97,   -97,   -15,   -97,   -97,   -97,    36,    37,    39,   -97,
      40,   -97,   -97,   -97,   -97,   -97,    42,   -97,    37,   -97,
     -97,    39,    74,   -50,    95,   -97,   -97,   -97,   -97,    73,
     -97,   -97,    35,    46,    77,    67,   -97,   -97,   -14,    50,
     -20,   -37,    58,    67,   -97,   -97,   -27,   -97,    47,   -97,
     -97,   -97,    78,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,    68,    65,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,    76,    96,   -97,   -97,    49,   -97
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     0,     0,   114,     0,     0,     0,
       2,     3,     0,     6,     7,     8,     9,    10,    11,    12,
      13,     0,    72,    67,    73,    68,    70,     0,     0,     0,
       0,   116,     0,   120,   137,     1,     4,     5,    18,    19,
      20,    21,    24,    25,    26,    40,    31,    22,    27,    31,
       0,    77,    71,    69,     0,     0,     0,     0,     0,   118,
     128,    39,     0,    37,    23,    16,     0,    29,    37,     0,
       0,    79,    82,    87,    88,    83,    85,    90,     0,     0,
       0,     0,   110,     0,   122,     0,    32,     0,    33,    28,
      30,    17,    33,     0,     0,     0,    88,    80,    89,   112,
      86,    84,     0,   115,     0,     0,   111,    88,   124,   125,
     126,   127,     0,   129,     0,   121,   131,    38,     0,    35,
      35,    75,    76,    74,    78,     0,    66,     0,   117,   119,
     113,   123,     0,   132,   130,    34,     0,     0,     0,    81,
       0,   136,   135,   134,   133,    36,    49,    14,    46,    44,
      15,    42,     0,     0,    52,    47,    45,    41,    43,    92,
      50,    51,     0,    55,     0,     0,    53,    54,     0,    58,
       0,     0,   110,    98,    56,    57,     0,    48,    64,    94,
      95,    96,     0,   101,   102,   103,   104,   105,   106,   108,
     109,   107,     0,    88,    99,    97,    60,    61,    62,    63,
      65,    59,     0,     0,    91,    93,     0,   100
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -97,   -97,   118,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,    81,    43,    13,    66,   -97,   -97,   -97,   -19,
     -17,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,     0,   -97,   -97,   -97,   -97,   -97,   -97,    69,    18,
      79,   -97,   -97,   -96,   -97,   -97,   -97,   -97,   -35,   -97,
     -97,   -97,   -33,   -97,   -97,   -97,   -97,   -97,   -97,   -97,
     -97,   -97,   -97,    20,   -97,   -97,   -97,   -97
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    13,    46,    47,    65,    48,
      67,    91,    63,   119,   137,    88,    49,   150,   157,   151,
     147,   156,   148,   154,   163,   169,   177,   178,   201,    14,
      24,    25,    53,    26,    51,   123,    71,    96,   106,    74,
      75,   101,    76,    99,    15,    16,   165,   182,   172,   195,
     173,   192,   107,    17,    18,    31,    59,    82,    19,    33,
     113,    84,    85,   115,   134,   116,   144,    20
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     126,     1,   141,   142,    72,    29,   108,   109,   110,   111,
      21,   130,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     2,    22,   179,   180,   181,    52,    23,     3,   160,
      27,   161,     4,    28,     5,    38,    39,    40,    41,    42,
      43,    44,    45,    22,   196,   197,   198,   199,   121,   122,
      30,    73,   166,   167,   174,   175,    32,     6,    34,    35,
      37,   143,    50,    54,     7,    55,    56,    58,    61,    60,
      57,    62,    69,    64,    66,     8,    70,    78,    77,    80,
      81,    83,    87,    79,    86,    89,    90,    93,    95,    94,
     102,    73,   103,   104,   112,   117,   118,   204,    98,   105,
     114,   125,   124,   127,   128,   129,   131,   132,   136,   135,
     162,   140,   159,   168,   164,   171,   146,   145,   149,   170,
     176,   152,   200,   205,   202,   153,   203,   206,   207,    36,
      68,   155,   158,   138,    92,   120,   133,     0,   194,   193,
      97,     0,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100
};

static const yytype_int16 yycheck[] =
{
      96,     3,    17,    18,    32,     5,    71,    72,    73,    74,
      79,   107,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    23,    79,    43,    44,    45,    26,    84,    30,    79,
      59,    81,    34,    79,    36,     4,     5,     6,     7,     8,
       9,    10,    11,    79,    71,    72,    73,    74,    26,    27,
      60,    79,    17,    18,    68,    69,    67,    59,    79,     0,
      82,    76,    24,    31,    66,    35,    37,    61,    12,    68,
      37,    20,    79,    16,    13,    77,    28,    81,    79,    62,
      64,    70,    19,    81,    81,    81,    14,    25,    30,    29,
      38,    79,    38,    40,    75,    81,    21,   193,    33,    65,
      79,    31,    79,    39,    63,    79,    79,    15,    22,    81,
      15,    40,    38,    67,    41,    48,    79,    81,    79,    42,
      70,    81,    75,    47,    46,    83,    58,    31,    79,    11,
      49,   148,   151,   120,    68,    92,   116,    -1,   173,   172,
      71,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    23,    30,    34,    36,    59,    66,    77,    86,
      87,    88,    89,    90,   114,   129,   130,   138,   139,   143,
     152,    79,    79,    84,   115,   116,   118,    59,    79,   116,
      60,   140,    67,   144,    79,     0,    87,    82,     4,     5,
       6,     7,     8,     9,    10,    11,    91,    92,    94,   101,
      24,   119,   116,   117,    31,    35,    37,    37,    61,   141,
      68,    12,    20,    97,    16,    93,    13,    95,    97,    79,
      28,   121,    32,    79,   124,   125,   127,    79,    81,    81,
      62,    64,   142,    70,   146,   147,    81,    19,   100,    81,
      14,    96,   100,    25,    29,    30,   122,   123,    33,   128,
     125,   126,    38,    38,    40,    65,   123,   137,    71,    72,
      73,    74,    75,   145,    79,   148,   150,    81,    21,    98,
      98,    26,    27,   120,    79,    31,   128,    39,    63,    79,
     128,    79,    15,   148,   149,    81,    22,    99,    99,   124,
      40,    17,    18,    76,   151,    81,    79,   105,   107,    79,
     102,   104,    81,    83,   108,   105,   106,   103,   104,    38,
      79,    81,    15,   109,    41,   131,    17,    18,    67,   110,
      42,    48,   133,   135,    68,    69,    70,   111,   112,    43,
      44,    45,   132,    49,    50,    51,    52,    53,    54,    55,
      56,    57,   136,   137,   133,   134,    71,    72,    73,    74,
      75,   113,    46,    58,   128,    47,    31,    79
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    87,    87,    88,    89,    89,    89,    89,
      89,    89,    89,    89,    90,    90,    91,    91,    92,    92,
      92,    92,    93,    93,    94,    94,    94,    95,    95,    96,
      96,    97,    97,    98,    98,    99,    99,   100,   100,   101,
     101,   102,   103,   103,   104,   105,   106,   106,   107,   108,
     108,   108,   109,   109,   109,   110,   110,   110,   111,   111,
     112,   112,   112,   112,   113,   113,   114,   115,   115,   116,
     117,   117,   118,   119,   119,   120,   120,   121,   121,   122,
     122,   123,   124,   124,   125,   126,   126,   127,   128,   128,
     129,   130,   131,   131,   132,   132,   132,   133,   134,   134,
     135,   136,   136,   136,   136,   136,   136,   136,   136,   136,
     137,   137,   138,   139,   140,   140,   141,   141,   142,   142,
     143,   144,   145,   145,   146,   146,   146,   146,   147,   147,
     148,   149,   149,   150,   150,   151,   151,   152
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     8,     8,     2,     3,     1,     1,
       1,     1,     0,     1,     1,     1,     1,     0,     2,     0,
       1,     0,     2,     0,     2,     0,     2,     0,     2,     2,
       1,     2,     0,     1,     1,     2,     0,     1,     5,     0,
       2,     2,     0,     2,     2,     0,     2,     2,     0,     2,
       2,     2,     2,     2,     0,     1,     6,     1,     1,     2,
       0,     1,     1,     0,     4,     1,     1,     0,     3,     0,
       1,     3,     1,     1,     2,     0,     1,     1,     0,     1,
       4,    13,     0,     5,     1,     1,     1,     2,     0,     1,
       5,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     5,     6,     0,     4,     0,     4,     0,     3,
       2,     4,     0,     2,     2,     2,     2,     2,     0,     2,
       2,     0,     1,     3,     3,     1,     1,     2
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
        yyerror (&yylloc, yyscanner, rtr, ralloc, palloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if ROUTER_YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined ROUTER_YYLTYPE_IS_TRIVIAL && ROUTER_YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location, yyscanner, rtr, ralloc, palloc); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (rtr);
  YYUSE (ralloc);
  YYUSE (palloc);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yytype, yyvaluep, yylocationp, yyscanner, rtr, ralloc, palloc);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       , yyscanner, rtr, ralloc, palloc);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, yyscanner, rtr, ralloc, palloc); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !ROUTER_YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !ROUTER_YYDEBUG */


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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
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
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

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
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
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
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
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
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
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
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (yyscanner);
  YYUSE (rtr);
  YYUSE (ralloc);
  YYUSE (palloc);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *yyscanner, router *rtr, allocator *ralloc, allocator *palloc)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined ROUTER_YYLTYPE_IS_TRIVIAL && ROUTER_YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
      yychar = yylex (&yylval, &yylloc, yyscanner, rtr, ralloc, palloc);
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 14:
#line 156 "conffile.y"
           {
	   	struct _clhost *w;
		char *err;
		int srvcnt;
		int replcnt;

		/* count number of servers for ch_new */
		for (srvcnt = 0, w = (yyvsp[0].cluster_hosts); w != NULL; w = w->next, srvcnt++)
			;

		if (((yyval.cluster) = cluster_new((yyvsp[-6].crSTRING), ralloc, (yyvsp[-5].cluster_type).t, NULL, router_queue_size(rtr), cluster_ttl((yyvsp[-3].ttl)))) == NULL) {
			logerr("malloc failed for cluster '%s'\n", (yyvsp[-6].crSTRING));
			YYABORT;
		}
		if (cluster_set_threshold((yyval.cluster), (yyvsp[-2].threshold_start), (yyvsp[-1].threshold_end)) == -1) {
			YYABORT;
		}
		switch ((yyval.cluster)->type) {
			case CARBON_CH:
			case FNV1A_CH:
			case JUMP_CH:
				(yyval.cluster)->members.ch = ra_malloc(ralloc, sizeof(chashring));
				if ((yyval.cluster)->members.ch == NULL) {
					logerr("malloc failed for ch in cluster '%s'\n", (yyvsp[-6].crSTRING));
					YYABORT;
				}
				replcnt = (yyvsp[-5].cluster_type).ival / 10;
				(yyval.cluster)->isdynamic = (yyvsp[-5].cluster_type).ival - (replcnt * 10) == 2;
				if (replcnt < 1 || replcnt > 255) {
					router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
						"replication count must be between 1 and 255");
					YYERROR;
				}
				(yyval.cluster)->members.ch->repl_factor = (unsigned char)replcnt;
				(yyval.cluster)->members.ch->ring = ch_new(ralloc,
					(yyval.cluster)->type == CARBON_CH ? CARBON :
					(yyval.cluster)->type == FNV1A_CH ? FNV1a :
					JUMP_FNV1a, srvcnt);
				(yyval.cluster)->members.ch->servers = NULL;
				(yyvsp[-5].cluster_type).ival = 0;  /* hack, avoid triggering use_all */
				break;
			case FORWARD:
				(yyval.cluster)->members.forward = NULL;
				break;
			case ANYOF:
			case FAILOVER:
			case LB:
				(yyval.cluster)->members.anyof = NULL;
				break;
			default:
				logerr("unknown cluster type %zd!\n", (ssize_t)(yyval.cluster)->type);
				YYABORT;
		}

		(yyval.cluster)->server_connections = (yyvsp[-4].connections);
		
		for (w = (yyvsp[0].cluster_hosts); w != NULL; w = w->next) {
			err = router_add_server(rtr, w->ip, w->port, w->inst,
					w->type, w->trnsp, w->proto,
					w->saddr, w->hint, (char)(yyvsp[-5].cluster_type).ival, (yyval.cluster));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}
		}

		err = router_add_cluster(rtr, (yyval.cluster));
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
	   }
#line 1952 "conffile.tab.c"
    break;

  case 15:
#line 229 "conffile.y"
           {
	   	struct _clhost *w;
		char *err;

		if (((yyval.cluster) = cluster_new((yyvsp[-6].crSTRING), ralloc, (yyvsp[-5].cluster_file).t, NULL, router_queue_size(rtr), cluster_ttl((yyvsp[-3].ttl)))) == NULL) {
			logerr("malloc failed for cluster '%s'\n", (yyvsp[-6].crSTRING));
			YYABORT;
		}
		if (cluster_set_threshold((yyval.cluster), (yyvsp[-2].threshold_start), (yyvsp[-1].threshold_end)) == -1) {
			YYABORT;
		}
		switch ((yyval.cluster)->type) {
			case FILELOG:
			case FILELOGIP:
				(yyval.cluster)->members.forward = NULL;
				break;
			default:
				logerr("unknown cluster type %zd!\n", (ssize_t)(yyval.cluster)->type);
				YYABORT;
		}

		(yyval.cluster)->server_connections = (yyvsp[-4].connections);
		
		for (w = (yyvsp[0].cluster_paths); w != NULL; w = w->next) {
			err = router_add_server(rtr, w->ip, w->port, w->inst,
					T_LINEMODE, W_PLAIN, w->proto,
					w->saddr, w->hint, (char)(yyvsp[-5].cluster_file).ival, (yyval.cluster));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}
		}

		err = router_add_cluster(rtr, (yyval.cluster));
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
	   }
#line 1996 "conffile.tab.c"
    break;

  case 16:
#line 272 "conffile.y"
                          { (yyval.cluster_type).t = (yyvsp[-1].cluster_useall); (yyval.cluster_type).ival = (yyvsp[0].cluster_opt_useall); }
#line 2002 "conffile.tab.c"
    break;

  case 17:
#line 274 "conffile.y"
                          { (yyval.cluster_type).t = (yyvsp[-2].cluster_ch); (yyval.cluster_type).ival = ((yyvsp[0].cluster_opt_dynamic) * 2) + ((yyvsp[-1].cluster_opt_repl) * 10); }
#line 2008 "conffile.tab.c"
    break;

  case 18:
#line 277 "conffile.y"
                           { (yyval.cluster_useall) = FORWARD; }
#line 2014 "conffile.tab.c"
    break;

  case 19:
#line 278 "conffile.y"
                                       { (yyval.cluster_useall) = ANYOF; }
#line 2020 "conffile.tab.c"
    break;

  case 20:
#line 279 "conffile.y"
                                       { (yyval.cluster_useall) = FAILOVER; }
#line 2026 "conffile.tab.c"
    break;

  case 21:
#line 280 "conffile.y"
                                 { (yyval.cluster_useall) = LB; }
#line 2032 "conffile.tab.c"
    break;

  case 22:
#line 283 "conffile.y"
                             { (yyval.cluster_opt_useall) = 0; }
#line 2038 "conffile.tab.c"
    break;

  case 23:
#line 284 "conffile.y"
                                             { (yyval.cluster_opt_useall) = 1; }
#line 2044 "conffile.tab.c"
    break;

  case 24:
#line 287 "conffile.y"
                            { (yyval.cluster_ch) = CARBON_CH; }
#line 2050 "conffile.tab.c"
    break;

  case 25:
#line 288 "conffile.y"
                                    { (yyval.cluster_ch) = FNV1A_CH; }
#line 2056 "conffile.tab.c"
    break;

  case 26:
#line 289 "conffile.y"
                                    { (yyval.cluster_ch) = JUMP_CH; }
#line 2062 "conffile.tab.c"
    break;

  case 27:
#line 292 "conffile.y"
                                              { (yyval.cluster_opt_repl) = 1; }
#line 2068 "conffile.tab.c"
    break;

  case 28:
#line 293 "conffile.y"
                                                              { (yyval.cluster_opt_repl) = (yyvsp[0].crINTVAL); }
#line 2074 "conffile.tab.c"
    break;

  case 29:
#line 296 "conffile.y"
                               { (yyval.cluster_opt_dynamic) = 0; }
#line 2080 "conffile.tab.c"
    break;

  case 30:
#line 297 "conffile.y"
                                               { (yyval.cluster_opt_dynamic) = 1; }
#line 2086 "conffile.tab.c"
    break;

  case 31:
#line 300 "conffile.y"
                           { (yyval.connections) = 1; }
#line 2092 "conffile.tab.c"
    break;

  case 32:
#line 301 "conffile.y"
                                                               { (yyval.connections) = (yyvsp[0].crINTVAL); }
#line 2098 "conffile.tab.c"
    break;

  case 33:
#line 304 "conffile.y"
                                  { (yyval.threshold_start) = 0; }
#line 2104 "conffile.tab.c"
    break;

  case 34:
#line 305 "conffile.y"
                                                                   { (yyval.threshold_start) = (yyvsp[0].crINTVAL); }
#line 2110 "conffile.tab.c"
    break;

  case 35:
#line 308 "conffile.y"
                                { (yyval.threshold_end) = 0; }
#line 2116 "conffile.tab.c"
    break;

  case 36:
#line 309 "conffile.y"
                                                                 { (yyval.threshold_end) = (yyvsp[0].crINTVAL); }
#line 2122 "conffile.tab.c"
    break;

  case 37:
#line 312 "conffile.y"
                                { (yyval.ttl) = 0; }
#line 2128 "conffile.tab.c"
    break;

  case 38:
#line 313 "conffile.y"
                                                       { (yyval.ttl) = (yyvsp[0].crINTVAL); }
#line 2134 "conffile.tab.c"
    break;

  case 39:
#line 316 "conffile.y"
                          { (yyval.cluster_file).t = FILELOGIP; (yyval.cluster_file).ival = 0; }
#line 2140 "conffile.tab.c"
    break;

  case 40:
#line 317 "conffile.y"
                                      { (yyval.cluster_file).t = FILELOG; (yyval.cluster_file).ival = 0; }
#line 2146 "conffile.tab.c"
    break;

  case 41:
#line 320 "conffile.y"
                                                   { (yyvsp[-1].cluster_path)->next = (yyvsp[0].cluster_opt_path); (yyval.cluster_paths) = (yyvsp[-1].cluster_path); }
#line 2152 "conffile.tab.c"
    break;

  case 42:
#line 322 "conffile.y"
                               { (yyval.cluster_opt_path) = NULL; }
#line 2158 "conffile.tab.c"
    break;

  case 43:
#line 323 "conffile.y"
                                               { (yyval.cluster_opt_path) = (yyvsp[0].cluster_path); }
#line 2164 "conffile.tab.c"
    break;

  case 44:
#line 326 "conffile.y"
                        {
				struct _clhost *ret = ra_malloc(palloc, sizeof(struct _clhost));
				char *err = router_validate_path(rtr, (yyvsp[0].crSTRING));
				if (err != NULL) {
					router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc, err);
					YYERROR;
				}
				ret->ip = (yyvsp[0].crSTRING);
				ret->port = GRAPHITE_PORT;
				ret->saddr = NULL;
				ret->hint = NULL;
				ret->inst = NULL;
				ret->proto = CON_FILE;
				ret->next = NULL;
				(yyval.cluster_path) = ret;
			}
#line 2186 "conffile.tab.c"
    break;

  case 45:
#line 345 "conffile.y"
                                                   { (yyvsp[-1].cluster_host)->next = (yyvsp[0].cluster_opt_host); (yyval.cluster_hosts) = (yyvsp[-1].cluster_host); }
#line 2192 "conffile.tab.c"
    break;

  case 46:
#line 347 "conffile.y"
                                { (yyval.cluster_opt_host) = NULL; }
#line 2198 "conffile.tab.c"
    break;

  case 47:
#line 348 "conffile.y"
                                                { (yyval.cluster_opt_host) = (yyvsp[0].cluster_hosts); }
#line 2204 "conffile.tab.c"
    break;

  case 48:
#line 353 "conffile.y"
                        {
			  	struct _clhost *ret = ra_malloc(palloc, sizeof(struct _clhost));
				char *err = router_validate_address(
						rtr,
						&(ret->ip), &(ret->port), &(ret->saddr), &(ret->hint),
						(yyvsp[-4].crSTRING), (yyvsp[-2].cluster_opt_proto));
				if (err != NULL) {
					router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc, err);
					YYERROR;
				}
				ret->inst = (yyvsp[-3].cluster_opt_instance);
				ret->proto = (yyvsp[-2].cluster_opt_proto);
				ret->type = (yyvsp[-1].cluster_opt_type);
				ret->trnsp = (yyvsp[0].cluster_opt_transport);
				ret->next = NULL;
				(yyval.cluster_host) = ret;
			  }
#line 2227 "conffile.tab.c"
    break;

  case 49:
#line 372 "conffile.y"
                                         { (yyval.cluster_opt_instance) = NULL; }
#line 2233 "conffile.tab.c"
    break;

  case 50:
#line 373 "conffile.y"
                                                             { (yyval.cluster_opt_instance) = (yyvsp[0].crSTRING); }
#line 2239 "conffile.tab.c"
    break;

  case 51:
#line 375 "conffile.y"
                                        {
						(yyval.cluster_opt_instance) = ra_malloc(palloc, sizeof(char) * 12);
						if ((yyval.cluster_opt_instance) == NULL) {
							logerr("out of memory\n");
							YYABORT;
						}
						snprintf((yyval.cluster_opt_instance), 12, "%d", (yyvsp[0].crINTVAL));
					}
#line 2252 "conffile.tab.c"
    break;

  case 52:
#line 384 "conffile.y"
                                 { (yyval.cluster_opt_proto) = CON_TCP; }
#line 2258 "conffile.tab.c"
    break;

  case 53:
#line 385 "conffile.y"
                                                 { (yyval.cluster_opt_proto) = CON_UDP; }
#line 2264 "conffile.tab.c"
    break;

  case 54:
#line 386 "conffile.y"
                                                 { (yyval.cluster_opt_proto) = CON_TCP; }
#line 2270 "conffile.tab.c"
    break;

  case 55:
#line 389 "conffile.y"
                                      { (yyval.cluster_opt_type) = T_LINEMODE; }
#line 2276 "conffile.tab.c"
    break;

  case 56:
#line 390 "conffile.y"
                                                      { (yyval.cluster_opt_type) = T_LINEMODE; }
#line 2282 "conffile.tab.c"
    break;

  case 57:
#line 391 "conffile.y"
                                                      { (yyval.cluster_opt_type) = T_SYSLOGMODE; }
#line 2288 "conffile.tab.c"
    break;

  case 58:
#line 394 "conffile.y"
                                      { (yyval.cluster_opt_transport) = W_PLAIN; }
#line 2294 "conffile.tab.c"
    break;

  case 59:
#line 397 "conffile.y"
                                         {
					 	if ((yyvsp[0].cluster_transport_opt_ssl) == W_PLAIN) {
							(yyval.cluster_opt_transport) = (yyvsp[-1].cluster_transport_trans);
						} else {
							(yyval.cluster_opt_transport) = (yyvsp[-1].cluster_transport_trans) | (yyvsp[0].cluster_transport_opt_ssl);
						}
					 }
#line 2306 "conffile.tab.c"
    break;

  case 60:
#line 405 "conffile.y"
                                              { (yyval.cluster_transport_trans) = W_PLAIN; }
#line 2312 "conffile.tab.c"
    break;

  case 61:
#line 406 "conffile.y"
                                                                  {
#ifdef HAVE_GZIP
							(yyval.cluster_transport_trans) = W_GZIP;
#else
							router_yyerror(&yylloc, yyscanner, rtr,
								ralloc, palloc,
								"feature gzip not compiled in");
							YYERROR;
#endif
					    }
#line 2327 "conffile.tab.c"
    break;

  case 62:
#line 416 "conffile.y"
                                                                   {
#ifdef HAVE_LZ4
							(yyval.cluster_transport_trans) = W_LZ4;
#else
							router_yyerror(&yylloc, yyscanner, rtr,
								ralloc, palloc,
								"feature lz4 not compiled in");
							YYERROR;
#endif
					    }
#line 2342 "conffile.tab.c"
    break;

  case 63:
#line 426 "conffile.y"
                                                                      {
#ifdef HAVE_SNAPPY
							(yyval.cluster_transport_trans) = W_SNAPPY;
#else
							router_yyerror(&yylloc, yyscanner, rtr,
								ralloc, palloc,
								"feature snappy not compiled in");
							YYERROR;
#endif
					    }
#line 2357 "conffile.tab.c"
    break;

  case 64:
#line 437 "conffile.y"
                           { (yyval.cluster_transport_opt_ssl) = W_PLAIN; }
#line 2363 "conffile.tab.c"
    break;

  case 65:
#line 439 "conffile.y"
                                                 {
#ifdef HAVE_SSL
							(yyval.cluster_transport_opt_ssl) = W_SSL;
#else
							router_yyerror(&yylloc, yyscanner, rtr,
								ralloc, palloc,
								"feature ssl not compiled in");
							YYERROR;
#endif
					     }
#line 2378 "conffile.tab.c"
    break;

  case 66:
#line 455 "conffile.y"
         {
	 	/* each expr comes with an allocated route, populate it */
		struct _maexpr *we;
		destinations *d = NULL;
		char *err;

		if ((yyvsp[-3].match_opt_validate) != NULL) {
			/* optional validate clause */
			if ((d = ra_malloc(ralloc, sizeof(destinations))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			d->next = NULL;
			if ((d->cl = cluster_new(NULL, ralloc, VALIDATION, NULL, 0, 0)) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			d->cl->members.validation = ra_malloc(ralloc, sizeof(validate));
			if (d->cl->members.validation == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			d->cl->members.validation->rule = (yyvsp[-3].match_opt_validate)->r;
			d->cl->members.validation->action = (yyvsp[-3].match_opt_validate)->drop ? VAL_DROP : VAL_LOG;
		}
		/* add destinations to the chain */
		if (d != NULL) {
			d->next = (yyvsp[-1].match_opt_send_to);
		} else {
			d = (yyvsp[-1].match_opt_send_to);
		}
		/* replace with copy on the router allocator */
		if ((yyvsp[-2].match_opt_route) != NULL)
			(yyvsp[-2].match_opt_route) = ra_strdup(ralloc, (yyvsp[-2].match_opt_route));
		for (we = (yyvsp[-4].match_exprs); we != NULL; we = we->next) {
			we->r->next = NULL;
			we->r->dests = d;
			we->r->masq = (yyvsp[-2].match_opt_route);
			we->r->stop = (yyvsp[-1].match_opt_send_to) == NULL ? 0 :
					(yyvsp[-1].match_opt_send_to)->cl->type == BLACKHOLE ? 1 : (yyvsp[0].match_opt_stop);
			err = router_add_route(rtr, we->r);
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc, err);
				YYERROR;
			}
		}
	 }
#line 2431 "conffile.tab.c"
    break;

  case 67:
#line 506 "conffile.y"
                   {
			if (((yyval.match_exprs) = ra_malloc(palloc, sizeof(struct _maexpr))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
		   	(yyval.match_exprs)->r = NULL;
			if (router_validate_expression(rtr, &((yyval.match_exprs)->r), "*") != NULL)
				YYABORT;
			(yyval.match_exprs)->drop = 0;
			(yyval.match_exprs)->next = NULL;
		   }
#line 2447 "conffile.tab.c"
    break;

  case 68:
#line 517 "conffile.y"
                                  { (yyval.match_exprs) = (yyvsp[0].match_exprs2); }
#line 2453 "conffile.tab.c"
    break;

  case 69:
#line 520 "conffile.y"
                                              { (yyvsp[-1].match_expr)->next = (yyvsp[0].match_opt_expr); (yyval.match_exprs2) = (yyvsp[-1].match_expr); }
#line 2459 "conffile.tab.c"
    break;

  case 70:
#line 522 "conffile.y"
                             { (yyval.match_opt_expr) = NULL; }
#line 2465 "conffile.tab.c"
    break;

  case 71:
#line 523 "conffile.y"
                                         { (yyval.match_opt_expr) = (yyvsp[0].match_exprs2); }
#line 2471 "conffile.tab.c"
    break;

  case 72:
#line 527 "conffile.y"
                  {
			char *err;
			if (((yyval.match_expr) = ra_malloc(palloc, sizeof(struct _maexpr))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
		   	(yyval.match_expr)->r = NULL;
		  	err = router_validate_expression(rtr, &((yyval.match_expr)->r), (yyvsp[0].crSTRING));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc, err);
				YYERROR;
			}
			(yyval.match_expr)->drop = 0;
			(yyval.match_expr)->next = NULL;
		  }
#line 2492 "conffile.tab.c"
    break;

  case 73:
#line 545 "conffile.y"
                    { (yyval.match_opt_validate) = NULL; }
#line 2498 "conffile.tab.c"
    break;

  case 74:
#line 547 "conffile.y"
                                  {
					char *err;
					if (((yyval.match_opt_validate) = ra_malloc(palloc, sizeof(struct _maexpr))) == NULL) {
						logerr("out of memory\n");
						YYABORT;
					}
					(yyval.match_opt_validate)->r = NULL;
					err = router_validate_expression(rtr, &((yyval.match_opt_validate)->r), (yyvsp[-2].crSTRING));
					if (err != NULL) {
						router_yyerror(&yylloc, yyscanner, rtr,
								ralloc, palloc, err);
						YYERROR;
					}
					(yyval.match_opt_validate)->drop = (yyvsp[0].match_log_or_drop);
					(yyval.match_opt_validate)->next = NULL;
				  }
#line 2519 "conffile.tab.c"
    break;

  case 75:
#line 565 "conffile.y"
                          { (yyval.match_log_or_drop) = 0; }
#line 2525 "conffile.tab.c"
    break;

  case 76:
#line 566 "conffile.y"
                                          { (yyval.match_log_or_drop) = 1; }
#line 2531 "conffile.tab.c"
    break;

  case 77:
#line 569 "conffile.y"
                 { (yyval.match_opt_route) = NULL; }
#line 2537 "conffile.tab.c"
    break;

  case 78:
#line 570 "conffile.y"
                                                            { (yyval.match_opt_route) = (yyvsp[0].crSTRING); }
#line 2543 "conffile.tab.c"
    break;

  case 79:
#line 573 "conffile.y"
                   { (yyval.match_opt_send_to) = NULL; }
#line 2549 "conffile.tab.c"
    break;

  case 80:
#line 574 "conffile.y"
                                                 { (yyval.match_opt_send_to) = (yyvsp[0].match_send_to); }
#line 2555 "conffile.tab.c"
    break;

  case 81:
#line 577 "conffile.y"
                                            { (yyval.match_send_to) = (yyvsp[0].match_dsts); }
#line 2561 "conffile.tab.c"
    break;

  case 82:
#line 581 "conffile.y"
                  {
			if (((yyval.match_dsts) = ra_malloc(ralloc, sizeof(destinations))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			if (router_validate_cluster(rtr, &((yyval.match_dsts)->cl), "blackhole") != NULL)
				YYABORT;
			(yyval.match_dsts)->next = NULL;
		  }
#line 2575 "conffile.tab.c"
    break;

  case 83:
#line 590 "conffile.y"
                                { (yyval.match_dsts) = (yyvsp[0].match_dsts2); }
#line 2581 "conffile.tab.c"
    break;

  case 84:
#line 593 "conffile.y"
                                           { (yyvsp[-1].match_dst)->next = (yyvsp[0].match_opt_dst); (yyval.match_dsts2) = (yyvsp[-1].match_dst); }
#line 2587 "conffile.tab.c"
    break;

  case 85:
#line 595 "conffile.y"
                           { (yyval.match_opt_dst) = NULL; }
#line 2593 "conffile.tab.c"
    break;

  case 86:
#line 596 "conffile.y"
                                       { (yyval.match_opt_dst) = (yyvsp[0].match_dsts2); }
#line 2599 "conffile.tab.c"
    break;

  case 87:
#line 600 "conffile.y"
                 {
			char *err;
			if (((yyval.match_dst) = ra_malloc(ralloc, sizeof(destinations))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			err = router_validate_cluster(rtr, &((yyval.match_dst)->cl), (yyvsp[0].crSTRING));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}
			(yyval.match_dst)->next = NULL;
		 }
#line 2617 "conffile.tab.c"
    break;

  case 88:
#line 615 "conffile.y"
                       { (yyval.match_opt_stop) = 0; }
#line 2623 "conffile.tab.c"
    break;

  case 89:
#line 616 "conffile.y"
                                   { (yyval.match_opt_stop) = 1; }
#line 2629 "conffile.tab.c"
    break;

  case 90:
#line 622 "conffile.y"
           {
		char *err;
		route *r = NULL;
		cluster *cl;

		err = router_validate_expression(rtr, &r, (yyvsp[-2].crSTRING));
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
		
		cl = cluster_new(NULL, ralloc, REWRITE, NULL, 0, 0);
		if (cl == NULL) {
			logerr("out of memory\n");
			YYABORT;
		}
		cl->members.replacement = ra_strdup(ralloc, (yyvsp[0].crSTRING));
		if (cl->members.replacement == NULL) {
			logerr("out of memory\n");
			YYABORT;
		}
		r->dests = ra_malloc(ralloc, sizeof(destinations));
		if (r->dests == NULL) {
			logerr("out of memory\n");
			YYABORT;
		}
		r->dests->cl = cl;
		r->dests->next = NULL;
		r->stop = 0;
		r->next = NULL;

		err = router_add_route(rtr, r);
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
	   }
#line 2671 "conffile.tab.c"
    break;

  case 91:
#line 669 "conffile.y"
                 {
		 	cluster *w;
			aggregator *a;
			destinations *d;
			struct _agcomp *acw;
			struct _maexpr *we;
			char *err;

			if ((yyvsp[-9].crINTVAL) <= 0) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
					"interval must be > 0");
				YYERROR;
			}
			if ((yyvsp[-5].crINTVAL) <= 0) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
					"expire must be > 0");
				YYERROR;
			}
			if ((yyvsp[-5].crINTVAL) <= (yyvsp[-9].crINTVAL)) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
					"expire must be greater than interval");
				YYERROR;
			}

			w = cluster_new(NULL, ralloc, AGGREGATION, NULL, 0, 0);
			if (w == NULL) {
				logerr("malloc failed for aggregate\n");
				YYABORT;
			}

			a = aggregator_new((yyvsp[-9].crINTVAL), (yyvsp[-5].crINTVAL), (yyvsp[-3].aggregate_opt_timestamp));
			if (a == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			if ((err = router_add_aggregator(rtr, a)) != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}

			w->members.aggregation = a;
			
			for (acw = (yyvsp[-2].aggregate_computes); acw != NULL; acw = acw->next) {
				if (aggregator_add_compute(a,
							acw->metric, acw->ctype, acw->pctl) != 0)
				{
					logerr("out of memory\n");
					YYABORT;
				}
			}

			err = router_add_cluster(rtr, w);
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}

			d = ra_malloc(ralloc, sizeof(destinations));
			if (d == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			d->cl = w;
			d->next = (yyvsp[-1].aggregate_opt_send_to);

			for (we = (yyvsp[-11].match_exprs2); we != NULL; we = we->next) {
				we->r->next = NULL;
				we->r->dests = d;
				we->r->stop = (yyvsp[0].match_opt_stop);
				err = router_add_route(rtr, we->r);
				if (err != NULL) {
					router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc, err);
					YYERROR;
				}
			}

			if ((yyvsp[-1].aggregate_opt_send_to) != NULL)
				router_add_stubroute(rtr, AGGRSTUB, w, (yyvsp[-1].aggregate_opt_send_to));
		 }
#line 2756 "conffile.tab.c"
    break;

  case 92:
#line 751 "conffile.y"
                         { (yyval.aggregate_opt_timestamp) = TS_END; }
#line 2762 "conffile.tab.c"
    break;

  case 93:
#line 754 "conffile.y"
                                           { (yyval.aggregate_opt_timestamp) = (yyvsp[-2].aggregate_ts_when); }
#line 2768 "conffile.tab.c"
    break;

  case 94:
#line 757 "conffile.y"
                            { (yyval.aggregate_ts_when) = TS_START; }
#line 2774 "conffile.tab.c"
    break;

  case 95:
#line 758 "conffile.y"
                                            { (yyval.aggregate_ts_when) = TS_MIDDLE; }
#line 2780 "conffile.tab.c"
    break;

  case 96:
#line 759 "conffile.y"
                                            { (yyval.aggregate_ts_when) = TS_END; }
#line 2786 "conffile.tab.c"
    break;

  case 97:
#line 763 "conffile.y"
                                  { (yyvsp[-1].aggregate_compute)->next = (yyvsp[0].aggregate_opt_compute); (yyval.aggregate_computes) = (yyvsp[-1].aggregate_compute); }
#line 2792 "conffile.tab.c"
    break;

  case 98:
#line 766 "conffile.y"
                                          { (yyval.aggregate_opt_compute) = NULL; }
#line 2798 "conffile.tab.c"
    break;

  case 99:
#line 767 "conffile.y"
                                                              { (yyval.aggregate_opt_compute) = (yyvsp[0].aggregate_computes); }
#line 2804 "conffile.tab.c"
    break;

  case 100:
#line 771 "conffile.y"
                                 {
					(yyval.aggregate_compute) = ra_malloc(palloc, sizeof(struct _agcomp));
					if ((yyval.aggregate_compute) == NULL) {
						logerr("malloc failed\n");
						YYABORT;
					}
				 	(yyval.aggregate_compute)->ctype = (yyvsp[-3].aggregate_comp_type).ctype;
					(yyval.aggregate_compute)->pctl = (yyvsp[-3].aggregate_comp_type).pctl;
					(yyval.aggregate_compute)->metric = (yyvsp[0].crSTRING);
					(yyval.aggregate_compute)->next = NULL;
				 }
#line 2820 "conffile.tab.c"
    break;

  case 101:
#line 784 "conffile.y"
                                  { (yyval.aggregate_comp_type).ctype = SUM; }
#line 2826 "conffile.tab.c"
    break;

  case 102:
#line 785 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = CNT; }
#line 2832 "conffile.tab.c"
    break;

  case 103:
#line 786 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MAX; }
#line 2838 "conffile.tab.c"
    break;

  case 104:
#line 787 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MIN; }
#line 2844 "conffile.tab.c"
    break;

  case 105:
#line 788 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = AVG; }
#line 2850 "conffile.tab.c"
    break;

  case 106:
#line 789 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MEDN; }
#line 2856 "conffile.tab.c"
    break;

  case 107:
#line 791 "conffile.y"
                                   {
				    if ((yyvsp[0].crPERCENTILE) < 1 || (yyvsp[0].crPERCENTILE) > 99) {
						router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
							"percentile<x>: value x must be between 1 and 99");
						YYERROR;
					}
				   	(yyval.aggregate_comp_type).ctype = PCTL;
					(yyval.aggregate_comp_type).pctl = (unsigned char)(yyvsp[0].crPERCENTILE);
				   }
#line 2870 "conffile.tab.c"
    break;

  case 108:
#line 800 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = VAR; }
#line 2876 "conffile.tab.c"
    break;

  case 109:
#line 801 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = SDEV; }
#line 2882 "conffile.tab.c"
    break;

  case 110:
#line 804 "conffile.y"
                                     { (yyval.aggregate_opt_send_to) = NULL; }
#line 2888 "conffile.tab.c"
    break;

  case 111:
#line 805 "conffile.y"
                                                         { (yyval.aggregate_opt_send_to) = (yyvsp[0].match_send_to); }
#line 2894 "conffile.tab.c"
    break;

  case 112:
#line 811 "conffile.y"
        {
		char *err = router_set_statistics(rtr, (yyvsp[-1].match_dsts));
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
		logerr("warning: 'send statistics to ...' is deprecated and will be "
				"removed in a future version, use 'statistics send to ...' "
				"instead\n");
	}
#line 2909 "conffile.tab.c"
    break;

  case 113:
#line 831 "conffile.y"
                  {
		  	char *err;
		  	err = router_set_collectorvals(rtr, (yyvsp[-4].statistics_opt_interval), (yyvsp[-2].statistics_opt_prefix), (yyvsp[-3].statistics_opt_counters));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
				YYERROR;
			}

			if ((yyvsp[-1].aggregate_opt_send_to) != NULL) {
				err = router_set_statistics(rtr, (yyvsp[-1].aggregate_opt_send_to));
				if (err != NULL) {
					router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc, err);
					YYERROR;
				}
			}
		  }
#line 2931 "conffile.tab.c"
    break;

  case 114:
#line 850 "conffile.y"
                         { (yyval.statistics_opt_interval) = -1; }
#line 2937 "conffile.tab.c"
    break;

  case 115:
#line 852 "conffile.y"
                                           {
					   	if ((yyvsp[-1].crINTVAL) <= 0) {
							router_yyerror(&yylloc, yyscanner, rtr,
									ralloc, palloc, "interval must be > 0");
							YYERROR;
						}
						(yyval.statistics_opt_interval) = (yyvsp[-1].crINTVAL);
					   }
#line 2950 "conffile.tab.c"
    break;

  case 116:
#line 862 "conffile.y"
                                                               { (yyval.statistics_opt_counters) = CUM; }
#line 2956 "conffile.tab.c"
    break;

  case 117:
#line 863 "conffile.y"
                                                                                   { (yyval.statistics_opt_counters) = SUB; }
#line 2962 "conffile.tab.c"
    break;

  case 118:
#line 866 "conffile.y"
                                                        { (yyval.statistics_opt_prefix) = NULL; }
#line 2968 "conffile.tab.c"
    break;

  case 119:
#line 867 "conffile.y"
                                                                            { (yyval.statistics_opt_prefix) = (yyvsp[0].crSTRING); }
#line 2974 "conffile.tab.c"
    break;

  case 120:
#line 873 "conffile.y"
          {
	  	struct _rcptr *walk;
		char *err;

		for (walk = (yyvsp[0].listener)->rcptr; walk != NULL; walk = walk->next) {
			err = router_add_listener(rtr, (yyvsp[0].listener)->type,
				(yyvsp[0].listener)->transport->mode, (yyvsp[0].listener)->transport->pemcert,
				walk->ctype, walk->ip, walk->port, walk->saddr);
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc, err);
				YYERROR;
			}
		}
	  }
#line 2994 "conffile.tab.c"
    break;

  case 121:
#line 891 "conffile.y"
                {
			if (((yyval.listener) = ra_malloc(palloc, sizeof(struct _lsnr))) == NULL) {
				logerr("malloc failed\n");
				YYABORT;
			}
			(yyval.listener)->type = T_LINEMODE;
			(yyval.listener)->transport = (yyvsp[-1].transport_mode);
			(yyval.listener)->rcptr = (yyvsp[0].receptors);
			if ((yyvsp[-1].transport_mode)->mode != W_PLAIN) {
				struct _rcptr *walk;

				for (walk = (yyvsp[0].receptors); walk != NULL; walk = walk->next) {
					if (walk->ctype == CON_UDP) {
						router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
							"cannot use UDP transport for "
							"compressed/encrypted stream");
						YYERROR;
					}
				}
			}
		}
#line 3020 "conffile.tab.c"
    break;

  case 122:
#line 915 "conffile.y"
                                 {
				 	(yyval.transport_opt_ssl) = NULL;
				 }
#line 3028 "conffile.tab.c"
    break;

  case 123:
#line 919 "conffile.y"
                                 {
#ifdef HAVE_SSL
					if (((yyval.transport_opt_ssl) = ra_malloc(palloc,
							sizeof(struct _rcptr_trsp))) == NULL)
					{
						logerr("malloc failed\n");
						YYABORT;
					}
					(yyval.transport_opt_ssl)->mode = W_SSL;
					(yyval.transport_opt_ssl)->pemcert = ra_strdup(ralloc, (yyvsp[0].crSTRING));
#else
					router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc,
						"feature ssl not compiled in");
					YYERROR;
#endif
				 }
#line 3050 "conffile.tab.c"
    break;

  case 124:
#line 939 "conffile.y"
                                        {
						if (((yyval.transport_mode_trans) = ra_malloc(palloc,
								sizeof(struct _rcptr_trsp))) == NULL)
						{
							logerr("malloc failed\n");
							YYABORT;
						}
						(yyval.transport_mode_trans)->mode = W_PLAIN;
					}
#line 3064 "conffile.tab.c"
    break;

  case 125:
#line 949 "conffile.y"
                                        {
#ifdef HAVE_GZIP
						if (((yyval.transport_mode_trans) = ra_malloc(palloc,
								sizeof(struct _rcptr_trsp))) == NULL)
						{
							logerr("malloc failed\n");
							YYABORT;
						}
						(yyval.transport_mode_trans)->mode = W_GZIP;
#else
						router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc,
							"feature gzip not compiled in");
						YYERROR;
#endif
					}
#line 3085 "conffile.tab.c"
    break;

  case 126:
#line 966 "conffile.y"
                                        {
#ifdef HAVE_LZ4
						if (((yyval.transport_mode_trans) = ra_malloc(palloc,
								sizeof(struct _rcptr_trsp))) == NULL)
						{
							logerr("malloc failed\n");
							YYABORT;
						}
						(yyval.transport_mode_trans)->mode = W_LZ4;
#else
						router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc,
							"feature lz4 not compiled in");
						YYERROR;
#endif
					}
#line 3106 "conffile.tab.c"
    break;

  case 127:
#line 983 "conffile.y"
                                        {
#ifdef HAVE_SNAPPY
						if (((yyval.transport_mode_trans) = ra_malloc(palloc,
								sizeof(struct _rcptr_trsp))) == NULL)
						{
							logerr("malloc failed\n");
							YYABORT;
						}
						(yyval.transport_mode_trans)->mode = W_SNAPPY;
#else
						router_yyerror(&yylloc, yyscanner, rtr,
							ralloc, palloc,
							"feature snappy not compiled in");
						YYERROR;
#endif
					}
#line 3127 "conffile.tab.c"
    break;

  case 128:
#line 1002 "conffile.y"
                          { 
				if (((yyval.transport_mode) = ra_malloc(palloc,
						sizeof(struct _rcptr_trsp))) == NULL)
				{
					logerr("malloc failed\n");
					YYABORT;
				}
				(yyval.transport_mode)->mode = W_PLAIN;
			  }
#line 3141 "conffile.tab.c"
    break;

  case 129:
#line 1012 "conffile.y"
                          {
			  	if ((yyvsp[0].transport_opt_ssl) == NULL) {
					(yyval.transport_mode) = (yyvsp[-1].transport_mode_trans);
				} else {
					(yyval.transport_mode) = (yyvsp[0].transport_opt_ssl);
					(yyval.transport_mode)->mode |= (yyvsp[-1].transport_mode_trans)->mode;
				}
			  }
#line 3154 "conffile.tab.c"
    break;

  case 130:
#line 1022 "conffile.y"
                                       { (yyvsp[-1].receptor)->next = (yyvsp[0].opt_receptor); (yyval.receptors) = (yyvsp[-1].receptor); }
#line 3160 "conffile.tab.c"
    break;

  case 131:
#line 1025 "conffile.y"
                        { (yyval.opt_receptor) = NULL; }
#line 3166 "conffile.tab.c"
    break;

  case 132:
#line 1026 "conffile.y"
                                    { (yyval.opt_receptor) = (yyvsp[0].receptors);   }
#line 3172 "conffile.tab.c"
    break;

  case 133:
#line 1030 "conffile.y"
                {
			char *err;
			void *hint = NULL;
			char *w;
			char bcip[24];

			if (((yyval.receptor) = ra_malloc(palloc, sizeof(struct _rcptr))) == NULL) {
				logerr("malloc failed\n");
				YYABORT;
			}
			(yyval.receptor)->ctype = (yyvsp[0].rcptr_proto);

			/* find out if this is just a port */
			for (w = (yyvsp[-2].crSTRING); *w != '\0'; w++)
				if (*w < '0' || *w > '9')
					break;
			if (*w == '\0') {
				snprintf(bcip, sizeof(bcip), ":%s", (yyvsp[-2].crSTRING));
				(yyvsp[-2].crSTRING) = bcip;
			}

			err = router_validate_address(
					rtr,
					&((yyval.receptor)->ip), &((yyval.receptor)->port), &((yyval.receptor)->saddr), &hint,
					(yyvsp[-2].crSTRING), (yyvsp[0].rcptr_proto));
			/* help some static analysis tools to see bcip isn't going
			 * out of scope */
			(yyvsp[-2].crSTRING) = NULL;
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc, err);
				YYERROR;
			}
			free(hint);
			(yyval.receptor)->next = NULL;
		}
#line 3213 "conffile.tab.c"
    break;

  case 134:
#line 1067 "conffile.y"
                {
			char *err;

			if (((yyval.receptor) = ra_malloc(palloc, sizeof(struct _rcptr))) == NULL) {
				logerr("malloc failed\n");
				YYABORT;
			}
			(yyval.receptor)->ctype = CON_UNIX;
			(yyval.receptor)->ip = (yyvsp[-2].crSTRING);
			(yyval.receptor)->port = 0;
			(yyval.receptor)->saddr = NULL;
			err = router_validate_path(rtr, (yyvsp[-2].crSTRING));
			if (err != NULL) {
				router_yyerror(&yylloc, yyscanner, rtr,
						ralloc, palloc, err);
				YYERROR;
			}
			(yyval.receptor)->next = NULL;
		}
#line 3237 "conffile.tab.c"
    break;

  case 135:
#line 1088 "conffile.y"
                   { (yyval.rcptr_proto) = CON_TCP; }
#line 3243 "conffile.tab.c"
    break;

  case 136:
#line 1089 "conffile.y"
                           { (yyval.rcptr_proto) = CON_UDP; }
#line 3249 "conffile.tab.c"
    break;

  case 137:
#line 1095 "conffile.y"
           {
	   	if (router_readconfig(rtr, (yyvsp[0].crSTRING), 0, 0, 0, 0, 0, 0, 0) == NULL)
			YYERROR;
	   }
#line 3258 "conffile.tab.c"
    break;


#line 3262 "conffile.tab.c"

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
  *++yylsp = yyloc;

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
      yyerror (&yylloc, yyscanner, rtr, ralloc, palloc, YY_("syntax error"));
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
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
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
        yyerror (&yylloc, yyscanner, rtr, ralloc, palloc, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

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
                      yytoken, &yylval, &yylloc, yyscanner, rtr, ralloc, palloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp, yyscanner, rtr, ralloc, palloc);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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
  yyerror (&yylloc, yyscanner, rtr, ralloc, palloc, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, yyscanner, rtr, ralloc, palloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp, yyscanner, rtr, ralloc, palloc);
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
