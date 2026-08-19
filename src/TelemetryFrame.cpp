#include "TelemetryFrame.h"
#include <chrono>

TelemetryFrame parseTelemetryFrame(const nlohmann::json& rawJson)
{
    TelemetryFrame frame;

    frame.timestampMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    frame.validation = rawJson.value("valid", false);
    if (!frame.validation)
    {
        return frame;
    }

    frame.gear = rawJson.value("gear %", 0);
    frame.height = rawJson.value("H, m", 0);
    frame.tas = rawJson.value("TAS, km/h", 0);
    frame.ias = rawJson.value("IAS, km/h", 0);
    frame.mach = rawJson.value("M", 0.0);
    frame.aoa = rawJson.value("AoA", 0.0);
    frame.throttle = rawJson.value("throttle 1", 0);

    return frame;
}