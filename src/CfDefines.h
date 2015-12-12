

#ifndef DEFINES_H
#define DEFINES_H

#define PROJECT_CAPTION _("CircuitFinder")

#define DEFAULT_FTP_PORT		21
#define DEFAULT_SFTP_PORT		22

#define MAX_RECENT_PROJECT	10
#define MAX_URL_NUM			5

enum t_rev_ctrl_id
{
	ID_Start = 0
	,ID_MessageWin
	,ID_WorkspaceExplorer 
	,ID_FileNotebook
	,ID_ElementProperty
	,ID_LayerView
	,ID_SettingDialog	//ID_Next = ID_SettingDialog + 50
	,ID_PropertyDialog = ID_SettingDialog + 50	
};

enum
{
	MENU_ID
	,MENU_ID_NewWorkspace
	,MENU_ID_OpenWorkspace
	,MENU_ID_CloseWorkspace
	,MENU_ID_SaveAll
    ,MENU_ID_Print//add print
	,MENU_ID_Preview//add preview
	,MENU_ID_RecentFiles
	,MENU_ID_NewProject
	,MENU_ID_OpenProject
	,MENU_ID_AddFolder
	,MENU_ID_AddNewFile
	,MENU_ID_AddExistingFile
	,MENU_ID_AddClassifier
	,MENU_ID_BackupProject		//MING20110415
	,MENU_ID_BackupWorkspace	//MING20110415
	,MENU_ID_OpenFile
	,MENU_ID_RemoveItem
	,MENU_ID_UploadProject
	,MENU_ID_UploadItem
	,MENU_ID_SetActiveProject
	,MENU_ID_CloseProject
	,MENU_ID_SaveProject
	,MENU_ID_OpenProperty
	,MENU_ID_RefreshWS
	,MENU_ID_SortProject		//MING_20110810
	,MENU_ID_Rename

	,MENU_ID_AnalyzeProject
	,MENU_ID_GenerateScript
	,MENU_ID_OpenWSProperty
	,MENU_ID_OpenWSRefresh
	,MENU_ID_FindNext
	,MENU_ID_FindInFiles
	,MENU_ID_Goto
    // editor menu IDs
    ,MENU_ID_PROPERTIES 
    ,MENU_ID_INDENTINC,
    MENU_ID_INDENTRED,
    MENU_ID_FINDNEXT,
    MENU_ID_REPLACE,
    MENU_ID_REPLACENEXT,
    MENU_ID_BRACEMATCH,
    MENU_ID_GOTO,
    MENU_ID_PAGEACTIVE,
    MENU_ID_DISPLAYEOL,
    MENU_ID_INDENTGUIDE,
    MENU_ID_LINENUMBER,
    MENU_ID_LONGLINEON,
    MENU_ID_WHITESPACE,
    MENU_ID_FOLDTOGGLE,
    MENU_ID_OVERTYPE,
    MENU_ID_READONLY,
    MENU_ID_WRAPMODEON,
    MENU_ID_CHANGECASE,
    MENU_ID_CHANGELOWER,
    MENU_ID_CHANGEUPPER,
    MENU_ID_HILIGHTLANG,
    MENU_ID_HILIGHTFIRST,
    MENU_ID_HILIGHTLAST = MENU_ID_HILIGHTFIRST + 99,
    MENU_ID_CONVERTEOL,
    MENU_ID_CONVERTCR,
    MENU_ID_CONVERTCRLF,
    MENU_ID_CONVERTLF,
    MENU_ID_USECHARSET,
    MENU_ID_CHARSETANSI,
    MENU_ID_CHARSETMAC,
    MENU_ID_PAGEPREV,
    MENU_ID_PAGENEXT,
    MENU_ID_SELECTLINE,

    // other IDs
    MENU_ID_STATUSBAR,
    MENU_ID_TITLEBAR,
    MENU_ID_ABOUTTIMER,
    MENU_ID_UPDATETIMER,

    // dialog find IDs
    MENU_ID_DLG_FIND_TEXT,

    // preferences IDs
    MENU_ID_PREFS_LANGUAGE,
    MENU_ID_PREFS_STYLETYPE,
    MENU_ID_PREFS_KEYWORDS,

