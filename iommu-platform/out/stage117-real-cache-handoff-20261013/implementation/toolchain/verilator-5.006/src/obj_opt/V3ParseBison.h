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
    yaID__aTYPE = 262,             /* "TYPE-IDENTIFIER"  */
    yaINTNUM = 263,                /* "INTEGER NUMBER"  */
    yaTIMENUM = 264,               /* "TIME NUMBER"  */
    yaSTRING = 265,                /* "STRING"  */
    yaSTRING__IGNORE = 266,        /* "STRING-ignored"  */
    yaTIMINGSPEC = 267,            /* "TIMING SPEC ELEMENT"  */
    ygenSTRENGTH = 268,            /* "STRENGTH keyword (strong1/etc)"  */
    yaTABLELINE = 269,             /* "TABLE LINE"  */
    yaSCHDR = 270,                 /* "`systemc_header BLOCK"  */
    yaSCINT = 271,                 /* "`systemc_ctor BLOCK"  */
    yaSCIMP = 272,                 /* "`systemc_dtor BLOCK"  */
    yaSCIMPH = 273,                /* "`systemc_interface BLOCK"  */
    yaSCCTOR = 274,                /* "`systemc_implementation BLOCK"  */
    yaSCDTOR = 275,                /* "`systemc_imp_header BLOCK"  */
    yVLT_CLOCKER = 276,            /* "clocker"  */
    yVLT_CLOCK_ENABLE = 277,       /* "clock_enable"  */
    yVLT_COVERAGE_BLOCK_OFF = 278, /* "coverage_block_off"  */
    yVLT_COVERAGE_OFF = 279,       /* "coverage_off"  */
    yVLT_COVERAGE_ON = 280,        /* "coverage_on"  */
    yVLT_FORCEABLE = 281,          /* "forceable"  */
    yVLT_FULL_CASE = 282,          /* "full_case"  */
    yVLT_HIER_BLOCK = 283,         /* "hier_block"  */
    yVLT_INLINE = 284,             /* "inline"  */
    yVLT_ISOLATE_ASSIGNMENTS = 285, /* "isolate_assignments"  */
    yVLT_LINT_OFF = 286,           /* "lint_off"  */
    yVLT_LINT_ON = 287,            /* "lint_on"  */
    yVLT_NO_CLOCKER = 288,         /* "no_clocker"  */
    yVLT_NO_INLINE = 289,          /* "no_inline"  */
    yVLT_PARALLEL_CASE = 290,      /* "parallel_case"  */
    yVLT_PROFILE_DATA = 291,       /* "profile_data"  */
    yVLT_PUBLIC = 292,             /* "public"  */
    yVLT_PUBLIC_FLAT = 293,        /* "public_flat"  */
    yVLT_PUBLIC_FLAT_RD = 294,     /* "public_flat_rd"  */
    yVLT_PUBLIC_FLAT_RW = 295,     /* "public_flat_rw"  */
    yVLT_PUBLIC_MODULE = 296,      /* "public_module"  */
    yVLT_SC_BV = 297,              /* "sc_bv"  */
    yVLT_SFORMAT = 298,            /* "sformat"  */
    yVLT_SPLIT_VAR = 299,          /* "split_var"  */
    yVLT_TIMING_OFF = 300,         /* "timing_off"  */
    yVLT_TIMING_ON = 301,          /* "timing_on"  */
    yVLT_TRACING_OFF = 302,        /* "tracing_off"  */
    yVLT_TRACING_ON = 303,         /* "tracing_on"  */
    yVLT_D_BLOCK = 304,            /* "--block"  */
    yVLT_D_COST = 305,             /* "--cost"  */
    yVLT_D_FILE = 306,             /* "--file"  */
    yVLT_D_FUNCTION = 307,         /* "--function"  */
    yVLT_D_LEVELS = 308,           /* "--levels"  */
    yVLT_D_LINES = 309,            /* "--lines"  */
    yVLT_D_MATCH = 310,            /* "--match"  */
    yVLT_D_MODEL = 311,            /* "--model"  */
    yVLT_D_MODULE = 312,           /* "--module"  */
    yVLT_D_MTASK = 313,            /* "--mtask"  */
    yVLT_D_RULE = 314,             /* "--rule"  */
    yVLT_D_SCOPE = 315,            /* "--scope"  */
    yVLT_D_TASK = 316,             /* "--task"  */
    yVLT_D_VAR = 317,              /* "--var"  */
    yaD_PLI = 318,                 /* "${pli-system}"  */
    yaT_NOUNCONNECTED = 319,       /* "`nounconnecteddrive"  */
    yaT_RESETALL = 320,            /* "`resetall"  */
    yaT_UNCONNECTED_PULL0 = 321,   /* "`unconnected_drive pull0"  */
    yaT_UNCONNECTED_PULL1 = 322,   /* "`unconnected_drive pull1"  */
    ya1STEP = 323,                 /* "1step"  */
    yALIAS = 324,                  /* "alias"  */
    yALWAYS = 325,                 /* "always"  */
    yALWAYS_COMB = 326,            /* "always_comb"  */
    yALWAYS_FF = 327,              /* "always_ff"  */
    yALWAYS_LATCH = 328,           /* "always_latch"  */
    yAND = 329,                    /* "and"  */
    yASSERT = 330,                 /* "assert"  */
    yASSIGN = 331,                 /* "assign"  */
    yASSUME = 332,                 /* "assume"  */
    yAUTOMATIC = 333,              /* "automatic"  */
    yBEFORE = 334,                 /* "before"  */
    yBEGIN = 335,                  /* "begin"  */
    yBIND = 336,                   /* "bind"  */
    yBIT = 337,                    /* "bit"  */
    yBREAK = 338,                  /* "break"  */
    yBUF = 339,                    /* "buf"  */
    yBUFIF0 = 340,                 /* "bufif0"  */
    yBUFIF1 = 341,                 /* "bufif1"  */
    yBYTE = 342,                   /* "byte"  */
    yCASE = 343,                   /* "case"  */
    yCASEX = 344,                  /* "casex"  */
    yCASEZ = 345,                  /* "casez"  */
    yCHANDLE = 346,                /* "chandle"  */
    yCLASS = 347,                  /* "class"  */
    yCLOCKING = 348,               /* "clocking"  */
    yCMOS = 349,                   /* "cmos"  */
    yCONSTRAINT = 350,             /* "constraint"  */
    yCONST__ETC = 351,             /* "const"  */
    yCONST__LEX = 352,             /* "const-in-lex"  */
    yCONST__REF = 353,             /* "const-then-ref"  */
    yCONTEXT = 354,                /* "context"  */
    yCONTINUE = 355,               /* "continue"  */
    yCOVER = 356,                  /* "cover"  */
    yDEASSIGN = 357,               /* "deassign"  */
    yDEFAULT = 358,                /* "default"  */
    yDEFPARAM = 359,               /* "defparam"  */
    yDISABLE = 360,                /* "disable"  */
    yDIST = 361,                   /* "dist"  */
    yDO = 362,                     /* "do"  */
    yEDGE = 363,                   /* "edge"  */
    yELSE = 364,                   /* "else"  */
    yEND = 365,                    /* "end"  */
    yENDCASE = 366,                /* "endcase"  */
    yENDCLASS = 367,               /* "endclass"  */
    yENDCLOCKING = 368,            /* "endclocking"  */
    yENDFUNCTION = 369,            /* "endfunction"  */
    yENDGENERATE = 370,            /* "endgenerate"  */
    yENDINTERFACE = 371,           /* "endinterface"  */
    yENDMODULE = 372,              /* "endmodule"  */
    yENDPACKAGE = 373,             /* "endpackage"  */
    yENDPRIMITIVE = 374,           /* "endprimitive"  */
    yENDPROGRAM = 375,             /* "endprogram"  */
    yENDPROPERTY = 376,            /* "endproperty"  */
    yENDSPECIFY = 377,             /* "endspecify"  */
    yENDTABLE = 378,               /* "endtable"  */
    yENDTASK = 379,                /* "endtask"  */
    yENUM = 380,                   /* "enum"  */
    yEVENT = 381,                  /* "event"  */
    yEXPORT = 382,                 /* "export"  */
    yEXTENDS = 383,                /* "extends"  */
    yEXTERN = 384,                 /* "extern"  */
    yFINAL = 385,                  /* "final"  */
    yFOR = 386,                    /* "for"  */
    yFORCE = 387,                  /* "force"  */
    yFOREACH = 388,                /* "foreach"  */
    yFOREVER = 389,                /* "forever"  */
    yFORK = 390,                   /* "fork"  */
    yFORKJOIN = 391,               /* "forkjoin"  */
    yFUNCTION = 392,               /* "function"  */
    yGENERATE = 393,               /* "generate"  */
    yGENVAR = 394,                 /* "genvar"  */
    yGLOBAL__CLOCKING = 395,       /* "global-then-clocking"  */
    yGLOBAL__ETC = 396,            /* "global"  */
    yGLOBAL__LEX = 397,            /* "global-in-lex"  */
    yHIGHZ0 = 398,                 /* "highz0"  */
    yHIGHZ1 = 399,                 /* "highz1"  */
    yIF = 400,                     /* "if"  */
    yIFF = 401,                    /* "iff"  */
    yIMPLEMENTS = 402,             /* "implements"  */
    yIMPORT = 403,                 /* "import"  */
    yINITIAL = 404,                /* "initial"  */
    yINOUT = 405,                  /* "inout"  */
    yINPUT = 406,                  /* "input"  */
    yINSIDE = 407,                 /* "inside"  */
    yINT = 408,                    /* "int"  */
    yINTEGER = 409,                /* "integer"  */
    yINTERFACE = 410,              /* "interface"  */
    yJOIN = 411,                   /* "join"  */
    yJOIN_ANY = 412,               /* "join_any"  */
    yJOIN_NONE = 413,              /* "join_none"  */
    yLOCALPARAM = 414,             /* "localparam"  */
    yLOCAL__COLONCOLON = 415,      /* "local-then-::"  */
    yLOCAL__ETC = 416,             /* "local"  */
    yLOCAL__LEX = 417,             /* "local-in-lex"  */
    yLOGIC = 418,                  /* "logic"  */
    yLONGINT = 419,                /* "longint"  */
    yMODPORT = 420,                /* "modport"  */
    yMODULE = 421,                 /* "module"  */
    yNAND = 422,                   /* "nand"  */
    yNEGEDGE = 423,                /* "negedge"  */
    yNEW__ETC = 424,               /* "new"  */
    yNEW__LEX = 425,               /* "new-in-lex"  */
    yNEW__PAREN = 426,             /* "new-then-paren"  */
    yNMOS = 427,                   /* "nmos"  */
    yNOR = 428,                    /* "nor"  */
    yNOT = 429,                    /* "not"  */
    yNOTIF0 = 430,                 /* "notif0"  */
    yNOTIF1 = 431,                 /* "notif1"  */
    yNULL = 432,                   /* "null"  */
    yOR = 433,                     /* "or"  */
    yOUTPUT = 434,                 /* "output"  */
    yPACKAGE = 435,                /* "package"  */
    yPACKED = 436,                 /* "packed"  */
    yPARAMETER = 437,              /* "parameter"  */
    yPMOS = 438,                   /* "pmos"  */
    yPOSEDGE = 439,                /* "posedge"  */
    yPRIMITIVE = 440,              /* "primitive"  */
    yPRIORITY = 441,               /* "priority"  */
    yPROGRAM = 442,                /* "program"  */
    yPROPERTY = 443,               /* "property"  */
    yPROTECTED = 444,              /* "protected"  */
    yPULL0 = 445,                  /* "pull0"  */
    yPULL1 = 446,                  /* "pull1"  */
    yPULLDOWN = 447,               /* "pulldown"  */
    yPULLUP = 448,                 /* "pullup"  */
    yPURE = 449,                   /* "pure"  */
    yRAND = 450,                   /* "rand"  */
    yRANDC = 451,                  /* "randc"  */
    yRANDCASE = 452,               /* "randcase"  */
    yRANDOMIZE = 453,              /* "randomize"  */
    yRCMOS = 454,                  /* "rcmos"  */
    yREAL = 455,                   /* "real"  */
    yREALTIME = 456,               /* "realtime"  */
    yREF = 457,                    /* "ref"  */
    yREG = 458,                    /* "reg"  */
    yRELEASE = 459,                /* "release"  */
    yREPEAT = 460,                 /* "repeat"  */
    yRESTRICT = 461,               /* "restrict"  */
    yRETURN = 462,                 /* "return"  */
    yRNMOS = 463,                  /* "rnmos"  */
    yRPMOS = 464,                  /* "rpmos"  */
    yRTRAN = 465,                  /* "rtran"  */
    yRTRANIF0 = 466,               /* "rtranif0"  */
    yRTRANIF1 = 467,               /* "rtranif1"  */
    ySCALARED = 468,               /* "scalared"  */
    ySHORTINT = 469,               /* "shortint"  */
    ySHORTREAL = 470,              /* "shortreal"  */
    ySIGNED = 471,                 /* "signed"  */
    ySOFT = 472,                   /* "soft"  */
    ySOLVE = 473,                  /* "solve"  */
    ySPECIFY = 474,                /* "specify"  */
    ySPECPARAM = 475,              /* "specparam"  */
    ySTATIC__CONSTRAINT = 476,     /* "static-then-constraint"  */
    ySTATIC__ETC = 477,            /* "static"  */
    ySTATIC__LEX = 478,            /* "static-in-lex"  */
    ySTRING = 479,                 /* "string"  */
    ySTRONG0 = 480,                /* "strong0"  */
    ySTRONG1 = 481,                /* "strong1"  */
    ySTRUCT = 482,                 /* "struct"  */
    ySUPER = 483,                  /* "super"  */
    ySUPPLY0 = 484,                /* "supply0"  */
    ySUPPLY1 = 485,                /* "supply1"  */
    yTABLE = 486,                  /* "table"  */
    yTASK = 487,                   /* "task"  */
    yTHIS = 488,                   /* "this"  */
    yTIME = 489,                   /* "time"  */
    yTIMEPRECISION = 490,          /* "timeprecision"  */
    yTIMEUNIT = 491,               /* "timeunit"  */
    yTRAN = 492,                   /* "tran"  */
    yTRANIF0 = 493,                /* "tranif0"  */
    yTRANIF1 = 494,                /* "tranif1"  */
    yTRI = 495,                    /* "tri"  */
    yTRI0 = 496,                   /* "tri0"  */
    yTRI1 = 497,                   /* "tri1"  */
    yTRIAND = 498,                 /* "triand"  */
    yTRIOR = 499,                  /* "trior"  */
    yTRIREG = 500,                 /* "trireg"  */
    yTRUE = 501,                   /* "true"  */
    yTYPE = 502,                   /* "type"  */
    yTYPEDEF = 503,                /* "typedef"  */
    yUNION = 504,                  /* "union"  */
    yUNIQUE = 505,                 /* "unique"  */
    yUNIQUE0 = 506,                /* "unique0"  */
    yUNSIGNED = 507,               /* "unsigned"  */
    yUNTYPED = 508,                /* "untyped"  */
    yVAR = 509,                    /* "var"  */
    yVECTORED = 510,               /* "vectored"  */
    yVIRTUAL__CLASS = 511,         /* "virtual-then-class"  */
    yVIRTUAL__ETC = 512,           /* "virtual"  */
    yVIRTUAL__INTERFACE = 513,     /* "virtual-then-interface"  */
    yVIRTUAL__LEX = 514,           /* "virtual-in-lex"  */
    yVIRTUAL__anyID = 515,         /* "virtual-then-identifier"  */
    yVOID = 516,                   /* "void"  */
    yWAIT = 517,                   /* "wait"  */
    yWAND = 518,                   /* "wand"  */
    yWEAK0 = 519,                  /* "weak0"  */
    yWEAK1 = 520,                  /* "weak1"  */
    yWHILE = 521,                  /* "while"  */
    yWIRE = 522,                   /* "wire"  */
    yWITH__BRA = 523,              /* "with-then-["  */
    yWITH__CUR = 524,              /* "with-then-{"  */
    yWITH__ETC = 525,              /* "with"  */
    yWITH__LEX = 526,              /* "with-in-lex"  */
    yWITH__PAREN = 527,            /* "with-then-("  */
    yWOR = 528,                    /* "wor"  */
    yWREAL = 529,                  /* "wreal"  */
    yXNOR = 530,                   /* "xnor"  */
    yXOR = 531,                    /* "xor"  */
    yD_ACOS = 532,                 /* "$acos"  */
    yD_ACOSH = 533,                /* "$acosh"  */
    yD_ASIN = 534,                 /* "$asin"  */
    yD_ASINH = 535,                /* "$asinh"  */
    yD_ATAN = 536,                 /* "$atan"  */
    yD_ATAN2 = 537,                /* "$atan2"  */
    yD_ATANH = 538,                /* "$atanh"  */
    yD_BITS = 539,                 /* "$bits"  */
    yD_BITSTOREAL = 540,           /* "$bitstoreal"  */
    yD_BITSTOSHORTREAL = 541,      /* "$bitstoshortreal"  */
    yD_C = 542,                    /* "$c"  */
    yD_CAST = 543,                 /* "$cast"  */
    yD_CEIL = 544,                 /* "$ceil"  */
    yD_CHANGED = 545,              /* "$changed"  */
    yD_CLOG2 = 546,                /* "$clog2"  */
    yD_COS = 547,                  /* "$cos"  */
    yD_COSH = 548,                 /* "$cosh"  */
    yD_COUNTBITS = 549,            /* "$countbits"  */
    yD_COUNTONES = 550,            /* "$countones"  */
    yD_DIMENSIONS = 551,           /* "$dimensions"  */
    yD_DISPLAY = 552,              /* "$display"  */
    yD_DISPLAYB = 553,             /* "$displayb"  */
    yD_DISPLAYH = 554,             /* "$displayh"  */
    yD_DISPLAYO = 555,             /* "$displayo"  */
    yD_DIST_CHI_SQUARE = 556,      /* "$dist_chi_square"  */
    yD_DIST_ERLANG = 557,          /* "$dist_erlang"  */
    yD_DIST_EXPONENTIAL = 558,     /* "$dist_exponential"  */
    yD_DIST_NORMAL = 559,          /* "$dist_normal"  */
    yD_DIST_POISSON = 560,         /* "$dist_poisson"  */
    yD_DIST_T = 561,               /* "$dist_t"  */
    yD_DIST_UNIFORM = 562,         /* "$dist_uniform"  */
    yD_DUMPALL = 563,              /* "$dumpall"  */
    yD_DUMPFILE = 564,             /* "$dumpfile"  */
    yD_DUMPFLUSH = 565,            /* "$dumpflush"  */
    yD_DUMPLIMIT = 566,            /* "$dumplimit"  */
    yD_DUMPOFF = 567,              /* "$dumpoff"  */
    yD_DUMPON = 568,               /* "$dumpon"  */
    yD_DUMPPORTS = 569,            /* "$dumpports"  */
    yD_DUMPVARS = 570,             /* "$dumpvars"  */
    yD_ERROR = 571,                /* "$error"  */
    yD_EXIT = 572,                 /* "$exit"  */
    yD_EXP = 573,                  /* "$exp"  */
    yD_FATAL = 574,                /* "$fatal"  */
    yD_FCLOSE = 575,               /* "$fclose"  */
    yD_FDISPLAY = 576,             /* "$fdisplay"  */
    yD_FDISPLAYB = 577,            /* "$fdisplayb"  */
    yD_FDISPLAYH = 578,            /* "$fdisplayh"  */
    yD_FDISPLAYO = 579,            /* "$fdisplayo"  */
    yD_FELL = 580,                 /* "$fell"  */
    yD_FEOF = 581,                 /* "$feof"  */
    yD_FERROR = 582,               /* "$ferror"  */
    yD_FFLUSH = 583,               /* "$fflush"  */
    yD_FGETC = 584,                /* "$fgetc"  */
    yD_FGETS = 585,                /* "$fgets"  */
    yD_FINISH = 586,               /* "$finish"  */
    yD_FLOOR = 587,                /* "$floor"  */
    yD_FMONITOR = 588,             /* "$fmonitor"  */
    yD_FMONITORB = 589,            /* "$fmonitorb"  */
    yD_FMONITORH = 590,            /* "$fmonitorh"  */
    yD_FMONITORO = 591,            /* "$fmonitoro"  */
    yD_FOPEN = 592,                /* "$fopen"  */
    yD_FREAD = 593,                /* "$fread"  */
    yD_FREWIND = 594,              /* "$frewind"  */
    yD_FSCANF = 595,               /* "$fscanf"  */
    yD_FSEEK = 596,                /* "$fseek"  */
    yD_FSTROBE = 597,              /* "$fstrobe"  */
    yD_FSTROBEB = 598,             /* "$fstrobeb"  */
    yD_FSTROBEH = 599,             /* "$fstrobeh"  */
    yD_FSTROBEO = 600,             /* "$fstrobeo"  */
    yD_FTELL = 601,                /* "$ftell"  */
    yD_FWRITE = 602,               /* "$fwrite"  */
    yD_FWRITEB = 603,              /* "$fwriteb"  */
    yD_FWRITEH = 604,              /* "$fwriteh"  */
    yD_FWRITEO = 605,              /* "$fwriteo"  */
    yD_HIGH = 606,                 /* "$high"  */
    yD_HYPOT = 607,                /* "$hypot"  */
    yD_INCREMENT = 608,            /* "$increment"  */
    yD_INFO = 609,                 /* "$info"  */
    yD_ISUNBOUNDED = 610,          /* "$isunbounded"  */
    yD_ISUNKNOWN = 611,            /* "$isunknown"  */
    yD_ITOR = 612,                 /* "$itor"  */
    yD_LEFT = 613,                 /* "$left"  */
    yD_LN = 614,                   /* "$ln"  */
    yD_LOG10 = 615,                /* "$log10"  */
    yD_LOW = 616,                  /* "$low"  */
    yD_MONITOR = 617,              /* "$monitor"  */
    yD_MONITORB = 618,             /* "$monitorb"  */
    yD_MONITORH = 619,             /* "$monitorh"  */
    yD_MONITORO = 620,             /* "$monitoro"  */
    yD_MONITOROFF = 621,           /* "$monitoroff"  */
    yD_MONITORON = 622,            /* "$monitoron"  */
    yD_ONEHOT = 623,               /* "$onehot"  */
    yD_ONEHOT0 = 624,              /* "$onehot0"  */
    yD_PAST = 625,                 /* "$past"  */
    yD_POW = 626,                  /* "$pow"  */
    yD_PRINTTIMESCALE = 627,       /* "$printtimescale"  */
    yD_RANDOM = 628,               /* "$random"  */
    yD_READMEMB = 629,             /* "$readmemb"  */
    yD_READMEMH = 630,             /* "$readmemh"  */
    yD_REALTIME = 631,             /* "$realtime"  */
    yD_REALTOBITS = 632,           /* "$realtobits"  */
    yD_REWIND = 633,               /* "$rewind"  */
    yD_RIGHT = 634,                /* "$right"  */
    yD_ROOT = 635,                 /* "$root"  */
    yD_ROSE = 636,                 /* "$rose"  */
    yD_RTOI = 637,                 /* "$rtoi"  */
    yD_SAMPLED = 638,              /* "$sampled"  */
    yD_SFORMAT = 639,              /* "$sformat"  */
    yD_SFORMATF = 640,             /* "$sformatf"  */
    yD_SHORTREALTOBITS = 641,      /* "$shortrealtobits"  */
    yD_SIGNED = 642,               /* "$signed"  */
    yD_SIN = 643,                  /* "$sin"  */
    yD_SINH = 644,                 /* "$sinh"  */
    yD_SIZE = 645,                 /* "$size"  */
    yD_SQRT = 646,                 /* "$sqrt"  */
    yD_SSCANF = 647,               /* "$sscanf"  */
    yD_STABLE = 648,               /* "$stable"  */
    yD_STACKTRACE = 649,           /* "$stacktrace"  */
    yD_STIME = 650,                /* "$stime"  */
    yD_STOP = 651,                 /* "$stop"  */
    yD_STROBE = 652,               /* "$strobe"  */
    yD_STROBEB = 653,              /* "$strobeb"  */
    yD_STROBEH = 654,              /* "$strobeh"  */
    yD_STROBEO = 655,              /* "$strobeo"  */
    yD_SWRITE = 656,               /* "$swrite"  */
    yD_SWRITEB = 657,              /* "$swriteb"  */
    yD_SWRITEH = 658,              /* "$swriteh"  */
    yD_SWRITEO = 659,              /* "$swriteo"  */
    yD_SYSTEM = 660,               /* "$system"  */
    yD_TAN = 661,                  /* "$tan"  */
    yD_TANH = 662,                 /* "$tanh"  */
    yD_TESTPLUSARGS = 663,         /* "$test$plusargs"  */
    yD_TIME = 664,                 /* "$time"  */
    yD_TIMEFORMAT = 665,           /* "$timeformat"  */
    yD_TIMEPRECISION = 666,        /* "$timeprecision"  */
    yD_TIMEUNIT = 667,             /* "$timeunit"  */
    yD_TYPENAME = 668,             /* "$typename"  */
    yD_UNGETC = 669,               /* "$ungetc"  */
    yD_UNIT = 670,                 /* "$unit"  */
    yD_UNPACKED_DIMENSIONS = 671,  /* "$unpacked_dimensions"  */
    yD_UNSIGNED = 672,             /* "$unsigned"  */
    yD_URANDOM = 673,              /* "$urandom"  */
    yD_URANDOM_RANGE = 674,        /* "$urandom_range"  */
    yD_VALUEPLUSARGS = 675,        /* "$value$plusargs"  */
    yD_WARNING = 676,              /* "$warning"  */
    yD_WRITE = 677,                /* "$write"  */
    yD_WRITEB = 678,               /* "$writeb"  */
    yD_WRITEH = 679,               /* "$writeh"  */
    yD_WRITEMEMB = 680,            /* "$writememb"  */
    yD_WRITEMEMH = 681,            /* "$writememh"  */
    yD_WRITEO = 682,               /* "$writeo"  */
    yVL_CLOCKER = 683,             /* "/\*verilator clocker*\/"  */
    yVL_CLOCK_ENABLE = 684,        /* "/\*verilator clock_enable*\/"  */
    yVL_COVERAGE_BLOCK_OFF = 685,  /* "/\*verilator coverage_block_off*\/"  */
    yVL_FORCEABLE = 686,           /* "/\*verilator forceable*\/"  */
    yVL_FULL_CASE = 687,           /* "/\*verilator full_case*\/"  */
    yVL_HIER_BLOCK = 688,          /* "/\*verilator hier_block*\/"  */
    yVL_INLINE_MODULE = 689,       /* "/\*verilator inline_module*\/"  */
    yVL_ISOLATE_ASSIGNMENTS = 690, /* "/\*verilator isolate_assignments*\/"  */
    yVL_NO_CLOCKER = 691,          /* "/\*verilator no_clocker*\/"  */
    yVL_NO_INLINE_MODULE = 692,    /* "/\*verilator no_inline_module*\/"  */
    yVL_NO_INLINE_TASK = 693,      /* "/\*verilator no_inline_task*\/"  */
    yVL_PARALLEL_CASE = 694,       /* "/\*verilator parallel_case*\/"  */
    yVL_PUBLIC = 695,              /* "/\*verilator public*\/"  */
    yVL_PUBLIC_FLAT = 696,         /* "/\*verilator public_flat*\/"  */
    yVL_PUBLIC_FLAT_RD = 697,      /* "/\*verilator public_flat_rd*\/"  */
    yVL_PUBLIC_FLAT_RW = 698,      /* "/\*verilator public_flat_rw*\/"  */
    yVL_PUBLIC_MODULE = 699,       /* "/\*verilator public_module*\/"  */
    yVL_SC_BV = 700,               /* "/\*verilator sc_bv*\/"  */
    yVL_SFORMAT = 701,             /* "/\*verilator sformat*\/"  */
    yVL_SPLIT_VAR = 702,           /* "/\*verilator split_var*\/"  */
    yVL_TAG = 703,                 /* "/\*verilator tag*\/"  */
    yVL_TRACE_INIT_TASK = 704,     /* "/\*verilator trace_init_task*\/"  */
    yP_TICK = 705,                 /* "'"  */
    yP_TICKBRA = 706,              /* "'{"  */
    yP_OROR = 707,                 /* "||"  */
    yP_ANDAND = 708,               /* "&&"  */
    yP_NOR = 709,                  /* "~|"  */
    yP_XNOR = 710,                 /* "^~"  */
    yP_NAND = 711,                 /* "~&"  */
    yP_EQUAL = 712,                /* "=="  */
    yP_NOTEQUAL = 713,             /* "!="  */
    yP_CASEEQUAL = 714,            /* "==="  */
    yP_CASENOTEQUAL = 715,         /* "!=="  */
    yP_WILDEQUAL = 716,            /* "==?"  */
    yP_WILDNOTEQUAL = 717,         /* "!=?"  */
    yP_GTE = 718,                  /* ">="  */
    yP_LTE = 719,                  /* "<="  */
    yP_LTE__IGNORE = 720,          /* "<=-ignored"  */
    yP_SLEFT = 721,                /* "<<"  */
    yP_SRIGHT = 722,               /* ">>"  */
    yP_SSRIGHT = 723,              /* ">>>"  */
    yP_POW = 724,                  /* "**"  */
    yP_COLON__BEGIN = 725,         /* ":-begin"  */
    yP_COLON__FORK = 726,          /* ":-fork"  */
    yP_PAR__STRENGTH = 727,        /* "(-for-strength"  */
    yP_LTMINUSGT = 728,            /* "<->"  */
    yP_PLUSCOLON = 729,            /* "+:"  */
    yP_MINUSCOLON = 730,           /* "-:"  */
    yP_MINUSGT = 731,              /* "->"  */
    yP_MINUSGTGT = 732,            /* "->>"  */
    yP_EQGT = 733,                 /* "=>"  */
    yP_ASTGT = 734,                /* "*>"  */
    yP_ANDANDAND = 735,            /* "&&&"  */
    yP_POUNDPOUND = 736,           /* "##"  */
    yP_DOTSTAR = 737,              /* ".*"  */
    yP_ATAT = 738,                 /* "@@"  */
    yP_COLONCOLON = 739,           /* "::"  */
    yP_COLONEQ = 740,              /* ":="  */
    yP_COLONDIV = 741,             /* ":/"  */
    yP_ORMINUSGT = 742,            /* "|->"  */
    yP_OREQGT = 743,               /* "|=>"  */
    yP_BRASTAR = 744,              /* "[*"  */
    yP_BRAEQ = 745,                /* "[="  */
    yP_BRAMINUSGT = 746,           /* "[->"  */
    yP_PLUSPLUS = 747,             /* "++"  */
    yP_MINUSMINUS = 748,           /* "--"  */
    yP_PLUSEQ = 749,               /* "+="  */
    yP_MINUSEQ = 750,              /* "-="  */
    yP_TIMESEQ = 751,              /* "*="  */
    yP_DIVEQ = 752,                /* "/="  */
    yP_MODEQ = 753,                /* "%="  */
    yP_ANDEQ = 754,                /* "&="  */
    yP_OREQ = 755,                 /* "|="  */
    yP_XOREQ = 756,                /* "^="  */
    yP_SLEFTEQ = 757,              /* "<<="  */
    yP_SRIGHTEQ = 758,             /* ">>="  */
    yP_SSRIGHTEQ = 759,            /* ">>>="  */
    prUNARYARITH = 760,            /* prUNARYARITH  */
    prREDUCTION = 761,             /* prREDUCTION  */
    prNEGATION = 762,              /* prNEGATION  */
    prLOWER_THAN_ELSE = 763        /* prLOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_V3PARSEBISON_PRETMP_H_INCLUDED  */
