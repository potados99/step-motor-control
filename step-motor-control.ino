#define _MOTOR_CLOCK_PIN_NUMBER 12
#define _MOTOR_DIRECTION_PIN_NUMBER 13
#define _ROTATION_CW 0
#define _ROTATION_CCW 1
#define _STEP_ANGLE (1.8)
#define _STEP_RATE (360 / _STEP_ANGLE * 2)
#define _MAX_SPEED 2100



void main_routine() {

  for (unsigned register int i = 1; i < _MAX_SPEED; i += 1) {
    rotate(0.01, i, _ROTATION_CCW);
  }

  for (unsigned register int i = _MAX_SPEED; i >= 1000; i -= 1) {
    rotate(0.01, i, _ROTATION_CCW);
  }
  
  delay(500);
  
  for (unsigned register int i = _MAX_SPEED - 600; i > 300 ; i -= 100) {
    rotate(1, i, (i/100) % 2);
  }
 

  for (unsigned register int i = 0; i < 8; ++ i) {
    rotate((double)_c_scale()[i] / 4000, _c_scale()[i], _ROTATION_CCW);
  }
  
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

