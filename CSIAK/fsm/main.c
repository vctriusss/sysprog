#include <stdio.h>

#define X 17

typedef enum FSM_State_Mealy {
    MealyStart = 0xFF,
    MealyS0 = 0x00,
    MealyS1,
    MealyS2,
    MealyS3,
    MealyS4,
    MealyS5,
    MealyS6,
    MealyS7,
    MealyS8,
    MealyS9,
    MealyS10,
    MealyS11,
    MealyS12,
    MealyS13,
    MealyS14,
    MealyS15,
    MealyS16,
} Mealy_State;

typedef enum FSM_State_Moore {
    MooreStart = 0xFF,
    MooreS0 = 0x00,
    MooreS1,
    MooreS2,
    MooreS3,
    MooreS4,
    MooreS5,
    MooreS6,
    MooreS7,
    MooreS8,
    MooreS9,
    MooreS10,
    MooreS11,
    MooreS12,
    MooreS13,
    MooreS14,
    MooreS15,
    MooreS16,
    MooreS17,
    MooreS18,
    MooreS19,
    MooreS20,
    MooreS21,
    MooreS22,
    MooreS23,
    MooreS24,
    MooreS25,
    MooreS26,
} Moore_State;

struct FSM_state {
    unsigned moore_curent_state;
    unsigned mealy_curent_state;
    int refund;
    int sell;
} globalState;


void doRefund() {
    int _1 = 0;
    int _2 = 0;
    int _2x2 = 0;
    int _5 = 0;
    int _10 = 0;

    while (globalState.refund > 0) {
        if (globalState.refund / 10) {
            _10 = 1;
            globalState.refund -= 10;
        } else if (globalState.refund / 5) {
            _5 = 1;
            globalState.refund -= 5;
        } else if (globalState.refund / 4) {
            _2x2 = 1;
            globalState.refund -= 4;
        } else if (globalState.refund / 2) {
            _2 = 1;
            globalState.refund -= 2;
        } else {
            _1 = 1;
            globalState.refund--;
        }
    }

    printf("Pick up the item\n"
    "Heres the exchange: \n"
    "(10) -> %d coins; "
    "(5) -> %d coins; "
    "(2) x 2 -> %d coins; "
    "(2) -> %d coins; "
    "(1) -> %d coins;\n",
    _10, _5, _2x2, _2, _1);

    globalState.sell = 0;
}


