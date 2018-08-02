
#define _ENABLE_PIN_NUMBER 2
#define _MODE0_PIM_NUMBER 3
#define _MODE1_PIM_NUMBER 4
#define _MOTOR_CLOCK_PIN_NUMBER 8
#define _MOTOR_DIRECTION_PIN_NUMBER 9

#define _STEP_ANGLE (1.8)
#define _STEPS_PER_ROTAION (360 / _STEP_ANGLE)/* One Step */
#define _MAX_SPEED 2100

double CLKS_PER_STEP = 2;

typedef enum _Mode {
  SIXTEENTH_STEP = 0,
  EIGHTH_STEP = 1,
  QUARTER_STEP = 2,
  HALF_STEP = 3
} Mode;

typedef enum _Direction {
  RIGHT = 0,
  LEFT
} Direction;


void setup() {
  Serial.begin(9600);
  init_pins();
  set_mode(SIXTEENTH_STEP);
}

void loop() {
  unsigned const infiniteLoop = true;
  unsigned const int repeats = 1;
  
  unsigned static int _count = 0;
  if (! infiniteLoop && _count >= repeats) return;

  main_routine();

  ++ _count;
}

void main_routine() {
  //test_acceleration(3600, 14400, RIGHT);

  rotate(1, 60, RIGHT);
  delay(100);
  //test_scale();

  //bnn();

  //rotate(30, 2000, 0);

}


void init_pins() {
  enable(true);
  pinMode(_MOTOR_CLOCK_PIN_NUMBER, OUTPUT);
  pinMode(_MOTOR_DIRECTION_PIN_NUMBER, OUTPUT);
}

void enable(bool enable) {
  if (enable) {
      pinMode(_ENABLE_PIN_NUMBER, OUTPUT);
      digitalWrite(_ENABLE_PIN_NUMBER, HIGH);
  }
}

void set_mode(Mode _mode) {
  pinMode(_MODE0_PIM_NUMBER, OUTPUT);
  pinMode(_MODE1_PIM_NUMBER, OUTPUT);
  
  char m0 = (_mode & B10) ? HIGH : LOW;
  char m1 = (_mode & B01) ? HIGH : LOW;

  digitalWrite(_MODE0_PIM_NUMBER, m0);
  digitalWrite(_MODE1_PIM_NUMBER, m1);

  Serial.println(_mode);
    Serial.println("ya");

  CLKS_PER_STEP = pow(2, 4 - _mode);
}


void rotate(double _rotations, double _speed, Direction _direction) {
  // speed: RPM
  Serial.println(CLKS_PER_STEP);
  Serial.println(CLKS_PER_STEP);

  _set_rotation_direction(_MOTOR_DIRECTION_PIN_NUMBER, _direction);

  double rotationsPerSecond = _speed / 60;
  double clocksPerRotation = CLKS_PER_STEP * _STEPS_PER_ROTAION; 
  double clocksPerSecond = rotationsPerSecond * clocksPerRotation;
  double clockCycle = 1000 / clocksPerSecond; /* milli second */
  
  double totalClocks = CLKS_PER_STEP * _STEPS_PER_ROTAION * _rotations; // 400
  
  for (unsigned register int i = 0; i < totalClocks; ++ i) {
     _do_one_clock(_MOTOR_CLOCK_PIN_NUMBER, clockCycle);
  }
}

inline void _do_one_clock(int _pinNumber, double _clockCycle) {
  digitalWrite(_pinNumber, HIGH);
  delayMicroseconds(_clockCycle * 500);
  digitalWrite(_pinNumber, LOW);
  delayMicroseconds(_clockCycle * 500);
}

void _set_rotation_direction(int _pinNumber, Direction _direction) {
  digitalWrite(_pinNumber, (_direction == LEFT) ? HIGH : LOW);
}



/* Test Functions */

void test_acceleration(int _minSpeed, int _maxSpeed, int _direction) {
  
  for (unsigned register int i = _minSpeed; i < _maxSpeed; i += 1) {
    rotate(0.01, i, _direction);
  }

  for (unsigned register int i = _maxSpeed; i >= _minSpeed; i -= 1) {
    rotate(0.01, i, _direction);
  }
}
