#include <gtkmm.h>
#include "app.hpp"

class App: public Gtk::Application {
  void on_activate() override {
    auto window = new Gtk::ApplicationWindow();
    add_window(*window);

    window->set_title("Planning");
    window->set_child(*planList());
    window->present();
  };

  public:
    App(): Gtk::Application("com.amazinaxel.planning") {}
};

int main(int argc, char** argv) {
  auto app = Glib::make_refptr_for_instance<App>(new App());
  return app->run(argc, argv);
};
