#include <gtkmm/applicationwindow.h>
#include <glibmm/main.h>
#include <thread>
#include <chrono>

#include "app.hpp"
#include "style.hpp"
#include "functionality/utils.hpp"

App* App::instance = nullptr;
App* App::get() { return instance; }

void App::on_activate() {
    App::instance = this;
    auto window = new Gtk::ApplicationWindow();
    add_window(*window);
    window->set_title("Planning");

    isSynced = downloadDataFromServer();
    appData = initLoadJSON();

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, CSS);
    GdkDisplay *display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);

    // App stack
    stack = Gtk::make_managed<Gtk::Stack>();
    stack->set_transition_type(Gtk::StackTransitionType::OVER_LEFT_RIGHT);
    stack->set_transition_duration(250);

    // Disable laggy animations on broadway
    if (isOnBroadway()) {
        stack->set_transition_type(Gtk::StackTransitionType::NONE);
        stack->set_transition_duration(0);
    }

    listPage = Gtk::make_managed<PlanListPage>(appData);
    stack->add(*listPage, "list", "List");

    stack->set_visible_child("list"); // initial page
    window->set_child(*stack);

    window->signal_close_request().connect([this]() {
        uploadDataToServer();
        return false; // Let window close
    }, false);

    window->set_decorated(false);
    if (isOnBroadway()) {
        window->set_default_size(700, 400);

        // Broadway tab open/close
        auto lastInactiveTime = std::make_shared<std::chrono::steady_clock::time_point>(std::chrono::steady_clock::now());
        auto skipFirst = std::make_shared<bool>(true);
        window->property_is_active().signal_changed().connect([window, lastInactiveTime, skipFirst]() {
            if (window->property_is_active().get_value()) {
                if (*skipFirst) { *skipFirst = false; return; } // ignore app-start activation
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::steady_clock::now() - *lastInactiveTime).count();
                if (elapsed >= 30) {
                    // open
                    downloadDataFromServer();
                    App::get()->appData = initLoadJSON();
                    App::get()->stack->set_visible_child("list");
                    App::get()->listPage->refresh();
                }
            } else {
                *lastInactiveTime = std::chrono::steady_clock::now();
                std::thread(uploadDataToServer).detach();
            }
        });
    } else {
        window->maximize();
    }

    window->present();
};

void App::openPlan(const Glib::ustring& planName, const std::string& fl) {
    focusedList = fl;
    if (auto lastStack = stack->get_child_by_name("plan"))
        stack->remove(*lastStack); // Remove previous stack

    auto* page = planPage(stack, appData, planName);
    stack->add(*page, "plan", "Plan");
    stack->set_visible_child("plan");
};

void App::refreshCurrentPlan(const std::string& fl) {
    focusedList = fl;
    if (currentListsBox)
        renderLists(currentListsBox, appData, currentPlanName);
};

int main(int argc, char** argv, char**) {
    auto app = Glib::make_refptr_for_instance<App>(new App());
    return app->run(argc, argv);
};
