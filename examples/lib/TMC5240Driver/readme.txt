
============================================================
              TMC5240Driver – Schnellreferenz
============================================================

🧭 Allgemeines
-------------
Diese Library kapselt alle Funktionen des TMC5240-Motortreibers
in leicht verständliche Befehle. Keine Hex-Codes nötig – alles
mit sprechenden Funktionsnamen.

Beispielnutzung:
  tmc.setVMax(60000);
  tmc.moveTo(20000);
  if (tmc.isPositionReached()) { ... }

------------------------------------------------------------

⚙️ Initialisierung & Setup
--------------------------
  init()                - Standard-Initialisierung
  resetGSTAT()          - Löscht Fehlerstatus
  stop()                - Setzt VMAX = 0 (Motor stoppt)
  resetPosition()       - Setzt XACTUAL auf 0

------------------------------------------------------------

🔄 Bewegungen
-------------
  moveTo(steps)              - Absolute Zielposition anfahren
  moveByRevolutions(rev)     - Relative Bewegung in Umdrehungen
  isPositionReached()        - true, wenn Ziel erreicht

------------------------------------------------------------

📏 Mikroschritte & Mechanik
---------------------------
  setMicrosteps(msteps)      - z.B. 16 = 1/16 Mikroschritt (1–256)
  setStepsPerRev(steps)      - z.B. 200 für 1.8° Stepper

------------------------------------------------------------

🚀 Rampenprofil
---------------
  setRampMode(mode)          - POSITION oder VELOCITY

  setVStart(val)             - Startgeschw. [1/256 steps/s], empfohl.: 5–20
  setVStop(val)              - Endgeschw. beim Stoppen
  setVMax(val)               - Maximale Geschw. z. B. 30000–100000
  setAMax(val)               - Beschleunigung (typ. 1000–10000)
  setDMax(val)               - Verzögerung (typ. 1000–10000)

------------------------------------------------------------

🔌 Stromsteuerung
-----------------
  setIRun(val)               - Bewegung: Stromstärke (0–31)
  setIHold(val)              - Stillstand: Strom (0–31)
  setIHoldDelay(val)         - Umschaltverzögerung (0–15)
  setIRunDelay(val)          - Einschaltverzögerung (0–15)

------------------------------------------------------------

🔊 Betriebsmodi & Features
--------------------------
  enablePWM(ON/OFF)          - StealthChop2 (leiser Modus)
  setChopConf(value)         - CHOPCONF manuell setzen
  enableCoolStep(true/false) - Stromabsenkung bei geringer Last
  enableStallGuard(true/false) - Blockadeerkennung aktivieren
  setSGThreshold(val)        - Empfindlichkeit (0–255)

------------------------------------------------------------

🧪 Status & Diagnose
--------------------
  printStatus()              - Gibt aktuelle Registerwerte aus
  resetGSTAT()               - Fehlerstatus löschen

============================================================
Hinweis: Positionen und Geschwindigkeiten immer in Steps!
Nutze resetPosition() nach Referenzfahrt oder Start.
============================================================



