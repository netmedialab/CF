 #include <stdlib.h>
 #include <crtdbg.h>
#include <wx/filename.h>
#include "CircuitFinderFrame.h"
#include "LogMessageCtrl.h"
#include "TextEditorCtrl.h"
#include "ImageEditorCtrl.h"

#include "EnvironmentUtil.h"
#include "ResourceUtil.h"
#include "FileUtil.h"
#include "Notification.h"
//#include "PropertyPortCtrl.h"
#include "systemimagelist.h"
#include "DrawingElementFactory.h"
#include <wx/stdpaths.h>
#include "NewProjectDialogCtrl.h"
#include "NewFileDialogCtrl.h"
#include "CfSettingDialogCtrl.h"
#include "NewFileInitializer.h"
//test
#include "Symbol.h"
#define CRTDBG_MAP_ALLOC
// `Main program' equivalent, creating windows and returning main app frame
bool CircuitFinderApp::OnInit()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
  // Create the main frame window
	CircuitFinderFrame *frame = new CircuitFinderFrame((argc>1)? argv[1] : wxEmptyString);

  // Show the frame
  frame->Show(true);

  return true;
}

wxBEGIN_EVENT_TABLE(CircuitFinderFrame,wxFrame)
    EVT_MENU(wxID_ANY, CircuitFinderFrame::OnClickMenu)
	EVT_CF_NOTIFICATION(wxID_ANY, CircuitFinderFrame::OnCircuitFinderEvent)
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_FileNotebook,CircuitFinderFrame::OnClosePage)
	EVT_AUINOTEBOOK_PAGE_CLOSED(ID_FileNotebook,CircuitFinderFrame::OnClosedPage)
	//Bind
	//EVT_AUINOTEBOOK_PAGE_CHANGED(ID_FileNotebook, CircuitFinderFrame::OnChangedPage)
	EVT_AUINOTEBOOK_ALLOW_DND(ID_FileNotebook, CircuitFinderFrame::OnAllowNotebookDnD)

	EVT_UPDATE_UI(MENU_ID_NewHorizontalTabGroup,CircuitFinderFrame::OnNewTabGroupUpdateUI)
	EVT_UPDATE_UI(MENU_ID_NewVerticalTabGroup,CircuitFinderFrame::OnNewTabGroupUpdateUI)
	//Drawing
	EVT_UPDATE_UI(MENU_ID_PORT,CircuitFinderFrame::OnSchematicAndSymbolEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_WIRE,CircuitFinderFrame::OnSchematicAndSymbolEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_ADDIMAGE,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_ADDSYMBOL,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_ADDLAYER,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_SYMBOL,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_LINK,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	//File
	EVT_UPDATE_UI(wxID_DELETE,CircuitFinderFrame::OnSchematicAndSymbolEditorUpdateUI)
	EVT_UPDATE_UI(wxID_SAVE,        CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_SAVEAS,      CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_CLOSE,       CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_CLOSE_ALL,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(MENU_ID_SaveAll,    CircuitFinderFrame::OnWorkspaceExistUpdateUI)
	//Edit
	EVT_UPDATE_UI(wxID_CLEAR,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_CUT,		CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_COPY,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_PASTE,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_SELECTALL,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_SELECTALL,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_REDO,	CircuitFinderFrame::OnFileExistUpdateUI)
	EVT_UPDATE_UI(wxID_UNDO,	CircuitFinderFrame::OnFileExistUpdateUI)
	//View
	EVT_UPDATE_UI(MENU_ID_ZOOMIN,CircuitFinderFrame::OnSchematicEditorUpdateUI)
	EVT_UPDATE_UI(MENU_ID_ZOOMOUT,CircuitFinderFrame::OnSchematicEditorUpdateUI)

wxEND_EVENT_TABLE()

CircuitFinderFrame::CircuitFinderFrame(wxString &workspace_file)
//: wxFrame(NULL, wxID_ANY, PROJECT_CAPTION, wxDefaultPosition, wxSize(2024, 768), REVERSIAN_FRAME_STYLE)
: wxFrame(NULL, wxID_ANY, PROJECT_CAPTION, wxDefaultPosition, wxDefaultSize, REVERSIAN_FRAME_STYLE)
,m_pWorkspace(NULL)
,m_pFileNotebook(NULL)
,m_iSettingPageNum(PID_GENERAL_PORT)
{
	//----- load environment  ----------/
	CfEnvironmentUtil::Init();
	m_pEnvironment = CfEnvironmentUtil::Get();

	InitializeComponent();
	m_pFileUtils =  new FileUtil();
	if(workspace_file.IsEmpty())
	{
		workspace_file = CfEnvironmentUtil::Get()->GetString(PROP_LASTOPENED_WORKSPACE,wxEmptyString);
		if(workspace_file.IsEmpty())
		{
			wxString dir = FileUtil::Get()->GetDirectory(ID_PROJDIR);
			workspace_file = wxString::Format(_("%s\\ChipReverse\\Example.flw"),dir);
		}
	}
	if(!workspace_file.IsEmpty())
		OpenWorkspace(workspace_file);

	this->Maximize();
}


CircuitFinderFrame::~CircuitFinderFrame(void)
{
	if(m_pWorkspace)
		CfEnvironmentUtil::Get()->SetString(PROP_LASTOPENED_WORKSPACE,m_pWorkspace->GetWorkspacePath());
	CfWorkspace::Destroy();
	//Release utils
	CfEnvironmentUtil::Destroy();
	ResourceUtil::Destroy();
	FileUtil::Destroy();
	CfIconList::Destroy();
	//Release layout manager
    m_AuiMgr.UnInit();
	CfDrawingElementFactory::DestroyDrawingElementFactory();
	if(m_pFileUtils) 
	{
		//Clear temp files
		m_pFileUtils->DelTempFiles();
		delete m_pFileUtils;
	}
}

void CircuitFinderFrame::InitializeComponent()
{
	m_AuiMgr.SetManagedWindow(this);

	//Init message window
	CfLogMessageCtrl::Init(this,ID_MessageWin);
	m_pLogMessgeWin = CfLogMessageCtrl::Get();
	//Init Workspace Explorer window
	m_pWorkspaceExplorer = new CfWorkspaceExplorerCtrl(this);
	//Init Element property window
	m_pElementPropertyCtrl= new CfPropertyGridCtrl(this);
	//Init layer view window
	m_pLayerViewCtrl = new CfLayerViewCtrl(this);
	m_pLayerListViewCtrl = new CfLayerListViewCtrl(this);

	//Set log message window
	wxLogTextCtrl *logWindow = new wxLogTextCtrl(m_pLogMessgeWin);
    delete wxLog::SetActiveTarget(logWindow);

	//Init messsage notebook
	m_pMessageNotebook = new wxAuiNotebook(this, wxID_ANY,wxDefaultPosition, wxSize(250,300),wxAUI_NB_BOTTOM);
	m_pMessageNotebook->AddPage(m_pLogMessgeWin,wxT("Message"),true,ResourceUtil::Get()->GetBitmap(IMG_MSG));

	//Init file notebook
	m_pFileNotebook = new CfFileNotebookCtrl(this, ID_FileNotebook,wxDefaultPosition,wxDefaultSize,wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_CLOSE_ON_ALL_TABS );

    m_AuiMgr.AddPane(m_pWorkspaceExplorer, wxAuiPaneInfo().
                  Name(wxT("project_tree")).Caption(wxT("Workspace Explorer")).
				  Left());
	m_AuiMgr.AddPane(m_pLayerListViewCtrl, wxAuiPaneInfo().
		          Name(wxT("layerlistview")).Caption(wxT("Layer List View")).
				  Left().Position(1));
	m_AuiMgr.AddPane(m_pFileNotebook, wxAuiPaneInfo().Name(wxT("notebook_content")).Caption(_("ddd")).
                  CenterPane().PaneBorder(false));
	m_AuiMgr.AddPane(m_pMessageNotebook, wxAuiPaneInfo().
                  Name(wxT("message_list")).Caption(wxT("Message")).
                  Bottom().
                  CloseButton(true).MaximizeButton(true));
	m_AuiMgr.AddPane(m_pElementPropertyCtrl, wxAuiPaneInfo().
		          Name(wxT("properties")).Caption(wxT("Properties")).
				  Right());
	m_AuiMgr.AddPane(m_pLayerViewCtrl, wxAuiPaneInfo().
		          Name(wxT("layerview")).Caption(wxT("Layer View")).
				  Right().Position(1));
 //                 Right(). CloseButton(true).MaximizeButton(true));
	
	AddMenu();
	AddToolBar();
	m_AuiMgr.Update();
	SetIcon(ResourceUtil::Get()->GetIcon(IMG_LOGO));

	Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED,&CircuitFinderFrame::OnChangedPage,this);

	//create status bar
	m_pStatusBar = new wxStatusBar(this);
    m_pStatusBar->SetFieldsCount(2);
    SetStatusBar(m_pStatusBar);
}


void CircuitFinderFrame::OnCircuitFinderEvent(wxEvent& event)
{
	CfNotification* pNotification= (CfNotification*)event.GetEventObject();
	wxASSERT(pNotification);
	switch (pNotification->GetID())
	{
	case cfId_OpenFile:
		{
			wxWindow *pwin = m_AuiMgr.GetManagedWindow();
			wxString class_name = typeid(*pwin).name();
			wxLogDebug(class_name);

			wxString file_path = pNotification->GetString();
			CfProject* project = (CfProject*)pNotification->GetData();
			CfFileEditorBase * reader = m_pFileNotebook->AddEditor(file_path,project);
			if(!reader) wxLogError(wxT("[File] Failed to open file - ") + file_path);
			else wxLogMessage(wxT("[File] Open ") + file_path);

			break;
		}
	case cfId_AddFile:
		{
			wxString file_path = pNotification->GetString();
			wxTreeItemId id = (CfProject*)pNotification->GetData();
			m_pWorkspaceExplorer->AddProjectItem(id,file_path,PIT_FILE,_("ChipSymbol"));
		}
	case cfId_UpdateWinTitle:
		UpdateWinTitle();
		break;
	case cfId_RefreshWorkspace:
		RefreshWS();
		break;

	//Editor
	case cfId_Editor_Modified:
		m_pFileNotebook->EditorModified();
		break;

	case cfId_ImageEditor_Refresh:
		m_pFileNotebook->RefreshEditor(_("class CfChipSchematicEditorCtrl"));
		break;

	//property view
	case cfId_PropertyView_ShowElement:
	case cfId_Schematic_ElementChange:
		{
			CfDrawingElement* pNewElement = (CfDrawingElement*) pNotification->GetElement();
			//if(pNewElement != NULL)
			{
				m_pElementPropertyCtrl->SetPropertyElement(pNewElement);
				m_pElementPropertyCtrl->InitGrid();
			}
			break;
		}

	//Layer view window
	case cfId_LayerView_Refresh:
		m_pLayerViewCtrl->Reload(); 
		m_pLayerListViewCtrl->Reload(); 
		break;

	case cfId_LayerView_Update:
	case cfId_Schematic_LayerChange:
		{
			IElementWithChildren *panel = dynamic_cast<IElementWithChildren*>(pNotification->GetElement());
			m_pLayerViewCtrl->LoadElement(panel); 
			m_pLayerListViewCtrl->LoadElement(panel); 
			break;
		}
	case cfId_StatusBar_Update:

		SetStatusText(pNotification->GetString(),1);
		break;
	}
	delete pNotification;
}

void CircuitFinderFrame::AddMenu()
{
    // create menu
	wxMenuBar* mb = GetMenuBar();
	wxMenu* amenu;
	wxMenuItem *item,*tempitem;
	// File
	wxMenu* file_menu = new wxMenu;

		
	wxMenu* new_sub_menu = new wxMenu;
	wxMenu* open_sub_menu = new wxMenu;

	item = new wxMenuItem(NULL,wxID_NEW, _("&New File...\tCtrl+N"),_("Create a new file"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_MESSAGE_BOX, wxSize(16,16)));
	new_sub_menu->Append(item);

	item = new wxMenuItem(NULL,MENU_ID_NewProject, _("New &Project..."),_("Create a new project"));
	item->SetBitmap(ResourceUtil::Get()->GetBitmap(IMG_PROJECT));
	new_sub_menu->Append(item);
	//item->Enable(false);
	
	item = new wxMenuItem(NULL,wxID_OPEN, _("&Open File...\tCtrl+O"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MESSAGE_BOX, wxSize(16,16)));
	open_sub_menu->Append(item);

	item = new wxMenuItem(NULL,MENU_ID_OpenProject, _("Open P&roject/Workspace...\tCtrl+Shift+O"));
	item->SetBitmap(ResourceUtil::Get()->GetBitmap(IMG_WORKSPACE));
	open_sub_menu->Append(item);

	file_menu->AppendSubMenu(new_sub_menu,_("&New"));
	file_menu->AppendSubMenu(open_sub_menu,_("&Open"));
	file_menu->AppendSeparator();
	item = new wxMenuItem(NULL,wxID_SAVE,_("Save &File\tCtrl+S"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MESSAGE_BOX, wxSize(16,16)));
	file_menu->Append(item);

	//item = new wxMenuItem(NULL,wxID_SAVEAS,_("Save File &As...\t"));
	//item->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MESSAGE_BOX, wxSize(16,16)));
	//file_menu->Append(item);

	item = new wxMenuItem(NULL,MENU_ID_SaveAll,_("Save &All\tCtrl+Shift+S"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_FLOPPY, wxART_MESSAGE_BOX, wxSize(16,16)));
	file_menu->Append(item);
	file_menu->AppendSeparator();

	file_menu->Append(wxID_CLOSE,_("C&lose File\tCtrl+W"));
	file_menu->Append(wxID_CLOSE_ALL,_("Cl&ose All Files"));
	file_menu->Append(MENU_ID_CloseWorkspace,_("&Close Workspace"));
	file_menu->AppendSeparator();

    file_menu->Append( wxID_EXIT, wxT("E&xit\tAlt-X") );

	//View
	wxMenu* view_menu = new wxMenu;
	item = new wxMenuItem(NULL,MENU_ID_ZOOMIN, _("Zoom In"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_PLUS, wxART_MESSAGE_BOX, wxSize(16,16)));
	view_menu->Append(item);
	item = new wxMenuItem(NULL,MENU_ID_ZOOMOUT, _("Zoom Out"));
	item->SetBitmap(wxArtProvider::GetBitmap(wxART_MINUS, wxART_MESSAGE_BOX, wxSize(16,16)));
	view_menu->Append(item);
	view_menu->AppendSeparator();
	item = new wxMenuItem(NULL,MENU_ID_GRID, _("Show/Hide Grid"));
	view_menu->Append(item);

	//Window
	wxMenu* window_menu = new wxMenu;
	item = new wxMenuItem(NULL,MENU_ID_NewVerticalTabGroup, _("New Vertical Tab Group"));
	window_menu->Append(item);
	item = new wxMenuItem(NULL,MENU_ID_NewHorizontalTabGroup, _("New Horizontal Tab Group"));
	window_menu->Append(item);

	// Edit
	wxMenu* edit_menu = new wxMenu;
	edit_menu->Append(wxID_UNDO,_("&Undo\tCtrl+Z"));
	edit_menu->Append(wxID_REDO,_("&Redo\tCtrl+Y"));
	edit_menu->AppendSeparator();
	edit_menu->Append(wxID_CUT,_("&Cut\tCtrl+X"));
	edit_menu->Append(wxID_COPY,_("&Copy\tCtrl+C"));
	edit_menu->Append(wxID_PASTE,_("&Paste\tCtrl+V"));


	//Draw
	wxMenu* draw_menu = new wxMenu;
	draw_menu->Append(MENU_ID_PARAMETER, _("Parameter"));
	wxMenu* sub_menu_add = new wxMenu;
	sub_menu_add->Append(MENU_ID_ADDLAYER, _("Add Layer"));
	sub_menu_add->Append(MENU_ID_ADDIMAGE, _("Add Image..."));
	sub_menu_add->Append(MENU_ID_ADDSYMBOL, _("Add Symbol Instance From File..."));
	draw_menu->AppendSubMenu(sub_menu_add,_("&Add"));
	draw_menu->Append(MENU_ID_PORT, _("Port"));
	draw_menu->Append(MENU_ID_WIRE, _("Wire"));
	draw_menu->Append(MENU_ID_SYMBOL, _("Symbol"));
	draw_menu->AppendSeparator();
	draw_menu->Append(wxID_DELETE, _("Delete\tDel"));
	//Help
	wxMenu* help_menu = new wxMenu;
    help_menu->Append(MENU_ID_Manual, _("Manual\tF2"));
	help_menu->AppendSeparator();
    help_menu->Append(wxID_ABOUT, _("About...\tF1"));

	wxMenu* setting_menu = new wxMenu;
    setting_menu->Append(MENU_ID_SettingPan, _("Setting\tF7"));

	if(!mb)
		mb = new wxMenuBar;
	mb->Append(file_menu, _("&File"));
	mb->Append(edit_menu, _("&Edit"));
	mb->Append(view_menu, _("&View"));
	mb->Append(draw_menu, _("&Draw"));
	mb->Append(window_menu, _("&Window"));
	mb->Append(setting_menu, _T("Setting"));
	mb->Append(help_menu, _("&Help"));
	SetMenuBar(mb);
}

void CircuitFinderFrame::AddToolBar()
{
	// tool bar
    wxAuiToolBar* tb = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_TEXT | wxTB_HORZ_LAYOUT);// | wxAUI_TB_OVERFLOW);
    tb->SetToolBitmapSize(wxSize(16,16));

    wxBitmap tb_bmp1 = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_MESSAGE_BOX, wxSize(32,32));
    tb->AddTool(wxID_NEW, wxT("New"),wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("New File"), wxT("New File"),NULL);

	tb->AddTool(wxID_OPEN, wxT("Open"),wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Open File"), wxT("Open File"),NULL);

	tb->AddSeparator();
	tb->AddTool(MENU_ID_SaveAll, wxT("Save All"),wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Save all files"), wxT("Save all files"),NULL);
	tb->AddTool(wxID_SAVE, wxT("Save"),wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Save file"), wxT("Save file"),NULL);

	tb->AddSeparator();
	tb->AddTool(wxID_CUT, wxT("Cut"),wxArtProvider::GetBitmap(wxART_CUT, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Cut"), wxT("Cut"),NULL);
	tb->AddTool(wxID_COPY, wxT("Copy"),wxArtProvider::GetBitmap(wxART_COPY, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Copy"), wxT("Copy"),NULL);
	tb->AddTool(wxID_PASTE, wxT("Paste"),wxArtProvider::GetBitmap(wxART_PASTE, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Paste"), wxT("Paste"),NULL);
	tb->AddSeparator();
	
	tb->AddTool(wxID_UNDO, wxT("Undo"),wxArtProvider::GetBitmap(wxART_UNDO, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Undo"), wxT("Undo"),NULL);
	tb->AddTool(wxID_REDO, wxT("Redo"),wxArtProvider::GetBitmap(wxART_REDO, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Redo"), wxT("Redo"),NULL);
	tb->AddSeparator();
	




	//Draw
	tb->AddTool(MENU_ID_ADDIMAGE, wxT("Image"),ResourceUtil::Get()->GetBitmap(IMG_IMAGE),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Add a Image"), wxT("Image"),NULL);
	tb->AddTool(MENU_ID_ADDSYMBOL, wxT("Instance"),ResourceUtil::Get()->GetBitmap(IMG_INSTANCE),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Add a symbol instance"), wxT("Add a Symbol Instance"),NULL);
	tb->AddTool(MENU_ID_ADDLAYER, wxT("Layer"),ResourceUtil::Get()->GetBitmap(IMG_LAYER),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Add a layer"), wxT("Layer"),NULL);
	tb->AddTool(MENU_ID_PORT, wxT("Port"),ResourceUtil::Get()->GetBitmap(IMG_PORT),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Draw a port"), wxT("Port"),NULL);
	tb->AddTool(MENU_ID_WIRE, wxT("Wire"),ResourceUtil::Get()->GetBitmap(IMG_WIRE),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Draw a wire"), wxT("Draw a wire"),NULL);
	tb->AddTool(MENU_ID_LINK, wxT("Link"),ResourceUtil::Get()->GetBitmap(IMG_LINK),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Draw a link"), wxT("Link"),NULL);
	tb->AddTool(MENU_ID_SYMBOL, wxT("Symbol"),ResourceUtil::Get()->GetBitmap(IMG_SYMBOL),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Draw a symbol"), wxT("Draw a Symbol"),NULL);

	//View
	tb->AddSeparator();
	tb->AddTool(MENU_ID_ZOOMIN, wxT("Zoom in"),wxArtProvider::GetBitmap(wxART_PLUS, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Zoom in"), wxT("Zoom In"),NULL);
	tb->AddTool(MENU_ID_ZOOMOUT, wxT("Zoom out"),wxArtProvider::GetBitmap(wxART_MINUS, wxART_MESSAGE_BOX, wxSize(16,16)),
					 wxNullBitmap, wxITEM_NORMAL,
                     wxT("Zoom out"), wxT("Zoom Out"),NULL);
	tb->Realize();
	m_pToolBar = tb;

    m_AuiMgr.AddPane(m_pToolBar, wxAuiPaneInfo().Name(wxT("tb")).Caption(wxT("Toolbar")).
                  ToolbarPane().Top().Row(1).
                  LeftDockable(false).RightDockable(false));

}
void CircuitFinderFrame::CloseAllFiles()
{
	Unbind(wxEVT_AUINOTEBOOK_PAGE_CHANGED,&CircuitFinderFrame::OnChangedPage,this);
	wxASSERT(m_pFileNotebook);
	m_pFileNotebook->CloseAllFiles();
	Bind(wxEVT_AUINOTEBOOK_PAGE_CHANGED,&CircuitFinderFrame::OnChangedPage,this);
}

void CircuitFinderFrame::CloseFile()
{
	m_pFileNotebook->CloseFile();

}

void CircuitFinderFrame::CloseWorkspace()
{
	CfWorkspace::Destroy();
	m_pWorkspace = NULL;
	CloseAllFiles();
	if(m_pWorkspaceExplorer) m_pWorkspaceExplorer->DeleteAllItems();
	UpdateWinTitle();
}

void CircuitFinderFrame::OnClickMenu(wxCommandEvent& event)
{

	int id = event.GetId();
	switch(id)
	{
	case wxID_EXIT:
		Close(true);
		break;
// File
	case wxID_NEW:
		NewFile();
		break;
	case wxID_OPEN:
		OpenFile();
		break;
	case wxID_SAVE:
		SaveFile();
		break;
	case MENU_ID_SaveAll:
		SaveAllFiles();
		break;
	case wxID_CLOSE:
		CloseFile();
		break;
	case wxID_CLOSE_ALL:
		CloseAllFiles();
		break;
	case MENU_ID_CloseWorkspace:
		CloseWorkspace();
		break;

	case MENU_ID_OpenProject:
		ShowOpenWorkspaceDialog();
		break;
	case MENU_ID_NewProject:
		NewProject();
		break;

//Window
	case MENU_ID_NewVerticalTabGroup:
		NewTabGroup(wxRIGHT);
		break;
	case MENU_ID_NewHorizontalTabGroup:
		NewTabGroup(wxBOTTOM);
		break;
//help 
	case wxID_ABOUT:
		ShowAbout();
		break;
	case MENU_ID_Manual:
		ShowManual();
		break;
//Setting
	case MENU_ID_SettingPan:
		ShowSettings();
		break;
//edit
	case wxID_CLEAR:
	case wxID_CUT:
	case wxID_COPY:
	case wxID_PASTE:
	case MENU_ID_INDENTINC:
	case MENU_ID_INDENTRED:
	case wxID_SELECTALL:
	case MENU_ID_SELECTLINE:
	case wxID_REDO:
	case wxID_UNDO:

//Draw
	case MENU_ID_ADDIMAGE:
	case MENU_ID_ADDSYMBOL:
	case MENU_ID_ADDLAYER:
	case MENU_ID_PORT:
	case MENU_ID_WIRE:
	case MENU_ID_SYMBOL:
	case wxID_DELETE:
//View		
	case MENU_ID_ZOOMIN:
	case MENU_ID_ZOOMOUT:
	case MENU_ID_GRID:
		wxPostEvent(m_pFileNotebook->GetCurrentPage(),event);
		break;

	case MENU_ID_PARAMETER:
		ShowParameter();
		break;
	}
}

void CircuitFinderFrame::OnWorkspaceExistUpdateUI(wxUpdateUIEvent &event)
{
	event.Enable(m_pWorkspace && m_pWorkspace->GetActiveProject());
}

void CircuitFinderFrame::OnFileExistUpdateUI(wxUpdateUIEvent &event)
{
	size_t n = m_pFileNotebook->GetPageCount();
	if (n > 0) 
		//wxPostEvent(m_pFileNotebook->GetCurrentPage(),event);
		event.Enable(true);
	else 
		event.Enable(false);
	
}


void CircuitFinderFrame::OpenFile()
{
	wxFileDialog dialog
                 (
                    this,
                    _T("Open File"),
                    GetOpenDir(1),
                    wxEmptyString,
                    _T("All Files (*.*)|*.*")
                 );

    dialog.CentreOnParent();

	//SetStatusBarText();
    if (dialog.ShowModal() == wxID_OK)
    {
		m_sOpenFileDir = dialog.GetPath();
		//SetStatusBarText(wxString::Format(_("Open file %s ..."),m_sOpenFileDir.c_str()));
		//m_pFileNotebook->OpenFilePage(m_sOpenFileDir.c_str());
		CfFileEditorBase * reader = m_pFileNotebook->AddEditor(dialog.GetPath(),m_pWorkspace->GetActiveProject());
    }	
	//SetStatusBarText(_("Ready"));

}
void CircuitFinderFrame::SaveFile()
{
	m_pFileNotebook->SaveCurrentFile();
}

void CircuitFinderFrame::SaveFileAs()
{
}

void CircuitFinderFrame::SaveAllFiles()
{
	m_pFileNotebook->SaveAllFile();
	m_pWorkspace->SaveWorkspace();
}

void CircuitFinderFrame::NewFile()
{
	//wxFileDialog dialog(this,
	//	_T("Create new file"),
	//	wxEmptyString,
	//	_T(""),
	//	_T("All files (*.cpsc)|*.cpsc"),
	//	wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	//if (dialog.ShowModal() == wxID_OK)
	//{
	//	wxString path = dialog.GetPath();
	//	wxTextFile file(path);
	//	if(file.Create())
	//		CfFileEditorBase * reader = m_pFileNotebook->AddEditor(path,m_pWorkspace->GetActiveProject());
	//	else
	//		wxLogMessage(_("Failed to create file '%s',%s"),path.c_str(),file.GetName());
	//}
	CfNewFileDialogCtrl dialog(this);
    if (dialog.ShowModal() == wxID_OK)
    {
		wxString path = dialog.GetFilePath();
		wxTextFile file(path);
		if(file.Create())
		{
			CfNewFileInitializer nfi(path);
			nfi.InitNewFile();
			CfFileEditorBase * reader = m_pFileNotebook->AddEditor(path,m_pWorkspace->GetActiveProject());
		}
		else
			wxLogMessage(_("Failed to create file '%s',%s"),path,file.GetName());
	}
}

void CircuitFinderFrame::NewProject()
{
	wxString proj_path, ws_path;
	CfNewProjectDialogCtrl dialog(this,proj_path,ws_path);
    if (dialog.ShowModal() == wxID_OK)
    {
		if(!ws_path.IsEmpty()) OpenWorkspace(ws_path);
		if(!proj_path.IsEmpty()) OpenProject(proj_path);		
	}
}
void CircuitFinderFrame::OnAllowNotebookDnD(wxAuiNotebookEvent& event)
{
	event.Allow();
}
void CircuitFinderFrame::OnChangedPage(wxAuiNotebookEvent& event)
{
	CfFileEditorBase *editor = m_pFileNotebook->GetEditor(event.GetSelection());
	editor->Activated();

	//m_pElementPropertyCtrl->SetPropertyElement(NULL);
	//m_pElementPropertyCtrl->InitGrid();
	wxLogDebug(_("Page changed %d -> %d"),event.GetOldSelection(),event.GetSelection());
}
void CircuitFinderFrame::OnClosedPage(wxAuiNotebookEvent& event)
{
	wxLogDebug(_("Page closed %d -> %d"),event.GetOldSelection(),event.GetSelection());
}

void CircuitFinderFrame::OnClosePage(wxAuiNotebookEvent& event)
{
	int index = event.GetSelection();
	m_pFileNotebook->CloseFile(index,false);
	//int cnt = m_pFileNotebook->GetPageCount();
	//if(cnt == 1)
	//{
	//	//clear property view 
	//	m_pElementPropertyCtrl->SetPropertyElement(NULL);
	//	m_pElementPropertyCtrl->InitGrid();

	//	//clear layer view
	//	m_pLayerListViewCtrl->DeleteAllItems();
	//	m_pLayerViewCtrl->ClearColumns();
	//	//m_pLayerListViewCtrl->ClearColumns(); 

	//}
}

void CircuitFinderFrame::OnSchematicAndSymbolEditorUpdateUI(wxUpdateUIEvent &event)
{
	CfFileEditorBase* editor = m_pFileNotebook->GetEditor();
	event.Enable(editor && (editor->GetReaderId()==FR_CHIP_SCHEMATIC_EDITOR 
		|| editor->GetReaderId()==FR_CHIP_SYMBOL_EDITOR));
}

void CircuitFinderFrame::OnSchematicEditorUpdateUI(wxUpdateUIEvent &event)
{
	CfFileEditorBase* editor = m_pFileNotebook->GetEditor();
	event.Enable(editor && (editor->GetReaderId()==FR_CHIP_SCHEMATIC_EDITOR));
}

void CircuitFinderFrame::OnNewTabGroupUpdateUI(wxUpdateUIEvent &event)
{
	size_t n = m_pFileNotebook->GetPageCount();
	if (n > 1) 
		event.Enable(true);
	else 
		event.Enable(false);
	
}

void CircuitFinderFrame::NewTabGroup(int direction)
{
	wxASSERT(m_pFileNotebook->GetPageCount() > 0);
	m_pFileNotebook->Split(m_pFileNotebook->GetSelection(),direction);
}
void CircuitFinderFrame::ShowOpenWorkspaceDialog()
{
	wxString ext_str = _T("FlowrianII project/workspace files (*.flp;*.flw)|*.flp;*.flw|FlowrianII project files (*.flp)|*.flp|FlowrianII workspace files (*.flw)|*.flw");
	if(!m_pWorkspace) ext_str = _T("FlowrianII workspace files (*.flw)|*.flw");

	wxFileDialog dialog
                 (
                    this,
                    _T("Open workspace/project"),
                    GetOpenDir(),
                    wxEmptyString,
					ext_str
                 );

    dialog.CentreOnParent();

    if (dialog.ShowModal() == wxID_OK)
    {
		m_sProjectWorkingDir = dialog.GetPath();
		wxString fn = dialog.GetFilename();
		if(fn.EndsWith(_(".flp")))
			OpenProject(m_sProjectWorkingDir.c_str());
		else
			OpenWorkspace(m_sProjectWorkingDir.c_str());
    }	
}
void CircuitFinderFrame::OpenProject(const wxChar* project_file)
{
	if(!m_pWorkspace) 
	{
		ShowWarningDialog(_("Open a workspace first!"));
		return;
	}
	//SetStatusBarText(wxString::Format(_("Load project '%s' ..."),project_file));
	CfProject* project=NULL;
	project=m_pWorkspace->LoadProject(project_file);
	if(project) 
	{
		m_pWorkspaceExplorer->AddProjectNode(project);
		//LoadProjectSuccessed();
	}
	//SetStatusBarText();
}
void CircuitFinderFrame::OpenWorkspace(const wxChar* workspace_file, bool is_refresh)
{
	if(!::wxFileExists(workspace_file))
	{
		wxLogError(wxString::Format(wxT("Workspace file '%s' does not exist!"),workspace_file));
		return ;
	}

	m_sProjectWorkingDir = workspace_file;

	if(!m_pWorkspace)
	{
		m_pWorkspace = CfWorkspace::Get();
	}

	//Already opened
	wxString cur_path = m_pWorkspace->GetWorkspacePath();
	if(cur_path.CmpNoCase(workspace_file) == 0)
	{
		if(is_refresh) CfWorkspace::Destroy();
		else
			return;
	}
	
	if(cur_path.IsEmpty())  // current workspace is non-specified
	{
		if(m_pWorkspace->Load(workspace_file))
		{
			wxLogMessage("Load workspace - "+m_pWorkspace->GetWorkspaceName());
			m_pWorkspaceExplorer->LoadWorkspace(m_pWorkspace);
		}
	}
	else 
	{
		CfWorkspace *ws = new CfWorkspace();
		if(ws->Load(workspace_file))
		{
			CfWorkspace::Destroy();
			CfWorkspace::SetWorkspace(ws);
			m_pWorkspace = ws;
			m_pWorkspaceExplorer->LoadWorkspace(ws);
		}
	}
}


void CircuitFinderFrame::RefreshWS()
{
	if(m_pWorkspace)
		//m_pWorkspaceExplorer->LoadWorkspace(m_pWorkspace);
		OpenWorkspace(m_pWorkspace->GetWorkspacePath(),true);
}

wxString CircuitFinderFrame::GetOpenDir(int mode) // 0 :proj/workspace;  1: file
{
	wxString dir;
	if(mode == 0) dir = m_sProjectWorkingDir;
	else dir = m_sOpenFileDir;
	if(dir.IsEmpty()) 
	{
		if(m_pWorkspace)
		{
			dir = m_pWorkspace->GetWorkspaceDir();
			if(!dir.IsEmpty()) return dir;
		}
		dir = FileUtil::Get()->GetDirectory(ID_PROJDIR);
	}
	return dir;
}
void CircuitFinderFrame::ShowAbout()
{
	CfAboutDialogCtrl aboutdlg(this,m_pFileUtils);
	aboutdlg.ShowModal();
    //wxMessageBox(_("Engineering SaaS CAD System\n(FlowrianII Professional R1.0 - Ver 1.0 2010.2.3)\n\nThis FlowrianII Professional is a client program communicating with the \nserver program (FlowrianII.Net Server). All kinds of CAD tools on remote \nserver are available in batch execution as well as in graphical interactive \nexecution. User¡¯s account can be created by administrator on server side.\n\n(c) Copyright 2009-2010, System Centroid Inc."), _("About FlowrianII Professional"), wxOK, this);
}
void CircuitFinderFrame::ShowManual()
{

	FileUtil fu;
	wxString path;
	path.Printf(_("%s\\CircuitFinder_Manual.docx"),fu.GetDirectory(ID_DOCDIR));
	wxLaunchDefaultBrowser(path);
}
//void CircuitFinderFrame::SetStatusBarText(wxString msg)
//{
//	if(msg.IsEmpty()) msg = _("Ready");
//	GetStatusBar()->SetStatusText(msg);
//}

void CircuitFinderFrame::LoadProjectSuccessed()
{
	UpdateWinTitle();
	CfProject* project = m_pWorkspace->GetActiveProject();
	//if(project) m_pEnvironment->AddToRecentProjects(project->GetProjectPath().c_str());
	if(project) m_pRecentProjectList->AddFileToHistory(project->GetProjectPath().c_str());

	//UpdateRecentProjectsMenu();
	//SentLoadProjSuccessNotify();
}

void CircuitFinderFrame::UpdateWinTitle()
{
	CfProject* project = NULL;
	if(m_pWorkspace && (project = m_pWorkspace->GetActiveProject()))
	{
		wxString project_name= project->GetProjectName();
		SetTitle(project_name + wxT(" - ") + PROJECT_CAPTION );
	}
	else
	{
		SetTitle(PROJECT_CAPTION );
	}
}
//void CircuitFinderFrame::SentLoadProjSuccessNotify()
//{
//	for(size_t i=0;i<m_LoadProjSuccuessNotifyListenerPtrList.GetCount();i++)
//		m_LoadProjSuccuessNotifyListenerPtrList[i]->LoadProjectSuccessed();
//}

void CircuitFinderFrame::ShowSettings()
{
	CfSettingDialogCtrl setting(this, m_pEnvironment,m_iSettingPageNum);
	setting.ShowModal();
}
void CircuitFinderFrame::ShowParameter()
{
	wxString path = FileUtil::Get()->GetDirectory(ID_APPDIR) +_("\\GlobalParameter.para");
	CfParameter param;
	if(wxFile::Exists(path))
		param.Load(&wxFileInputStream(path));
	CfParameterDialogCtrl parameter(this, &param);
	if(parameter.ShowModal() == wxID_OK)
	{
		param.Dump(&wxFileOutputStream(path));
	}

}