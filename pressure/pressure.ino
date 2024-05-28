#define SERVO_PIN 3  
volatile uint16_t value = 0;

int tick_calc(int x0, int x1, int y0, int y1, int x) {
  return y0 + ((x - x0) * (y1 - y0))/ (x1 - x0);
}

int tick0 = 33;  // 0때 측정한 tick 
int tick180 = 143;  // 180때 측정한 tick

//ADC interrupt
ISR (ADC_vect){
  value = ADC;
}

void brake(){
  //테스트 용도 angle
  int angle = random(0,91);
  Serial.print("angle: ");
  Serial.print(angle);

  if (angle > 10){
    int desired_tick = tick_calc(0, 180, tick0, tick180, angle);
    OCR2B = desired_tick;
    Serial.print("desired_tick: ");
    Serial.print(desired_tick);
    Serial.println("brake activated");
  }
  else{
    Serial.println("not activated");
  }
}

void setup(){
  Serial.begin(9600);

  //ADC
  ADMUX |= (0 << REFS1) | (1 << REFS0);
  ADMUX |= (0 << ADLAR);
  ADMUX |= (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);

  ADCSRA |= (1 << ADATE);
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADIE);
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  ADCSRB |= (0 << ADTS0) |(0 << ADTS1) |(0 << ADTS0);

  SREG |= 0x01 << SREG_I;
  ADCSRA |= (1 << ADSC);

  //SERVO
  pinMode(SERVO_PIN, OUTPUT);
  TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2B1); 
  TCCR2B = (1 << CS21) | (1 << CS22);  
}
 
void loop(){
  Serial.print(value);
  if (value != 0){
    brake();
  }
  delay(100);
}