# TMC5240-Libary

Dieses Repository soll eine dedizierte ESP32-Bibliothek bereitstellen, mit der der TMC5240-Schrittmotortreiber **über SPI** angesteuert werden kann. Die Implementierung basiert auf der offiziellen **TMC-API** und stellt Arduino-ähnliche C++-Klassen für eine einfache Integration in ESP32-Projekte bereit. Unterstützung für die UART-Kommunikation ist derzeit nicht vorgesehen.

## Ziele
- Die TMC-API umschließen, um einfache Initialisierungs- und Konfigurationshilfen für den TMC5240 über SPI bereitzustellen
- Das Lesen und Schreiben aller Treiberregister ermöglichen
- Hochwertige Bewegungsbefehle anbieten (z.\u00a0B. Positions- und Geschwindigkeitsregelung)
- Einfache Status- und Diagnosefunktionen bereitstellen
- Beispiele f\u00fcr die Nutzung mit der ESP32-Toolchain enthalten
- Die gleichzeitige Ansteuerung mehrerer Motoren unterst\u00fctzen, wobei separate Konfigurationsdateien verwendet werden, damit der Hauptanwendungscode schlank bleibt

In dieser Phase konzentriert sich die Bibliothek ausschlie\u00dflich auf die SPI-Kommunikation mit dem Treiber. Der UART-Modus wird noch nicht unterst\u00fctzt.

Das Projekt enth\u00e4lt derzeit nur Notizen und gesammelte Dokumentation. Die geplante Funktionsliste befindet sich in der Datei `To Do`.

