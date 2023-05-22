#include "Stepper.h"

/**
 * @brief Set the pins for controlling the stepper motor.
 * @param pin The pin number connected to the STEP signal of the stepper motor driver.
 * @param enable The pin number connected to the ENABLE signal of the stepper motor driver.
 * @param steps The number of steps per revolution of the stepper motor.
 * @param radius The radius of the pulley or gear attached to the stepper motor.
*/
void Stepper::setPins(int pin, int enable, int steps, int radius){

    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; 
    PORTE->PCR[pin] = 0x100;
    PORTE->PCR[enable] = 0x100;
    this->pin = (1<<pin);
    this->enable = (1<<enable);
    this->steps = steps; 
    PTE->PDDR |= ((this->pin) | (this->enable));
}

/**
 * @brief Disable the stepper motor by setting the ENABLE pin to the appropriate level.
*/
void Stepper::disableMotor(){
    PTE->PDOR &= ~(this->enable);
}


/**
 * @brief Enable the stepper motor by setting the ENABLE pin to the appropriate level.
*/
void Stepper::enableMotor(){
    PTE->PDOR |= (this->enable);
}

/**
 * @brief Generate a pulse on the STEP pin to move the stepper motor by one step.
*/
void Stepper::pulse(){
    PTE->PDOR |= (this->pin);        // Set the STEP pin high
    delay_ms(10);           // Delay between pulses
    PTE->PDOR &= ~(this->pin);       // Set the STEP pin low
    delay_ms(10);           // Delay between pulses
}

/*
    N pulsos -> 1rev

    steps -> Diametro * pi
    steps? -> distancia

    distancia * steps / (2*radius*pi)
*/

/**
 * @brief Move the stepper motor a specified distance in millimeters.
 * @param distance The distance to move in millimeters.
*/
void Stepper::move_mm(float distance, int* x, int* y, int* z){
    int stepsToMove = ((distance * steps) / (2.0 * PI * radius));  // Calculate the number of steps to move based on distance steps and the gear radius

    for (int i = 0; i < stepsToMove; i++) {
        pulse();  // Generate a pulse for each step
    }
}

/**
 * @brief Getter function to retrieve the current position.
*/
int Stepper::getPosition() {
    return position;
}
