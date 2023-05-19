#ifndef APPS_BATTERY_VIEW_H
#define APPS_BATTERY_VIEW_H

#include <escher/metric.h>
#include <escher/transparent_view.h>
#include <ion.h>

class BatteryView : public Escher::TransparentView {
public:
  BatteryView() :
    m_chargeState(Ion::Battery::Charge::FULL),
    m_isCharging(false),
    m_isPlugged(false),
    m_percentage(1.0f)
  {}

  bool setPercentage(float percentage) {
    const float previous = m_percentage;
    m_percentage = percentage;
    const float diff = previous - m_percentage;
    return diff > 0.1f || diff < -0.1f;};
  bool setChargeState(Ion::Battery::Charge chargeState);
  bool setIsCharging(bool isCharging);
  bool setIsPlugged(bool isPlugged);

  void drawRect(KDContext * ctx, KDRect rect) const override;
  KDSize minimalSizeForOptimalDisplay() const override;
  constexpr static int k_flashHeight = 8;
  constexpr static int k_flashWidth = 4;
  constexpr static int k_tickHeight = 6;
  constexpr static int k_tickWidth = 8;
private:

  void drawBatteryIcon(KDContext * ctx, KDColor color = KDColorWhite) const;
  void drawInsideBatteryLevel(KDContext * ctx, KDCoordinate width, KDColor color = KDColorWhite) const;

constexpr static KDRect percentage_rect = KDRect(0, 1, 32, 14);
  constexpr static KDRect icon_rect = KDRect(32, 3, 15, 8);
  constexpr static KDCoordinate k_elementWidth = 1;
  constexpr static KDCoordinate k_capHeight = 4;
  constexpr static KDCoordinate k_separatorThickness = Escher::Metric::CellSeparatorThickness;
  constexpr static KDCoordinate k_batteryInsideX = k_elementWidth + k_separatorThickness;

  constexpr static KDCoordinate k_batteryInsideWidth = 15 - 3 * k_elementWidth - 2 * k_separatorThickness;
  Ion::Battery::Charge m_chargeState;
  bool m_isCharging;
  float m_percentage;
  bool m_isPlugged;
};

#endif
