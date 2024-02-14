---
layout: page
title: 4-Gewinnt
lang: de
lang-ref: index
---

## Kurzer Überblick

Hallo und willkommen bei Ihrem Praktikum bei WAGO. In diesem Modul geht es darum, Vier-Gewinnt aufzubauen, zu löten und gegebenenfalls zu programmieren. Eine kurze Beschreibung des Aufbaus und der Bedienung des Spiels kann [hier](Platinenbeschreibung-VierGewinnt.pdf) gefunden werden. In dem WAGO-Karton den Sie bekommen haben befinden sich alle notwendigen Bauteile, um Vier-Gewinnt aufbauen zu können. Am besten prüfen Sie einmal ob alle Bauteile vorhanden sind. Anhand [dieser Bauteilliste](Bauteilliste.pdf) können Sie sicherstellen, dass der Bausatz vollständig und korrekt ist. Bei Problemen oder Rückfragen können Sie sich gerne an Herrn Rehburg oder Frau Lüder wenden.

## Aufbauanleitung

Wenn sie sichergestellt haben dass der Bauteilsatz vollständig ist, können Sie mit dem Aufbau starten. Um das Spiel aufbauen zu können, werden alle Bauteile in dem WAGO-Karton benötigt. Die Platine dient dabei als Bauteilträger, dort werden also alle Bauteile (Mit Ausnahme des ESP32 und der 9 V Batterie) angelötet. Bevor Sie mit dem Löten anfangen können muss zunächst einmal der Arbeitsplatz eingerichtet werden. Wie er beispielhaft aussehen kann sehen Sie [hier](Arbeitsplatzausstattung.pdf). Wie Sie beim Löten am besten vorgehen wird [hier](LoetAnleitung.pdf) erklärt. Wenn Sie sich an der Anleitung hangeln sollte es beim Aufbau des Spiels keine Probleme geben.

## Informationen zur Programmierung des Moduls

Wenn Sie das Spiel aufgebaut haben können Sie anfangen zu programmieren. Um die Programmierumgebung "Arduino" einmalig einzurichten können Sie sich an [diese Anleitung](Programmierung.pdf) halten. Zur Programmierung können wahlweise entweder die "Arduino-IDE" oder "Visual Studio Code" benutzt werden. Programmiert wird auf dem Breakout-Board "DOIT ESP32 DEVKIT V1". Der fertige Code mit dem aktuellen lauffähigen Programm für [Visual Studio Code](../src/VSC_Vier-Gewinnt) und [Arduino](../src/Arduino_Vier-Gewinnt/master) kann bei den jeweiligen Links heruntergeladen werden. Spielen Sie am am besten das fertige Programm einmal auf den Mikrocontroller um zu prüfen, ob Sie alles richtig verlötet haben. Die Bedienung des Geräts kann [hier](Platinenbeschreibung-VierGewinnt.pdf) nachgelesen werden.
<a href="../src/Arduino_Vier-Gewinnt" download>style.css</a>

## Aufgaben

Sie haben das Programm auf den ESP32 geladen und alles hat funktioniert? Sehr gut! Dann können Sie mit den nächsten Aufgabenstellungen starten. Im Falle, dass das Spiel noch nicht funktioniert, müssen Sie ihre Platine nochmal überprüfen.

### Aufgabensatz 1

Um das technische Prinzip der LED-Matrix zu verstehen können Sie [diese Aufgaben](Aufgabenstellung.pdf) bearbeiten.

### Aufgabensatz 2

In [diesem Aufgabensatz](ErweiterteAufgaben.pdf) können Sie selbst Hand an den Code legen, sich in den Programmcode hinein zu arbeiten und einige Zeilen Code eigenständig schreiben.

## Ansprechpartner

<maik.rehburg@wago.com>

<lisa.lueder@wago.com>
