#include "interest_view.h"
#include "../app.h"
#include "graph_controller.h"
#include "graph_view.h"
#include <poincare/circuit_breaker_checkpoint.h>

using namespace Shared;
using namespace Poincare;

namespace Graph {

void InterestView::drawRect(KDContext * ctx, KDRect rect) const {
  if (!m_parentView->hasFocus()) {
    return;
  }
  ContinuousFunctionStore * functionStore = App::app()->functionStore();
  Ion::Storage::Record selectedRecord = m_parentView->selectedRecord();
  ExpiringPointer<ContinuousFunction> f = functionStore->modelForRecord(selectedRecord);
  if (!f->properties().isCartesian() || m_parentView->recordWasInterrupted(selectedRecord)) {
    return;
  }

  AbstractPlotView::Axis axis = f->isAlongY() ? AbstractPlotView::Axis::Vertical : AbstractPlotView::Axis::Horizontal;
  PointsOfInterestCache * pointsOfInterestCache = App::app()->graphController()->pointsOfInterestForRecord(m_parentView->selectedRecord());
  PointOfInterest p;
  do {
    Coordinate2D<float> dotCoordinates = Coordinate2D<float>(NAN, NAN);

    /* Use a checkpoint each time a dot is computed so that plot can be
     * navigated in parallel of computation. */
    UserCircuitBreakerCheckpoint checkpoint;
    // Clone the cache to prevent modifying the pool before the checkpoint
    PointsOfInterestCache pointsOfInterestCacheClone = pointsOfInterestCache->clone();
    if (AnyKeyCircuitBreakerRun(checkpoint)) {
      p = pointsOfInterestCacheClone.computePointAtIndex(m_numberOfDrawnDots);
      if (!p.isUninitialized() && (m_interest == Poincare::Solver<double>::Interest::None || m_interest == p.interest())) {
        dotCoordinates = axis == AbstractPlotView::Axis::Horizontal ? static_cast<Coordinate2D<float>>(p.xy()) : Coordinate2D<float>(p.y(), p.x());
      }
    } else {
      break;
    }
    checkpoint.discard();
    *pointsOfInterestCache = pointsOfInterestCacheClone;

    m_numberOfDrawnDots++;
    if (std::isnan(dotCoordinates.x1()) || std::isnan(dotCoordinates.x2())) {
      continue;
    }

    // Draw the dot
    MemoizedCursorView * cursor = static_cast<MemoizedCursorView *>(m_parentView->cursorView());
    // If the point of interest is below the cursor, erase it and redraw it
    KDRect cursorFrame = cursor->frame();
    bool redrawCursor = cursorFrame.intersects(m_parentView->dotRect(k_dotsSize, dotCoordinates));
    if (redrawCursor) {
      // Erase cursor and make rect dirty
      cursor->setCursorFrame(cursorFrame, true);
    }
    m_parentView->drawDot(ctx, rect, k_dotsSize, dotCoordinates, Escher::Palette::GrayDarkest);
    if (redrawCursor) {
      cursor->redrawCursor(rect);
    }

  } while (!p.isUninitialized());
}

void InterestView::dirty() {
  markRectAsDirty(m_parentView->bounds());
}

}