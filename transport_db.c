#include "transport_db.h"
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
    struct stations_t *stations;
    struct transport_t *next_line;

} Transport;

struct transport_db
{
    struct transport_t *lines;
};

// ------------------------------------- HELPERS ------------------------------------- //

static TransportType parse_type(const char *type)
{
    if (!type)
        return 0;
    if (strcmp(type, "ALL") == 0)
    {
        return ALL;
    }
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

static Transport *FindLine(TransportDB *tdb, int line_id)
{
    Transport *line = tdb->lines;

    while (line && line->id != line_id)
    {
        line = line->next_line;
    }

    if (!line)
    {
        return NULL;
    }

    return line;
}

static Transport *FindPrevLine(TransportDB *tdb, int line_id)
{
    Transport *curr_line = tdb->lines;
    Transport *prev_line = NULL;

    while (curr_line && curr_line->id != line_id)
    {
        prev_line = curr_line;
        curr_line = curr_line->next_line;
    }

    return prev_line;
}

static void DeleteStationsList(Station *station)
{
    if(station == NULL){

        return;
    }
    Station *curr = station;
    while (curr)
    {
        Station *next = curr->next_station;
        free(curr->name_station);
        free(curr);
        curr = next;
    }
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

void TransportDestroy(TransportDB *tdb)
{
    if (!tdb)
    {
        return;
    }

    Transport *curr = tdb->lines;

    while (curr)
    {
        Transport *next = curr->next_line;
        DeleteStationsList(curr->stations);
        free(curr);

        curr = next;
    }
    free(tdb);
}

static Transport *TransportAddByPrice(TransportDB *tempdb, Transport *line)
{
    Transport *curr = tempdb->lines;
    Transport *prev = NULL;

    while (curr && curr->price < line->price)
    {
        prev = curr;
        curr = curr->next_line;
    }

    if (prev == NULL)
    {
        line->next_line = tempdb->lines;
        tempdb->lines = line;
    }

    else
    {
        prev->next_line = line;
        line->next_line = curr;
    }

    return line;
}

TransportResult TransportAddLine(TransportDB *tdb, const char *type, int line_id, float price)
{

    if (!tdb)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }
    // invlaid line type
    TransportType type_line = parse_type(type);
    if (type_line == 0)
    {
        return TRANSPORT_INVALID_LINE_TYPE;
    }

    if (line_id <= 0)
    {
        return TRANSPORT_INVALID_LINE_NUMBER;
    }

    if (price <= 0)
    {
        return TRANSPORT_INVALID_PRICE;
    }

    Transport *new_line = (Transport *)malloc(sizeof(Transport));
    if (new_line == NULL)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    new_line->id = line_id;
    new_line->price = price;
    new_line->type = type_line;
    new_line->stations = NULL;

    // case: db empty
    if (tdb->lines == NULL)
    {
        tdb->lines = new_line;
        new_line->next_line = NULL;
        return TRANSPORT_SUCCESS;
    }

    Transport *prev = tdb->lines;
    Transport *curr = tdb->lines;

    while (curr)
    {
        if (curr->id >= new_line->id)
        {

            if (curr->id == new_line->id)
            {
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

TransportResult TransportRemoveLine(TransportDB *tdb, int line_id)
{

    if (!tdb)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    if (line_id <= 0)
    {
        return TRANSPORT_INVALID_LINE_NUMBER;
    }

    Transport *line = FindLine(tdb, line_id);
    if (!line)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    Transport *prev_line = FindPrevLine(tdb, line_id);

    if (!prev_line)
    {
        tdb->lines = line->next_line;
    }
    else
    {
        prev_line->next_line = line->next_line;
    }

    // REMOVE ALL STATIONS

    DeleteStationsList(line->stations);
    free(line);

    return TRANSPORT_SUCCESS;
}
TransportResult TransportAddStation(TransportDB *tdb, int line_id, const char *station)
{

    if (!tdb || !station)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    if (line_id <= 0)
    {
        return TRANSPORT_INVALID_LINE_NUMBER;
    }

    Transport *line = FindLine(tdb, line_id);
    if (!line)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    // allocate memory
    Station *new_station = (Station *)malloc(sizeof(Station));
    if (new_station == NULL)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    char *name = (char *)malloc(strlen(station) * sizeof(char) + 1);
    if (name == NULL)
    {
        free(new_station);
        return TRANSPORT_OUT_OF_MEMORY;
    }

    strcpy(name, station);

    /// -----------------------------
    new_station->name_station = name;
    new_station->next_station = NULL;

    Station *curr_station = line->stations;
    Station *prev_station = line->stations;

    // case no stations yet
    if (curr_station == NULL)
    {
        line->stations = new_station;
        return TRANSPORT_SUCCESS;
    }

    while (curr_station)
    {
        prev_station = curr_station;
        curr_station = curr_station->next_station;
    }

    prev_station->next_station = new_station;

    return TRANSPORT_SUCCESS;
}
TransportResult TransportReportLines(TransportDB *tdb, const char *type)
{

    if (!tdb)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    TransportType type_line = parse_type(type);

    if (type_line == 0)
    {
        return TRANSPORT_INVALID_LINE_TYPE;
    }

    Transport *curr_line = tdb->lines;
    Station *curr_station = NULL;
    int numeber_of_stations = 0;
    int not_empty = 0; // check if there is at least one line of this type

    while (curr_line != NULL)
    {
        numeber_of_stations = 0;
        if (curr_line->type == type_line || type_line == ALL)
        {
            curr_station = curr_line->stations;
            while (curr_station != NULL)
            {
                numeber_of_stations++;
                curr_station = curr_station->next_station;
            }
            not_empty++;

            prog2_report_line(curr_line->id, curr_line->type, numeber_of_stations, curr_line->price);
        }

        curr_line = curr_line->next_line;
    }
    if (not_empty)
    {
        return TRANSPORT_SUCCESS;
    }
    return TRANSPORT_EMPTY;
}

TransportResult TransportRemoveStation(TransportDB *tdb, int line_id, unsigned int index)
{

    if (!tdb)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    if (line_id <= 0)
    {
        return TRANSPORT_INVALID_LINE_NUMBER;
    }

    Transport *line = FindLine(tdb, line_id);

    if (!line)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    if (line->stations == NULL)
    {
        return TRANSPORT_EMPTY;
    }

    Station *prev_station = line->stations;
    Station *curr_station = line->stations;

    // case 1: first station
    if (index == 0)
    {
        line->stations = prev_station->next_station;
        free(curr_station->name_station);
        free(curr_station);
        return TRANSPORT_SUCCESS;
    }

    for (unsigned int i = 0; i < index; i++)
    {
        if (curr_station->next_station == NULL)
        {
            return TRANSPORT_DOESNT_EXIST;
        }

        prev_station = curr_station;
        curr_station = curr_station->next_station;
    }

    if (curr_station->next_station == NULL)
    {
        prev_station->next_station = NULL;
        free(curr_station->name_station);
        free(curr_station);
        return TRANSPORT_SUCCESS;
    }

    prev_station->next_station = curr_station->next_station;
    free(curr_station->name_station);
    free(curr_station);
    return TRANSPORT_SUCCESS;
}

TransportResult TransportReportStations(TransportDB *tdb, int line_id)
{
    if (!tdb)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    if (line_id <= 0)
    {
        return TRANSPORT_INVALID_LINE_NUMBER;
    }

    Transport *line = FindLine(tdb, line_id);

    if (!line)
    {
        return TRANSPORT_DOESNT_EXIST;
    }

    if (!line->stations)
    {
        return TRANSPORT_EMPTY;
    }

    int num_of_stations = 0;

    Station *curr_station = line->stations;

    while (curr_station)
    {
        num_of_stations++;
        curr_station = curr_station->next_station;
    }

    prog2_report_line(line->id, line->type, num_of_stations, line->price);

    curr_station = line->stations;

    while (curr_station)
    {
        prog2_report_station(curr_station->name_station);
        curr_station = curr_station->next_station;
    }

    return TRANSPORT_SUCCESS;
}

TransportResult TransportReportDirections(TransportDB *tdb, const char *from, const char *to)
{

    if (!tdb || !from || !to)
    {
        return TRANSPORT_NULL_ARGUMENTS;
    }

    if (!tdb->lines)
    {
        return TRANSPORT_EMPTY;
    }

    Transport *curr_line = tdb->lines;

    TransportDB *temp_db = TransportCreate();
    if (!temp_db)
    {
        return TRANSPORT_OUT_OF_MEMORY;
    }

    while (curr_line)
    {
        // entering the line
        Station *curr_station = curr_line->stations;

        int found_from = 0;
        int found_to = 0;

        while (curr_station)
        {
            if (strcmp(curr_station->name_station, from) == 0)

            {
                found_from = 1;
            }

            if (strcmp(curr_station->name_station, to) == 0 && found_from)
            {
                found_to = 1;
                break;
            }

            curr_station = curr_station->next_station;
        }

        // if found we add it to the temp_db

        if (found_from && found_to)
        {

            Transport *copy_line = (Transport *)malloc(sizeof(Transport));
            if (!copy_line)
            {
                return TRANSPORT_OUT_OF_MEMORY;
            }

            copy_line->id = curr_line->id;
            copy_line->type = curr_line->type;
            copy_line->price = curr_line->price;
            copy_line->stations = NULL;
            copy_line->next_line = NULL;

            TransportAddByPrice(temp_db, copy_line);
            Station *curr_station = curr_line->stations;

            while (curr_station)
            {
                TransportResult add_result = TransportAddStation(temp_db, copy_line->id, curr_station->name_station);
                if (add_result != TRANSPORT_SUCCESS)
                {
                    TransportDestroy(temp_db);
                    return add_result;
                }
                curr_station = curr_station->next_station;
            }
        }
        curr_line = curr_line->next_line;
    }

    curr_line = temp_db->lines;
    if (!curr_line)
    {
        TransportDestroy(temp_db);
        return TRANSPORT_DOESNT_EXIST;
    }

    while (curr_line)
    {
        int num_of_stations = 0;
        Station *curr_station = curr_line->stations;
        const char *first_station = NULL;
        const char *last_station = NULL;

        while (curr_station)
        {
            if (!first_station)
            {
                first_station = curr_station->name_station;
            }
            last_station = curr_station->name_station;
            num_of_stations++;
            curr_station = curr_station->next_station;
        }

        prog2_report_line(curr_line->id, curr_line->type, num_of_stations, curr_line->price);
        prog2_report_station(first_station);
        prog2_report_station(last_station);

        curr_line = curr_line->next_line;
    }

    TransportDestroy(temp_db);

    return TRANSPORT_SUCCESS;
}
