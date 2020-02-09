#include "TileOrganizer.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Tree.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Browser.H>
#include <stdexcept>
#include <math.h>

using namespace std;

TileOrganizer::TileOrganizer(Fl_Tile *pTile, int cols, int rows) :
		_pTile(pTile),
		_X(pTile->x()),
		_Y(pTile->y()),
		_W(pTile->w()),
		_H(pTile->h()),
		_cols(cols), _rows(rows)
{
	//initialize WidgetTable:
	_pWidgets = new WidgetTable;
	/*
	 * e.g. rows = 2, cols = 3:
	 * _pWidgets (WidgetTable) contains 3 Widgets objects representing its columns
	 * o o o
	 * o o o
	 * |
	 * v
	 * each column is a Widgets object (vector<widget*>) containing 2 widget structs.
	 */
	//default widths:
	int w = _W / _cols;
	int w2 = _W - w * (_cols - 1);

	for (int c = 0; c < cols; c++) {
		Widgets *pCol = new Widgets;
		for (int r = 0; r < rows; r++) {
			pCol->push_back((widget*) NULL);
		}
		_pWidgets->push_back(pCol);

		//initialize column default width:
		_column_widths.push_back((c == cols - 1) ? w2 : w);
	}

	//initialize row definitions
	int h = _H / _rows;
	int h2 = _H - h * (_rows - 1);
	for (int r = 0; r < rows; r++) {
		_row_heights.push_back((r == _rows - 1) ? h2 : h);
	}
	_pTile->begin();
}

TileOrganizer::~TileOrganizer() {
	for (auto pCol : *_pWidgets) { //each Widgets object representing a column
		for (auto pw : *pCol) { //each widget object (struct)
			if (pw)
				delete pw;
		}
		delete pCol;
	}
	delete _pWidgets;
}

void TileOrganizer::configure_columns(int widths_percent[]) {
	int ww = 0;
	for (int c = 0; c < _cols - 1; c++) {
		int w = _W / 100 * widths_percent[c];
		_column_widths[c] = w;
		ww += w;
	}

	_column_widths[_cols - 1] = (_W - ww);
	organize();
}

void TileOrganizer::configure_rows(int heights_percent[]) {
	int hh = 0;
	for (int r = 0; r < _rows - 1; r++) {
		int h = _H / 100 * heights_percent[r];
		_row_heights[r] = h;
		hh += h;
	}

	_row_heights[_rows - 1] = (_H - hh);
	organize();
}

Fl_Widget* TileOrganizer::add(widget_type wtype, int col, int row,
								int col_span, int row_span, const char *pLabel)
{
	widget *pw = new widget;
	pw->wtype = wtype;
	pw->col_span = col_span;
	pw->row_span = row_span;
	pw->pWidget = create_widget(pw->wtype, pLabel);

	add_widget(pw, col, row);

	return pw->pWidget;
}

Fl_Widget* TileOrganizer::get(int col, int row) const {
	Widgets *pCol = _pWidgets->at(col);
	return ((*pCol)[row])->pWidget;
}

void TileOrganizer::organize() {
	_pTile->end();

	//iterate through all widgets
	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			widget *pw = get_widget(col, row);
			if (pw) {
				//calculate & set position and size
				Fl_Widget *pW = pw->pWidget;
				int x = get_x_offset_for_column(col) + _X;
				int y = get_y_offset_for_row(row) + _Y;
				pW->position(x, y);
				int w, h;
				get_cell_size(col, row, w, h);
				pW->size(w, h);
			}
		}
	}
}

void TileOrganizer::add_widget(widget *pw, int col, int row) {
	Widgets *pCol = get_widgets_in_column(col);
	(*pCol)[row] = pw;
}

widget* TileOrganizer::get_widget(int col, int row) const {
	Widgets *pCol = get_widgets_in_column(col);
	return (*pCol)[row];
}

int TileOrganizer::get_x_offset_for_column(int col) const {
	int offs = 0;
	for (int c = 0; c < col; c++) {
		offs += _column_widths[c];
	}
	return offs;
}

int TileOrganizer::get_y_offset_for_row(int row) const {
	int offs = 0;
	for (int r = 0; r < row; r++) {
		offs += _row_heights[r];
	}
	return offs;
}

void TileOrganizer::get_cell_size(int col, int row, int& w, int& h) const {
	widget* pw = get_widget(col, row);
	w = _column_widths[col]; //default
	for (int c = 1; c < pw->col_span; c++) {
		int cc = col + c;
		if (cc >= _cols) break; //should we throw an exception here?
		w += _column_widths[cc];
	}

	h = _row_heights[row]; //default
	for (int r = 1; r < pw->row_span; r++) {
		int rr = row + r;
		if (rr >= _rows) break; //should we throw an exception here?
		h += _row_heights[rr];
	}
}

Widgets* TileOrganizer::get_widgets_in_column(int col) const {
	return _pWidgets->at(col);
}

Widgets TileOrganizer::get_widgets_in_row(int row) const {
	Widgets widgets;
	for (int c = 0; c < _cols; c++) {
		widgets.push_back(get_widget(c, row));
	}
	return widgets;
}

Fl_Widget* TileOrganizer::create_widget(widget_type wtype, const char *pLabel) {
	Fl_Widget *pW = NULL;
	switch (wtype) {
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

