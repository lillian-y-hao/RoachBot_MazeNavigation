#include <stdio.h>
#include "timers.h"
#include "Roach_Events.h"
#include "roach.h"
#include "Roach_State_Machine.h"
#define NAV_TIMER 1
//a list of states that this SM uses:
const int stallThreshold = 10;

enum {
    Forward,
    Right,
    Left,
    First_Turn,
    Reversing,
    Hiding,
};

int current_state;
int stallCount = 0;

/* This function initializes the roach state machine.
 * At a minimum, this requires setting the first state.
 * Also, execute any actions associated with initializing the SM
 * (that is, the actions on the arrow from the black dot in the SM diagram)*/
void Initialize_RoachStateMachine(void) {


    Roach_LeftMtrSpeed(75);
    Roach_RightMtrSpeed(-75);

    TIMERS_InitTimer(NAV_TIMER, 500);
    current_state = First_Turn;
    //Roach_LeftMtrSpeed(0);
    //Roach_RightMtrSpeed(0);
    //seed rand:
    srand(Roach_LightLevel());
};



/*
 * @briefThis function feeds newly detected events to the roach state machine.
 * @param event:  The most recently detected event*/
void Run_RoachStateMachine(Event event) {
    switch (current_state) {

        case First_Turn:
            printf("Current state = First Turn\r\n");
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                printf("Current state = Forward\r\n");
                TIMERS_InitTimer(NAV_TIMER, 1000);
                Roach_LeftMtrSpeed(75);
                Roach_RightMtrSpeed(75);
                current_state = Forward;
            }
            break;

        case Forward:
            printf("Current state = Forward\r\n");
            //            if (event == FRONT_LEFT_BUMP_PRESSED && event == FRONT_RIGHT_BUMP_PRESSED) {
            //                printf("Current state = Left\r\n");
            //                TIMERS_InitTimer(NAV_TIMER, 200);
            //                Roach_LeftMtrSpeed(-75);
            //                Roach_RightMtrSpeed(75);
            //                current_state = Left;
            //            }
           // Run_CheckStallCount();
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                printf("Current state = Right\r\n");
                TIMERS_InitTimer(NAV_TIMER, 400);
                Roach_LeftMtrSpeed(-75);
                Roach_RightMtrSpeed(75);
                current_state = Left;
            }
            if (event == FRONT_LEFT_BUMP_PRESSED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(75);
                printf("Current state = Left\r\n");
                TIMERS_InitTimer(NAV_TIMER, 300);
                current_state = Left;
            }
            if (CheckForLightEvents() == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }

            break;

        case Left:
           // Run_CheckStallCount();
            if (event == NAV_TIMER_EXPIRED) {
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
                current_state = Forward;
                Roach_LeftMtrSpeed(75);
                Roach_RightMtrSpeed(75);
                TIMERS_InitTimer(NAV_TIMER, 1200);
            }
            if (CheckForLightEvents() == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;

        case Right:
            //            if (event == FRONT_LEFT_BUMP_PRESSED && event == FRONT_RIGHT_BUMP_PRESSED) {
            //                printf("Current state = Left\r\n");
            //                TIMERS_InitTimer(NAV_TIMER, 200);
            //                Roach_LeftMtrSpeed(-75);
            //                Roach_RightMtrSpeed(75);
            //                current_state = Left;
            //            }
           // Run_CheckStallCount();
            if (event == FRONT_RIGHT_BUMP_PRESSED) {
                Roach_LeftMtrSpeed(-75);
                Roach_RightMtrSpeed(75);
                printf("Current state = Left\r\n");
                TIMERS_InitTimer(NAV_TIMER, 200);
                current_state = Left;
            }
            if (event == NAV_TIMER_EXPIRED) {
                current_state = Forward;
                Roach_LeftMtrSpeed(75);
                Roach_RightMtrSpeed(75);
                TIMERS_InitTimer(NAV_TIMER, 1000);
            }
            if (CheckForLightEvents() == ENTERED_DARK) {
                current_state = Hiding;
                Roach_LeftMtrSpeed(0);
                Roach_RightMtrSpeed(0);
            }
            break;

        case Reversing:
            if (event == NAV_TIMER_EXPIRED) {
                printf("REVERSING!!!!!!!!\r\n");
                TIMERS_InitTimer(NAV_TIMER, 400);
                current_state = Left;
                Roach_LeftMtrSpeed(-75);
                Roach_RightMtrSpeed(75);

            }
            break;

    };




}

