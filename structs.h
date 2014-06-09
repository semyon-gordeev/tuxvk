#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>

using namespace std;

struct Attachment {
    string type;
    string url;
};

struct Message {
    string body;
    int out;
    int read_state;
    time_t date;
    vector<struct Attachment> attachments;
};

struct Friend {
    string name;
    int unread;
    bool online;
};


#endif // STRUCTS_H