void MealyFSM(int input) {
    unsigned next_state;  
    // Начальная инициализация автомата
    if ( MealyStart == globalState.mealy_curent_state) {
        globalState.mealy_curent_state = MealyS0;
        globalState.refund = 0;
        globalState.sell = 0;
    }


    switch (globalState.mealy_curent_state) {
        case MealyS0:
            if      (1 == input)                next_state = MealyS1;
            else if (2 == input)                next_state = MealyS2;
            else if (5 == input)                next_state = MealyS5;
            else if (10 == input)               next_state = MealyS10;
            break;

        case MealyS1:
            if      (1 == input)                next_state = MealyS2;
            else if (2 == input)                next_state = MealyS3;
            else if (5 == input)                next_state = MealyS6;
            else if (10 == input)               next_state = MealyS11;
            break;

        case MealyS2:
            if      (1 == input)                next_state = MealyS3;
            else if (2 == input)                next_state = MealyS4;
            else if (5 == input)                next_state = MealyS7;
            else if (10 == input)               next_state = MealyS12;
            break;

        case MealyS3:
            if      (1 == input)                next_state = MealyS4;
            else if (2 == input)                next_state = MealyS5;
            else if (5 == input)                next_state = MealyS8;
            else if (10 == input)               next_state = MealyS13;
            break;

        case MealyS4:
            if      (1 == input)                next_state = MealyS5;
            else if (2 == input)                next_state = MealyS6;
            else if (5 == input)                next_state = MealyS9;
            else if (10 == input)               next_state = MealyS14;
            break;

        case MealyS5:
            if      (1 == input)                next_state = MealyS6;
            else if (2 == input)                next_state = MealyS7;
            else if (5 == input)                next_state = MealyS10;
            else if (10 == input)               next_state = MealyS15;
            break;

        case MealyS6:
            if      (1 == input)                next_state = MealyS7;
            else if (2 == input)                next_state = MealyS8;
            else if (5 == input)                next_state = MealyS11;
            else if (10 == input)               next_state = MealyS16;
            break;

        case MealyS7:
            if      (1 == input)                next_state = MealyS8;
            else if (2 == input)                next_state = MealyS9;
            else if (5 == input)                next_state = MealyS12;
            else if (10 == input)               {next_state = MealyS0; globalState.sell = 1; globalState.refund = 17 - X; doRefund();}
            break;

        case MealyS8:
            if      (1 == input)                next_state = MealyS9;
            else if (2 == input)                next_state = MealyS10;
            else if (5 == input)                next_state = MealyS13;
            else if (10 == input)               {next_state = MealyS1; globalState.sell = 1; globalState.refund = 18 - X; doRefund();}
            break;
        case MealyS9:
            if      (1 == input)                next_state = MealyS10;
            else if (2 == input)                next_state = MealyS11;
            else if (5 == input)                next_state = MealyS14;
            else if (10 == input)               {next_state = MealyS2; globalState.sell = 1; globalState.refund = 19 - X; doRefund();}
            break;
        case MealyS10:
            if      (1 == input)                next_state = MealyS11;
            else if (2 == input)                next_state = MealyS12;
            else if (5 == input)                next_state = MealyS15;
            else if (10 == input)               {next_state = MealyS3; globalState.sell = 1; globalState.refund = 20 - X; doRefund();}
            break;
        case MealyS11:
            if      (1 == input)                next_state = MealyS12;
            else if (2 == input)                next_state = MealyS13;
            else if (5 == input)                next_state = MealyS16;
            else if (10 == input)               {next_state = MealyS4; globalState.sell = 1; globalState.refund = 21 - X; doRefund();}
            break;
        case MealyS12:
            if      (1 == input)                next_state = MealyS13;
            else if (2 == input)                next_state = MealyS14;
            else if (5 == input)                {next_state = MealyS0; globalState.sell = 1; globalState.refund = 17 - X; doRefund();}
            else if (10 == input)               {next_state = MealyS5; globalState.sell = 1; globalState.refund = 22 - X; doRefund();}
            break;
        case MealyS13:
            if      (1 == input)                next_state = MealyS14;
            else if (2 == input)                next_state = MealyS15;
            else if (5 == input)                {next_state = MealyS1; globalState.sell = 1; globalState.refund = 18 - X; doRefund();}
            else if (10 == input)               {next_state = MealyS6; globalState.sell = 1; globalState.refund = 23 - X; doRefund();}
            break;
        case MealyS14:
            if      (1 == input)                next_state = MealyS15;
            else if (2 == input)                next_state = MealyS16;
            else if (5 == input)                {next_state = MealyS2; globalState.sell = 1; globalState.refund = 19 - X; doRefund();}
            else if (10 == input)               {next_state = MealyS7; globalState.sell = 1; globalState.refund = 24 - X; doRefund();}
            break;
        case MealyS15:
            if      (1 == input)                next_state = MealyS16;
            else if (2 == input)                {next_state = MealyS0; globalState.sell = 1; globalState.refund = 17 - X; doRefund();}
            else if (5 == input)                {next_state = MealyS3; globalState.sell = 1; globalState.refund = 20 - X; doRefund();}
            else if (10 == input)               {next_state = MealyS8; globalState.sell = 1; globalState.refund = 25 - X; doRefund();}
            break;
        case MealyS16:
            if      (1 == input)                {next_state = MealyS0; globalState.sell = 1; globalState.refund = 17 - X; doRefund();}
            else if (2 == input)                {next_state = MealyS1; globalState.sell = 1; globalState.refund = 18 - X; doRefund();}
            else if (5 == input)                {next_state = MealyS4; globalState.sell = 1; globalState.refund = 21 - X; doRefund();}
            else if (10 == input)               {next_state = MealyS9; globalState.sell = 1; globalState.refund = 26 - X; doRefund();}
            break;
    }
    globalState.mealy_curent_state = next_state;
    printf("Current Mealy State is: %u\n", globalState.mealy_curent_state);
}

