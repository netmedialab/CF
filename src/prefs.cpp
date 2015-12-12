//////////////////////////////////////////////////////////////////////////////
// File:        contrib/samples/stc/prefs.cpp
// Purpose:     STC test Preferences initialization
// Maintainer:  Wyo
// Created:     2003-09-01
// RCS-ID:      $Id: prefs.cpp 37440 2006-02-10 11:59:52Z ABX $
// Copyright:   (c) wxGuide
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//! wxWidgets headers

//! wxWidgets/contrib headers

//! application headers
#include "defsext.h"     // Additional definitions
#include "prefs.h"       // Preferences


//============================================================================
// declarations
//============================================================================

//----------------------------------------------------------------------------
//! language types
CommonInfo g_EditorCommonPrefs = {
    // editor functionality prefs
    true,  // syntaxEnable
    true,  // foldEnable
    true,  // indentEnable
    // display defaults prefs
    false, // overTypeInitial
    false, // readOnlyInitial
    false,  // wrapModeInitial
    false, // displayEOLEnable
    false, // IndentGuideEnable
    true,  // lineNumberEnable
    false, // longLineOnEnable
    false, // whiteSpaceEnable
};

//----------------------------------------------------------------------------
// keywordlists
// C++
wxChar* CppWordlist1 =
    _T("asm auto bool break case catch char class const const_cast ")
    _T("continue default delete do double dynamic_cast else enum explicit ")
    _T("export extern false float for friend goto if inline int long ")
    _T("mutable namespace new operator private protected public register ")
    _T("reinterpret_cast return short signed sizeof static static_cast ")
    _T("struct switch template this throw true try typedef typeid ")
    _T("typename union unsigned using virtual void volatile wchar_t ")
    _T("while");
wxChar* CppWordlist2 =
    _T("file");
wxChar* CppWordlist3 =
    _T("a addindex addtogroup anchor arg attention author b brief bug c ")
    _T("class code date def defgroup deprecated dontinclude e em endcode ")
    _T("endhtmlonly endif endlatexonly endlink endverbatim enum example ")
    _T("exception f$ f[ f] file fn hideinitializer htmlinclude ")
    _T("htmlonly if image include ingroup internal invariant interface ")
    _T("latexonly li line link mainpage name namespace nosubgrouping note ")
    _T("overload p page par param post pre ref relates remarks return ")
    _T("retval sa section see showinitializer since skip skipline struct ")
    _T("subsection test throw todo typedef union until var verbatim ")
    _T("verbinclude version warning weakgroup $ @ \"\" & < > # { }");

// Python
wxChar* PythonWordlist1 =
    _T("and assert break class continue def del elif else except exec ")
    _T("finally for from global if import in is lambda None not or pass ")
    _T("print raise return try while yield");
wxChar* PythonWordlist2 =
    _T("ACCELERATORS ALT AUTO3STATE AUTOCHECKBOX AUTORADIOBUTTON BEGIN ")
    _T("BITMAP BLOCK BUTTON CAPTION CHARACTERISTICS CHECKBOX CLASS ")
    _T("COMBOBOX CONTROL CTEXT CURSOR DEFPUSHBUTTON DIALOG DIALOGEX ")
    _T("DISCARDABLE EDITTEXT END EXSTYLE FONT GROUPBOX ICON LANGUAGE ")
    _T("LISTBOX LTEXT MENU MENUEX MENUITEM MESSAGETABLE POPUP PUSHBUTTON ")
    _T("RADIOBUTTON RCDATA RTEXT SCROLLBAR SEPARATOR SHIFT STATE3 ")
    _T("STRINGTABLE STYLE TEXTINCLUDE VALUE VERSION VERSIONINFO VIRTKEY");


wxChar* VerilogWordlist1 =
	_T("always and assign automatic ")
	_T("begin buf bufif0 bufif1 \
		case casex casez cell cmos config \
		deassign default defparam design disable \
		edge else end endcase endconfig endfunction endgenerate endmodule endprimitive endspecify endtable endtask event \
		for force forever fork function \
		generate genvar \
		highz0 highz1 \
		if ifnone incdir include initial inout input instance integer \
		join \
		large liblist library localparam \
		macromodule medium module \
		nand negedge nmos nor noshowcancelled not notif0 notif1 \
		or output \
		parameter pmos posedge primitive pull0 pull1 pulldown pullup pulsestyle_ondetect pulsestyle_onevent \
		rcmos real realtime reg release repeat rnmos rpmos rtran rtranif0 rtranif1 \
		scalared showcancelled signed small specify specparam strong0 strong1 supply0 supply1 \
		table task time tran tranif0 tranif1 tri tri0 tri1 triand trior trireg \
		unsigned use \
		vectored \
		wait wand weak0 weak1 while wire wor \
		xnor xor \
		always_comb always_ff always_latch assert");

