#include "Worm.hpp"
#include "behaviors.hpp"

Worm::Worm() {
  this->_leftMuscle = 0;
  this->_rightMuscle = 0;

  this->_motorFireAvg = 16.0;

  ctm_init(&this->_connectome);
}

void Worm::chemotaxis() {
  this->_update(CHEMOTAXIS, CHEMOTAXIS_LEN);
}

void Worm::noseTouch() {
  this->_update(NOSE_TOUCH, NOSE_TOUCH_LEN);
}

int Worm::getLeftMuscle() {
  return this->_leftMuscle;
}

int Worm::getRightMuscle() {
  return this->_rightMuscle;
}

void Worm::_update(const uint16_t* stim_neuron, int len_stim_neuron) {

  Connectome* ctm = &this->_connectome;

  //
  // Run one tick of neural emulation
  //

  ctm_neural_cycle(ctm, stim_neuron, len_stim_neuron);

  //
  // Aggregate muscle states
  //

  uint16_t body_total = 0;
  // Gather totals on body muscles
  for(int i = 0; i < BODY_MUSCLES; i++) {
    uint16_t left_id = READ_WORD(left_body_muscle, i);
    uint16_t right_id = READ_WORD(right_body_muscle, i);

    int16_t left_val = ctm_get_weight(ctm, left_id);
    int16_t right_val = ctm_get_weight(ctm, right_id);

    if(left_val < 0) {
      left_val = 0;
    }

    if(right_val < 0) {
      right_val = 0;
    }

    body_total += (left_val + right_val);
  }

  uint16_t norm_body_total = 255.0 * ((float) body_total) / 600.0;

  // Gather total for neck muscles
  uint16_t left_neck_total = 0;
  uint16_t right_neck_total = 0;
  for(int i = 0; i < NECK_MUSCLES; i++) {
    uint16_t left_id = READ_WORD(left_neck_muscle, i);
    uint16_t right_id = READ_WORD(right_neck_muscle, i);

    int16_t left_val = ctm_get_weight(ctm, left_id);
    int16_t right_val = ctm_get_weight(ctm, right_id);

    if(left_val < 0) {
      left_val = 0;
    }

    if(right_val < 0) {
      right_val = 0;
    }

    left_neck_total += left_val;
    right_neck_total += right_val;
  }

  // Combine neck contribution with body
  int32_t neck_contribution = left_neck_total - right_neck_total;
  int32_t left_total;
  int32_t right_total;
  if(neck_contribution < 0) {
    left_total = 6*abs(neck_contribution) + norm_body_total;
    right_total = norm_body_total;
  }
  else {
    left_total = norm_body_total;
    right_total = 6*abs(neck_contribution) + norm_body_total;
  }

  // Log A and B type motor neuron activity
  double motor_neuron_sum = 0;

  /*
  for(int i = 0; i < MOTOR_B; i++) {
    uint16_t id = READ_WORD(motor_neuron_b, i);
    motor_neuron_sum += ctm_get_discharge(ctm, id);
  }
  */

  for(int i = 0; i < MOTOR_A; i++) {
    uint16_t id = READ_WORD(motor_neuron_a, i);
    motor_neuron_sum += ctm_get_discharge(ctm, id);
  }

  //const double motor_total = MOTOR_A + MOTOR_B;
  const double motor_total = MOTOR_A;
  const int avg_window = 15;
  double motor_neuron_percent = 100.0 * motor_neuron_sum / motor_total;

  this->_motorFireAvg = (motor_neuron_percent + (avg_window*this->_motorFireAvg))/(avg_window + 1.0);

  if(this->_motorFireAvg > 19.0) { // Magic number read off from c_matoduino simulation
    left_total *= -1;
    right_total *= -1;
  }

  this->_leftMuscle = left_total;
  this->_rightMuscle = right_total;
}
