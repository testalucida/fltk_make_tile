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
	 * Adds a widget description.
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

	WidgetTable* _pWidgets = NULL;
	std::vector<int> _column_widths;
	std::vector<int> _row_heights;
};

#endif /* TILEORGANIZER_H_ */