wxChar* VerilogWordlist2 = 
	_T("$async$and$array $async$and$plane $async$nand$array $async$nand$plane $async$nor$array $async$nor$plane $async$or$array $async$or$plane \
		$bitstoreal \
		$countdrivers \
		$display $displayb $displayh $displayo \
		$dist_chi_square $dist_erlang $dist_exponential $dist_normal $dist_poisson $dist_t $dist_uniform \
		$dumpall $dumpfile $dumpflush $dumplimit $dumpoff $dumpon $dumpportsall $dumpportsflush $dumpportslimit $dumpportsoff $dumpportson $dumpvars \
		$fclose $fdisplayh $fdisplay $fdisplayf $fdisplayb $ferror $fflush $fgetc $fgets $finish $fmonitorb $fmonitor $fmonitorf $fmonitorh $fopen $fread $fscanf $fseek $fsscanf $fstrobe $fstrobebb $fstrobef $fstrobeh $ftel $fullskew $fwriteb $fwritef $fwriteh $fwrite \
		$getpattern \
		$history $hold \
		$incsave $input $itor \
		$key \
		$list $log \
		$monitorb $monitorh $monitoroff $monitoron $monitor $monitoro \
		$nochange $nokey $nolog \
		$period $printtimescale \
		$q_add $q_exam $q_full $q_initialize $q_remove \
		$random $readmemb $readmemh $readmemh $realtime $realtobits $recovery $recrem $removal $reset_count $reset $reset_value $restart $rewind $rtoi \
		$save $scale $scope $sdf_annotate $setup $setuphold $sformat $showscopes $showvariables $showvars $signed $skew $sreadmemb $sreadmemh $stime $stop $strobeb $strobe $strobeh $strobeo $swriteb $swriteh $swriteo $swrite $sync$and$array $sync$and$plane $sync$nand$array $sync$nand$plane $sync$nor$array $sync$nor$plane $sync$or$array $sync$or$plane \
		$test$plusargs $time $timeformat $timeskew \
		$ungetc $unsigned \
		$value$plusargs \
		$width $writeb $writeh $write $writeo");

////////////////////////////////////
////systemVerilog keyword
wxChar* SystemVerilogWordlist =
	_T("alias assume before bind bins binsof bit break byte chandle class clocking const constraint context continue cover covergroup coverpoint cross dist do endclass endclocking endgroup endinterface endpackage endprimitive \
		endprogram endproperty endsequence enum expect export extends extern final first_match foreach forkjoin iff ignore_bins illegal_bins import inside int interface intersect join_any join_none local logic longint matches \
		modport new null package packed priority program property protected pure rand randc randcase randsequence ref return sequence shortint \
		shortreal solve static string struct super tagged this throughout timeprecision timeunit type typedef union unique var virtual void wait_order wildcard with within \
	    ");

wxChar* VhdlWordlist1 =
	_T("access after alias all architecture array assert attribute begin block \
		body buffer bus case component configuration constant disconnect downto else elsif end entity exit file \
		for function generate generic group guarded if impure in inertial inout is label library linkage literal \
		loop map new next null of on open others out package port postponed procedure process pure range record \
		register reject report return select severity shared signal subtype then to transport type unaffected \
		units until use variable wait when while with");

wxChar* VhdlWordlist2 =
	_T("abs and mod nand nor not or rem rol ror sla sll sra srl xnor xor");
wxChar* VhdlWordlist3 =
	_T("left right low high ascending image value pos val succ pred leftof rightof base range reverse_range	\
		length delayed stable quiet transaction event active last_event last_active last_value driving \
		driving_value simple_name path_name instance_name");
wxChar* VhdlWordlist4 =
	_T("now readline read writeline write endfile resolved to_bit to_bitvector to_stdulogic to_stdlogicvector      \
		to_stdulogicvector to_x01 to_x01z to_UX01 rising_edge falling_edge is_x shift_left shift_right rotate_left \
		rotate_right resize to_integer to_unsigned to_signed std_match to_01");
