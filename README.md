# IS2026 SUMMER - PROJECT VECTOR
___

## Problem Statement
Tennis players, especially at the junior and recreational level, have no access to affordable, real-time feedback on their shot accuracy and reaction time during solo practice sessions, meaning weeks of training can go by with no objective measure of whether they are actually improving.
___

## Solution
Project Vector is a wall-mounted smart target system made up of 12 interactive pads arranged in a grid, each embedded with a piezoelectric sensor that detects the impact of a tennis ball and a WS2812B LED ring that lights up to indicate the target. An Arduino Mega processes all sensor inputs in real time, randomly activating pads one at a time and tracking whether the player hits the correct target within the time window, giving instant audio-visual feedback through colour-coded LEDs and a buzzer. The entire system is built from low-cost, off-the-shelf electronics and laser-cut components, making it affordable and replicable, while the firmware supports multiple game modes so players can train for pure accuracy, speed, or a combination of both.
___

## Additional Information
Project Vector started as a personal frustration — Vidyut trains tennis seriously and always felt that solo practice was essentially guesswork, hitting ball after ball with no real way to know if shot placement was getting better or just feeling better. That frustration turned into a question: could we build something that gives a player the kind of feedback a coach would give, but available any time, anywhere, without needing another person on the court?

The build itself happened at Makers Asylum Goa, where the team had five days to go from a concept and a box of components to a working wall-mounted system. A lot went wrong along the way — sensors that needed recalibrating, wiring that had to be redone, foam layers that didn't sit right — and honestly that process of breaking things and fixing them taught the team more than any project before it.

Looking ahead, the immediate goal is to add a small display that shows live session stats so a player can walk away knowing their accuracy percentage and average reaction time. Further down the line, the team wants to explore connecting the system to a simple app over WiFi using the ESP32, so players can track their improvement over weeks and months rather than just within a single session. The longer term dream is to make a version of this cheap enough and simple enough to install in school sports programmes across India, where access to quality sports coaching is limited but the desire to improve is very much there.
___

## TLDR
A wall-mounted smart tennis training system that lights up target pads, detects your hits in real time, and scores your accuracy and speed.
___

## License

Licenses

<a href="LICENSE.md"><img src="Licenses_facts.svg" width="400" alt="Open Source Licenses Facts"/></a>

#### Hardware
CERN Open Hardware License Version 2 - Strongly Reciprocal ([CERN-OHL-S-2.0](https://spdx.org/licenses/CERN-OHL-S-2.0.html)).

#### Software
MIT open source [license](http://opensource.org/licenses/MIT).

#### Documentation:
<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.

---
## [OSHWA Certification](https://certification.oshwa.org/list.html)

[OSHW] IN000097 | Certified open source hardware | oshwa.org/cert

<a href="https://certification.oshwa.org/in000097.html"><img src="/certification-mark-IN000097-stacked.png" width="300"/></a>

---

## 📬 Contact/Team

> _List team members and contact emails or GitHub profiles._
 
[Vidyut Sundar](https://github.com/vidyutsundar-cloud)

Anjali Randery

Saanvi Singh

[Maker's Asylum](https://github.com/MakersAsylumIndia)

---
