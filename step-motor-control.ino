#define _MOTOR_CLOCK_PIN_NUMBER 12
#define _MOTOR_DIRECTION_PIN_NUMBER 13
#define _ROTATION_CW 0
#define _ROTATION_CCW 1
#define _STEP_ANGLE (1.8)
#define _STEP_RATE (360 / _STEP_ANGLE * 2)
#define _MAX_SPEED 2100



void main_routine() {
  test_acceleration();

  test_scale();

  //bnn();

}



void setup() {
  init_pins();
}

void loop() {
  unsigned const infiniteLoop = false;
  unsigned const int repeats = 1;
  
  unsigned static int _count = 0;
  if (! infiniteLoop && _count >= repeats) return;

  main_routine();

  ++ _count;
}


void init_pins() {
  pinMode(_MOTOR_CLOCK_PIN_NUMBER, OUTPUT);
  pinMode(_MOTOR_DIRECTION_PIN_NUMBER, OUTPUT);
}

void rotate(double _rotations, double _speed, int _direction) {
  // speed: How many clocks in 1000 milli seconds (400 clocks in one rotations)

  _set_rotation_direction(_MOTOR_DIRECTION_PIN_NUMBER, _direction);

  double _clockCycle = 1000 / _speed;
  
  for (unsigned register int i = 0; i < _rotations * _STEP_RATE; ++ i) {
     _do_one_clock(_MOTOR_CLOCK_PIN_NUMBER, _clockCycle);
  }
}

void _do_one_clock(int _pinNumber, double _clockCycle) {
  digitalWrite(_pinNumber, HIGH);
  delayMicroseconds(_clockCycle * 500);
  digitalWrite(_pinNumber, LOW);
  delayMicroseconds(_clockCycle * 500);
}

void _set_rotation_direction(int _pinNumber, int _direction) {
  digitalWrite(_pinNumber, _direction);
}

int *_c_scale() {
  static int scale[] = {262, 294, 330, 349, 392, 440, 494, 523};
  return scale;
}

int *_low_c_scale() {
  static int scale[] = {131, 147, 165, 175, 196, 220, 247, 262};
  return scale;
}


void test_acceleration() {
  for (unsigned register int i = 100; i < _MAX_SPEED; i += 1) {
    rotate(0.01, i, _ROTATION_CCW);
  }

  for (unsigned register int i = _MAX_SPEED; i >= 100; i -= 1) {
    rotate(0.01, i, _ROTATION_CCW);
  }
}

void test_scale() {
  for (unsigned register int i = 0; i < 8; ++ i) {
    rotate((double)_c_scale()[i] / 16000, _c_scale()[i] / 4, _ROTATION_CCW);
  }

  for (unsigned register int i = 0; i < 8; ++ i) {
    rotate((double)_c_scale()[i] / 8000, _c_scale()[i] / 2, _ROTATION_CCW);
  }
  
  for (unsigned register int i = 1; i < 8; ++ i) {
    rotate((double)_c_scale()[i] / 4000, _c_scale()[i], _ROTATION_CCW);
  }
  
  for (unsigned register int i = 1; i < 8; ++ i) {
    rotate((double)_c_scale()[i] / 2000, _c_scale()[i] * 2, _ROTATION_CCW);
  }
}




void bnn() {
  
  bnn_unit(_c_scale()[0]);
  delay(300);
  bnn_unit(_c_scale()[2]);
  delay(300);
  bnn_unit(_c_scale()[4]);
  delay(300);
  bnn_unit(_c_scale()[7]);

  for (int i = 0; i < 5; ++ i) {

    for (unsigned register int i = 0; i <= 30; i += 1) {
      rotate(0.01, _c_scale()[7] - i, _ROTATION_CCW);
    }
    
    for (unsigned register int i = 30; i <= 30; i += 1) {
      rotate(0.01, _c_scale()[7] - 30 + i, _ROTATION_CCW);
    }
    
  }
  rotate((double)_c_scale()[7] / 1000, _c_scale()[7], _ROTATION_CCW);  
}

inline void bnn_unit(int note) {
   rotate((double)note / 2000, note, _ROTATION_CCW);
   delay(150);
   rotate((double)note / 2000, note, _ROTATION_CCW);
   delay(150);
   rotate((double)note / 1000, note, _ROTATION_CCW);

   delay(150);
   rotate((double)note / 2000, note, _ROTATION_CCW);
   delay(150);
   rotate((double)note / 2000, note, _ROTATION_CCW);
   delay(150);
   rotate((double)note / 2000, note, _ROTATION_CCW);
   delay(1);
   rotate((double)note / 1000, note, _ROTATION_CCW);
}

