#ifndef MANAGER_H
#define MANAGER_H

#include <gtkmm.h>
#include "player.h"
#include "usermodel.h"
#include "connector.h"
#include "longpoll.h"
#include <webkit/webkit.h>

using namespace std;

class Manager: public Gtk::Window
{
    Glib::RefPtr<Gtk::Builder> _builder;
    Player *_player;
    Connector *_connector;
    LongPoll *_longPoll;
    std::map<int, Friend> _friends;
    vector<int> _sort;
    int _current;
    Gtk::SearchEntry *_search;
    bool searching;
    Gtk::ScrolledWindow *_userListScroll;
    Gtk::TextView *_messageViewer, *_inputMessage;
    Glib::RefPtr<Gtk::TextBuffer> _messageBuffer;
    Glib::RefPtr<Gtk::TextBuffer::Mark> _unreadMark;
    Gtk::Button *_logout;
    Gtk::TreeView _userTreeView;
    Glib::RefPtr<Gtk::ListStore> _refUserStore;
    UserModel userModel;
    mutex changeUserStore, pollMutex;
    thread _pollThread;
    void connect();
    void userOnline(vector<int> ids, bool change);
    void userOffline(vector<int> ids);
    void inputMessageFrom(std::map<int, int> unread, bool change);
    void addMessagesInBuffer(vector<Message> messages);
    void initMessageBuffer();
    int waitNotification();
    bool exit;
    string _token;
    void authorize();
    void loadFinish(WebKitWebView *web_view, WebKitWebFrame *web_frame, gpointer data);
    gboolean closeWebView(WebKitWebView* webView, GtkWidget* window);
    void destroyAuth(GtkWidget* widget, GtkWidget* window);

public:
    Manager(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
    void logOut();
    bool sendMessage(GdkEventKey *event );
    bool searchContact(GdkEventKey *event);
    bool readMessage(GdkEventFocus *event);
    void selectUser();
    ~Manager();

};

#endif // MANAGER_H
