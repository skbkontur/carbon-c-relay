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
	int threads;
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

#line 191 "conffile.tab.c"

/* Token type.  */
#ifndef ROUTER_YYTOKENTYPE
# define ROUTER_YYTOKENTYPE
  enum router_yytokentype
  {
    crCLUSTER = 258,
    crFORWARD = 259,
    crANY_OF = 260,
    crFAILOVER = 261,
    crCARBON_CH = 262,
    crFNV1A_CH = 263,
    crJUMP_FNV1A_CH = 264,
    crFILE = 265,
    crIP = 266,
    crREPLICATION = 267,
    crDYNAMIC = 268,
    crPROTO = 269,
    crUSEALL = 270,
    crUDP = 271,
    crTCP = 272,
    crTTL = 273,
    crTHREADS = 274,
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
  /* threads  */
  int threads;
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
#line 409 "conffile.tab.c"

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
#define YYLAST   153

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  138
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  211

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
       0,   135,   135,   138,   139,   142,   145,   146,   147,   148,
     149,   150,   151,   152,   156,   233,   281,   283,   287,   288,
     289,   292,   293,   296,   297,   298,   301,   302,   305,   306,
     309,   310,   313,   314,   317,   318,   321,   322,   325,   326,
     329,   330,   333,   335,   336,   338,   358,   360,   361,   363,
     385,   386,   387,   397,   398,   399,   402,   403,   404,   407,
     408,   418,   419,   429,   439,   450,   451,   466,   518,   530,
     533,   535,   536,   539,   558,   559,   578,   579,   582,   583,
     586,   587,   590,   593,   603,   606,   608,   609,   612,   628,
     629,   634,   676,   764,   765,   770,   771,   772,   775,   779,
     780,   782,   797,   798,   799,   800,   801,   802,   803,   813,
     814,   817,   818,   823,   838,   863,   864,   875,   876,   879,
     880,   885,   903,   928,   931,   951,   961,   978,   995,  1015,
    1024,  1035,  1038,  1039,  1042,  1079,  1101,  1102,  1107
};
#endif

#if ROUTER_YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "crCLUSTER", "crFORWARD", "crANY_OF",
  "crFAILOVER", "crCARBON_CH", "crFNV1A_CH", "crJUMP_FNV1A_CH", "crFILE",
  "crIP", "crREPLICATION", "crDYNAMIC", "crPROTO", "crUSEALL", "crUDP",
  "crTCP", "crTTL", "crTHREADS", "crCONNECTIONS", "crTHRESHOLD_START",
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
  "ttl", "threads", "cluster_file", "cluster_paths", "cluster_opt_path",
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

