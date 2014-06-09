#ifndef PLAYER_H
#define PLAYER_H

#include <gtkmm.h>

class Player
{
    Gtk::Button *_play, *_next, *_back;
    Gtk::Label *_songTitle;

public:
    Player(Glib::RefPtr<Gtk::Builder> builder);
    ~Player();
    void playClick();
    void nextClick();
    void backClick();
};

#endif // PLAYER_H