	//Window
    MENU_ID_NewVerticalTabGroup,
    MENU_ID_NewHorizontalTabGroup,

	//View
	MENU_ID_ZOOMIN,
	MENU_ID_ZOOMOUT,
	MENU_ID_GRID,

	//Draw
	MENU_ID_PARAMETER,
	MENU_ID_ADDLAYER,
	MENU_ID_ADDIMAGE,
	MENU_ID_ADDSYMBOL,
	MENU_ID_PORT,
	MENU_ID_WIRE,
	MENU_ID_LINK,
	MENU_ID_SYMBOL,
	MENU_ID_SAVESYMBOL,
	//Help
	MENU_ID_Manual,

	//Setting
	MENU_ID_SettingPan,

    MENU_ID_END
};
// type names for grid table values
#define cfGRID_VALUE_STRING     wxT("string")
#define cfGRID_VALUE_BOOL       wxT("bool")
#define cfGRID_VALUE_NUMBER     wxT("long")
#define cfGRID_VALUE_FLOAT      wxT("double")
#define cfGRID_VALUE_CHOICE     wxT("choice")

#define cfSTRING_STAR			'*'
#define cfSTRING_GROUP			_("Group")
#define cfSTRING_COMMON			_("Common")
//Shape
#define cfSTRING_APPEARANCE		_("Appearance")
#define cfSTRING_SHAPE			_("Shape")
#define cfSTRING_RECTANGLE		_("Rectangle")
#define cfSTRING_LINE_SEGMENT	_("LineSegment")
#define cfSTRING_INPUT_PORT		_("InputPort")
#define cfSTRING_OUTPUT_PORT	_("OutputPort")
#define cfSTRING_INOUT_PORT		_("InoutPort")
//Element
#define cfSTRING_ELEMENTS		_("Elements")
#define cfSTRING_IMAGE			_("Image")
#define cfSTRING_COLOR			_("Color")
#define cfSTRING_BACKGROUND		_("Background")
#define cfSTRING_SOLIDCOLOR_BACKGROUND _("SolidColorBackground")
#define cfSTRING_PORT			_("Port")
#define cfSTRING_WIRE			_("Wire")
#define cfSTRING_SYMBOL			_("Symbol")
#define cfSTRING_INSTANCE		_("Instance")
#define cfSTRING_LIBRARY		_("Library")
#define cfSTRING_LAYER			_("Layer")
#define cfSTRING_ID				_("ID")
#define cfSTRING_NAME			_("Name")
#define cfSTRING_PATH			_("Path")
#define cfSTRING_DIRECTION		_("Direction")
#define cfSTRING_PORT_WIDTH		_("Port Width")
#define cfSTRING_X_POS			_("X Pos")
#define cfSTRING_Y_POS			_("Y Pos")
#define cfSTRING_START_X_POS	_("Start X Pos")
#define cfSTRING_START_Y_POS	_("Start Y Pos")
#define cfSTRING_END_X_POS		_("End X Pos")
#define cfSTRING_END_Y_POS		_("End Y Pos")
#define cfSTRING_HEIGHT			_("Height")
#define cfSTRING_WIDTH			_("Width")
#define cfSTRING_PORT_WIDTH		_("Port Width")
#define cfSTRING_FILLED_COLOR	_("Filled Color")
#define cfSTRING_LINE_COLOR		_("Line Color")
#define cfSTRING_LINE_WIDTH		_("Line Width")
#define cfSTRING_LINE_REALLINE	_("Real Line")
//Image properties
#define cfSTRING_DEPTH			_("Depth")
#define cfSTRING_NONE			_("None")
//Property
#define cfSTRING_PORPERTY		_("Property")
//Parameter
#define cfSTRING_PARAMETER			_("Parameter")
#define cfSTRING_PARAMETER_ITEM		_("Item")
#define cfSTRING_PARAMETER_NAME		_("Name")
#define cfSTRING_PARAMETER_VALUE	_("Value")
#define cfSTRING_PARAMETER_TYPE		_("Type")

#undef wxUSE_PRINTING_ARCHITECTURE
//#define wxUSE_PRINTING_ARCHITECTURE 0
#endif //DEFINES_H