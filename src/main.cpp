#include <chrono>
#include <thread>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "TelemetryFrame.h"

int main()
{
    while (true)
    {
        httplib::Client client("127.0.0.1", 8111);
        auto response = client.Get("/state");

        if (response && response->status == 200)
        {
            auto data = nlohmann::json::parse(response->body);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    return 0;
}