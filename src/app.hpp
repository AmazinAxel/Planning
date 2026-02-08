#pragma once
#include <gtkmm.h>
#include <functional>

Gtk::Box* planList(std::function<void(const Glib::ustring&)> onSelect);

Gtk::Button* planItem(const Glib::ustring& name, std::function<void(const Glib::ustring&)> onSelect);

class PlanPage : public Gtk::Box { public: PlanPage(std::function<void()> onBack); void setPlanName(const Glib::ustring& name); private: Gtk::Label* titleLabel = nullptr; };