#define YYPACT_NINF (-96)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      -2,   -69,   -57,   -26,   -49,   -25,   -10,   -12,   -23,    58,
     -96,    -2,   -22,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,    31,   -96,   -96,    35,   -96,   -25,    30,    28,    29,
      33,     4,    -1,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,    57,    50,    56,    60,    50,    -6,
      46,   -96,   -96,   -28,    -3,    -4,     1,    16,    15,    10,
     -96,     2,    61,   -96,   -96,     3,    72,    61,    62,    59,
      63,   -96,   -96,    53,   -96,    11,   -96,    51,    54,    55,
      26,    63,   -65,    19,    18,   -96,    17,    81,   -96,   -96,
     -96,    81,    20,    21,    70,    53,   -96,   -96,   -96,   -96,
     -96,    64,   -96,    39,    25,   -96,    53,   -96,   -96,   -96,
     -96,    27,   -96,    91,   -96,    18,   -96,    34,    86,    86,
     -96,   -96,   -96,   -96,   -28,   -96,    68,   -96,   -96,   -96,
     -96,   -14,   -96,   -96,   -96,    36,    87,    87,   -96,    37,
     -96,   -96,   -96,   -96,   -96,    38,    41,    42,    73,   -96,
      40,   -96,    41,   -96,   -96,    42,    69,   -50,    98,   -96,
     -96,   -96,   -96,    71,    66,   -96,   -96,    32,    49,   -20,
     -37,    63,    66,   -96,   -96,   -16,    52,   -96,   -96,   -96,
      78,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
      67,    53,   -96,   -96,   -96,   -96,   -29,   -96,    65,    79,
      96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,    74,
     -96
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     0,     0,   115,     0,     0,     0,
       2,     3,     0,     6,     7,     8,     9,    10,    11,    12,
      13,     0,    73,    68,    74,    69,    71,     0,     0,     0,
       0,   117,     0,   121,   138,     1,     4,     5,    18,    19,
      20,    23,    24,    25,    41,    38,    21,    26,    38,     0,
      78,    72,    70,     0,     0,     0,     0,     0,   119,   129,
      40,     0,    30,    22,    16,     0,    28,    30,     0,     0,
      80,    83,    88,    89,    84,    86,    91,     0,     0,     0,
       0,   111,     0,   123,     0,    39,     0,    36,    27,    29,
      17,    36,     0,     0,     0,    89,    81,    90,   113,    87,
      85,     0,   116,     0,     0,   112,    89,   125,   126,   127,
     128,     0,   130,     0,   122,   132,    31,     0,    32,    32,
      76,    77,    75,    79,     0,    67,     0,   118,   120,   114,
     124,     0,   133,   131,    37,     0,    34,    34,    82,     0,
     137,   136,   135,   134,    33,     0,     0,     0,     0,    35,
      50,    14,    47,    45,    15,    43,    93,     0,    53,    48,
      46,    42,    44,     0,     0,    51,    52,     0,    56,     0,
       0,   111,    99,    54,    55,     0,    59,    95,    96,    97,
       0,   102,   103,   104,   105,   106,   107,   109,   110,   108,
       0,    89,   100,    98,    57,    58,     0,    49,    65,     0,
       0,    92,    61,    62,    63,    64,    66,    60,    94,     0,
     101
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -96,   -96,   117,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,    75,    12,    -8,    43,    82,   -96,   -96,   -96,
     -19,   -17,   -96,   -96,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,     0,   -96,   -96,   -96,   -96,   -96,   -96,    76,
       8,    77,   -96,   -96,   -95,   -96,   -96,   -96,   -96,   -39,
     -96,   -96,   -96,   -34,   -96,   -96,   -96,   -96,   -96,   -96,
     -96,   -96,   -96,   -96,    23,   -96,   -96,   -96,   -96
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     9,    10,    11,    12,    13,    45,    46,    64,    47,
      66,    90,    87,   136,   146,   118,    62,    48,   154,   161,
     155,   151,   160,   152,   158,   168,   176,   197,   198,   207,
      14,    24,    25,    52,    26,    50,   122,    70,    95,   105,
      73,    74,   100,    75,    98,    15,    16,   164,   180,   171,
     193,   172,   190,   106,    17,    18,    31,    58,    81,    19,
      33,   112,    83,    84,   114,   133,   115,   143,    20
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
     125,     1,   140,   141,    71,    29,   107,   108,   109,   110,
      21,   129,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     2,    22,   177,   178,   179,    51,    23,     3,   165,
      28,   166,     4,    27,     5,    38,    39,    40,    41,    42,
      43,    44,   202,   203,   204,   205,   120,   121,   173,   174,
      30,    72,   194,   195,    22,    32,    34,     6,    35,    49,
      37,    53,   142,    54,     7,    57,    55,    59,    60,    61,
      56,    63,    65,    68,    69,     8,    76,    77,    79,    80,
      82,    86,    78,    85,    88,    89,    97,    92,    93,   101,
      72,   104,   102,    94,   111,   103,   201,   113,   116,   117,
     123,   124,   127,   126,   128,   131,   130,   135,   139,   145,
     163,   156,   167,   169,   170,   134,   175,   144,   148,   149,
     150,   153,   196,   157,   199,   200,   208,   209,    36,   147,
      67,   137,   138,   192,   119,   159,   162,   191,   132,     0,
     206,     0,    91,     0,     0,     0,    96,     0,     0,     0,
       0,     0,    99,   210
};

