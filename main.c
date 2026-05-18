#include "prog2_ex2.h"
#include "prog2_ex2.c"

int main()
{

    TransportDB *db = TransportCreate();
    TransportAddLine(db, "METRO", 1, 23);
    TransportAddLine(db, "TRAIN", 2, 3);


    return 0;
}