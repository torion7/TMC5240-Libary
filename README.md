# TMC5240-Libary

Dieses Repository soll eine dedizierte ESP32-Bibliothek bereitstellen, mit der der TMC5240-Schrittmotortreiber **über SPI** angesteuert werden kann. Unterstützung für die UART-Kommunikation ist derzeit nicht vorgesehen.

## Ziele
- Die TMC-API umschließen, um einfache Initialisierungs- und Konfigurationshilfen für den TMC5240 über SPI bereitzustellen
- Das Lesen und Schreiben aller Treiberregister ermöglichen
- Hochwertige Bewegungsbefehle anbieten (z.b. Positions- und Geschwindigkeitsregelung)
- komplexe stalguard steuerung und stromregelung
- Einfache Status- und Diagnosefunktionen bereitstellen
- Beispiele für die Nutzung mit der ESP32-Toolchain enthalten
- Die gleichzeitige Ansteuerung mehrerer Motoren unterstützen, wobei separate Konfigurationsdateien verwendet werden, damit der Hauptanwendungscode schlank bleibt

In dieser Phase konzentriert sich die Bibliothek ausschließlich auf die SPI-Kommunikation mit dem Treiber. Der UART-Modus wird noch nicht unterstüzt.

Das Projekt enthält derzeit nur Notizen und gesammelte Dokumentation. Die geplante Funktionsliste befindet sich in der Datei `To Do`.

