#include "prog2_ex2.h"
#include <string.h>

// ------------------------------------- STRUCTS ------------------------------------- //
typedef struct stations_t
{
    char *name_station;
    struct stations_t *next_station;

} Station;

typedef struct transport_t
{
    line_id id;
    char *type;
    float price;
    struct transport_t *next_line;

} TransportDB;

const char *error_strings[] = {
    "Success",
    "Invalid Arguments",
    "Cannot open file",
    "Out of memory",
    "Invalid line type",
    "Invalid line number",
    "Invalid price",
    "Line already exists",
    "Line Does not exist",
    "Station overflow",
    "Transport database is empty"};

const char *type_strings[] = {NULL, "Bus", "Train", NULL, "Metro"};

// ------------------------------------- HELPERS ------------------------------------- //

const char *error_to_string(TransportResult error)
{
    if (error <= TRANSPORT_EMPTY && error >= 0)
    {
        return error_strings[error];
    }

    return "Unknown error"; // Should not happen
}

const char *type_to_string(TransportType type)
{
    if (type <= METRO && type >= 0)
    {
        return type_strings[type];
    }
    return "Unknown transport type";
}

void prog2_report_error_message(TransportResult error)
{
    fprintf(stderr, "%s\n", error_to_string(error));
}

TransportType parse_type(const char *type)
{
    if (!type)
        return NULL;

    if (strcmp(type, "BUS") == 0)
    {
        return BUS;
    }
    if (strcmp(type, "METRO") == 0)
    {
        return METRO;
    }
    if (strcmp(type, "TRAIN") == 0)
    {
        return TRAIN;
    }
    return NULL;
}

// ------------------------------------- APP FUNCTIONS ------------------------------------- //

TransportDB *TransportCreate(void)
{
    // error checking
    TransportDB *head = NULL;
    return head;
}

void TransportDestory(TransportDB *tdb);

TransportResult TransportAddLine(TransportDB *tdb, const char *type, int line_id, float price)
{

    // invlaid line type

    if (parse_type(type) == NULL)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_TYPE);
    }

    if (line_id <= 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
    }

    // invalid price

    if (price <= 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_PRICE);
    }


    // TODO: ALREADY EXIST CHECK


}

// already exist

// execute

TransportResult TransportAddLine(TransportDB *tdb, const char *type, int line_id, float price);
TransportResult TransportRemoveLine(TransportDB *tdb, int line_id);
TransportResult TransportAddStation(TransportDB *tdb, int line_id, const char *station);
TransportResult TransportReportLines(TransportDB *tdb, const char *type);
TransportResult TransportReportStations(TransportDB *tdb, int line_id);
TransportResult TransportReportDirections(TransportDB *tdb, const char *from, const char *to);