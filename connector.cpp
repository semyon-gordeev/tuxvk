#include "connector.h"

Connector::Connector(string t)
{
    _token = t;
}

Connector::~Connector()
{

}

size_t Connector::handle(char * data, size_t size, size_t nmemb, void * p)
{
    return static_cast<Connector*>(p)->handle_impl(data, size, nmemb);
}

size_t Connector::handle_impl(char* data, size_t size, size_t nmemb)
{
    answer = "";
    answer.append(data, size * nmemb);
    waitResponse.unlock();
    cout<<answer<<endl;
    return size * nmemb;
}

int Connector::request(string func, string params)
{
    curl = curl_easy_init();
    if (!curl)
        cerr<<"Curl init error!"<<endl;
    cout<<"--------------------------------------------------------------------------\n";
    string URL = "https://api.vk.com/method/"+func+"?"+params+"&v=5.21&access_token="+_token;
    cout<<URL<<endl;
    waitResponse.lock();
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->handle);
    res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        cerr<<"curl_easy_perform() failed: "<<curl_easy_strerror(res)<<endl;
        return 1;
    }

    waitResponse.lock();
    curl_easy_cleanup(curl);
    waitResponse.unlock();
    return 0;
}

map<int, Friend> Connector::getAllFriends()
{
    request("friends.get", "order=hints&fields=online");
    reader.parse(answer, root);
    map<int, Friend> friends;
    for (int i=0; i<root["response"]["items"].size(); ++i) {
        Friend t;
        t.name = root["response"]["items"][i]["last_name"].asString() + " " \
                + root["response"]["items"][i]["first_name"].asString();
        t.online = root["response"]["items"][i]["online"].asBool()==1;
        t.unread = 0;
        friends[root["response"]["items"][i]["id"].asInt()] = t;
    }

    request("messages.getDialogs", "unread=1&v=5.21");
    reader.parse(answer, root);
    for (int i=0; i<root["response"]["items"].size(); ++i)
        friends[root["response"]["items"][i]["message"]["user_id"].asInt()].unread = \
                root["response"]["items"][i]["unread"].asInt();

    return friends;
}


vector<Message> Connector::getDialogWithUser(int id, int last)
{
    request("messages.getHistory", "user_id="+to_string(id)+(last ? "&count="+to_string(last) : ""));
    reader.parse(answer, root);
    vector<Message> list;
    for (int i=0; i<root["response"]["items"].size(); ++i) {
        Message m;
        m.body = root["response"]["items"][i]["body"].asString();
        m.date = (time_t)root["response"]["items"][i]["date"].asInt();
        m.out = root["response"]["items"][i]["out"].asInt();
        m.read_state = root["response"]["items"][i]["read_state"].asInt();
//        if (root["response"]["items"][i].get("attachments", "no") != "no")
//            m.attachments = parseAttachments(root["response"]["items"][i]["attachments"]);
        list.push_back(m);
    }
    return list;
}

vector<Attachment> Connector::parseAttachments(Json::Value attachments)
{
    vector<Attachment> list;
    for (int i=0; i<attachments.size(); ++i) {
        Attachment a;
        a.type = attachments[i]["type"].asString();
        if (a.type == "doc")
            a.url = attachments[i]["doc"]["url"].asString();
        else if (a.type == "photo") {
            Json::Value ph = attachments[i]["photo"];
            a.url = ph.get("photo_2560", ph.get("1280", ph.get("807", ph.get("604", \
                                            ph.get("130", ph.get("75", "error")))))).asString();
        } else if (a.type == "audio")
            a.url = attachments[i]["audio"]["url"].asString();
        else if (a.type == "wall")
            a.url = "http://vk.com/wall"+to_string(attachments[i]["wall"]["to_id"].asInt()) + "_" \
                    + to_string(attachments[i]["wall"]["id"].asInt());
        else if (a.type == "video")
            a.url = "http://vk.com/video"+to_string(attachments[i]["video"]["owner_id"].asInt()) + "_" \
                    + to_string(attachments[i]["video"]["id"].asInt());

        list.push_back(a);
    }
    return list;
}

vector<int> Connector::getSortIdByHints()
{
    request("friends.get", "order=hints");
    reader.parse(answer, root);
    vector<int> list;
    for (int i=0; i<root["response"]["items"].size(); ++i)
        list.push_back(root["response"]["items"][i].asInt());
    return list;
}

bool Connector::sendMessage(string func, string params)
{
    request(func, params);
    reader.parse(answer, root);
    if (root.get("error", "no_error") == "no_error")
        return true;
    else
        return false;
}


