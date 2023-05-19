
#include <poincare/print.h>
#include "battery_view.h"
#include <escher/palette.h>

using namespace Escher;


/*
  Flash pictogram (when charging)
  1101 1011 0000 0000 0000 0000 1111 1111
  1011 0111 0000 0000 0110 1101 1111 1111
  0110 1101 0000 0000 1101 1011 1111 1111
  0010 0100 0000 0000 0000 0000 0000 0000
  0000 0000 0000 0000 0000 0000 0010 0100
  1111 1111 1101 1011 0000 0000 0110 1101
  1111 1111 0110 1101 0000 0000 1011 0111
  1111 1111 0000 0000 0000 0000 1101 1011
*/
const uint8_t flashMask[BatteryView::k_flashHeight][BatteryView::k_flashWidth] = {
  {0xDB, 0x00, 0x00, 0xFF},
  {0xB7, 0x00, 0x6D, 0xFF},
  {0x6D, 0x00, 0xDB, 0xFF},
  {0x24, 0x00, 0x00, 0x00},
  {0x00, 0x00, 0x00, 0x24},
  {0xFF, 0xDB, 0x00, 0x6D},
  {0xFF, 0x6D, 0x00, 0xB7},
  {0xFF, 0x00, 0x00, 0xDB},
};

/*
  Tick pictogram (when plugged and full)
  1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 1101 1011 0000 0000 0010 0100
  1111 1111 1111 1111 1111 1111 1111 1111 1111 1111 0110 1101 0000 0000 1101 1011
  0110 1101 0000 0000 1011 0111 1111 1111 1011 0111 0000 0000 0010 0100 1111 1111
  1101 1011 0000 0000 0000 0000 1111 1111 0000 0000 0000 0000 1111 1111 1111 1111
  1111 1111 1011 0111 0000 0000 0010 0100 0000 0000 1011 0111 1111 1111 1111 1111
  1111 1111 1111 1111 0010 0100 0000 0000 0010 0100 1111 1111 1111 1111 1111 1111
*/
const uint8_t tickMask[BatteryView::k_tickHeight][BatteryView::k_tickWidth] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDB, 0x00, 0x24},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0x00, 0xDB},
  {0x6D, 0x00, 0xB7, 0xFF, 0xB7, 0x00, 0x24, 0xFF},
  {0xDB, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF, 0xFF},
  {0xFF, 0xB7, 0x00, 0x24, 0x00, 0xB7, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x24, 0x00, 0x24, 0xFF, 0xFF, 0xFF},

};

bool BatteryView::setChargeState(Ion::Battery::Charge chargeState) {
  /* There is no specific battery picto for 'empty' battery as the whole device
   * shut down. Still, there might be a redrawing of the window before shutting
   * down so we handle this case as the 'low' battery one. Plus, we avoid
   * trigerring a redrawing by not marking anything as dirty when switching
   * from 'low' to 'empty' battery. */
  chargeState = chargeState == Ion::Battery::Charge::EMPTY ? Ion::Battery::Charge::LOW : chargeState;
  if (chargeState != m_chargeState) {
    m_chargeState = chargeState;
    markRectAsDirty(bounds());
    return true;
  }
  return false;
}

bool BatteryView::setIsCharging(bool isCharging) {
  if (m_isCharging != isCharging) {
    m_isCharging = isCharging;
    markRectAsDirty(bounds());
    return true;
  }
  return false;
}

bool BatteryView::setIsPlugged(bool isPlugged) {
  if (m_isPlugged != isPlugged) {
    m_isPlugged = isPlugged;
    markRectAsDirty(bounds());
    return true;
  }
  return false;
}

void BatteryView::drawInsideBatteryLevel(KDContext * ctx, KDCoordinate width, KDColor color) const {
  ctx->fillRect(KDRect(k_batteryInsideX, 0, width, icon_rect.height()).translatedBy(icon_rect.topLeft()), color);
  ctx->fillRect(KDRect(k_batteryInsideX + width, 0, k_batteryInsideWidth - width, icon_rect.height()).translatedBy(icon_rect.topLeft()), Palette::YellowLight);
}

void BatteryView::drawRect(KDContext * ctx, KDRect rect) const {
  assert(m_chargeState != Ion::Battery::Charge::EMPTY);
  /* We draw from left to right. The middle part representing the battery
   *'content' depends on the charge */

  constexpr size_t text_size = 5;
  char text[text_size];

  // Draw the left part
  ctx->fillRect(KDRect(0, 0, k_elementWidth, icon_rect.height()).translatedBy(icon_rect.topLeft()), KDColorWhite);
  Poincare::Print::CustomPrintf(text, text_size, "%i%%", (int)(m_percentage*100));


  ctx->drawString(text, percentage_rect.topLeft(), KDFont::Size::Small, KDColorWhite, Palette::YellowDark);



  // Draw the middle part
  if (m_isCharging) {
    // Charging: Yellow background with flash
    ctx->fillRect(KDRect(k_batteryInsideX , 0, k_batteryInsideWidth, icon_rect.height()).translatedBy(icon_rect.topLeft()), Palette::YellowLight);
    KDRect frame((icon_rect.width() - k_flashWidth) / 2, 0, k_flashWidth, k_flashHeight);
    KDColor flashWorkingBuffer[k_flashHeight * k_flashWidth];
    ctx->blendRectWithMask(frame, KDColorWhite, (const uint8_t *)flashMask, flashWorkingBuffer);
  } else if (m_chargeState == Ion::Battery::Charge::LOW) {
    assert(!m_isPlugged);
    // LOW: Quite empty battery
    drawInsideBatteryLevel(ctx, 2 * k_elementWidth, Palette::LowBattery);
  } else if (m_chargeState == Ion::Battery::Charge::MID) {
    assert(!m_isPlugged);
    // MID: Half full battery
    drawInsideBatteryLevel(ctx, k_batteryInsideWidth / 2);
  } else if (m_chargeState == Ion::Battery::Charge::THREE_QUARTERS) {
    assert(!m_isPlugged);
    // THREE_QUARTERS: 3/4 full battery
    drawInsideBatteryLevel(ctx, 3 * k_batteryInsideWidth / 4);
  } else {
    assert(m_chargeState == Ion::Battery::Charge::FULL);
    // FULL but not plugged: Full battery
    drawInsideBatteryLevel(ctx, k_batteryInsideWidth);
    if (m_isPlugged) {
      // FULL and plugged: Full battery with tick
      KDRect frame((icon_rect.width() - k_tickWidth) / 2, (icon_rect.height() - k_tickHeight) / 2, k_tickWidth, k_tickHeight);
      KDColor tickWorkingBuffer[k_tickHeight * k_tickWidth];
      ctx->blendRectWithMask(frame.translatedBy(icon_rect.topLeft()), Palette::YellowDark, (const uint8_t *)tickMask, tickWorkingBuffer);
    }
  }

  // Draw the right part
  ctx->fillRect(KDRect(icon_rect.width() - 2 * k_elementWidth, 0, k_elementWidth, icon_rect.height()).translatedBy(icon_rect.topLeft()), KDColorWhite);
  ctx->fillRect(KDRect(icon_rect.width() - k_elementWidth, (icon_rect.height() - k_capHeight) / 2, k_elementWidth, k_capHeight).translatedBy(icon_rect.topLeft()), KDColorWhite);
}

KDSize BatteryView::minimalSizeForOptimalDisplay() const {
  return KDSize(percentage_rect.width() + icon_rect.width(), percentage_rect.height());
}
