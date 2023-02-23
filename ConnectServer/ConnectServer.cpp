
#include <iostream>
#include <string>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>


//Strucrure is joined to each connection..
using json = nlohmann::json;

void process_public_msg(auto data, auto* ws) {
    json payload{
        {"command", data["command"]},
        {"text", data["text"]},
        {"user_from", ws->getUserData->user_id}
    };
    ws->publish("public", payload.dump());
}


void process_private_msg(auto data, auto* ws) {
    json payload{
        {"command", data["command"]},
        {"text", data["text"]},
        {"user_from", ws->getUserData->user_id}
    };
    int user_to = data["user_to"];
    ws->publish("public" + std::string(user_to), payload.dump());
}




int main() {
    int latestUserId = 10;

    struct UserData {
        int user_id;
        std::string name;
    };
    

    /* Keep in mind that uWS::SSLApp({options}) is the same as uWS::App() when compiled without SSL support.
     * You may swap to using uWS:App() if you don't need SSL */
    uWS::App().ws<UserData>("/*", {
            /* Settings */

            .idleTimeout = 16,

            /* Handlers */

            .open = [&latestUserId](auto*ws) {
                /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
            auto data = ws-> getUserData();
            data->user_id = latestUserId++;
            data->name = "noname";
            ws->subscribe("public");
            ws->subscribe("user " + std::to_string(data->user_id));

            },
            .message = [](auto* ws, std::string_view data, uWS::OpCode opCode) {
                json parsed_data = json::parse(data);

                if (parsed_data["command"] == "public_msg") {
                    process_public_msg(parsed_data, ws);
                }

            },
            .close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/) {
                /* You may access ws->getUserData() here */
            }
            }).listen(9001, [](auto* listen_socket) {
                if (listen_socket) {
                    std::cout << "Listening on port " << 9001 << std::endl;
                }
                }).run();
}