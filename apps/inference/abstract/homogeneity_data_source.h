#ifndef PROBABILITY_ABSTRACT_HOMOGENEITY_DATA_SOURCE_H
#define PROBABILITY_ABSTRACT_HOMOGENEITY_DATA_SOURCE_H

#include <apps/i18n.h>
#include <escher/even_odd_buffer_text_cell.h>
#include <escher/solid_color_cell.h>

#include "inference/abstract/categorical_table_view_data_source.h"
#include "inference/abstract/dynamic_cells_data_source.h"
#include "inference/models/statistic/homogeneity_test.h"


namespace Inference {

/* This class wraps a TableViewDataSource by adding a Row & Column header around it.
 * Specifically meant for InputHomogeneity and HomogeneityResults. */
class HomogeneityTableDataSource : public CategoricalTableViewDataSource, public DynamicCellsDataSource<Escher::EvenOddBufferTextCell, k_homogeneityTableNumberOfReusableHeaderCells>,  public DynamicCellsDataSourceDelegate<Escher::EvenOddBufferTextCell> {
public:
  HomogeneityTableDataSource();

  // TableViewDataSource
  int numberOfRows() const override { return innerNumberOfRows() + 1; }
  int numberOfColumns() const override { return innerNumberOfColumns() + 1; }
  int reusableCellCount(int type) override;
  int typeAtLocation(int i, int j) override;
  HighlightCell * reusableCell(int i, int type) override;
  void willDisplayCellAtLocation(Escher::HighlightCell * cell, int column, int row) override;

  KDCoordinate columnWidth(int i) override { return k_columnWidth; }

  // DynamicCellsDataSource
  void initCell(Escher::EvenOddBufferTextCell, void * cell, int index) override;

  // SelectableTableViewDelegate
  void unselectTopLeftCell(SelectableTableView * t, int previousSelectedCellX, int previousSelectedCellY);

  constexpr static int k_columnWidth = 80;
  constexpr static int k_numberOfReusableColumns = Ion::Display::Width /k_columnWidth + 2;
  constexpr static int k_numberOfReusableCells = k_maxNumberOfReusableRows * k_numberOfReusableColumns;

protected:
  virtual int innerNumberOfRows() const = 0;
  virtual int innerNumberOfColumns() const = 0;
  virtual void willDisplayInnerCellAtLocation(Escher::HighlightCell * cell, int column, int row) = 0;
  virtual Escher::HighlightCell * innerCell(int i) = 0;

private:
  constexpr static int k_maxNumberOfColumns = HomogeneityTest::k_maxNumberOfColumns;
  constexpr static int k_maxNumberOfRows = HomogeneityTest::k_maxNumberOfRows;

  constexpr static int k_typeOfTopLeftCell = k_typeOfHeaderCells + 1;
  constexpr static int k_headerTranslationBuffer = 20;



  I18n::Message m_headerPrefix;
  Escher::SolidColorCell m_topLeftCell;
};

}  // namespace Inference

#endif /* PROBABILITY_ABSTRACT_HOMOGENEITY_DATA_SOURCE_H */