wxChar* VhdlWordlist5 =
	_T("std ieee work standard textio std_logic_1164 std_logic_arith std_logic_misc std_logic_signed               \
		std_logic_textio std_logic_unsigned numeric_bit numeric_std math_complex math_real vital_primitives        \
		vital_timing");
wxChar* VhdlWordlist6 =
	_T("boolean bit character severity_level integer real time delay_length natural positive string bit_vector     \
		file_open_kind file_open_status line text side width std_ulogic std_ulogic_vector std_logic                \
		std_logic_vector X01 X01Z UX01 UX01Z unsigned signed");

wxChar* BashWordlist1 =
	_T("alias \
		ar asa awk banner basename bash bc bdiff break \
		bunzip2 bzip2 cal calendar case cat cc cd chmod cksum \
		clear cmp col comm compress continue cp cpio crypt \
		csplit ctags cut date dc dd declare deroff dev df diff diff3 \
		dircmp dirname do done du echo ed egrep elif else env \
		esac eval ex exec exit expand export expr false fc \
		fgrep fi file find fmt fold for function functions \
		getconf getopt getopts grep gres hash head help \
		history iconv id if in integer jobs join kill local lc \
		let line ln logname look ls m4 mail mailx make \
		man mkdir more mt mv newgrp nl nm nohup ntps od \
		pack paste patch pathchk pax pcat perl pg pr print \
		printf ps pwd read readonly red return rev rm rmdir \
		sed select set sh shift size sleep sort source spell \
		split start stop strings strip stty sum suspend \
		sync tail tar tee test then time times touch tr \
		trap true tsort tty type typeset ulimit umask unalias \
		uname uncompress unexpand uniq unpack unset until \
		uudecode uuencode vi vim vpax wait wc whence which \
		while who wpaste wstart xargs zcat");

wxChar* BashWordlist2 =
	_T("chgrp chown chroot dir dircolors \
		factor groups hostid install link md5sum mkfifo \
		mknod nice pinky printenv ptx readlink seq \
		sha1sum shred stat su tac unlink users vdir whoami yes");
