
#include <stdio.h>
#include "timers.h"
#include "Roach_Events.h"
#include "roach.h"
#include "Roach_State_Machine.h"
#define NAV_TIMER 1
const int stallThreshold = 3;
const int Speed = 75;

//a list of states that this SM uses:

enum {
    Forward,
    Left,
    Right,
    Hiding,
    Reversing,
};

int current_state;
int stallCount = 0;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM
 * (that is, the actions on the arrow from the black dot in the SM diagram)
 */

void Initialize_RoachStateMachine(void) {
    current_state = Forward;
    Roach_LeftMtrSpeed(Speed);
    Roach_RightMtrSpeed(Speed);
    TIMERS_InitTimer(NAV_TIMER, 800);
    
    //seed rand:
        srand(Roach_LightLevel());
};

/*
 * @briefThis function feeds newly detected events to the roach state machine.
 * @param event:  The most recently detected event
 */

void Run_RoachStateMachine(Event event) {
    
    switch (current_state) {
            
        case Forward:
            printf("current_state = Forward");
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                Roach_LeftMtrSpeed(-Speed);
                Roach_RightMtrSpeed(Speed);
                printf("Current state = Left");
                TIMERS_InitTimer(NAV_TIMER, 150);
                current_state = Left;
            }
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                printf("Current state = Right");
                TIMERS_InitTimer(NAV_TIMER, 250);
                Roach_LeftMtrSpeed(Speed);
                Roach_RightMtrSpeed(-Speed);
                current_state = Right;
            }
            if(event == STALL){
                  Roach_LeftMtrSpeed(-100);
                  Roach_RightMtrSpeed(-60);
                  TIMERS_InitTimer(NAV_TIMER, 700);
                  current_state = Reversing;
            }
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;

        case Left:
            printf("current_state = Left");
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                current_state = Forward;
                Roach_LeftMtrSpeed(Speed);
                Roach_RightMtrSpeed(Speed);
                TIMERS_InitTimer(NAV_TIMER, 400);
            }
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;

        case Reversing:
                if(event == NAV_TIMER_EXPIRED){
                    Roach_LeftMtrSpeed(Speed);
                    Roach_RightMtrSpeed(Speed);
                    current_state = Forward;
                }
                if (event == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                }
            break;

        case Right:
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                Roach_LeftMtrSpeed(-Speed);
                Roach_RightMtrSpeed(Speed);
                printf("Current state = Left");
                TIMERS_InitTimer(NAV_TIMER, 200);
                current_state = Left;
            }
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Forward;
                Roach_LeftMtrSpeed(Speed);
                Roach_RightMtrSpeed(Speed);
                TIMERS_InitTimer(NAV_TIMER, 1200);
            }
            if (event == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;

        case Hiding:
            printf("Current state:  Hiding\r\n");
            //no transitions out of hiding (for now)
            break;
    }
}
