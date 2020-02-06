#include "TileOrganizer.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Browser.H>
#include <stdexcept>

using namespace std;

TileOrganizer::TileOrganizer(Fl_Tile* pTile, int cols, int rows)
: _pTile(pTile), _cols(cols), _rows(rows)
{
	_pWidgets = new WidgetTable;
	/*
	 * e.g. rows = 2, cols = 3:
	 * _pWidgets contains 3 Widgets objects representing its columns
	 * o o o
	 * o o o
	 * |
	 * v
	 * each column is a Widgets object (vector<widget*>) containing 2 widget structs.
	 */
	for(int c = 0; c < cols; c++) {
		Widgets* pCol = new Widgets;
		for (int r = 0; r < rows; r++) {
			pCol->push_back((widget*)NULL);
		}
		_pWidgets->push_back(pCol);
	}
}

TileOrganizer::~TileOrganizer() {
	for(auto pCol : *_pWidgets) { //each Widgets object representing a column
		for(auto pw : *pCol) { //each widget object (struct)
			if (pw)
				delete pw;
		}
		delete pCol;
	}
	delete _pWidgets;
}

Fl_Widget* TileOrganizer::add(widget_type wtype, int col, int row,
							   int percent_w, int percent_h,
							   const char* pLabel)
{
	widget* pw = new widget;
	pw->wtype = wtype;
	pw->percent_w = percent_w;
	pw->percent_h = percent_h;
	pw->pWidget = create_widget(pw->wtype, pLabel);

	add_widget(pw, col, row);

	return pw->pWidget;
}

Fl_Widget* TileOrganizer::get(int col, int row) const {
	Widgets* pCol = _pWidgets->at(col);
	return ((*pCol)[row])->pWidget;
}

void TileOrganizer::organize() {
	int x1 = _pTile->x();
	int y1 = _pTile->y();
	int w1 = _pTile->w();
	int h1 = _pTile->h();

	_pTile->begin();

	_pTile->end();
}

void TileOrganizer::add_widget(widget* pw, int col, int row) {
	Widgets* pCol = _pWidgets->at(col);
	(*pCol)[row] = pw;
}

widget* TileOrganizer::get_widget(int col, int row) {
	Widgets* pCol = _pWidgets->at(col);
	return (*pCol)[row];
}

void TileOrganizer::calculate(int& x, int& y, int& w, int& h) {

}

Fl_Widget* TileOrganizer::create_widget(widget_type wtype, const char* pLabel) {
	Fl_Widget* pW = NULL;
	switch(wtype) {
	case widget_type::BOX:
		pW = new Fl_Box(1, 1, 1, 1, pLabel);
		break;
	case widget_type::GROUP:
		pW = new Fl_Group(1, 1, 1, 1, pLabel);
		break;
	case widget_type::TABS:
		pW = new Fl_Tabs(1, 1, 1, 1, pLabel);
		break;
	case widget_type::TILE:
		pW = new Fl_Tile(1, 1, 1, 1, pLabel);
			break;
	case widget_type::TABLE:
		pW = new Fl_Table_Row(1, 1, 1, 1, pLabel);
			break;
	case widget_type::TREE:
		pW = new Fl_Tree(1, 1, 1, 1, pLabel);
			break;
	case widget_type::TEXT:
		pW = new Fl_Text_Editor(1, 1, 1, 1, pLabel);
			break;
	case widget_type::BROWSER:
		pW = new Fl_Browser(1, 1, 1, 1, pLabel);
			break;
	case widget_type::SCROLL:
		pW = new Fl_Scroll(1, 1, 1, 1, pLabel);
			break;
	default:
		throw runtime_error("no such widget type.");
	}
	return pW;
}

