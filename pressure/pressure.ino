#define SERVO_PIN 3  
volatile uint16_t pressure = 0;

//ADC interrupt
ISR (ADC_vect){
  pressure = ADC;
}

//tick 값 계산
int tick_calc(int x){
  int tick0 = 33;  // 0때 측정한 tick 
  int tick180 = 143;  // 180때 측정한 tick
  int x0 = 0;
  int x1 = 180;
  return tick0 + ((x - x0) * (tick180 - tick0))/ (x1 - x0);
}

void brake(){
  //테스트 용도 random angle (IMU에서 가져올 것)
  int angle = random(0,91);
  Serial.print("angle: ");
  Serial.print(angle);

  if (angle > 10){ // 일정 각도 이상에서
    int desired_tick = tick_calc(angle); // tick 계산 
    Serial.print("desired_tick: ");
    Serial.print(desired_tick);
    OCR2B = desired_tick; // 값 입력(브레이크 작동)
    Serial.println("brake activated");
  }
  else{
    Serial.println("not activated"); //일정 각도 이하 -> 브레이크 작동X
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

  //SERVO setting
  pinMode(SERVO_PIN, OUTPUT);
  TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2B1); 
  TCCR2B = (1 << CS21) | (1 << CS22);  
}
 
void loop(){
  Serial.print(pressure);
  if (pressure == 0){ //압력 센서 값
    brake();
  }
}