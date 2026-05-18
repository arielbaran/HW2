#include "prog2_ex2.h"
#include "prog2_ex2.c"

int main()
{

    line_id num = 9;
    TransportDB *db = TransportCreate();
    TransportAddLine(db, "METRO", 1, 23);
    TransportAddLine(db, "TRAIN", 2, 3);
    TransportAddLine(db, "METRO", 5, 23);
    TransportAddLine(db, "METRO", 4, 23);

    TransportRemoveLine(db, 1);
    TransportRemoveLine(db, 1);
    // TransportRemoveLine(db, 5);
    TransportRemoveLine(db, 4);
    TransportRemoveLine(db, 4);

    TransportAddStation(db, 2, "station A");
    TransportAddStation(db, 2, "station b");

    TransportRemoveStation(db, 2, 6);

    prog2_report_line(2, 2, 2, 2.2);

    return 0;
}