void MooreFSM(int input) {
    unsigned next_state;  
    // Начальная инициализация автомата
    if (MooreStart == globalState.moore_curent_state) {
        globalState.moore_curent_state = MooreS0;
        globalState.refund = 0;
        globalState.sell = 0;
    }

    switch (globalState.moore_curent_state) {
        case MooreS0:
            if      (1 == input)                next_state = MooreS1;
            else if (2 == input)                next_state = MooreS2;
            else if (5 == input)                next_state = MooreS5;
            else if (10 == input)               next_state = MooreS10;
            break;

        case MooreS1:
            if      (1 == input)                next_state = MooreS2;
            else if (2 == input)                next_state = MooreS3;
            else if (5 == input)                next_state = MooreS6;
            else if (10 == input)               next_state = MooreS11;
            break;

        case MooreS2:
            if      (1 == input)                next_state = MooreS3;
            else if (2 == input)                next_state = MooreS4;
            else if (5 == input)                next_state = MooreS7;
            else if (10 == input)               next_state = MooreS12;
            break;

        case MooreS3:
            if      (1 == input)                next_state = MooreS4;
            else if (2 == input)                next_state = MooreS5;
            else if (5 == input)                next_state = MooreS8;
            else if (10 == input)               next_state = MooreS13;
            break;

        case MooreS4:
            if      (1 == input)                next_state = MooreS5;
            else if (2 == input)                next_state = MooreS6;
            else if (5 == input)                next_state = MooreS9;
            else if (10 == input)               next_state = MooreS14;
            break;

        case MooreS5:
            if      (1 == input)                next_state = MooreS6;
            else if (2 == input)                next_state = MooreS7;
            else if (5 == input)                next_state = MooreS10;
            else if (10 == input)               next_state = MooreS15;
            break;

        case MooreS6:
            if      (1 == input)                next_state = MooreS7;
            else if (2 == input)                next_state = MooreS8;
            else if (5 == input)                next_state = MooreS11;
            else if (10 == input)               next_state = MooreS16;
            break;

        case MooreS7:
            if      (1 == input)                next_state = MooreS8;
            else if (2 == input)                next_state = MooreS9;
            else if (5 == input)                next_state = MooreS12;
            else if (10 == input)               next_state = MooreS17;
            break;

        case MooreS8:
            if      (1 == input)                next_state = MooreS9;
            else if (2 == input)                next_state = MooreS10;
            else if (5 == input)                next_state = MooreS13;
            else if (10 == input)               next_state = MooreS18;
            break;
        case MooreS9:
            if      (1 == input)                next_state = MooreS10;
            else if (2 == input)                next_state = MooreS11;
            else if (5 == input)                next_state = MooreS14;
            else if (10 == input)               next_state = MooreS19;
            break;
        case MooreS10:
            if      (1 == input)                next_state = MooreS11;
            else if (2 == input)                next_state = MooreS12;
            else if (5 == input)                next_state = MooreS15;
            else if (10 == input)               next_state = MooreS20;
            break;
        case MooreS11:
            if      (1 == input)                next_state = MooreS12;
            else if (2 == input)                next_state = MooreS13;
            else if (5 == input)                next_state = MooreS16;
            else if (10 == input)               next_state = MooreS21;
            break;
        case MooreS12:
            if      (1 == input)                next_state = MooreS13;
            else if (2 == input)                next_state = MooreS14;
            else if (5 == input)                next_state = MooreS17;
            else if (10 == input)               next_state = MooreS22;
            break;
        case MooreS13:
            if      (1 == input)                next_state = MooreS14;
            else if (2 == input)                next_state = MooreS15;
            else if (5 == input)                next_state = MooreS18;
            else if (10 == input)               next_state = MooreS23;
            break;
        case MooreS14:
            if      (1 == input)                next_state = MooreS15;
            else if (2 == input)                next_state = MooreS16;
            else if (5 == input)                next_state = MooreS19;
            else if (10 == input)               next_state = MooreS24;
            break;
        case MooreS15:
            if      (1 == input)                next_state = MooreS16;
            else if (2 == input)                next_state = MooreS17;
            else if (5 == input)                next_state = MooreS20;
            else if (10 == input)               next_state = MooreS25;
            break;
        case MooreS16:
            if      (1 == input)                next_state = MooreS17;
            else if (2 == input)                next_state = MooreS18;
            else if (5 == input)                next_state = MooreS21;
            else if (10 == input)               next_state = MooreS26;
            break;
    }

    globalState.moore_curent_state = next_state;
    
    switch (globalState.moore_curent_state) {
        case MooreS17: globalState.sell = 1; globalState.refund = 17 - X; doRefund(); next_state = MooreS0; break;
        case MooreS18: globalState.sell = 1; globalState.refund = 18 - X; doRefund(); next_state = MooreS1; break;
        case MooreS19: globalState.sell = 1; globalState.refund = 19 - X; doRefund(); next_state = MooreS2; break;
        case MooreS20: globalState.sell = 1; globalState.refund = 20 - X; doRefund(); next_state = MooreS3; break;
        case MooreS21: globalState.sell = 1; globalState.refund = 21 - X; doRefund(); next_state = MooreS4; break;
        case MooreS22: globalState.sell = 1; globalState.refund = 22 - X; doRefund(); next_state = MooreS5; break;
        case MooreS23: globalState.sell = 1; globalState.refund = 23 - X; doRefund(); next_state = MooreS6; break;
        case MooreS24: globalState.sell = 1; globalState.refund = 24 - X; doRefund(); next_state = MooreS7; break;
        case MooreS25: globalState.sell = 1; globalState.refund = 25 - X; doRefund(); next_state = MooreS8; break;
        case MooreS26: globalState.sell = 1; globalState.refund = 26 - X; doRefund(); next_state = MooreS9; break;
    }
    
    printf("Current Moore State is: %u\n", globalState.moore_curent_state);
}

int main() {
    globalState.mealy_curent_state = MealyStart;
    globalState.moore_curent_state = MooreStart;

    printf("Welcome to wending machine\nPlease insert one of coins 1, 2, 5, 10\nIf you want to quit, add 0\n");
    for (int coin = 1; coin != 0;) {
        printf("Coin: ");
        scanf("%d", &coin);
        if (coin == 0) {
            printf("Bye!\n");
            return 0;
        }
        if (!(coin == 1 || coin == 2 || coin == 5 || coin == 10)) {
            printf("Please insert one of coins 1, 2, 5, 10\n");
            continue;
        }
        // MealyFSM(coin);
        MooreFSM(coin);
    }
}