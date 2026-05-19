#include "prog2_ex2.h"
#include "prog2_ex2.c"

int main(void){

    TransportDB *db = TransportCreate();
    TransportAddLine(db, "BUS", 10, 23);
    TransportAddLine(db, "TRAIN", 2, 3);
    TransportAddLine(db, "METRO", 5, 23);
    TransportAddLine(db, "METRO", 4, 23);


    //TransportRemoveLine(db, 1);
    //TransportRemoveLine(db, 1);
    // TransportRemoveLine(db, 5);
    //TransportRemoveLine(db, 4);


    TransportAddStation(db, 2, "station a");
    TransportAddStation(db, 2, "station b");
    TransportAddStation(db, 2, "station c");
    TransportAddStation(db, 2, "station d");

    TransportRemoveStation(db, 2, 0);
    TransportReportLines(db, "TRAIN");
    printf("%s\n", db->lines->stations->name_station);
    printf("%s\n", db->lines->stations->next_station->name_station);
    printf("%s\n", db->lines->stations->next_station->next_station->name_station);

    return 0;
}
