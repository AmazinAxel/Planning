#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/stack.h>
#include "app.hpp"
#include "functionality/utils.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class App: public Gtk::Application {
    Gtk::Stack* stack = nullptr;
    PlanPage* planPageWidget = nullptr;
    json appData;

    void on_activate() override {
        auto window = new Gtk::ApplicationWindow();
        add_window(*window);
        window->set_title("Planning");

        appData = initLoadJSON();

        // CSS
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(provider, "src/style.css");
        GdkDisplay *display = gdk_display_get_default();
        gtk_style_context_add_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_USER
        );
        g_object_unref(provider);

        // App stack
        stack = Gtk::make_managed<Gtk::Stack>();
        stack->set_transition_type(Gtk::StackTransitionType::SLIDE_LEFT_RIGHT);

        planPageWidget = Gtk::make_managed<PlanPage>(stack);

        auto listPage = planListPage(appData, [this](const Glib::ustring& name) {
            openPlan(name);
        });

        stack->add(*listPage, "list", "List");
        stack->add(*planPageWidget, "plan", "Plan");

        stack->set_visible_child("list"); // inital page
        window->set_child(*stack);
        window->present();
    };

    void openPlan(const Glib::ustring& name) {
        if (planPageWidget) {
            planPageWidget->setPlanName(name);
            stack->set_visible_child("plan");
        };
    };
};

int main(int argc, char** argv) {
    auto app = Glib::make_refptr_for_instance<App>(new App());
    return app->run(argc, argv);
};
