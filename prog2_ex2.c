#include "prog2_ex2.h"

static const char *error_strings[] = {
    "Success",
    "Invalid Arguments",
    "Out of memory",
    "Invalid line type",
    "Invalid line number",
    "Invalid price",
    "Line already exists",
    "Line Does not exist",
    "Transport database is empty"};

static const char *type_strings[] = {NULL, "  Bus", "Train", NULL, "Metro"};

static const char *error_to_string(TransportResult error)
{
    if (error <= TRANSPORT_EMPTY && error >= 0)
    {
        return error_strings[error];
    }

    return "Unknown error"; // Should not happen
}

static const char *type_to_string(TransportType type)
{
    if (type <= METRO && type >= 0)
    {
        return type_strings[type];
    }
    return "Unknown transport type";
}

void prog2_report_error_message(TransportResult error)
{
    fprintf(stderr, "[ERROR] %s\n", error_to_string(error));
}

void prog2_report_line(line_id number, TransportType type, int num_stations, double price)
{
    printf("%s Line %8d [%4d stations] %.2f$\n", type_to_string(type), number, num_stations, price);
}

void prog2_report_station(const char *name)
{
    printf("\tStation: %s\n", name);
}
