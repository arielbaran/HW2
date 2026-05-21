#include "prog2_ex2.h"
#include <stdio.h>
#include "prog2_ex2.c"

void print_result(const char *action, TransportResult result)
{
    printf("%-45s -> %s\n", action, error_to_string(result));
}

void print_separator(const char *title)
{
    printf("\n");
    printf("============================================================\n");
    printf("%s\n", title);
    printf("============================================================\n");
}

int main(void)
{
    TransportResult result;

    print_separator("CREATE DATABASE");

    TransportDB *db = TransportCreate();

    if (!db)
    {
        printf("Failed to create database\n");
        return 1;
    }

    printf("Database created successfully\n");

    print_separator("ADD VALID LINES");

    result = TransportAddLine(db, "TRAIN", 1, 3.50);
    print_result("Add TRAIN line 1 price 3.50", result);

    result = TransportAddLine(db, "METRO", 3, 2.20);
    print_result("Add METRO line 3 price 2.20", result);

    result = TransportAddLine(db, "BUS", 2, 5.00);
    print_result("Add BUS line 2 price 5.00", result);

    result = TransportAddLine(db, "TRAIN", 10, 10.00);
    print_result("Add TRAIN line 10 price 10.00", result);

    result = TransportAddLine(db, "BUS", 7, 1.90);
    print_result("Add BUS line 7 price 1.90", result);

    print_separator("REPORT ALL LINES AFTER INSERTION");
    result = TransportReportLines(db, "ALL");
    print_result("Report ALL lines", result);

    print_separator("ADD INVALID LINES");

    result = TransportAddLine(db, "PLANE", 4, 4.50);
    print_result("Add invalid type PLANE", result);

    result = TransportAddLine(db, "BUS", -1, 4.50);
    print_result("Add invalid line number -1", result);

    result = TransportAddLine(db, "BUS", 0, 4.50);
    print_result("Add invalid line number 0", result);

    result = TransportAddLine(db, "BUS", 4, -10.00);
    print_result("Add invalid price -10.00", result);

    result = TransportAddLine(db, "BUS", 4, 0.00);
    print_result("Add invalid price 0.00", result);

    result = TransportAddLine(db, "TRAIN", 1, 9.99);
    print_result("Add duplicate line ID 1", result);

    print_separator("REPORT BY TYPE");

    result = TransportReportLines(db, "BUS");
    print_result("Report BUS lines", result);

    result = TransportReportLines(db, "TRAIN");
    print_result("Report TRAIN lines", result);

    result = TransportReportLines(db, "METRO");
    print_result("Report METRO lines", result);

    result = TransportReportLines(db, "PLANE");
    print_result("Report invalid type PLANE", result);

    print_separator("ADD STATIONS TO LINES");

    result = TransportAddStation(db, 1, "A");
    print_result("Line 1 add station A", result);

    result = TransportAddStation(db, 1, "B");
    print_result("Line 1 add station B", result);

    result = TransportAddStation(db, 1, "C");
    print_result("Line 1 add station C", result);

    result = TransportAddStation(db, 1, "D");
    print_result("Line 1 add station D", result);

    result = TransportAddStation(db, 2, "A");
    print_result("Line 2 add station A", result);

    result = TransportAddStation(db, 2, "B");
    print_result("Line 2 add station B", result);

    result = TransportAddStation(db, 2, "E");
    print_result("Line 2 add station E", result);

    result = TransportAddStation(db, 3, "B");
    print_result("Line 3 add station B", result);

    result = TransportAddStation(db, 3, "C");
    print_result("Line 3 add station C", result);

    result = TransportAddStation(db, 3, "D");
    print_result("Line 3 add station D", result);

    result = TransportAddStation(db, 7, "A");
    print_result("Line 7 add station A", result);

    result = TransportAddStation(db, 7, "X");
    print_result("Line 7 add station X", result);

    result = TransportAddStation(db, 7, "B");
    print_result("Line 7 add station B", result);

    result = TransportAddStation(db, 10, "D");
    print_result("Line 10 add station D", result);

    result = TransportAddStation(db, 10, "C");
    print_result("Line 10 add station C", result);

    result = TransportAddStation(db, 10, "B");
    print_result("Line 10 add station B", result);

    print_separator("ADD STATION ERROR CASES");

    result = TransportAddStation(db, 999, "Ghost Station");
    print_result("Add station to non-existing line 999", result);

    result = TransportAddStation(db, -5, "Bad Line");
    print_result("Add station to invalid line -5", result);

    result = TransportAddStation(db, 1, NULL);
    print_result("Add NULL station to line 1", result);

    print_separator("REPORT STATIONS FOR EACH LINE");

    result = TransportReportStations(db, 1);
    print_result("Report stations for line 1", result);

    result = TransportReportStations(db, 2);
    print_result("Report stations for line 2", result);

    result = TransportReportStations(db, 3);
    print_result("Report stations for line 3", result);

    result = TransportReportStations(db, 7);
    print_result("Report stations for line 7", result);

    result = TransportReportStations(db, 10);
    print_result("Report stations for line 10", result);

    print_separator("REPORT STATIONS ERROR CASES");

    result = TransportReportStations(db, 999);
    print_result("Report stations for non-existing line 999", result);

    result = TransportReportStations(db, -1);
    print_result("Report stations for invalid line -1", result);

    print_separator("DIRECTION TESTS");

    printf("\nExpected: lines that contain A before B, sorted by price\n");
    result = TransportReportDirections(db, "A", "B");
    print_result("Directions A -> B", result);

    printf("\nExpected: lines that contain B before D, sorted by price\n");
    result = TransportReportDirections(db, "B", "D");
    print_result("Directions B -> D", result);

    printf("\nExpected: line 10 maybe, because D before B exists there\n");
    result = TransportReportDirections(db, "D", "B");
    print_result("Directions D -> B", result);

    printf("\nExpected: empty/no route, because B before A does not exist in most lines except check data\n");
    result = TransportReportDirections(db, "B", "A");
    print_result("Directions B -> A", result);

    printf("\nExpected: empty/no route\n");
    result = TransportReportDirections(db, "A", "Z");
    print_result("Directions A -> Z", result);

    printf("\nExpected: empty/no route\n");
    result = TransportReportDirections(db, "Z", "A");
    print_result("Directions Z -> A", result);

    printf("\nExpected: line 7 has A -> X\n");
    result = TransportReportDirections(db, "A", "X");
    print_result("Directions A -> X", result);

    printf("\nExpected: empty/no route because X is before B, but not before A\n");
    result = TransportReportDirections(db, "X", "A");
    print_result("Directions X -> A", result);

    print_separator("DIRECTION NULL ARGUMENT TESTS");

    result = TransportReportDirections(db, NULL, "B");
    print_result("Directions NULL -> B", result);

    result = TransportReportDirections(db, "A", NULL);
    print_result("Directions A -> NULL", result);

    result = TransportReportDirections(NULL, "A", "B");
    print_result("Directions with NULL db", result);

    print_separator("REMOVE STATION TESTS");

    printf("\nBefore removing stations from line 1:\n");
    result = TransportReportStations(db, 1);
    print_result("Report line 1 before removals", result);

    result = TransportRemoveStation(db, 1, 0);
    print_result("Remove station index 0 from line 1", result);

    printf("\nAfter removing index 0 from line 1:\n");
    result = TransportReportStations(db, 1);
    print_result("Report line 1", result);

    result = TransportRemoveStation(db, 1, 1);
    print_result("Remove station index 1 from line 1", result);

    printf("\nAfter removing index 1 from line 1:\n");
    result = TransportReportStations(db, 1);
    print_result("Report line 1", result);

    result = TransportRemoveStation(db, 1, 50);
    print_result("Remove invalid station index 50 from line 1", result);

    result = TransportRemoveStation(db, 999, 0);
    print_result("Remove station from non-existing line 999", result);

    result = TransportRemoveStation(db, -1, 0);
    print_result("Remove station from invalid line -1", result);

    print_separator("REMOVE LINE TESTS");

    printf("\nBefore removing line 3:\n");
    result = TransportReportLines(db, "ALL");
    print_result("Report ALL lines", result);

    result = TransportRemoveLine(db, 3);
    print_result("Remove line 3", result);

    printf("\nAfter removing line 3:\n");
    result = TransportReportLines(db, "ALL");
    print_result("Report ALL lines", result);

    result = TransportRemoveLine(db, 999);
    print_result("Remove non-existing line 999", result);

    result = TransportRemoveLine(db, -4);
    print_result("Remove invalid line -4", result);

    result = TransportRemoveLine(NULL, 1);
    print_result("Remove line from NULL db", result);

    print_separator("REPORT AFTER REMOVALS");

    result = TransportReportLines(db, "ALL");
    print_result("Report ALL lines", result);

    print_separator("DIRECTIONS AFTER REMOVALS");

    result = TransportReportDirections(db, "A", "B");
    print_result("Directions A -> B after removals", result);

    result = TransportReportDirections(db, "B", "D");
    print_result("Directions B -> D after removals", result);

    print_separator("EMPTY DATABASE TEST");

    TransportDB *empty_db = TransportCreate();

    result = TransportReportLines(empty_db, "ALL");
    print_result("Report ALL on empty db", result);

    result = TransportReportDirections(empty_db, "A", "B");
    print_result("Directions A -> B on empty db", result);

    result = TransportReportStations(empty_db, 1);
    print_result("Report stations line 1 on empty db", result);

    result = TransportRemoveLine(empty_db, 1);
    print_result("Remove line 1 from empty db", result);

    TransportDestroy(empty_db);

    print_separator("DESTROY DATABASE");

    TransportDestroy(db);

    printf("Database destroyed successfully\n");

    return 0;
}