#ifndef CNCCONTROLLER_H
#define CNCCONTROLLER_H

#include <MKL25Z4.h>
#include "Stepper.h"
#include "math.h"

class CNCController{
    private:
        Stepper motorX;
        Stepper motorY;
        Stepper motorZ;

        float motorXPosition;  // Motor X position in millimeters
        float motorYPosition;  // Motor Y position in millimeters
        float motorZPosition;  // Motor Z position in millimeters

        // Stop switch pin numbers
        int stopSwitchX;
        int stopSwitchY;
        int stopSwitchZ;

        bool isXStopSwitchPressed();
        bool isYStopSwitchPressed();
        bool isZStopSwitchPressed();

        void configureTPM();
        float calculateRadius(float centerX, float centerY, float endX, float endY);
        float calculateAngle(float centerX, float centerY, float x, float y, float radius);

    public:
        CNCController();
        void setPinX(int pin, int enable, int steps, int radius, int stopSwitch);
        void setPinY(int pin, int enable, int steps, int radius, int stopSwitch);
        void setPinZ(int pin, int enable, int steps, int radius, int stopSwitch);

        void initStopSwitches();

        void moveX(float distance); 
        void moveY(float distance);
        void moveZ(float distance);

        void moveArc(float centerX, float centerY, float endX, float endY, bool isClockwise);

        void home();
        void setRelativePosition(float x, float y, float z);
};

#endif // CNCCONTROLLER_H