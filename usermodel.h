#ifndef USERMODEL_H
#define USERMODEL_H
#include <gtkmm.h>
#include <string>

using namespace std;

class UserModel : public Gtk::TreeModelColumnRecord
{
public:
    UserModel();
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> status;
    Gtk::TreeModelColumn<int> id;
};

#endif // USERMODEL_H
