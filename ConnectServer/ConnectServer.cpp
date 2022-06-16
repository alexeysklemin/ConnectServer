
#include <iostream>
#include <uwebsockets/App.h>
//#include <nlohmann/json.hpp>


//Strucrure is joined to each connection..
struct UserData {
    int user_id;
    std::string name;
};

int main() {
    int latestUserId = 10;

    uWS::App().get("/hello", [](auto* res, auto* req) {

            /* You can efficiently stream huge files too */
            res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");

            }).ws<UserData>("/*", {

                /* Just a few of the available handlers */
                .open = [](auto* ws) {
                    UserData* data = ws->getUserData();
                    data->user_id = latestUserId++;

                    /* MQTT syntax */
                    ws->subscribe("sensors/+/house");
                },
                .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                    ws->send(message, opCode);
                }

                }).listen(9001, [](auto* listenSocket) {

                    if (listenSocket) {
                        std::cout << "Listening on port " << 9001 << std::endl;
                    }

                    }).run();
}