#pragma once

#include <nlohmann/json.hpp>

struct TelemetryFrame
{
    bool validation = false;
    unsigned int gear = 0;
    int height = 0;
    int tas = 0;
    int ias = 0;
    double mach = 0.0;
    double aoa = 0.0;
    unsigned int throttle = 0;

    long long timestampMs = 0;
};

TelemetryFrame parseTelemetryFrame(const nlohmann::json& rawJson);

