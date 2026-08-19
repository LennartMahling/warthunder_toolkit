#include <gtest/gtest.h>
#include "TelemetryFrame.h"

// ==============================================================================
// Test 1: Überprüft, ob ein vollständig ausgefülltes JSON korrekt eingelesen wird
// ==============================================================================
TEST(TelemetryFrameTest, ParsesValidFrame) {
    // Diese Schlüsselnamen entsprechen der echten Telemetrie-Schnittstelle von War Thunder
    nlohmann::json raw = {
        {"valid", true},
        {"gear, %", 100},
        {"H, m", 3200},
        {"TAS, km/h", 450},
        {"IAS, km/h", 410},
        {"M", 0.36},
        {"AoA, deg", 3.5},
        {"throttle 1, %", 92}
    };

    // Die zu testende Parse-Funktion aufrufen
    TelemetryFrame frame = parseTelemetryFrame(raw);

    // Erwartete Werte mit deinen Struktur-Variablen abgleichen
    EXPECT_TRUE(frame.validation);
    EXPECT_EQ(frame.gear, 100);
    EXPECT_EQ(frame.height, 3200);
    EXPECT_EQ(frame.tas, 450);
    EXPECT_EQ(frame.ias, 410);
    EXPECT_DOUBLE_EQ(frame.mach, 0.36);
    EXPECT_DOUBLE_EQ(frame.aoa, 3.5);
    EXPECT_EQ(frame.throttle, 92);

    // Der Timestamp wird vermutlich in der Funktion generiert (z.B. aktuelle Zeit),
    // daher prüfen wir hier nur, ob er ungleich dem Standardwert 0 ist.
    EXPECT_GT(frame.timestampMs, 0);
}

// ==============================================================================
// Test 2: Überprüft den "Early Return"-Schutzmechanismus bei ungültigen Daten
// ==============================================================================
TEST(TelemetryFrameTest, HandlesInvalidFrame) {
    nlohmann::json raw = {{"valid", false}};

    TelemetryFrame frame = parseTelemetryFrame(raw);

    // Wenn 'valid' false ist, muss die Validierung fehlschlagen...
    EXPECT_FALSE(frame.validation);

    // ...und alle anderen Werte müssen auf ihren Standardwerten (0) bleiben.
    EXPECT_EQ(frame.tas, 0);
    EXPECT_EQ(frame.height, 0);
    EXPECT_DOUBLE_EQ(frame.mach, 0.0);
}

// ==============================================================================
// Test 3: Überprüft die Ausfallsicherheit bei fehlenden JSON-Schlüsseln (Fallback)
// ==============================================================================
TEST(TelemetryFrameTest, HandlesMissingFields) {
    // Das JSON sagt zwar, es sei gültig, liefert aber außer den Triebwerk-Daten nichts mit
    nlohmann::json raw = {
        {"valid", true},
        {"throttle 1, %", 85}
    };

    TelemetryFrame frame = parseTelemetryFrame(raw);

    EXPECT_TRUE(frame.validation);
    EXPECT_EQ(frame.throttle, 85);    // Vorhandener Wert wird gelesen
    EXPECT_EQ(frame.tas, 0);          // Fehlender Wert fällt sicher auf 0 zurück (kein Crash!)
    EXPECT_EQ(frame.height, 0);       // Fehlender Wert fällt sicher auf 0 zurück
}
