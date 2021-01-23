
#include "botao.h"

void init_bt(){
    pinMode(BT,INPUT_PULLUP);
}

bool verifica_bt(){
    return digitalRead(BT);
}