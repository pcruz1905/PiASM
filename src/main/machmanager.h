#ifndef MACHINE_MANAGER_H
#define MACHINE_MANAGER_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char state[4];
    time_t timestamp;
} Operation;

typedef struct
{
    char name[100];           // Name of the machine
    float temp_current;       // Current temperature
    float hum_current;        // Current humidity
    float temp_min;           // Minimum acceptable temperature
    float temp_max;           // Maximum acceptable temperature
    float hum_min;            // Minimum acceptable humidity
    float hum_max;            // Maximum acceptable humidity
    int buffer_lenght;        // Length of the buffer
    int median_window_lenght; // Median window length for temperature/humidity data
    int id;                   // Unique machine identifier
    char state[20];           // Current state of the machine: "OP", "ON", "OFF"
    time_t timestamp;         // Timestamp of last operation

    Operation operations[32]; // Container to hold machine's operations
    int operation_count;      // Count of operations performed
    float temp_buffer[50];    // Temperature data buffer
    float hum_buffer[50];     // Humidity data buffer
    int buffer_index;         // Index to manage buffers
} Machine;

extern Machine machines[32];
Machine *selectMachine(int id);
void initializeMachinesFromFile(const char *filename);
void loadOperationsFromFile(const char *filename);
int configureSerialPort(const char *port);
void initializeBuffer(float *buffer, int length, float value);
void sendCommandToMachine(int serial_port, const char *command);
void readSensorData(int serial_port, Machine *machine);
void updateState(Machine *machine);
void alertCheck(Machine *machine);
int processCommand(char *op, int n, char *cmd);
void assign_operation();
void show_machine_state();

#endif