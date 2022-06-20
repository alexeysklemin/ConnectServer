
#include <iostream>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>


//Strucrure is joined to each connection..
using json = nlohmann::json;

struct UserData {
    int user_id;
    std::string name;
};

const std::string COMMAND = "COMMAND";
const std::string PRIVATE_MSG = "PRIVATE_MSG";
const std::string USER_ID_TO = "user_id_to";
const std::string TEXT = "text";

void processMessage(auto *ws, auto message) {
    json data = json::parse(message);

    switch (data[COMMAND]){
    case PRIVATE_MSG:
        break;
    case PUBLIC_MSG:
        break;
    }
}

int main() {
    int latestUserId = 10;

    uWS::App().get("/hello", [](auto* res, auto* req) {

            /* You can efficiently stream huge files too */
            res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");

            }).ws<UserData>("/*", {

                /* Just a few of the available handlers */
                .open = [&latestUserId](auto* ws) {
                    UserData* data = ws->getUserData();
                    data->user_id = latestUserId++;
                    std::cout << "User connected ID " << data->user_id << std::endl;

                    /* MQTT syntax */
                   // ws->subscribe("sensors/+/house");
                },
                .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                   // ws->send(message, opCode);
                    processMessage(ws, message);
                },
                .close = [](auto* ws, int code, std::string_view message) {
                    //
}


                }).listen(9001, [](auto* listenSocket) {

                    if (listenSocket) {
                        std::cout << "Listening on port " << 9001 << std::endl;
                    }

                    }).run();
}