//----------------------------------------------------------------------------
//! languages
const LanguageInfo g_LanguagePrefs [] = {
    // C++
    {_T("C++"),
     _T("*.c;*.cc;*.cpp;*.cxx;*.cs;*.h;*.hh;*.hpp;*.hxx;*.sma"),
    wxSTC_LEX_CPP,
     {{TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_COMMENT, NULL},
      {TOKEN_TYPE_COMMENT_LINE, NULL},
      {TOKEN_TYPE_COMMENT_DOC, NULL},
      {TOKEN_TYPE_NUMBER, NULL},
      {TOKEN_TYPE_WORD1, CppWordlist1}, // KEYWORDS
      {TOKEN_TYPE_STRING, NULL},
      {TOKEN_TYPE_CHARACTER, NULL},
      {TOKEN_TYPE_UUID, NULL},
      {TOKEN_TYPE_PREPROCESSOR, NULL},
      {TOKEN_TYPE_OPERATOR, NULL},
      {TOKEN_TYPE_IDENTIFIER, NULL},
      {TOKEN_TYPE_STRING_EOL, NULL},
      {TOKEN_TYPE_DEFAULT, NULL}, // VERBATIM
      {TOKEN_TYPE_REGEX, NULL},
      {TOKEN_TYPE_COMMENT_SPECIAL, NULL}, // DOXY
      {TOKEN_TYPE_WORD2, CppWordlist2}, // EXTRA WORDS
      {TOKEN_TYPE_WORD3, CppWordlist3}, // DOXY KEYWORDS
      {TOKEN_TYPE_ERROR, NULL}, // KEYWORDS ERROR
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENT | TOKEN_FOLD_COMPACT | TOKEN_FOLD_PREPROC},
    // Verilog
    {_T("Verilog|Systemverilog"),
     _T("*.v;*.V;*.vh;*.sv;*.SV"),
	wxSTC_LEX_VERILOG,
     {{TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_COMMENT, NULL},
      {TOKEN_TYPE_COMMENT_LINE, NULL},
      {TOKEN_TYPE_COMMENT_SPECIAL, NULL},	// Bang comment
      {TOKEN_TYPE_NUMBER, NULL},
      {TOKEN_TYPE_WORD1, VerilogWordlist1}, // KEYWORDS
      {TOKEN_TYPE_STRING, NULL},
      {TOKEN_TYPE_WORD2, VerilogWordlist2}, // EXTRA WORDS
      {TOKEN_TYPE_WORD1, SystemVerilogWordlist}, // System Verilog WORDS  cheng 20120321
      {TOKEN_TYPE_ERROR, NULL},				// System tasks
      {TOKEN_TYPE_PREPROCESSOR, NULL},
      {TOKEN_TYPE_OPERATOR, NULL},
      {TOKEN_TYPE_IDENTIFIER, NULL},
      {TOKEN_TYPE_STRING_EOL, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
//      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENT | TOKEN_FOLD_COMPACT | TOKEN_FOLD_PREPROC},
    // VHDL
    {_T("VHDL"),
     _T("*.vhd;*.VHD;*.vhdl;*.VHDL"),
	 wxSTC_LEX_VHDL,
     {{TOKEN_TYPE_DEFAULT, NULL},			// white space
      {TOKEN_TYPE_COMMENT, NULL},
      {TOKEN_TYPE_COMMENT_SPECIAL, NULL},	// Bang comment
      {TOKEN_TYPE_NUMBER, NULL},
      {TOKEN_TYPE_STRING, NULL},
      {-1, NULL},							// Operators
      {-1, NULL},							// Identifiers
      {TOKEN_TYPE_STRING_EOL, NULL},
      {TOKEN_TYPE_WORD1, VhdlWordlist1}, // KEYWORDS
      {TOKEN_TYPE_WORD2, VhdlWordlist2}, // Std operator
      {TOKEN_TYPE_WORD3, VhdlWordlist3}, // Attribute
      {TOKEN_TYPE_WORD4, VhdlWordlist4}, // Std Function
      {TOKEN_TYPE_WORD5, VhdlWordlist5}, // Std Package
      {TOKEN_TYPE_WORD6, VhdlWordlist6}, // Std Type
      {TOKEN_TYPE_ERROR, NULL},			 // User defined identifiers and tasks
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENT | TOKEN_FOLD_COMPACT | TOKEN_FOLD_PREPROC},
    // Makefile
    {_T("Makefile"),
     _T("makefile;Makefile;*.mak;configure*"),
	 wxSTC_LEX_MAKEFILE,
     {{TOKEN_TYPE_DEFAULT, NULL},			// white space
      {TOKEN_TYPE_COMMENT, NULL},
      {TOKEN_TYPE_PREPROCESSOR, NULL},		// Pre-processor or other comment: !
      {TOKEN_TYPE_COMMAND, NULL},			// Variable: $(x)
      {TOKEN_TYPE_OPERATOR, NULL},			// Operators
      {TOKEN_TYPE_ERROR, NULL},				// Target
      {TOKEN_TYPE_CHARACTER, NULL},			// Error
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENT},
    // Shell
    {_T("BASH"),
     _T("*.sh;*.bsh;configure"),
	 wxSTC_LEX_BASH,
     {{TOKEN_TYPE_DEFAULT, NULL},			// white space
      {TOKEN_TYPE_ERROR, NULL},				// Error
      {TOKEN_TYPE_COMMENT, NULL},
      {TOKEN_TYPE_NUMBER, NULL},
      {TOKEN_TYPE_WORD1, BashWordlist1},	// KEYWORDS
      {TOKEN_TYPE_STRING, NULL},
      {TOKEN_TYPE_CHARACTER, NULL},
      {TOKEN_TYPE_OPERATOR, NULL},			// Operators
      {TOKEN_TYPE_IDENTIFIER, NULL},
      {TOKEN_TYPE_COMMAND, NULL},			// Scalars: $var
      {TOKEN_TYPE_COMMAND, NULL},			// Parameter expansion: ${var}
      {TOKEN_TYPE_PUNCTUATION, NULL},		// Back Ticks
	  {TOKEN_TYPE_DELIMITER, NULL},			// Here-doc (delimiter)
      {TOKEN_TYPE_VALUE, NULL},				// Here-doc (single quoted, q)
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENT},
	// Python
    {_T("Python"),
     _T("*.py;*.pyw"),
     wxSTC_LEX_PYTHON,
     {{TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_COMMENT_LINE, NULL},
      {TOKEN_TYPE_NUMBER, NULL},
      {TOKEN_TYPE_STRING, NULL},
      {TOKEN_TYPE_CHARACTER, NULL},
      {TOKEN_TYPE_WORD1, PythonWordlist1}, // KEYWORDS
      {TOKEN_TYPE_DEFAULT, NULL}, // TRIPLE
      {TOKEN_TYPE_DEFAULT, NULL}, // TRIPLEDOUBLE
      {TOKEN_TYPE_DEFAULT, NULL}, // CLASSNAME
      {TOKEN_TYPE_DEFAULT, PythonWordlist2}, // DEFNAME
      {TOKEN_TYPE_OPERATOR, NULL},
      {TOKEN_TYPE_IDENTIFIER, NULL},
      {TOKEN_TYPE_DEFAULT, NULL}, // COMMENT_BLOCK
      {TOKEN_TYPE_STRING_EOL, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     TOKEN_FOLD_COMMENTPY | TOKEN_FOLD_QUOTESPY},
    // * (any)
    { DEFAULT_LANGUAGE,
     _T("*.*"),
     wxSTC_LEX_PROPERTIES,
     {{TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_DEFAULT, NULL},
      {TOKEN_TYPE_DEFAULT, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL},
      {-1, NULL}},
     0},
    };

const int g_LanguagePrefsSize = WXSIZEOF(g_LanguagePrefs);

//----------------------------------------------------------------------------
//! style types
const StyleInfo g_StylePrefs [] = {
    // TOKEN_TYPE_DEFAULT
    {_T("Default"),
     _T("BLACK"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_WORD1
    {_T("Keyword1"),
     _T("BLUE"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_TYPE_WORD2
    {_T("Keyword2"),
     _T("DARK BLUE"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_TYPE_WORD3
    {_T("Keyword3"),
     _T("CORNFLOWER BLUE"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_WORD4
    {_T("Keyword4"),
     _T("CYAN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_WORD5
    {_T("Keyword5"),
     _T("DARK GREY"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_TYPE_WORD6
    {_T("Keyword6"),
     _T("GREY"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_COMMENT
    {_T("Comment"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_COMMENT_DOC
    {_T("Comment (Doc)"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_COMMENT_LINE
    {_T("Comment line"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_COMMENT_SPECIAL
    {_T("Special comment"),
     _T("FOREST GREEN"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_TYPE_CHARACTER
    {_T("Character"),
     _T("KHAKI"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_CHARACTER_EOL
    {_T("Character (EOL)"),
     _T("KHAKI"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_STRING
    {_T("String"),
     _T("BROWN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_STRING_EOL
    {_T("String (EOL)"),
     _T("BROWN"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_DELIMITER
    {_T("Delimiter"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_PUNCTUATION
    {_T("Punctuation"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_OPERATOR
    {_T("Operator"),
     _T("BLACK"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_BOLD, 0},

    // TOKEN_TYPE_BRACE
    {_T("Label"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_COMMAND
    {_T("Command"),
     _T("BLUE"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_IDENTIFIER
    {_T("Identifier"),
     _T("BLACK"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_LABEL
    {_T("Label"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_NUMBER
    {_T("Number"),
     _T("SIENNA"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_PARAMETER
    {_T("Parameter"),
     _T("VIOLET"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_TYPE_REGEX
    {_T("Regular expression"),
     _T("ORCHID"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_UUID
    {_T("UUID"),
     _T("ORCHID"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_VALUE
    {_T("Value"),
     _T("ORCHID"), _T("WHITE"),
     _T(""), 10, TOKEN_STYLE_ITALIC, 0},

    // TOKEN_TYPE_PREPROCESSOR
    {_T("Preprocessor"),
     _T("GREY"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_SCRIPT
    {_T("Script"),
     _T("DARK GREY"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_ERROR
    {_T("Error"),
     _T("RED"), _T("WHITE"),
     _T(""), 10, 0, 0},

    // TOKEN_TYPE_UNDEFINED
    {_T("Undefined"),
     _T("ORANGE"), _T("WHITE"),
     _T(""), 10, 0, 0}

    };

const int g_StylePrefsSize = WXSIZEOF(g_StylePrefs);
