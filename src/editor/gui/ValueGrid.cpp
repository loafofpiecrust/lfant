
#include <editor/gui/ValueGrid.h>

// internal
#include <lfant/Console.h>

// external

namespace lfant {
namespace editor {
namespace gui {

BEGIN_EVENT_TABLE(ValueGrid, wxGrid) 
EVT_SIZE(ValueGrid::OnSize)
END_EVENT_TABLE()

ValueGrid::ValueGrid(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name) :
	wxGrid(parent, id, pos, size, style, name)
{
}

void ValueGrid::SizeColumns()
{
	long num_cols = GetNumberCols();
	long width = GetRowLabelSize();

//	Log("Column count: ", num_cols);
	for(int col=0; col<num_cols-1; col++)
	{
	//	Log("Autosizing column ", col);
		AutoSizeColumn(col, true);
		width += GetColSize(col);
	}
	if(num_cols > 0)
	{
		width = GetClientSize().GetWidth()-width-1;
	//	Log("Sizing column ", num_cols-1, " to ", width);
		SetColSize(num_cols-1,width);
	}
}

void ValueGrid::OnSize(wxSizeEvent& evt)
{
	SizeColumns();
	evt.Skip();
}

}
}
}