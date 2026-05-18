#include "prog2_ex2.h"
#include "prog2_ex2.c"

int main()
{

    TransportDB *db = TransportCreate();
    TransportAddLine(db, "METRO", 1, 23);
    TransportAddLine(db, "TRAIN", 2, 3);
    TransportAddLine(db, "METRO", 5, 23);
    TransportAddLine(db, "METRO", 4, 23);
    TransportReportLines(db, "ALL");
    printf("\n----------------\n");
    // TransportRemoveLine(db, 1);
    TransportRemoveLine(db, 1);



    TransportAddStation(db, 2, "station A");
    TransportAddStation(db, 2, "station b");

    
  


    printf("%d\n", db->lines->id);
    printf("%s\n", db->lines->stations->name_station);
    printf("%s\n", db->lines->stations->next_station->name_station);
    printf("\n----------------\n");
    TransportReportLines(db, "ALL");
    return 0;
}