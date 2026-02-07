#pragma once
#include <gtkmm.h>
#include <functional>

Gtk::ScrolledWindow* planList();
Gtk::Button* planItem(const Glib::ustring& title_text);