static const yytype_int16 yycheck[] =
{
      95,     3,    16,    17,    32,     5,    71,    72,    73,    74,
      79,   106,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    23,    79,    43,    44,    45,    26,    84,    30,    79,
      79,    81,    34,    59,    36,     4,     5,     6,     7,     8,
       9,    10,    71,    72,    73,    74,    26,    27,    16,    17,
      60,    79,    68,    69,    79,    67,    79,    59,     0,    24,
      82,    31,    76,    35,    66,    61,    37,    68,    11,    19,
      37,    15,    12,    79,    28,    77,    79,    81,    62,    64,
      70,    20,    81,    81,    81,    13,    33,    25,    29,    38,
      79,    65,    38,    30,    75,    40,   191,    79,    81,    18,
      79,    31,    63,    39,    79,    14,    79,    21,    40,    22,
      41,    38,    14,    42,    48,    81,    67,    81,    81,    81,
      79,    79,    70,    83,    46,    58,    47,    31,    11,   137,
      48,   119,   124,   172,    91,   152,   155,   171,   115,    -1,
      75,    -1,    67,    -1,    -1,    -1,    70,    -1,    -1,    -1,
      -1,    -1,    75,    79
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    23,    30,    34,    36,    59,    66,    77,    86,
      87,    88,    89,    90,   115,   130,   131,   139,   140,   144,
     153,    79,    79,    84,   116,   117,   119,    59,    79,   117,
      60,   141,    67,   145,    79,     0,    87,    82,     4,     5,
       6,     7,     8,     9,    10,    91,    92,    94,   102,    24,
     120,   117,   118,    31,    35,    37,    37,    61,   142,    68,
      11,    19,   101,    15,    93,    12,    95,   101,    79,    28,
     122,    32,    79,   125,   126,   128,    79,    81,    81,    62,
      64,   143,    70,   147,   148,    81,    20,    97,    81,    13,
      96,    97,    25,    29,    30,   123,   124,    33,   129,   126,
     127,    38,    38,    40,    65,   124,   138,    71,    72,    73,
      74,    75,   146,    79,   149,   151,    81,    18,   100,   100,
      26,    27,   121,    79,    31,   129,    39,    63,    79,   129,
      79,    14,   149,   150,    81,    21,    98,    98,   125,    40,
      16,    17,    76,   152,    81,    22,    99,    99,    81,    81,
      79,   106,   108,    79,   103,   105,    38,    83,   109,   106,
     107,   104,   105,    41,   132,    79,    81,    14,   110,    42,
      48,   134,   136,    16,    17,    67,   111,    43,    44,    45,
     133,    49,    50,    51,    52,    53,    54,    55,    56,    57,
     137,   138,   134,   135,    68,    69,    70,   112,   113,    46,
      58,   129,    71,    72,    73,    74,    75,   114,    47,    31,
      79
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    87,    87,    88,    89,    89,    89,    89,
      89,    89,    89,    89,    90,    90,    91,    91,    92,    92,
      92,    93,    93,    94,    94,    94,    95,    95,    96,    96,
      97,    97,    98,    98,    99,    99,   100,   100,   101,   101,
     102,   102,   103,   104,   104,   105,   106,   107,   107,   108,
     109,   109,   109,   110,   110,   110,   111,   111,   111,   112,
     112,   113,   113,   113,   113,   114,   114,   115,   116,   116,
     117,   118,   118,   119,   120,   120,   121,   121,   122,   122,
     123,   123,   124,   125,   125,   126,   127,   127,   128,   129,
     129,   130,   131,   132,   132,   133,   133,   133,   134,   135,
     135,   136,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   138,   138,   139,   140,   141,   141,   142,   142,   143,
     143,   144,   145,   146,   146,   147,   147,   147,   147,   148,
     148,   149,   150,   150,   151,   151,   152,   152,   153
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     9,     9,     2,     3,     1,     1,
       1,     0,     1,     1,     1,     1,     0,     2,     0,     1,
       0,     2,     0,     2,     0,     2,     0,     2,     0,     2,
       2,     1,     2,     0,     1,     1,     2,     0,     1,     5,
       0,     2,     2,     0,     2,     2,     0,     2,     2,     0,
       2,     2,     2,     2,     2,     0,     1,     6,     1,     1,
       2,     0,     1,     1,     0,     4,     1,     1,     0,     3,
       0,     1,     3,     1,     1,     2,     0,     1,     1,     0,
       1,     4,    13,     0,     5,     1,     1,     1,     2,     0,
       1,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     5,     6,     0,     4,     0,     4,     0,
       3,     2,     4,     0,     2,     2,     2,     2,     2,     0,
       2,     2,     0,     1,     3,     3,     1,     1,     2
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
#line 157 "conffile.y"
           {
	   	struct _clhost *w;
		char *err;
		int srvcnt;
		int replcnt;

		if ((yyvsp[-5].threads) < 1) {
			logerr("threads for cluster '%s' must be greater than 0\n", (yyvsp[-7].crSTRING));
			YYABORT;
		}

		/* count number of servers for ch_new */
		for (srvcnt = 0, w = (yyvsp[0].cluster_hosts); w != NULL; w = w->next, srvcnt++)
			;

		if (((yyval.cluster) = cluster_new((yyvsp[-7].crSTRING), ralloc, (yyvsp[-6].cluster_type).t, NULL, router_queue_size(rtr), cluster_ttl((yyvsp[-3].ttl)), (yyvsp[-5].threads))) == NULL) {
			logerr("malloc failed for cluster '%s'\n", (yyvsp[-7].crSTRING));
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
					logerr("malloc failed for ch in cluster '%s'\n", (yyvsp[-7].crSTRING));
					YYABORT;
				}
				replcnt = (yyvsp[-6].cluster_type).ival / 10;
				(yyval.cluster)->isdynamic = (yyvsp[-6].cluster_type).ival - (replcnt * 10) == 2;
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
				(yyvsp[-6].cluster_type).ival = 0;  /* hack, avoid triggering use_all */
				break;
			case FORWARD:
				(yyval.cluster)->members.forward = NULL;
				break;
			case ANYOF:
			case FAILOVER:
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
					w->saddr, w->hint, (char)(yyvsp[-6].cluster_type).ival, (yyval.cluster));
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
#line 1962 "conffile.tab.c"
    break;

  case 15:
#line 234 "conffile.y"
           {
	   	struct _clhost *w;
		char *err;

		if ((yyvsp[-5].threads) < 1) {
			logerr("threads for cluster '%s' must be greater than 0\n", (yyvsp[-7].crSTRING));
			YYABORT;
		}

		if (((yyval.cluster) = cluster_new((yyvsp[-7].crSTRING), ralloc, (yyvsp[-6].cluster_file).t, NULL, router_queue_size(rtr), cluster_ttl((yyvsp[-3].ttl)), (yyvsp[-5].threads))) == NULL) {
			logerr("malloc failed for cluster '%s'\n", (yyvsp[-7].crSTRING));
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
					w->saddr, w->hint, (char)(yyvsp[-6].cluster_file).ival, (yyval.cluster));
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
#line 2011 "conffile.tab.c"
    break;

  case 16:
#line 282 "conffile.y"
                          { (yyval.cluster_type).t = (yyvsp[-1].cluster_useall); (yyval.cluster_type).ival = (yyvsp[0].cluster_opt_useall); }
#line 2017 "conffile.tab.c"
    break;

  case 17:
#line 284 "conffile.y"
                          { (yyval.cluster_type).t = (yyvsp[-2].cluster_ch); (yyval.cluster_type).ival = ((yyvsp[0].cluster_opt_dynamic) * 2) + ((yyvsp[-1].cluster_opt_repl) * 10); }
#line 2023 "conffile.tab.c"
    break;

  case 18:
#line 287 "conffile.y"
                           { (yyval.cluster_useall) = FORWARD; }
#line 2029 "conffile.tab.c"
    break;

  case 19:
#line 288 "conffile.y"
                                       { (yyval.cluster_useall) = ANYOF; }
#line 2035 "conffile.tab.c"
    break;

  case 20:
#line 289 "conffile.y"
                                       { (yyval.cluster_useall) = FAILOVER; }
#line 2041 "conffile.tab.c"
    break;

  case 21:
#line 292 "conffile.y"
                             { (yyval.cluster_opt_useall) = 0; }
#line 2047 "conffile.tab.c"
    break;

  case 22:
#line 293 "conffile.y"
                                             { (yyval.cluster_opt_useall) = 1; }
#line 2053 "conffile.tab.c"
    break;

  case 23:
#line 296 "conffile.y"
                            { (yyval.cluster_ch) = CARBON_CH; }
#line 2059 "conffile.tab.c"
    break;

  case 24:
#line 297 "conffile.y"
                                    { (yyval.cluster_ch) = FNV1A_CH; }
#line 2065 "conffile.tab.c"
    break;

  case 25:
#line 298 "conffile.y"
                                    { (yyval.cluster_ch) = JUMP_CH; }
#line 2071 "conffile.tab.c"
    break;

  case 26:
#line 301 "conffile.y"
                                              { (yyval.cluster_opt_repl) = 1; }
#line 2077 "conffile.tab.c"
    break;

  case 27:
#line 302 "conffile.y"
                                                              { (yyval.cluster_opt_repl) = (yyvsp[0].crINTVAL); }
#line 2083 "conffile.tab.c"
    break;

  case 28:
#line 305 "conffile.y"
                               { (yyval.cluster_opt_dynamic) = 0; }
#line 2089 "conffile.tab.c"
    break;

  case 29:
#line 306 "conffile.y"
                                               { (yyval.cluster_opt_dynamic) = 1; }
#line 2095 "conffile.tab.c"
    break;

  case 30:
#line 309 "conffile.y"
                           { (yyval.connections) = 1; }
#line 2101 "conffile.tab.c"
    break;

  case 31:
#line 310 "conffile.y"
                                                               { (yyval.connections) = (yyvsp[0].crINTVAL); }
#line 2107 "conffile.tab.c"
    break;

  case 32:
#line 313 "conffile.y"
                                  { (yyval.threshold_start) = 0; }
#line 2113 "conffile.tab.c"
    break;

  case 33:
#line 314 "conffile.y"
                                                                   { (yyval.threshold_start) = (yyvsp[0].crINTVAL); }
#line 2119 "conffile.tab.c"
    break;

  case 34:
#line 317 "conffile.y"
                                { (yyval.threshold_end) = 0; }
#line 2125 "conffile.tab.c"
    break;

  case 35:
#line 318 "conffile.y"
                                                                 { (yyval.threshold_end) = (yyvsp[0].crINTVAL); }
#line 2131 "conffile.tab.c"
    break;

  case 36:
#line 321 "conffile.y"
                                { (yyval.ttl) = 0; }
#line 2137 "conffile.tab.c"
    break;

  case 37:
#line 322 "conffile.y"
                                                       { (yyval.ttl) = (yyvsp[0].crINTVAL); }
#line 2143 "conffile.tab.c"
    break;

  case 38:
#line 325 "conffile.y"
                                { (yyval.threads) = 1; }
#line 2149 "conffile.tab.c"
    break;

  case 39:
#line 326 "conffile.y"
                                                           { (yyval.threads) = (yyvsp[0].crINTVAL); }
#line 2155 "conffile.tab.c"
    break;

  case 40:
#line 329 "conffile.y"
                          { (yyval.cluster_file).t = FILELOGIP; (yyval.cluster_file).ival = 0; }
#line 2161 "conffile.tab.c"
    break;

  case 41:
#line 330 "conffile.y"
                                      { (yyval.cluster_file).t = FILELOG; (yyval.cluster_file).ival = 0; }
#line 2167 "conffile.tab.c"
    break;

  case 42:
#line 333 "conffile.y"
                                                   { (yyvsp[-1].cluster_path)->next = (yyvsp[0].cluster_opt_path); (yyval.cluster_paths) = (yyvsp[-1].cluster_path); }
#line 2173 "conffile.tab.c"
    break;

  case 43:
#line 335 "conffile.y"
                               { (yyval.cluster_opt_path) = NULL; }
#line 2179 "conffile.tab.c"
    break;

  case 44:
#line 336 "conffile.y"
                                               { (yyval.cluster_opt_path) = (yyvsp[0].cluster_path); }
#line 2185 "conffile.tab.c"
    break;

  case 45:
#line 339 "conffile.y"
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
#line 2207 "conffile.tab.c"
    break;

  case 46:
#line 358 "conffile.y"
                                                   { (yyvsp[-1].cluster_host)->next = (yyvsp[0].cluster_opt_host); (yyval.cluster_hosts) = (yyvsp[-1].cluster_host); }
#line 2213 "conffile.tab.c"
    break;

  case 47:
#line 360 "conffile.y"
                                { (yyval.cluster_opt_host) = NULL; }
#line 2219 "conffile.tab.c"
    break;

  case 48:
#line 361 "conffile.y"
                                                { (yyval.cluster_opt_host) = (yyvsp[0].cluster_hosts); }
#line 2225 "conffile.tab.c"
    break;

  case 49:
#line 366 "conffile.y"
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
#line 2248 "conffile.tab.c"
    break;

  case 50:
#line 385 "conffile.y"
                                         { (yyval.cluster_opt_instance) = NULL; }
#line 2254 "conffile.tab.c"
    break;

  case 51:
#line 386 "conffile.y"
                                                             { (yyval.cluster_opt_instance) = (yyvsp[0].crSTRING); }
#line 2260 "conffile.tab.c"
    break;

  case 52:
#line 388 "conffile.y"
                                        {
						(yyval.cluster_opt_instance) = ra_malloc(palloc, sizeof(char) * 12);
						if ((yyval.cluster_opt_instance) == NULL) {
							logerr("out of memory\n");
							YYABORT;
						}
						snprintf((yyval.cluster_opt_instance), 12, "%d", (yyvsp[0].crINTVAL));
					}
#line 2273 "conffile.tab.c"
    break;

  case 53:
#line 397 "conffile.y"
                                 { (yyval.cluster_opt_proto) = CON_TCP; }
#line 2279 "conffile.tab.c"
    break;

  case 54:
#line 398 "conffile.y"
                                                 { (yyval.cluster_opt_proto) = CON_UDP; }
#line 2285 "conffile.tab.c"
    break;

  case 55:
#line 399 "conffile.y"
                                                 { (yyval.cluster_opt_proto) = CON_TCP; }
#line 2291 "conffile.tab.c"
    break;

  case 56:
#line 402 "conffile.y"
                                      { (yyval.cluster_opt_type) = T_LINEMODE; }
#line 2297 "conffile.tab.c"
    break;

  case 57:
#line 403 "conffile.y"
                                                      { (yyval.cluster_opt_type) = T_LINEMODE; }
#line 2303 "conffile.tab.c"
    break;

  case 58:
#line 404 "conffile.y"
                                                      { (yyval.cluster_opt_type) = T_SYSLOGMODE; }
#line 2309 "conffile.tab.c"
    break;

  case 59:
#line 407 "conffile.y"
                                      { (yyval.cluster_opt_transport) = W_PLAIN; }
#line 2315 "conffile.tab.c"
    break;

  case 60:
#line 410 "conffile.y"
                                         {
					 	if ((yyvsp[0].cluster_transport_opt_ssl) == W_PLAIN) {
							(yyval.cluster_opt_transport) = (yyvsp[-1].cluster_transport_trans);
						} else {
							(yyval.cluster_opt_transport) = (yyvsp[-1].cluster_transport_trans) | (yyvsp[0].cluster_transport_opt_ssl);
						}
					 }
#line 2327 "conffile.tab.c"
    break;

  case 61:
#line 418 "conffile.y"
                                              { (yyval.cluster_transport_trans) = W_PLAIN; }
#line 2333 "conffile.tab.c"
    break;

  case 62:
#line 419 "conffile.y"
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
#line 2348 "conffile.tab.c"
    break;

  case 63:
#line 429 "conffile.y"
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
#line 2363 "conffile.tab.c"
    break;

  case 64:
#line 439 "conffile.y"
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
#line 2378 "conffile.tab.c"
    break;

  case 65:
#line 450 "conffile.y"
                           { (yyval.cluster_transport_opt_ssl) = W_PLAIN; }
#line 2384 "conffile.tab.c"
    break;

  case 66:
#line 452 "conffile.y"
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
#line 2399 "conffile.tab.c"
    break;

  case 67:
#line 468 "conffile.y"
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
			if ((d->cl = cluster_new(NULL, ralloc, VALIDATION, NULL, 0, 0, 1)) == NULL) {
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
#line 2452 "conffile.tab.c"
    break;

  case 68:
#line 519 "conffile.y"
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
#line 2468 "conffile.tab.c"
    break;

  case 69:
#line 530 "conffile.y"
                                  { (yyval.match_exprs) = (yyvsp[0].match_exprs2); }
#line 2474 "conffile.tab.c"
    break;

  case 70:
#line 533 "conffile.y"
                                              { (yyvsp[-1].match_expr)->next = (yyvsp[0].match_opt_expr); (yyval.match_exprs2) = (yyvsp[-1].match_expr); }
#line 2480 "conffile.tab.c"
    break;

  case 71:
#line 535 "conffile.y"
                             { (yyval.match_opt_expr) = NULL; }
#line 2486 "conffile.tab.c"
    break;

  case 72:
#line 536 "conffile.y"
                                         { (yyval.match_opt_expr) = (yyvsp[0].match_exprs2); }
#line 2492 "conffile.tab.c"
    break;

  case 73:
#line 540 "conffile.y"
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
#line 2513 "conffile.tab.c"
    break;

  case 74:
#line 558 "conffile.y"
                    { (yyval.match_opt_validate) = NULL; }
#line 2519 "conffile.tab.c"
    break;

  case 75:
#line 560 "conffile.y"
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
#line 2540 "conffile.tab.c"
    break;

  case 76:
#line 578 "conffile.y"
                          { (yyval.match_log_or_drop) = 0; }
#line 2546 "conffile.tab.c"
    break;

  case 77:
#line 579 "conffile.y"
                                          { (yyval.match_log_or_drop) = 1; }
#line 2552 "conffile.tab.c"
    break;

  case 78:
#line 582 "conffile.y"
                 { (yyval.match_opt_route) = NULL; }
#line 2558 "conffile.tab.c"
    break;

  case 79:
#line 583 "conffile.y"
                                                            { (yyval.match_opt_route) = (yyvsp[0].crSTRING); }
#line 2564 "conffile.tab.c"
    break;

  case 80:
#line 586 "conffile.y"
                   { (yyval.match_opt_send_to) = NULL; }
#line 2570 "conffile.tab.c"
    break;

  case 81:
#line 587 "conffile.y"
                                                 { (yyval.match_opt_send_to) = (yyvsp[0].match_send_to); }
#line 2576 "conffile.tab.c"
    break;

  case 82:
#line 590 "conffile.y"
                                            { (yyval.match_send_to) = (yyvsp[0].match_dsts); }
#line 2582 "conffile.tab.c"
    break;

  case 83:
#line 594 "conffile.y"
                  {
			if (((yyval.match_dsts) = ra_malloc(ralloc, sizeof(destinations))) == NULL) {
				logerr("out of memory\n");
				YYABORT;
			}
			if (router_validate_cluster(rtr, &((yyval.match_dsts)->cl), "blackhole") != NULL)
				YYABORT;
			(yyval.match_dsts)->next = NULL;
		  }
#line 2596 "conffile.tab.c"
    break;

  case 84:
#line 603 "conffile.y"
                                { (yyval.match_dsts) = (yyvsp[0].match_dsts2); }
#line 2602 "conffile.tab.c"
    break;

  case 85:
#line 606 "conffile.y"
                                           { (yyvsp[-1].match_dst)->next = (yyvsp[0].match_opt_dst); (yyval.match_dsts2) = (yyvsp[-1].match_dst); }
#line 2608 "conffile.tab.c"
    break;

  case 86:
#line 608 "conffile.y"
                           { (yyval.match_opt_dst) = NULL; }
#line 2614 "conffile.tab.c"
    break;

  case 87:
#line 609 "conffile.y"
                                       { (yyval.match_opt_dst) = (yyvsp[0].match_dsts2); }
#line 2620 "conffile.tab.c"
    break;

  case 88:
#line 613 "conffile.y"
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
#line 2638 "conffile.tab.c"
    break;

  case 89:
#line 628 "conffile.y"
                       { (yyval.match_opt_stop) = 0; }
#line 2644 "conffile.tab.c"
    break;

  case 90:
#line 629 "conffile.y"
                                   { (yyval.match_opt_stop) = 1; }
#line 2650 "conffile.tab.c"
    break;

  case 91:
#line 635 "conffile.y"
           {
		char *err;
		route *r = NULL;
		cluster *cl;

		err = router_validate_expression(rtr, &r, (yyvsp[-2].crSTRING));
		if (err != NULL) {
			router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc, err);
			YYERROR;
		}
		
		cl = cluster_new(NULL, ralloc, REWRITE, NULL, 0, 0, 1);
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
#line 2692 "conffile.tab.c"
    break;

  case 92:
#line 682 "conffile.y"
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

			w = cluster_new(NULL, ralloc, AGGREGATION, NULL, 0, 0, 0);
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
#line 2777 "conffile.tab.c"
    break;

  case 93:
#line 764 "conffile.y"
                         { (yyval.aggregate_opt_timestamp) = TS_END; }
#line 2783 "conffile.tab.c"
    break;

  case 94:
#line 767 "conffile.y"
                                           { (yyval.aggregate_opt_timestamp) = (yyvsp[-2].aggregate_ts_when); }
#line 2789 "conffile.tab.c"
    break;

  case 95:
#line 770 "conffile.y"
                            { (yyval.aggregate_ts_when) = TS_START; }
#line 2795 "conffile.tab.c"
    break;

  case 96:
#line 771 "conffile.y"
                                            { (yyval.aggregate_ts_when) = TS_MIDDLE; }
#line 2801 "conffile.tab.c"
    break;

  case 97:
#line 772 "conffile.y"
                                            { (yyval.aggregate_ts_when) = TS_END; }
#line 2807 "conffile.tab.c"
    break;

  case 98:
#line 776 "conffile.y"
                                  { (yyvsp[-1].aggregate_compute)->next = (yyvsp[0].aggregate_opt_compute); (yyval.aggregate_computes) = (yyvsp[-1].aggregate_compute); }
#line 2813 "conffile.tab.c"
    break;

  case 99:
#line 779 "conffile.y"
                                          { (yyval.aggregate_opt_compute) = NULL; }
#line 2819 "conffile.tab.c"
    break;

  case 100:
#line 780 "conffile.y"
                                                              { (yyval.aggregate_opt_compute) = (yyvsp[0].aggregate_computes); }
#line 2825 "conffile.tab.c"
    break;

  case 101:
#line 784 "conffile.y"
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
#line 2841 "conffile.tab.c"
    break;

  case 102:
#line 797 "conffile.y"
                                  { (yyval.aggregate_comp_type).ctype = SUM; }
#line 2847 "conffile.tab.c"
    break;

  case 103:
#line 798 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = CNT; }
#line 2853 "conffile.tab.c"
    break;

  case 104:
#line 799 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MAX; }
#line 2859 "conffile.tab.c"
    break;

  case 105:
#line 800 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MIN; }
#line 2865 "conffile.tab.c"
    break;

  case 106:
#line 801 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = AVG; }
#line 2871 "conffile.tab.c"
    break;

  case 107:
#line 802 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = MEDN; }
#line 2877 "conffile.tab.c"
    break;

  case 108:
#line 804 "conffile.y"
                                   {
				    if ((yyvsp[0].crPERCENTILE) < 1 || (yyvsp[0].crPERCENTILE) > 99) {
						router_yyerror(&yylloc, yyscanner, rtr, ralloc, palloc,
							"percentile<x>: value x must be between 1 and 99");
						YYERROR;
					}
				   	(yyval.aggregate_comp_type).ctype = PCTL;
					(yyval.aggregate_comp_type).pctl = (unsigned char)(yyvsp[0].crPERCENTILE);
				   }
#line 2891 "conffile.tab.c"
    break;

  case 109:
#line 813 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = VAR; }
#line 2897 "conffile.tab.c"
    break;

  case 110:
#line 814 "conffile.y"
                                                  { (yyval.aggregate_comp_type).ctype = SDEV; }
#line 2903 "conffile.tab.c"
    break;

  case 111:
#line 817 "conffile.y"
                                     { (yyval.aggregate_opt_send_to) = NULL; }
#line 2909 "conffile.tab.c"
    break;

  case 112:
#line 818 "conffile.y"
                                                         { (yyval.aggregate_opt_send_to) = (yyvsp[0].match_send_to); }
#line 2915 "conffile.tab.c"
    break;

  case 113:
#line 824 "conffile.y"
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
#line 2930 "conffile.tab.c"
    break;

  case 114:
#line 844 "conffile.y"
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
#line 2952 "conffile.tab.c"
    break;

  case 115:
#line 863 "conffile.y"
                         { (yyval.statistics_opt_interval) = -1; }
#line 2958 "conffile.tab.c"
    break;

  case 116:
#line 865 "conffile.y"
                                           {
					   	if ((yyvsp[-1].crINTVAL) <= 0) {
							router_yyerror(&yylloc, yyscanner, rtr,
									ralloc, palloc, "interval must be > 0");
							YYERROR;
						}
						(yyval.statistics_opt_interval) = (yyvsp[-1].crINTVAL);
					   }
#line 2971 "conffile.tab.c"
    break;

  case 117:
#line 875 "conffile.y"
                                                               { (yyval.statistics_opt_counters) = CUM; }
#line 2977 "conffile.tab.c"
    break;

  case 118:
#line 876 "conffile.y"
                                                                                   { (yyval.statistics_opt_counters) = SUB; }
#line 2983 "conffile.tab.c"
    break;

  case 119:
#line 879 "conffile.y"
                                                        { (yyval.statistics_opt_prefix) = NULL; }
#line 2989 "conffile.tab.c"
    break;

  case 120:
#line 880 "conffile.y"
                                                                            { (yyval.statistics_opt_prefix) = (yyvsp[0].crSTRING); }
#line 2995 "conffile.tab.c"
    break;

  case 121:
#line 886 "conffile.y"
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
#line 3015 "conffile.tab.c"
    break;

  case 122:
#line 904 "conffile.y"
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
#line 3041 "conffile.tab.c"
    break;

  case 123:
#line 928 "conffile.y"
                                 {
				 	(yyval.transport_opt_ssl) = NULL;
				 }
#line 3049 "conffile.tab.c"
    break;

  case 124:
#line 932 "conffile.y"
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
#line 3071 "conffile.tab.c"
    break;

  case 125:
#line 952 "conffile.y"
                                        {
						if (((yyval.transport_mode_trans) = ra_malloc(palloc,
								sizeof(struct _rcptr_trsp))) == NULL)
						{
							logerr("malloc failed\n");
							YYABORT;
						}
						(yyval.transport_mode_trans)->mode = W_PLAIN;
					}
#line 3085 "conffile.tab.c"
    break;

  case 126:
#line 962 "conffile.y"
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
#line 3106 "conffile.tab.c"
    break;

  case 127:
#line 979 "conffile.y"
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
#line 3127 "conffile.tab.c"
    break;

  case 128:
#line 996 "conffile.y"
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
#line 3148 "conffile.tab.c"
    break;

  case 129:
#line 1015 "conffile.y"
                          { 
				if (((yyval.transport_mode) = ra_malloc(palloc,
						sizeof(struct _rcptr_trsp))) == NULL)
				{
					logerr("malloc failed\n");
					YYABORT;
				}
				(yyval.transport_mode)->mode = W_PLAIN;
			  }
#line 3162 "conffile.tab.c"
    break;

  case 130:
#line 1025 "conffile.y"
                          {
			  	if ((yyvsp[0].transport_opt_ssl) == NULL) {
					(yyval.transport_mode) = (yyvsp[-1].transport_mode_trans);
				} else {
					(yyval.transport_mode) = (yyvsp[0].transport_opt_ssl);
					(yyval.transport_mode)->mode |= (yyvsp[-1].transport_mode_trans)->mode;
				}
			  }
#line 3175 "conffile.tab.c"
    break;

  case 131:
#line 1035 "conffile.y"
                                       { (yyvsp[-1].receptor)->next = (yyvsp[0].opt_receptor); (yyval.receptors) = (yyvsp[-1].receptor); }
#line 3181 "conffile.tab.c"
    break;

  case 132:
#line 1038 "conffile.y"
                        { (yyval.opt_receptor) = NULL; }
#line 3187 "conffile.tab.c"
    break;

  case 133:
#line 1039 "conffile.y"
                                    { (yyval.opt_receptor) = (yyvsp[0].receptors);   }
#line 3193 "conffile.tab.c"
    break;

  case 134:
#line 1043 "conffile.y"
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
#line 3234 "conffile.tab.c"
    break;

  case 135:
#line 1080 "conffile.y"
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
#line 3258 "conffile.tab.c"
    break;

  case 136:
#line 1101 "conffile.y"
                   { (yyval.rcptr_proto) = CON_TCP; }
#line 3264 "conffile.tab.c"
    break;

  case 137:
#line 1102 "conffile.y"
                           { (yyval.rcptr_proto) = CON_UDP; }
#line 3270 "conffile.tab.c"
    break;

  case 138:
#line 1108 "conffile.y"
           {
	   	if (router_readconfig(rtr, (yyvsp[0].crSTRING), 0, 0, 0, 0, 0, 0, 0) == NULL)
			YYERROR;
	   }
#line 3279 "conffile.tab.c"
    break;


#line 3283 "conffile.tab.c"

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
