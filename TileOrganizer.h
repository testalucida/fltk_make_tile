#ifndef TILEORGANIZER_H_
#define TILEORGANIZER_H_

#include <string>
#include <vector>

class Fl_Tile;
class Fl_Widget;

enum widget_type {
	BOX,
	GROUP,
	TABS,
	TILE,
	TABLE,
	TREE,
	TEXT,
	BROWSER,
	SCROLL,
	UNK
};

struct widget {
	widget_type wtype = widget_type::UNK;
	int col_span = 1;
	int row_span = 1;
	Fl_Widget* pWidget = 0;
};

struct column_def {
	int col = -1;
	int percent_w = 0;
};

struct row_def {
	int row = -1;
	int percent_h = 0;
};

typedef std::vector<widget*> Widgets;
typedef std::vector<Widgets*> WidgetTable;

/**
 * TileOrganizer helps you providing Widgets to a Fl_Tile without beeing
 * bothered by counting pixels.
 *
 * You may add this class to your project's source code or create a (static)
 * lib from it. There are no dependencies except from fltk lib.
 *
 * How to use TileOrganizer:
 * 1. create your Fl_Tile group.
 * 2. Instantiate TileOrganizer by telling it how many columns and rows
 *    Fl_Tile will contain.
 * 3. add() your widgets to Fl_Tile, by passing its type, position (col/row),
 *    span and label. add() returns a pointer to the created Widget.
 *    x, y, width and height of the new widget will be set to 1 at that
 *    moment.
 *    See enum widget_type for knowing which types you may add to your Fl_Tile.
 *    The widgets will be layed out in a precalculated default manner.
 *    To change it...
 * 4. ... call configure_columns() and configure_rows().
 *    You pass these methods an array of int*s representing the percentage
 *    width of each column (configure_columns()) and the percentage
 *    height of each row, respectively.
 * 5. organize() the added widgets.
 *
 * Example:
 *
    void create4boxes(Fl_Tile* pTile) {
		TileOrganizer to = TileOrganizer(pTile, 3, 2);
		Fl_Box *p1 = (Fl_Box*) to.add(widget_type::BOX, 0, 0, 1, 1, "BOX1");
		p1->box(FL_DOWN_BOX);
		p1->color(FL_RED);
		Fl_Box *p2 = (Fl_Box*) to.add(widget_type::BOX, 1, 0, 1, 1, "BOX2");
		p2->box(FL_DOWN_BOX);
		p2->color(FL_BLUE);
		Fl_Box *p3 = (Fl_Box*) to.add(widget_type::BOX, 2, 0, 1, 1, "BOX3");
		p3->box(FL_DOWN_BOX);
		p3->color(FL_YELLOW);

		//row 2
		Fl_Box *p4 = (Fl_Box*) to.add(widget_type::BOX, 0, 1, 3, 1, "BOX4");
		p4->box(FL_DOWN_BOX);
		p4->color(FL_GREEN);

		int widths[] = {20, 60, 20};
		to.configure_columns(widths);
		int heights[] = {80, 20};
		to.configure_rows(heights);

		to.organize();
	}

    int main() {
		Fl_Double_Window* pWin = new Fl_Double_Window(100, 20, 800, 800, "Test");
		Fl_Tile* pTile = new Fl_Tile(0, 0, 800, 800);

		create4boxes(pTile);

		pWin->resizable(pTile);

		pWin->end();
		pWin->show();
		return Fl::run();
	}
 *
 *
 */

class TileOrganizer {
public:
	TileOrganizer(Fl_Tile*, int cols, int rows);
	virtual ~TileOrganizer();
	/**
	 * Sets individual widths to each column.
	 * Expects an array of the size _cols - 1.
	 */
	void configure_columns(int widths_percent[]);
	/**
	 * Sets individual heights to each row.
	 * Expects an int array of the size _rows - 1.
	 */
	void configure_rows(int heights_percent[]);
	/**
	 * Adds a widget by passing its description.
	 * Creates a Fl_Widget object from that description and
	 * returns its pointer.
	 * Be aware that all position and size data are set to 1.
	 * You have to call organize() before using the created
	 * Fl_Widget*s.
	 */
	Fl_Widget* add(widget_type, int col, int row,
				   int col_span = 1, int row_span = 1,
				   const char* pLabel = NULL);
	/**
	 * Returns a pointer to the Fl_Widget at position col, row
	 */
	Fl_Widget* get(int col, int row) const;
	/**
	 * Calculates and sets position and size of all previously
	 * added widgets.
	 */
	void organize();
private:
	Fl_Widget* create_widget(widget_type, const char* pLabel = NULL);
	void add_widget(widget*, int col, int row);
	widget* get_widget(int col, int row) const;
	Widgets* get_widgets_in_column(int col) const;
	Widgets get_widgets_in_row(int row) const;
	int get_x_offset_for_column(int col) const;
	int get_y_offset_for_row(int row) const;
	void get_cell_size(int col, int row, int& w, int& h) const;

private:
	Fl_Tile* _pTile;
	int _X, _Y, _W, _H;
	int _cols;
	int _rows;
	bool _isOrganized = false;

	WidgetTable* _pWidgets = NULL;
	std::vector<int> _column_widths;
	std::vector<int> _row_heights;
};

#endif /* TILEORGANIZER_H_ */
