#ifndef LONGPOLL_H
#define LONGPOLL_H

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <curl/curl.h>
#include <json/json.h>

using namespace std;

class LongPoll
{
    string _key;
    string _server;
    int _ts;
    CURL *curl;
    CURLcode res;
    string _token;
    mutex waitPoll;
    Json::Value root;
    Json::Reader reader;
    string answer;
    bool fail;

public:
    LongPoll(string token);
    ~LongPoll();
    std::map<int, vector<int>> request();
    static size_t handle(char * data, size_t size, size_t nmemb, void * p);
    size_t handle_impl(char* data, size_t size, size_t nmemb);
    void init();
};

#endif // LONGPOLL_H
