#include "player.h"
#include <iostream>

Player::Player(Glib::RefPtr<Gtk::Builder> builder)
{
    builder->get_widget("musicPlay", _play);
    builder->get_widget("musicNext", _next);
    builder->get_widget("musicBack", _back);
    builder->get_widget("songTitle", _songTitle);

    _play->signal_clicked().connect(sigc::mem_fun(*this, &Player::playClick));
    _back->signal_clicked().connect(sigc::mem_fun(*this, &Player::backClick));
    _next->signal_clicked().connect(sigc::mem_fun(*this, &Player::nextClick));

}

void Player::playClick()
{
    std::cout<<"play"<<std::endl;
}


void Player::backClick()
{
    std::cout<<"back"<<std::endl;
}


void Player::nextClick()
{
    std::cout<<"next"<<std::endl;
}


Player::~Player()
{

}
