// #pragma once

extern int step_number = 0;
extern float temp = 0;
extern float beatsss = 0;

int get_stepNum() {
  return step_number;
}

float get_temperature() {
  return temp;
}

float get_beats() {
  return beatsss;
}

void set_stepNum(int stepNum_p) {
  step_number = stepNum_p;
}

void set_temperature(float temperature_p) {
  temp = temperature_p;
}

void set_beats(float beats_p) {
  beatsss = beats_p;
}