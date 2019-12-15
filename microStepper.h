#include <stdint.h>
class periodic_uint {
    private:
        uint8_t data;
        uint8_t init;
        uint8_t max;
    public:
        periodic_uint(uint8_t init,uint8_t max)
        : init(init),max(max), data(0){}
        void operator ++(int n){
            if(this->data == this->max){
                this->data = this->init;
            }else{
                this->data ++;
            }
        }
        void operator --(int n){
            if(this->data == this->init){
                this->data = this->max;
            }else{
                this->data --;
            }
        }
        operator uint8_t() const {
            return this->data;
        }
}

class microStepper {
    private:
        const uint16_t steps_per_rev;
        const uint8_t pin1;
        const uint8_t pin2;
        const uint8_t pin3;
        const uint8_t pin4;
        const int16_t step_num;
        uint8_t *wave;
        int16_t rpm;
    public:
        microStepper(uint16_t steps_per_rev, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
            : steps_per_rev(steps_per_rev), pin1(pin1), pin2(pin2), pin3(pin3), pin4(pin4), step_num(64) {
                pinMode(pin1, OUTPUT);
                pinMode(pin2, OUTPUT);
                pinMode(pin3, OUTPUT);
                pinMode(pin4, OUTPUT);
                wave = new uint8_t[step_num+1];
                for(int i=0;i<step_num;i++){
                    wave[i]=(uint8_t)(255 * sin(PI/(2*step_num)*i));
                }
                wave[step_num]=255;
            }
        void setSpeed(int16_t rpm) {
            this->rpm = rpm;
        }
        void step(int16_t step) {
            int16_t step_interval = (int16_t)(60*(long)1000 / (long)(this->rpm * steps_per_rev)); 
            for (int16_t i = 0; i < step ; i++) {
                delay(step_interval);
                output_by_step(i);
            }
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,LOW);
            digitalWrite(pin3,LOW);
            digitalWrite(pin4,LOW);
        }
        void micro_step(int16_t step){
            unsigned int step_interval = (unsigned int)(60*1000000 / ((unsigned long)this->rpm * steps_per_rev)); 
            for (int16_t i = 0; i < step ; i++) {

                for(int16_t phase = 0 ; phase < step_num ; phase++ ){
                    output_by_phase_in_micro(i,phase);
                    delayMicroseconds(step_interval/step_num);
                }
            }
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,LOW);
            digitalWrite(pin3,LOW);
            digitalWrite(pin4,LOW);

        }

    private:
        void output_by_step(int16_t step) {
            uint8_t pins[4] = {pin1, pin3, pin2, pin4};
            for (uint8_t i = 0; i < 4 ; i++) {
                if (step % 4 == i) {
                    digitalWrite(pins[i], HIGH);
                } else {
                    digitalWrite(pins[i], LOW);
                }
            }
        }

        void output_by_phase_in_micro(int16_t step,int16_t phase) {
            uint8_t pins[4] = {pin1, pin3, pin2, pin4};
            for (uint8_t i = 0; i < 4 ; i++) {
                if (step % 4 == i) {
                    analogWrite(pins[i], wave[phase]); // 0,1,2,3
                } else if (step%4 == (i+1)%4){
                    analogWrite(pins[i], wave[step_num-phase]); // 4,3,2,1
                }else {
                    digitalWrite(pins[i], LOW);
                }
            }
        }
};

