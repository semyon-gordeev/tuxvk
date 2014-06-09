#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <json/json.h>
#include <ctime>
#include "structs.h"

using namespace std;

class Connector
{
    CURL *curl;
    CURLcode res;
    mutex waitResponse;
    static size_t handle(char * data, size_t size, size_t nmemb, void * p);
    size_t handle_impl(char * data, size_t size, size_t nmemb);
    Json::Value root;
    Json::Reader reader;  
    vector<Attachment> parseAttachments(Json::Value attachments);
    string _token;

public:
    string answer;
    Connector(string t);
    ~Connector();
    int request(string func, string params);
    map<int, Friend> getAllFriends();
    vector <Message> getDialogWithUser(int id, int last=0);
    bool sendMessage(string func, string params);
    vector<int> getSortIdByHints();
};

#endif // CONNECTOR_H
