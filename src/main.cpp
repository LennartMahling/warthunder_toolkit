#include <iostream>
#include <chrono>
#include <thread>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "TelemetryFrame.h"

int main()
{
    // OPTIMIERUNG: Client VOR der Schleife erstellen.
    // Dadurch wird die Verbindung effizient wiederverwendet (Keep-Alive),
    // statt sie alle 500ms komplett neu aufzubauen.
    httplib::Client client("127.0.0.1", 8111);

    // Timeout setzen, falls das Spiel abstürzt oder schließt
    client.set_connection_timeout(0, 500000); // 500 Millisekunden

    std::cout << "War Thunder Telemetrie-Tracker gestartet (Beenden mit Strg+C)...\n\n";

    while (true)
    {
        auto response = client.Get("/state");

        if (response && response->status == 200)
        {
            try {
                // 1. Das rohe JSON parsen
                auto data = nlohmann::json::parse(response->body);

                // 2. Deine Parse-Funktion füttern, um die Struktur zu befüllen
                // (Diese Logik hast du mit deinen Unit-Tests abgesichert!)
                TelemetryFrame frame = parseTelemetryFrame(data);

                // 3. Auswertung der Daten
                if (frame.validation)
                {
                    // Wenn gültige Flugdaten vorliegen (\r überschreibt die Zeile in der Konsole)
                    std::cout << "\r[FLUG] "
                              << "TAS: " << frame.tas << " km/h | "
                              << "H\u00f6he: " << frame.height << " m | "
                              << "IAS: " << frame.ias << " km/h | "
                              << "Gear: " << frame.gear << " % | "
                              << "AOA: " << frame.aoa << " ° | "
                              << "Schub: " << frame.throttle << "% | "
                              << "Mach: " << frame.mach << "   "
                              << std::flush;
                }
                else
                {
                    // Wenn 'valid' im JSON false war (z.B. im Hauptmenü)
                    std::cout << "\r[MEN\u00dc] Spiel l\u00e4uft, aber keine Flugdaten verf\u00fcgbar.         " << std::flush;
                }
            }
            catch (const nlohmann::json::parse_error& e) {
                std::cout << "\r[FEHLER] Fehler beim Parsen der JSON-Daten.               " << std::flush;
            }
        }
        else
        {
            // Wenn die HTTP-Abfrage fehlschlägt (Spiel ist aus oder lädt noch)
            std::cout << "\r[VERBINDUNG] War Thunder nicht gefunden. L\u00e4uft ein Testflug?        " << std::flush;
        }

        // OPTIMIERUNG: Die Pause gehört ans Ende des Schleifendurchlaufs.
        // 500 Millisekunden sorgen für 2 Abfragen pro Sekunde.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
