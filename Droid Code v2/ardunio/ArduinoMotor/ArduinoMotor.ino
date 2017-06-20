#define MIN_SIG 0
#define MAX_SIG 255
#define SIG_TIMEOUT_MS 40

typedef struct {
  int pwm_pin, dir_pin, speed;
} Motor;

Motor m_a = { 3,  12, 125 };
Motor m_b = { 11, 13, 125 };

void updateSpeed(Motor m) {
  analogWrite(m.pwm_pin, m.speed);
}

void updateSpeed(Motor m, const int target) {
  if(target >= MIN_SIG || target <= MAX_SIG) {
    m.speed = target;
    updateSpeed(m);
  }
}

void slowDownTo(const int target) {
  // (m_a.speed + m_b.speed) / 2 = average speed
  for(int i = (m_a.speed + m_b.speed) / 2; i >= target; i--) {
    updateSpeed(m_a, i);
    updateSpeed(m_b, i);
    Serial.println(i);
    delay(SIG_TIMEOUT_MS);
  }
}

void speedUpTo(const int target) {
  // (m_a.speed + m_b.speed) / 2 = average speed
  for(int i = (m_a.speed + m_b.speed) / 2; i <= target; i++) {
    updateSpeed(m_a, i);
    updateSpeed(m_b, i);
    Serial.println(i);
    delay(SIG_TIMEOUT_MS);
  }
}

void setDirectionForwards(Motor m) {
  digitalWrite(m.dir_pin, HIGH);
}

void setDirectionBackwards(Motor m) {
  digitalWrite(m.dir_pin, LOW);
}

void setup() {
  pinMode(m_a.pwm_pin, OUTPUT);
  pinMode(m_b.pwm_pin, OUTPUT);
}

void loop() {
  Serial.begin(9600);
  // execution waits until we send something to serial monitor

  int timeout = 2000;
  slowDownTo(0);
  setDirectionForwards(m_a);
  setDirectionForwards(m_b);
  speedUpTo(255);
  delay(timeout);
  slowDownTo(0);
  delay(timeout);

  setDirectionForwards(m_a);
  setDirectionBackwards(m_b);
  speedUpTo(255);
  delay(timeout);
  slowDownTo(0);
  delay(timeout);

  setDirectionBackwards(m_a);
  setDirectionForwards(m_b);
  speedUpTo(255);
  delay(timeout);
  slowDownTo(0);
  delay(timeout);

  setDirectionBackwards(m_a);
  setDirectionBackwards(m_b);
  speedUpTo(255);
  delay(timeout);
  slowDownTo(0);
  delay(timeout);

  delay(timeout * 2);
  Serial.println("Iteration complete.\n");
}

