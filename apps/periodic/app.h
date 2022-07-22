#ifndef PERIODIC_APP_H
#define PERIODIC_APP_H

#include "elements_view_data_source.h"
#include "main_controller.h"
#include <apps/shared/text_field_delegate_app.h>
#include <apps/shared/shared_app.h>
#include <escher/container.h>
#include <escher/stack_view_controller.h>

namespace Periodic {

class App : public Shared::TextFieldDelegateApp {
public:
  class Descriptor : public Escher::App::Descriptor {
  public:
    I18n::Message name() const override;
    I18n::Message upperName() const override;
    const Escher::Image * icon() const override;
  };

  class Snapshot : public Shared::SharedApp::Snapshot {
  public:
    Snapshot();

    App * unpack(Escher::Container * container) override;
    const Descriptor * descriptor() const override;

    const DataField * field() const { return m_field; }
    AtomicNumber selectedElement() const { return m_selectedElement; }
    AtomicNumber previousElement() const { return m_previousElement; }

  private:
    const DataField * m_field;
    AtomicNumber m_selectedElement;
    AtomicNumber m_previousElement;
  };
  TELEMETRY_ID("PeriodicTable");

  static App * app() { return static_cast<App *>(Escher::Container::activeApp()); }

  ElementsViewDataSource * elementsViewDataSource() { return &m_dataSource; }

private:
  App(Snapshot * snapshot);

  Escher::StackViewController m_stackController;
  MainController m_mainController;
  ElementsViewDataSource m_dataSource;
};

}

#endif