#include "longpoll.h"

LongPoll::LongPoll(string token)
{
    string _token = token;
}

LongPoll::~LongPoll()
{
    delete curl;
}

map<int, vector<int>> LongPoll::request()
{
    if (fail) {
        init();
        fail = false;
    }
    map<int, vector<int>> notes;
    curl = curl_easy_init();
    if (!curl)
        cerr<<"Curl init error!"<<endl;
    cout<<"--------------------------------------------------------------------------\n";
    string URL = "http://"+_server+"?act=a_check&key="+_key+"&ts="+to_string(_ts)+"&wait=25&mode=2";
    cout<<URL<<endl;
    waitPoll.lock();
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->handle);
    //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        cerr<<"curl_easy_perform() failed: "<<curl_easy_strerror(res)<<endl;
    curl_easy_cleanup(curl);
    waitPoll.lock();
    reader.parse(answer, root);
    if (root["failes"].asInt()==2) {
        fail = true;
        return notes;
    }
    _ts = root["ts"].asInt();
    for (int i=0; i<root["updates"].size(); ++i) {
        int type = root["updates"][i][0].asInt();
        if (type==8 || type==9)
            notes[type].push_back(root["updates"][i][1].asInt()*-1);
        if (type==4)
            notes[type].push_back(root["updates"][i][3].asInt());
    }
    waitPoll.unlock();
    return notes;
}

size_t LongPoll::handle(char * data, size_t size, size_t nmemb, void * p)
{
    return static_cast<LongPoll*>(p)->handle_impl(data, size, nmemb);
}

size_t LongPoll::handle_impl(char* data, size_t size, size_t nmemb)
{
    answer = "";
    answer.append(data, size * nmemb);
    cout<<answer<<endl;
    waitPoll.unlock();
    return size * nmemb;
}

void LongPoll::init()
{
    curl = curl_easy_init();
    if (!curl)
        cerr<<"Curl init error!"<<endl;
    cout<<"--------------------------------------------------------------------------\n";
    string URL = "https://api.vk.com/method/messages.getLongPollServer?v=5.21&access_token=" + _token;
    cout<<URL<<endl;
    waitPoll.lock();
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->handle);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        cerr<<"curl_easy_perform() failed: "<<curl_easy_strerror(res)<<endl;
    curl_easy_cleanup(curl);
    waitPoll.lock();
    reader.parse(answer, root);
    _key = root["response"]["key"].asString();
    _ts = root["response"]["ts"].asInt();
    _server = root["response"]["server"].asString();
    waitPoll.unlock();
}
