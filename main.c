#include "prog2_ex2.h"
#include <stdio.h>
#include "prog2_ex2.c"

void print_result(const char *action, TransportResult result)
{
    printf("%s -> %s\n", action, error_to_string(result));
}

int main(void)
{
    TransportDB *db = TransportCreate();

    TransportAddLine(db, "TRAIN", 1, 3);
    TransportAddLine(db, "METRO", 3, 2);
    TransportAddLine(db, "TRAIN", 2, 213);

    TransportAddStation(db, 2, "a");
    TransportAddStation(db, 2, "b");

    TransportAddStation(db, 1, "a");
    TransportAddStation(db, 1, "b");

    TransportAddStation(db, 3, "a");
    TransportAddStation(db, 3, "b");

    TransportReportDirections(db, "a", "b");
    TransportReportStations(db, 2);

    return 0;
}