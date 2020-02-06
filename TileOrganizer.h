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
	int percent_w = 0;
	int percent_h = 0;
	Fl_Widget* pWidget = 0;
};


typedef std::vector<widget*> Widgets;
typedef std::vector<Widgets*> WidgetTable;

class TileOrganizer {
public:
	TileOrganizer(Fl_Tile*, int cols, int rows);
	virtual ~TileOrganizer();
	Fl_Widget* add(widget_type, int col, int row,
				   int percent_w, int percent_h,
				   const char* pLabel = NULL);
	Fl_Widget* get(int col, int row) const;
	void organize();
private:
	void calculate(int& x, int& y, int& w, int& h);
	Fl_Widget* create_widget(widget_type, const char* pLabel = NULL);
	void add_widget(widget*, int col, int row);
	widget* get_widget(int col, int row);
private:
	Fl_Tile* _pTile;
	int _cols;
	int _rows;
	WidgetTable* _pWidgets = NULL;
};

#endif /* TILEORGANIZER_H_ */
