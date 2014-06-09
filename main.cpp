#include <iostream>
#include <gtkmm.h>
#include <manager.h>
#include <mutex>

#define UI_FILE "tuxui.glade"


int main(int argc, char **argv)
{

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv);
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();

    refBuilder->add_from_file(UI_FILE);
    Manager* topWindow;
    refBuilder->get_widget_derived("topWindow", topWindow);
    if(topWindow)
    {
        app->run(*topWindow);
    }

    delete topWindow;
    return 0;
}
