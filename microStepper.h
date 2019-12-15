#include <stdint.h>
class periodic_uint {
    private:
        uint8_t data;
        uint8_t init;
        uint8_t max_val;
    public:
        periodic_uint(uint8_t init,uint8_t max)
        : init(init),max_val(max), data(0){}
        void operator ++(int n){
            if(this->is_max()){
                this->data = this->init;
            }else{
                this->data ++;
            }
        }
        // periodic_uint operator +(uint8_t n)const {
        //     periodic_uint copy = *this;
        //     for(uint8_t i=0; i<n;i++){
        //         copy++;
        //     }
        // }
        // periodic_uint operator -(uint8_t n) const{
        //     periodic_uint copy = *this;
        //     for(uint8_t i=0; i<n;i++){
        //         copy--;
        //     }
        // }
        void operator --(int n){
            if(this->is_init()){
                this->data = this->max_val;
            }else{
                this->data --;
            }
        }
        operator uint8_t() const {
            return this->data;
        }
        // bool operator == (uint8_t right) {
        //     return this->data == right;
        // }
        bool is_max() {
            return this->data == this->max_val;
        }
        bool is_init() {
            return this->data == this-> init;
        }
};

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
        periodic_uint step;
        periodic_uint phase;
    public:
        microStepper(uint16_t steps_per_rev, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
            : microStepper(steps_per_rev, 16, pin1, pin2, pin3, pin4) {}
        microStepper(uint16_t steps_per_rev, uint8_t step_num, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
            : steps_per_rev(steps_per_rev), pin1(pin1), pin2(pin2), pin3(pin3), pin4(pin4), step_num(step_num),step(periodic_uint(0,3)),phase(0,step_num-1)
        {
            pinMode(pin1, OUTPUT);
            pinMode(pin2, OUTPUT);
            pinMode(pin3, OUTPUT);
            pinMode(pin4, OUTPUT);
            wave = new uint8_t[step_num + 1];
            for (int i = 0; i < step_num; i++)
            {
                wave[i] = (uint8_t)(255 * sin(PI / (2 * step_num) * i));
            }
            wave[step_num] = 255;
        }
        void setSpeed(int16_t rpm) {
            this->rpm = rpm;
        }
        // void step(int16_t step) {
        //     int16_t step_interval = (int16_t)(60*(long)1000 / (long)(this->rpm * steps_per_rev)); 
        //     for (int16_t i = 0; i < step ; i++) {
        //         delay(step_interval);
        //         output_by_step(i);
        //     }
        //     digitalWrite(pin1,LOW);
        //     digitalWrite(pin2,LOW);
        //     digitalWrite(pin3,LOW);
        //     digitalWrite(pin4,LOW);
        // }
        void micro_step(uint16_t step,bool rotation){
            unsigned long step_interval = (60UL*1000000UL / ((unsigned long)this->rpm * (unsigned long)steps_per_rev)); 
            // Serial.print("step_interval:");Serial.println(step_interval);
            // Serial.println(step_interval*step/1000000UL);
            
            for (int16_t i = 0; i < step ; i++) {
                do{
                    // Serial.print("Step:");
                    // Serial.print(this->step);
                    // Serial.print("Phase:");
                    // Serial.println(phase);
                    unsigned long last = micros();
                    while(micros()-last < step_interval/step_num){}
                    output_by_phase_in_micro(this->step,this->phase);
                    this->phase++;
                }while (! this->phase.is_init());
                this->step++;
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

        void output_by_phase_in_micro(periodic_uint step,periodic_uint phase) {
            uint8_t pins[4] = {pin1, pin3, pin2, pin4};
            for (uint8_t i = 0; i < 4 ; i++) {
                if (step == i) {
                    analogWrite(pins[i], wave[phase]); // 0,1,2,3
                } else if (step == ((i+1)%4)){
                    analogWrite(pins[i], wave[step_num-phase]); // 4,3,2,1
                }else {
                    digitalWrite(pins[i], LOW);
                }
            }
            // Serial.print(pins[step]);
            // Serial.print(':');
            // Serial.print(wave[phase]);
            // Serial.print('\t');
            // periodic_uint copy = step;
            // copy--;
            // Serial.print(pins[copy]);
            // Serial.print(':');
            // Serial.println(wave[step_num-phase]);
        }
};

