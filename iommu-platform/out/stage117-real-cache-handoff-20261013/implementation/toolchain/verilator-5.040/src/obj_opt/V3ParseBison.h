/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_V3PARSEBISON_PRETMP_H_INCLUDED
# define YY_YY_V3PARSEBISON_PRETMP_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    yaFLOATNUM = 258,              /* "FLOATING-POINT NUMBER"  */
    yaID__ETC = 259,               /* "IDENTIFIER"  */
    yaID__CC = 260,                /* "IDENTIFIER-::"  */
    yaID__LEX = 261,               /* "IDENTIFIER-in-lex"  */
    yaID__PATHPULSE = 262,         /* "IDENTIFIER-for-pathpulse"  */
    yaID__aINST = 263,             /* "IDENTIFIER-for-instance"  */
    yaID__aTYPE = 264,             /* "IDENTIFIER-for-type"  */
    yaINTNUM = 265,                /* "INTEGER NUMBER"  */
    yaTIMENUM = 266,               /* "TIME NUMBER"  */
    yaSTRING = 267,                /* "STRING"  */
    yaSTRING__IGNORE = 268,        /* "STRING-ignored"  */
    yaEDGEDESC = 269,              /* "EDGE DESCRIPTOR"  */
    yaTIMINGSPEC = 270,            /* "TIMING SPEC ELEMENT"  */
    ygenSTRENGTH = 271,            /* "STRENGTH keyword (strong1/etc)"  */
    yaTABLE_FIELD = 272,           /* "UDP table field"  */
    yaTABLE_LRSEP = 273,           /* ":"  */
    yaTABLE_LINEEND = 274,         /* "UDP table line end"  */
    yaSCCTOR = 275,                /* "`systemc_ctor block"  */
    yaSCDTOR = 276,                /* "`systemc_dtor block"  */
    yaSCHDR = 277,                 /* "`systemc_header block"  */
    yaSCHDRP = 278,                /* "`systemc_header_post block"  */
    yaSCIMP = 279,                 /* "`systemc_implementation block"  */
    yaSCIMPH = 280,                /* "`systemc_imp_header block"  */
    yaSCINT = 281,                 /* "`systemc_interface block"  */
    yVLT_CLOCKER = 282,            /* "clocker"  */
    yVLT_CLOCK_ENABLE = 283,       /* "clock_enable"  */
    yVLT_COVERAGE_BLOCK_OFF = 284, /* "coverage_block_off"  */
    yVLT_COVERAGE_OFF = 285,       /* "coverage_off"  */
    yVLT_COVERAGE_ON = 286,        /* "coverage_on"  */
    yVLT_FORCEABLE = 287,          /* "forceable"  */
    yVLT_FULL_CASE = 288,          /* "full_case"  */
    yVLT_HIER_BLOCK = 289,         /* "hier_block"  */
    yVLT_HIER_PARAMS = 290,        /* "hier_params"  */
    yVLT_HIER_WORKERS = 291,       /* "hier_workers"  */
    yVLT_INLINE = 292,             /* "inline"  */
    yVLT_ISOLATE_ASSIGNMENTS = 293, /* "isolate_assignments"  */
    yVLT_LINT_OFF = 294,           /* "lint_off"  */
    yVLT_LINT_ON = 295,            /* "lint_on"  */
    yVLT_NO_CLOCKER = 296,         /* "no_clocker"  */
    yVLT_NO_INLINE = 297,          /* "no_inline"  */
    yVLT_PARALLEL_CASE = 298,      /* "parallel_case"  */
    yVLT_PROFILE_DATA = 299,       /* "profile_data"  */
    yVLT_PUBLIC = 300,             /* "public"  */
    yVLT_PUBLIC_FLAT = 301,        /* "public_flat"  */
    yVLT_PUBLIC_FLAT_RD = 302,     /* "public_flat_rd"  */
    yVLT_PUBLIC_FLAT_RW = 303,     /* "public_flat_rw"  */
    yVLT_PUBLIC_MODULE = 304,      /* "public_module"  */
    yVLT_SC_BV = 305,              /* "sc_bv"  */
    yVLT_SFORMAT = 306,            /* "sformat"  */
    yVLT_SPLIT_VAR = 307,          /* "split_var"  */
    yVLT_TIMING_OFF = 308,         /* "timing_off"  */
    yVLT_TIMING_ON = 309,          /* "timing_on"  */
    yVLT_TRACING_OFF = 310,        /* "tracing_off"  */
    yVLT_TRACING_ON = 311,         /* "tracing_on"  */
    yVLT_D_BLOCK = 312,            /* "--block"  */
    yVLT_D_CONTENTS = 313,         /* "--contents"  */
    yVLT_D_COST = 314,             /* "--cost"  */
    yVLT_D_FILE = 315,             /* "--file"  */
    yVLT_D_FUNCTION = 316,         /* "--function"  */
    yVLT_D_HIER_DPI = 317,         /* "--hier-dpi"  */
    yVLT_D_LEVELS = 318,           /* "--levels"  */
    yVLT_D_LINES = 319,            /* "--lines"  */
    yVLT_D_MATCH = 320,            /* "--match"  */
    yVLT_D_MODEL = 321,            /* "--model"  */
    yVLT_D_MODULE = 322,           /* "--module"  */
    yVLT_D_MTASK = 323,            /* "--mtask"  */
    yVLT_D_RULE = 324,             /* "--rule"  */
    yVLT_D_SCOPE = 325,            /* "--scope"  */
    yVLT_D_TASK = 326,             /* "--task"  */
    yVLT_D_VAR = 327,              /* "--var"  */
    yVLT_D_WORKERS = 328,          /* "--workers"  */
    yaD_PLI = 329,                 /* "${pli-system}"  */
    yaT_NOUNCONNECTED = 330,       /* "`nounconnecteddrive"  */
    yaT_RESETALL = 331,            /* "`resetall"  */
    yaT_UNCONNECTED_PULL0 = 332,   /* "`unconnected_drive pull0"  */
    yaT_UNCONNECTED_PULL1 = 333,   /* "`unconnected_drive pull1"  */
    y1STEP = 334,                  /* "1step"  */
    yACCEPT_ON = 335,              /* "accept_on"  */
    yALIAS = 336,                  /* "alias"  */
    yALWAYS = 337,                 /* "always"  */
    yALWAYS_COMB = 338,            /* "always_comb"  */
    yALWAYS_FF = 339,              /* "always_ff"  */
    yALWAYS_LATCH = 340,           /* "always_latch"  */
    yAND = 341,                    /* "and"  */
    yASSERT = 342,                 /* "assert"  */
    yASSIGN = 343,                 /* "assign"  */
    yASSUME = 344,                 /* "assume"  */
    yAUTOMATIC = 345,              /* "automatic"  */
    yBEFORE = 346,                 /* "before"  */
    yBEGIN = 347,                  /* "begin"  */
    yBIND = 348,                   /* "bind"  */
    yBINS = 349,                   /* "bins"  */
    yBINSOF = 350,                 /* "binsof"  */
    yBIT = 351,                    /* "bit"  */
    yBREAK = 352,                  /* "break"  */
    yBUF = 353,                    /* "buf"  */
    yBUFIF0 = 354,                 /* "bufif0"  */
    yBUFIF1 = 355,                 /* "bufif1"  */
    yBYTE = 356,                   /* "byte"  */
    yCASE = 357,                   /* "case"  */
    yCASEX = 358,                  /* "casex"  */
    yCASEZ = 359,                  /* "casez"  */
    yCELL = 360,                   /* "cell"  */
    yCHANDLE = 361,                /* "chandle"  */
    yCHECKER = 362,                /* "checker"  */
    yCLASS = 363,                  /* "class"  */
    yCLOCKING = 364,               /* "clocking"  */
    yCMOS = 365,                   /* "cmos"  */
    yCONFIG = 366,                 /* "config"  */
    yCONSTRAINT = 367,             /* "constraint"  */
    yCONST__ETC = 368,             /* "const"  */
    yCONST__LEX = 369,             /* "const-in-lex"  */
    yCONST__REF = 370,             /* "const-then-ref"  */
    yCONTEXT = 371,                /* "context"  */
    yCONTINUE = 372,               /* "continue"  */
    yCOVER = 373,                  /* "cover"  */
    yCOVERGROUP = 374,             /* "covergroup"  */
    yCOVERPOINT = 375,             /* "coverpoint"  */
    yCROSS = 376,                  /* "cross"  */
    yDEASSIGN = 377,               /* "deassign"  */
    yDEFAULT = 378,                /* "default"  */
    yDEFPARAM = 379,               /* "defparam"  */
    yDESIGN = 380,                 /* "design"  */
    yDISABLE = 381,                /* "disable"  */
    yDIST = 382,                   /* "dist"  */
    yDO = 383,                     /* "do"  */
    yEDGE = 384,                   /* "edge"  */
    yELSE = 385,                   /* "else"  */
    yEND = 386,                    /* "end"  */
    yENDCASE = 387,                /* "endcase"  */
    yENDCHECKER = 388,             /* "endchecker"  */
    yENDCLASS = 389,               /* "endclass"  */
    yENDCLOCKING = 390,            /* "endclocking"  */
    yENDCONFIG = 391,              /* "endconfig"  */
    yENDFUNCTION = 392,            /* "endfunction"  */
    yENDGENERATE = 393,            /* "endgenerate"  */
    yENDGROUP = 394,               /* "endgroup"  */
    yENDINTERFACE = 395,           /* "endinterface"  */
    yENDMODULE = 396,              /* "endmodule"  */
    yENDPACKAGE = 397,             /* "endpackage"  */
    yENDPRIMITIVE = 398,           /* "endprimitive"  */
    yENDPROGRAM = 399,             /* "endprogram"  */
    yENDPROPERTY = 400,            /* "endproperty"  */
    yENDSEQUENCE = 401,            /* "endsequence"  */
    yENDSPECIFY = 402,             /* "endspecify"  */
    yENDTABLE = 403,               /* "endtable"  */
    yENDTASK = 404,                /* "endtask"  */
    yENUM = 405,                   /* "enum"  */
    yEVENT = 406,                  /* "event"  */
    yEVENTUALLY = 407,             /* "eventually"  */
    yEXPECT = 408,                 /* "expect"  */
    yEXPORT = 409,                 /* "export"  */
    yEXTENDS = 410,                /* "extends"  */
    yEXTERN = 411,                 /* "extern"  */
    yFINAL = 412,                  /* "final"  */
    yFIRST_MATCH = 413,            /* "first_match"  */
    yFOR = 414,                    /* "for"  */
    yFORCE = 415,                  /* "force"  */
    yFOREACH = 416,                /* "foreach"  */
    yFOREVER = 417,                /* "forever"  */
    yFORK = 418,                   /* "fork"  */
    yFORKJOIN = 419,               /* "forkjoin"  */
    yFUNCTION = 420,               /* "function"  */
    yGENERATE = 421,               /* "generate"  */
    yGENVAR = 422,                 /* "genvar"  */
    yGLOBAL__CLOCKING = 423,       /* "global-then-clocking"  */
    yGLOBAL__ETC = 424,            /* "global"  */
    yGLOBAL__LEX = 425,            /* "global-in-lex"  */
    yHIGHZ0 = 426,                 /* "highz0"  */
    yHIGHZ1 = 427,                 /* "highz1"  */
    yIF = 428,                     /* "if"  */
    yIFF = 429,                    /* "iff"  */
    yIGNORE_BINS = 430,            /* "ignore_bins"  */
    yILLEGAL_BINS = 431,           /* "illegal_bins"  */
    yIMPLEMENTS = 432,             /* "implements"  */
    yIMPLIES = 433,                /* "implies"  */
    yIMPORT = 434,                 /* "import"  */
    yINITIAL = 435,                /* "initial"  */
    yINOUT = 436,                  /* "inout"  */
    yINPUT = 437,                  /* "input"  */
    yINSIDE = 438,                 /* "inside"  */
    yINSTANCE = 439,               /* "instance"  */
    yINT = 440,                    /* "int"  */
    yINTEGER = 441,                /* "integer"  */
    yINTERCONNECT = 442,           /* "interconnect"  */
    yINTERFACE = 443,              /* "interface"  */
    yINTERSECT = 444,              /* "intersect"  */
    yJOIN = 445,                   /* "join"  */
    yJOIN_ANY = 446,               /* "join_any"  */
    yJOIN_NONE = 447,              /* "join_none"  */
    yLET = 448,                    /* "let"  */
    yLIBLIST = 449,                /* "liblist"  */
    yLOCALPARAM = 450,             /* "localparam"  */
    yLOCAL__COLONCOLON = 451,      /* "local-then-::"  */
    yLOCAL__ETC = 452,             /* "local"  */
    yLOCAL__LEX = 453,             /* "local-in-lex"  */
    yLOGIC = 454,                  /* "logic"  */
    yLONGINT = 455,                /* "longint"  */
    yMATCHES = 456,                /* "matches"  */
    yMODPORT = 457,                /* "modport"  */
    yMODULE = 458,                 /* "module"  */
    yNAND = 459,                   /* "nand"  */
    yNEGEDGE = 460,                /* "negedge"  */
    yNETTYPE = 461,                /* "nettype"  */
    yNEW__ETC = 462,               /* "new"  */
    yNEW__LEX = 463,               /* "new-in-lex"  */
    yNEW__PAREN = 464,             /* "new-then-paren"  */
    yNEXTTIME = 465,               /* "nexttime"  */
    yNMOS = 466,                   /* "nmos"  */
    yNOR = 467,                    /* "nor"  */
    yNOT = 468,                    /* "not"  */
    yNOTIF0 = 469,                 /* "notif0"  */
    yNOTIF1 = 470,                 /* "notif1"  */
    yNULL = 471,                   /* "null"  */
    yOR = 472,                     /* "or"  */
    yOUTPUT = 473,                 /* "output"  */
    yPACKAGE = 474,                /* "package"  */
    yPACKED = 475,                 /* "packed"  */
    yPARAMETER = 476,              /* "parameter"  */
    yPMOS = 477,                   /* "pmos"  */
    yPOSEDGE = 478,                /* "posedge"  */
    yPRIMITIVE = 479,              /* "primitive"  */
    yPRIORITY = 480,               /* "priority"  */
    yPROGRAM = 481,                /* "program"  */
    yPROPERTY = 482,               /* "property"  */
    yPROTECTED = 483,              /* "protected"  */
    yPULL0 = 484,                  /* "pull0"  */
    yPULL1 = 485,                  /* "pull1"  */
    yPULLDOWN = 486,               /* "pulldown"  */
    yPULLUP = 487,                 /* "pullup"  */
    yPURE = 488,                   /* "pure"  */
    yRAND = 489,                   /* "rand"  */
    yRANDC = 490,                  /* "randc"  */
    yRANDCASE = 491,               /* "randcase"  */
    yRANDOMIZE = 492,              /* "randomize"  */
    yRANDSEQUENCE = 493,           /* "randsequence"  */
    yRCMOS = 494,                  /* "rcmos"  */
    yREAL = 495,                   /* "real"  */
    yREALTIME = 496,               /* "realtime"  */
    yREF = 497,                    /* "ref"  */
    yREG = 498,                    /* "reg"  */
    yREJECT_ON = 499,              /* "reject_on"  */
    yRELEASE = 500,                /* "release"  */
    yREPEAT = 501,                 /* "repeat"  */
    yRESTRICT = 502,               /* "restrict"  */
    yRETURN = 503,                 /* "return"  */
    yRNMOS = 504,                  /* "rnmos"  */
    yRPMOS = 505,                  /* "rpmos"  */
    yRTRAN = 506,                  /* "rtran"  */
    yRTRANIF0 = 507,               /* "rtranif0"  */
    yRTRANIF1 = 508,               /* "rtranif1"  */
    ySCALARED = 509,               /* "scalared"  */
    ySEQUENCE = 510,               /* "sequence"  */
    ySHORTINT = 511,               /* "shortint"  */
    ySHORTREAL = 512,              /* "shortreal"  */
    ySIGNED = 513,                 /* "signed"  */
    ySOFT = 514,                   /* "soft"  */
    ySOLVE = 515,                  /* "solve"  */
    ySPECIFY = 516,                /* "specify"  */
    ySPECPARAM = 517,              /* "specparam"  */
    ySTATIC__CONSTRAINT = 518,     /* "static-then-constraint"  */
    ySTATIC__ETC = 519,            /* "static"  */
    ySTATIC__LEX = 520,            /* "static-in-lex"  */
    ySTRING = 521,                 /* "string"  */
    ySTRONG = 522,                 /* "strong"  */
    ySTRONG0 = 523,                /* "strong0"  */
    ySTRONG1 = 524,                /* "strong1"  */
    ySTRUCT = 525,                 /* "struct"  */
    ySUPER = 526,                  /* "super"  */
    ySUPPLY0 = 527,                /* "supply0"  */
    ySUPPLY1 = 528,                /* "supply1"  */
    ySYNC_ACCEPT_ON = 529,         /* "sync_accept_on"  */
    ySYNC_REJECT_ON = 530,         /* "sync_reject_on"  */
    yS_ALWAYS = 531,               /* "s_always"  */
    yS_EVENTUALLY = 532,           /* "s_eventually"  */
    yS_NEXTTIME = 533,             /* "s_nexttime"  */
    yS_UNTIL = 534,                /* "s_until"  */
    yS_UNTIL_WITH = 535,           /* "s_until_with"  */
    yTABLE = 536,                  /* "table"  */
    yTAGGED = 537,                 /* "tagged"  */
    yTASK = 538,                   /* "task"  */
    yTHIS = 539,                   /* "this"  */
    yTHROUGHOUT = 540,             /* "throughout"  */
    yTIME = 541,                   /* "time"  */
    yTIMEPRECISION = 542,          /* "timeprecision"  */
    yTIMEUNIT = 543,               /* "timeunit"  */
    yTRAN = 544,                   /* "tran"  */
    yTRANIF0 = 545,                /* "tranif0"  */
    yTRANIF1 = 546,                /* "tranif1"  */
    yTRI = 547,                    /* "tri"  */
    yTRI0 = 548,                   /* "tri0"  */
    yTRI1 = 549,                   /* "tri1"  */
    yTRIAND = 550,                 /* "triand"  */
    yTRIOR = 551,                  /* "trior"  */
    yTRIREG = 552,                 /* "trireg"  */
    yTRUE = 553,                   /* "true"  */
    yTYPEDEF = 554,                /* "typedef"  */
    yTYPE__EQ = 555,               /* "type-then-eqneq"  */
    yTYPE__ETC = 556,              /* "type"  */
    yTYPE__LEX = 557,              /* "type-in-lex"  */
    yUNION = 558,                  /* "union"  */
    yUNIQUE = 559,                 /* "unique"  */
    yUNIQUE0 = 560,                /* "unique0"  */
    yUNSIGNED = 561,               /* "unsigned"  */
    yUNTIL = 562,                  /* "until"  */
    yUNTIL_WITH = 563,             /* "until_with"  */
    yUNTYPED = 564,                /* "untyped"  */
    yUSE = 565,                    /* "use"  */
    yVAR = 566,                    /* "var"  */
    yVECTORED = 567,               /* "vectored"  */
    yVIRTUAL__CLASS = 568,         /* "virtual-then-class"  */
    yVIRTUAL__ETC = 569,           /* "virtual"  */
    yVIRTUAL__INTERFACE = 570,     /* "virtual-then-interface"  */
    yVIRTUAL__LEX = 571,           /* "virtual-in-lex"  */
    yVIRTUAL__anyID = 572,         /* "virtual-then-identifier"  */
    yVOID = 573,                   /* "void"  */
    yWAIT = 574,                   /* "wait"  */
    yWAIT_ORDER = 575,             /* "wait_order"  */
    yWAND = 576,                   /* "wand"  */
    yWEAK = 577,                   /* "weak"  */
    yWEAK0 = 578,                  /* "weak0"  */
    yWEAK1 = 579,                  /* "weak1"  */
    yWHILE = 580,                  /* "while"  */
    yWILDCARD = 581,               /* "wildcard"  */
    yWIRE = 582,                   /* "wire"  */
    yWITHIN = 583,                 /* "within"  */
    yWITH__BRA = 584,              /* "with-then-["  */
    yWITH__CUR = 585,              /* "with-then-{"  */
    yWITH__ETC = 586,              /* "with"  */
    yWITH__LEX = 587,              /* "with-in-lex"  */
    yWITH__PAREN = 588,            /* "with-then-("  */
    yWOR = 589,                    /* "wor"  */
    yWREAL = 590,                  /* "wreal"  */
    yXNOR = 591,                   /* "xnor"  */
    yXOR = 592,                    /* "xor"  */
    yD_ACOS = 593,                 /* "$acos"  */
    yD_ACOSH = 594,                /* "$acosh"  */
    yD_ASIN = 595,                 /* "$asin"  */
    yD_ASINH = 596,                /* "$asinh"  */
    yD_ASSERTCTL = 597,            /* "$assertcontrol"  */
    yD_ASSERTKILL = 598,           /* "$assertkill"  */
    yD_ASSERTOFF = 599,            /* "$assertoff"  */
    yD_ASSERTON = 600,             /* "$asserton"  */
    yD_ATAN = 601,                 /* "$atan"  */
    yD_ATAN2 = 602,                /* "$atan2"  */
    yD_ATANH = 603,                /* "$atanh"  */
    yD_BITS = 604,                 /* "$bits"  */
    yD_BITSTOREAL = 605,           /* "$bitstoreal"  */
    yD_BITSTOSHORTREAL = 606,      /* "$bitstoshortreal"  */
    yD_C = 607,                    /* "$c"  */
    yD_CAST = 608,                 /* "$cast"  */
    yD_CEIL = 609,                 /* "$ceil"  */
    yD_CHANGED = 610,              /* "$changed"  */
    yD_CHANGED_GCLK = 611,         /* "$changed_gclk"  */
    yD_CHANGING_GCLK = 612,        /* "$changing_gclk"  */
    yD_CLOG2 = 613,                /* "$clog2"  */
    yD_COS = 614,                  /* "$cos"  */
    yD_COSH = 615,                 /* "$cosh"  */
    yD_COUNTBITS = 616,            /* "$countbits"  */
    yD_COUNTONES = 617,            /* "$countones"  */
    yD_DIMENSIONS = 618,           /* "$dimensions"  */
    yD_DISPLAY = 619,              /* "$display"  */
    yD_DISPLAYB = 620,             /* "$displayb"  */
    yD_DISPLAYH = 621,             /* "$displayh"  */
    yD_DISPLAYO = 622,             /* "$displayo"  */
    yD_DIST_CHI_SQUARE = 623,      /* "$dist_chi_square"  */
    yD_DIST_ERLANG = 624,          /* "$dist_erlang"  */
    yD_DIST_EXPONENTIAL = 625,     /* "$dist_exponential"  */
    yD_DIST_NORMAL = 626,          /* "$dist_normal"  */
    yD_DIST_POISSON = 627,         /* "$dist_poisson"  */
    yD_DIST_T = 628,               /* "$dist_t"  */
    yD_DIST_UNIFORM = 629,         /* "$dist_uniform"  */
    yD_DUMPALL = 630,              /* "$dumpall"  */
    yD_DUMPFILE = 631,             /* "$dumpfile"  */
    yD_DUMPFLUSH = 632,            /* "$dumpflush"  */
    yD_DUMPLIMIT = 633,            /* "$dumplimit"  */
    yD_DUMPOFF = 634,              /* "$dumpoff"  */
    yD_DUMPON = 635,               /* "$dumpon"  */
    yD_DUMPPORTS = 636,            /* "$dumpports"  */
    yD_DUMPVARS = 637,             /* "$dumpvars"  */
    yD_ERROR = 638,                /* "$error"  */
    yD_EXIT = 639,                 /* "$exit"  */
    yD_EXP = 640,                  /* "$exp"  */
    yD_FALLING_GCLK = 641,         /* "$falling_gclk"  */
    yD_FATAL = 642,                /* "$fatal"  */
    yD_FCLOSE = 643,               /* "$fclose"  */
    yD_FDISPLAY = 644,             /* "$fdisplay"  */
    yD_FDISPLAYB = 645,            /* "$fdisplayb"  */
    yD_FDISPLAYH = 646,            /* "$fdisplayh"  */
    yD_FDISPLAYO = 647,            /* "$fdisplayo"  */
    yD_FELL = 648,                 /* "$fell"  */
    yD_FELL_GCLK = 649,            /* "$fell_gclk"  */
    yD_FEOF = 650,                 /* "$feof"  */
    yD_FERROR = 651,               /* "$ferror"  */
    yD_FFLUSH = 652,               /* "$fflush"  */
    yD_FGETC = 653,                /* "$fgetc"  */
    yD_FGETS = 654,                /* "$fgets"  */
    yD_FINISH = 655,               /* "$finish"  */
    yD_FLOOR = 656,                /* "$floor"  */
    yD_FMONITOR = 657,             /* "$fmonitor"  */
    yD_FMONITORB = 658,            /* "$fmonitorb"  */
    yD_FMONITORH = 659,            /* "$fmonitorh"  */
    yD_FMONITORO = 660,            /* "$fmonitoro"  */
    yD_FOPEN = 661,                /* "$fopen"  */
    yD_FREAD = 662,                /* "$fread"  */
    yD_FREWIND = 663,              /* "$frewind"  */
    yD_FSCANF = 664,               /* "$fscanf"  */
    yD_FSEEK = 665,                /* "$fseek"  */
    yD_FSTROBE = 666,              /* "$fstrobe"  */
    yD_FSTROBEB = 667,             /* "$fstrobeb"  */
    yD_FSTROBEH = 668,             /* "$fstrobeh"  */
    yD_FSTROBEO = 669,             /* "$fstrobeo"  */
    yD_FTELL = 670,                /* "$ftell"  */
    yD_FUTURE_GCLK = 671,          /* "$future_gclk"  */
    yD_FWRITE = 672,               /* "$fwrite"  */
    yD_FWRITEB = 673,              /* "$fwriteb"  */
    yD_FWRITEH = 674,              /* "$fwriteh"  */
    yD_FWRITEO = 675,              /* "$fwriteo"  */
    yD_GLOBAL_CLOCK = 676,         /* "$global_clock"  */
    yD_HIGH = 677,                 /* "$high"  */
    yD_HYPOT = 678,                /* "$hypot"  */
    yD_INCREMENT = 679,            /* "$increment"  */
    yD_INFERRED_DISABLE = 680,     /* "$inferred_disable"  */
    yD_INFO = 681,                 /* "$info"  */
    yD_ISUNBOUNDED = 682,          /* "$isunbounded"  */
    yD_ISUNKNOWN = 683,            /* "$isunknown"  */
    yD_ITOR = 684,                 /* "$itor"  */
    yD_LEFT = 685,                 /* "$left"  */
    yD_LN = 686,                   /* "$ln"  */
    yD_LOG10 = 687,                /* "$log10"  */
    yD_LOW = 688,                  /* "$low"  */
    yD_MONITOR = 689,              /* "$monitor"  */
    yD_MONITORB = 690,             /* "$monitorb"  */
    yD_MONITORH = 691,             /* "$monitorh"  */
    yD_MONITORO = 692,             /* "$monitoro"  */
    yD_MONITOROFF = 693,           /* "$monitoroff"  */
    yD_MONITORON = 694,            /* "$monitoron"  */
    yD_ONEHOT = 695,               /* "$onehot"  */
    yD_ONEHOT0 = 696,              /* "$onehot0"  */
    yD_PAST = 697,                 /* "$past"  */
    yD_PAST_GCLK = 698,            /* "$past_gclk"  */
    yD_POW = 699,                  /* "$pow"  */
    yD_PRINTTIMESCALE = 700,       /* "$printtimescale"  */
    yD_RANDOM = 701,               /* "$random"  */
    yD_READMEMB = 702,             /* "$readmemb"  */
    yD_READMEMH = 703,             /* "$readmemh"  */
    yD_REALTIME = 704,             /* "$realtime"  */
    yD_REALTOBITS = 705,           /* "$realtobits"  */
    yD_REWIND = 706,               /* "$rewind"  */
    yD_RIGHT = 707,                /* "$right"  */
    yD_RISING_GCLK = 708,          /* "$rising_gclk"  */
    yD_ROOT = 709,                 /* "$root"  */
    yD_ROSE = 710,                 /* "$rose"  */
    yD_ROSE_GCLK = 711,            /* "$rose_gclk"  */
    yD_RTOI = 712,                 /* "$rtoi"  */
    yD_SAMPLED = 713,              /* "$sampled"  */
    yD_SDF_ANNOTATE = 714,         /* "$sdf_annotate"  */
    yD_SETUPHOLD = 715,            /* "$setuphold"  */
    yD_SFORMAT = 716,              /* "$sformat"  */
    yD_SFORMATF = 717,             /* "$sformatf"  */
    yD_SHORTREALTOBITS = 718,      /* "$shortrealtobits"  */
    yD_SIGNED = 719,               /* "$signed"  */
    yD_SIN = 720,                  /* "$sin"  */
    yD_SINH = 721,                 /* "$sinh"  */
    yD_SIZE = 722,                 /* "$size"  */
    yD_SQRT = 723,                 /* "$sqrt"  */
    yD_SSCANF = 724,               /* "$sscanf"  */
    yD_STABLE = 725,               /* "$stable"  */
    yD_STABLE_GCLK = 726,          /* "$stable_gclk"  */
    yD_STACKTRACE = 727,           /* "$stacktrace"  */
    yD_STEADY_GCLK = 728,          /* "$steady_gclk"  */
    yD_STIME = 729,                /* "$stime"  */
    yD_STOP = 730,                 /* "$stop"  */
    yD_STROBE = 731,               /* "$strobe"  */
    yD_STROBEB = 732,              /* "$strobeb"  */
    yD_STROBEH = 733,              /* "$strobeh"  */
    yD_STROBEO = 734,              /* "$strobeo"  */
    yD_SWRITE = 735,               /* "$swrite"  */
    yD_SWRITEB = 736,              /* "$swriteb"  */
    yD_SWRITEH = 737,              /* "$swriteh"  */
    yD_SWRITEO = 738,              /* "$swriteo"  */
    yD_SYSTEM = 739,               /* "$system"  */
    yD_TAN = 740,                  /* "$tan"  */
    yD_TANH = 741,                 /* "$tanh"  */
    yD_TESTPLUSARGS = 742,         /* "$test$plusargs"  */
    yD_TIME = 743,                 /* "$time"  */
    yD_TIMEFORMAT = 744,           /* "$timeformat"  */
    yD_TIMEPRECISION = 745,        /* "$timeprecision"  */
    yD_TIMEUNIT = 746,             /* "$timeunit"  */
    yD_TYPENAME = 747,             /* "$typename"  */
    yD_UNGETC = 748,               /* "$ungetc"  */
    yD_UNIT = 749,                 /* "$unit"  */
    yD_UNPACKED_DIMENSIONS = 750,  /* "$unpacked_dimensions"  */
    yD_UNSIGNED = 751,             /* "$unsigned"  */
    yD_URANDOM = 752,              /* "$urandom"  */
    yD_URANDOM_RANGE = 753,        /* "$urandom_range"  */
    yD_VALUEPLUSARGS = 754,        /* "$value$plusargs"  */
    yD_WARNING = 755,              /* "$warning"  */
    yD_WRITE = 756,                /* "$write"  */
    yD_WRITEB = 757,               /* "$writeb"  */
    yD_WRITEH = 758,               /* "$writeh"  */
    yD_WRITEMEMB = 759,            /* "$writememb"  */
    yD_WRITEMEMH = 760,            /* "$writememh"  */
    yD_WRITEO = 761,               /* "$writeo"  */
    yVL_CLOCKER = 762,             /* "/\*verilator clocker*\/"  */
    yVL_CLOCK_ENABLE = 763,        /* "/\*verilator clock_enable*\/"  */
    yVL_COVERAGE_BLOCK_OFF = 764,  /* "/\*verilator coverage_block_off*\/"  */
    yVL_FORCEABLE = 765,           /* "/\*verilator forceable*\/"  */
    yVL_FULL_CASE = 766,           /* "/\*verilator full_case*\/"  */
    yVL_HIER_BLOCK = 767,          /* "/\*verilator hier_block*\/"  */
    yVL_INLINE_MODULE = 768,       /* "/\*verilator inline_module*\/"  */
    yVL_ISOLATE_ASSIGNMENTS = 769, /* "/\*verilator isolate_assignments*\/"  */
    yVL_NO_CLOCKER = 770,          /* "/\*verilator no_clocker*\/"  */
    yVL_NO_INLINE_MODULE = 771,    /* "/\*verilator no_inline_module*\/"  */
    yVL_NO_INLINE_TASK = 772,      /* "/\*verilator no_inline_task*\/"  */
    yVL_PARALLEL_CASE = 773,       /* "/\*verilator parallel_case*\/"  */
    yVL_PUBLIC = 774,              /* "/\*verilator public*\/"  */
    yVL_PUBLIC_FLAT = 775,         /* "/\*verilator public_flat*\/"  */
    yVL_PUBLIC_FLAT_ON = 776,      /* "/\*verilator public_flat_on*\/"  */
    yVL_PUBLIC_FLAT_RD = 777,      /* "/\*verilator public_flat_rd*\/"  */
    yVL_PUBLIC_FLAT_RD_ON = 778,   /* "/\*verilator public_flat_rd_on*\/"  */
    yVL_PUBLIC_FLAT_RW = 779,      /* "/\*verilator public_flat_rw*\/"  */
    yVL_PUBLIC_FLAT_RW_ON = 780,   /* "/\*verilator public_flat_rw_on*\/"  */
    yVL_PUBLIC_FLAT_RW_ON_SNS = 781, /* "/\*verilator public_flat_rw_on_sns*\/"  */
    yVL_PUBLIC_ON = 782,           /* "/\*verilator public_on*\/"  */
    yVL_PUBLIC_OFF = 783,          /* "/\*verilator public_off*\/"  */
    yVL_PUBLIC_MODULE = 784,       /* "/\*verilator public_module*\/"  */
    yVL_SC_BV = 785,               /* "/\*verilator sc_bv*\/"  */
    yVL_SFORMAT = 786,             /* "/\*verilator sformat*\/"  */
    yVL_SPLIT_VAR = 787,           /* "/\*verilator split_var*\/"  */
    yVL_TAG = 788,                 /* "/\*verilator tag*\/"  */
    yVL_UNROLL_DISABLE = 789,      /* "/\*verilator unroll_disable*\/"  */
    yVL_UNROLL_FULL = 790,         /* "/\*verilator unroll_full*\/"  */
    yP_TICK = 791,                 /* "'"  */
    yP_TICKBRA = 792,              /* "'{"  */
    yP_OROR = 793,                 /* "||"  */
    yP_ANDAND = 794,               /* "&&"  */
    yP_NOR = 795,                  /* "~|"  */
    yP_XNOR = 796,                 /* "^~"  */
    yP_NAND = 797,                 /* "~&"  */
    yP_EQUAL = 798,                /* "=="  */
    yP_NOTEQUAL = 799,             /* "!="  */
    yP_CASEEQUAL = 800,            /* "==="  */
    yP_CASENOTEQUAL = 801,         /* "!=="  */
    yP_WILDEQUAL = 802,            /* "==?"  */
    yP_WILDNOTEQUAL = 803,         /* "!=?"  */
    yP_GTE = 804,                  /* ">="  */
    yP_LTE = 805,                  /* "<="  */
    yP_LTE__IGNORE = 806,          /* "<=-ignored"  */
    yP_SLEFT = 807,                /* "<<"  */
    yP_SRIGHT = 808,               /* ">>"  */
    yP_SSRIGHT = 809,              /* ">>>"  */
    yP_POW = 810,                  /* "**"  */
    yP_COLON__BEGIN = 811,         /* ":-then-begin"  */
    yP_COLON__FORK = 812,          /* ":-then-fork"  */
    yP_EQ__NEW = 813,              /* "=-then-new"  */
    yP_PAR__IGNORE = 814,          /* "(-ignored"  */
    yP_PAR__STRENGTH = 815,        /* "(-for-strength"  */
    yP_LTMINUSGT = 816,            /* "<->"  */
    yP_PLUSCOLON = 817,            /* "+:"  */
    yP_MINUSCOLON = 818,           /* "-:"  */
    yP_MINUSGT = 819,              /* "->"  */
    yP_MINUSGTGT = 820,            /* "->>"  */
    yP_EQGT = 821,                 /* "=>"  */
    yP_ASTGT = 822,                /* "*>"  */
    yP_ANDANDAND = 823,            /* "&&&"  */
    yP_POUNDPOUND = 824,           /* "##"  */
    yP_POUNDMINUSPD = 825,         /* "#-#"  */
    yP_POUNDEQPD = 826,            /* "#=#"  */
    yP_DOTSTAR = 827,              /* ".*"  */
    yP_ATAT = 828,                 /* "@@"  */
    yP_COLONCOLON = 829,           /* "::"  */
    yP_COLONEQ = 830,              /* ":="  */
    yP_COLONDIV = 831,             /* ":/"  */
    yP_ORMINUSGT = 832,            /* "|->"  */
    yP_OREQGT = 833,               /* "|=>"  */
    yP_BRASTAR = 834,              /* "[*"  */
    yP_BRAEQ = 835,                /* "[="  */
    yP_BRAMINUSGT = 836,           /* "[->"  */
    yP_BRAPLUSKET = 837,           /* "[+]"  */
    yP_PLUSPLUS = 838,             /* "++"  */
    yP_MINUSMINUS = 839,           /* "--"  */
    yP_PLUSEQ = 840,               /* "+="  */
    yP_MINUSEQ = 841,              /* "-="  */
    yP_TIMESEQ = 842,              /* "*="  */
    yP_DIVEQ = 843,                /* "/="  */
    yP_MODEQ = 844,                /* "%="  */
    yP_ANDEQ = 845,                /* "&="  */
    yP_OREQ = 846,                 /* "|="  */
    yP_XOREQ = 847,                /* "^="  */
    yP_SLEFTEQ = 848,              /* "<<="  */
    yP_SRIGHTEQ = 849,             /* ">>="  */
    yP_SSRIGHTEQ = 850,            /* ">>>="  */
    yP_PLUSSLASHMINUS = 851,       /* "+/-"  */
    yP_PLUSPCTMINUS = 852,         /* "+%-"  */
    prTAGGED = 853,                /* prTAGGED  */
    prPOUNDPOUND_MULTI = 854,      /* prPOUNDPOUND_MULTI  */
    prUNARYARITH = 855,            /* prUNARYARITH  */
    prREDUCTION = 856,             /* prREDUCTION  */
    prNEGATION = 857,              /* prNEGATION  */
    prLOWER_THAN_ELSE = 858        /* prLOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_V3PARSEBISON_PRETMP_H_INCLUDED  */
