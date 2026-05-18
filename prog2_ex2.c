#include "prog2_ex2.h"
#include <stdlib.h>
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
    TransportType type;
    float price;
    struct transport_t *next_line;

} Transport;

typedef struct transport_db
{
    struct transport_t *lines;
} TransportDB;

const char *error_strings[] = {
    "Success",
    "Invalid Arguments",
    "Out of memory",
    "Invalid line type",
    "Invalid line number",
    "Invalid price",
    "Line already exists",
    "Line Does not exist",
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
        return 0;

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
    return 0;
}

// ------------------------------------- APP FUNCTIONS ------------------------------------- //

TransportDB *TransportCreate(void)
{
    TransportDB *db = (TransportDB *)malloc(sizeof(TransportDB));
    if (db == NULL)
    {
        prog2_report_error_message(TRANSPORT_OUT_OF_MEMORY);
        return NULL;
    }
    db->lines = NULL;

    return db;
}

void TransportDestory(TransportDB *tdb);

TransportResult TransportAddLine(TransportDB *tdb, const char *type, int line_id, float price)
{

    // invlaid line type
    TransportType type_line = parse_type(type);
    if (type_line == 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_TYPE);
    }

    if (line_id <= 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_LINE_NUMBER);
    }

    if (price <= 0)
    {
        prog2_report_error_message(TRANSPORT_INVALID_PRICE);
    }

    Transport *new_line = (Transport *)malloc(sizeof(Transport));
    new_line->id = line_id;
    new_line->price = price;
    new_line->type = type_line;

    if (tdb->lines == NULL)
    {
        new_line->next_line = NULL;
        tdb->lines = new_line;
        return TRANSPORT_SUCCESS;
    }

    Transport *prev = tdb->lines;
    Transport *curr = tdb->lines;

    while (curr != NULL)
    {
        if (curr->id >= new_line->id)
        {

            if (curr->id == new_line->id)
            {
                prog2_report_error_message(TRANSPORT_ALREADY_EXISTS);
                free(new_line);
                return TRANSPORT_ALREADY_EXISTS;
            }

            if (curr == tdb->lines)
            {
                tdb->lines = new_line;
                new_line->next_line = curr;
                return TRANSPORT_SUCCESS;
            }

            prev->next_line = new_line;
            new_line->next_line = curr;
            return TRANSPORT_SUCCESS;
        }
        prev = curr;
        curr = curr->next_line;
    }
    prev->next_line = new_line;
    new_line->next_line = NULL;
    return TRANSPORT_SUCCESS;
}

TransportResult TransportRemoveLine(TransportDB *tdb, int line_id);
TransportResult TransportAddStation(TransportDB *tdb, int line_id, const char *station);
TransportResult TransportReportLines(TransportDB *tdb, const char *type);
TransportResult TransportReportStations(TransportDB *tdb, int line_id);
TransportResult TransportReportDirections(TransportDB *tdb, const char *from, const char *to);
