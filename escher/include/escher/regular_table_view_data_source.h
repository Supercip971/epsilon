#ifndef ESCHER_REGULAR_TABLE_VIEW_DATA_SOURCE_H
#define ESCHER_REGULAR_TABLE_VIEW_DATA_SOURCE_H

#include <escher/table_view.h>
#include <escher/highlight_cell.h>

namespace Escher {

class RegularHeightTableViewDataSource : public TableViewDataSource {
private:
  KDCoordinate defaultRowHeight() override = 0; // This must be implemented
  KDCoordinate nonMemoizedRowHeight(int j) override final { return defaultRowHeight(); };
  TableSize1DManager * rowHeightManager() override final { return &m_heightManager; }
  RegularTableSize1DManager m_heightManager;
};

class RegularTableViewDataSource : public RegularHeightTableViewDataSource {
private:
  KDCoordinate defaultColumnWidth() override = 0; // This must be implemented
  KDCoordinate nonMemoizedColumnWidth(int i) override final { return defaultColumnWidth(); };
  TableSize1DManager * columnWidthManager() override final { return &m_widthManager; }
  RegularTableSize1DManager m_widthManager;
};

class SimpleTableViewDataSource : public RegularTableViewDataSource {
public:
  virtual HighlightCell * reusableCell(int index) = 0;
  virtual int reusableCellCount() const = 0;
  HighlightCell * reusableCell(int index, int type) override { assert(type == 0); return reusableCell(index); }
  int reusableCellCount(int type) override { assert(type == 0); return reusableCellCount();}
  int typeAtLocation(int i, int j) override { return 0; }
};

}
#endif