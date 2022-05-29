
#include <iostream>
#include <uwebsockets/App.h>
#include <nlohmann/json.hpp>


int main()
{
    uWS::SSLApp({

        /* There are tons of SSL options, see uSockets */
        .cert_file_name = "cert.pem",
        .key_file_name = "key.pem"

        }).get("/hello", [](auto* res, auto* req) {

            /* You can efficiently stream huge files too */
            res->writeHeader("Content-Type", "text/html; charset=utf-8")->end("Hello HTTP!");

            }).ws<UserData>("/*", {

                /* Just a few of the available handlers */
                .open = [](auto* ws) {
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
