#include <gtkmm.h>
#include "app.hpp"

class App: public Gtk::Application {
  void on_activate() override {
    auto window = new Gtk::ApplicationWindow();
    add_window(*window);

    window->set_title("Planning");

    // App styling
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "src/style.css");
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );
    g_object_unref(provider);


    auto planList = createPlanList();

    planList.listBox->append(*planItem("Math"));
    planList.listBox->append(*planItem("General"));
    planList.listBox->append(*planItem("Hack Club"));
    window->set_child(*planList.scrolled);

    window->present();
  };
};

int main(int argc, char** argv) {
  auto app = Glib::make_refptr_for_instance<App>(new App());
  return app->run(argc, argv);
};
