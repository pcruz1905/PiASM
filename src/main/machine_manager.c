#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "asm.h"
#include "machmanager.h"

Machine machines[32];

Machine *selectMachine(int id)
{
    for (int i = 0; i < 32; i++)
    {
        if (machines[i].id == id)
        {
            return &machines[i];
        }
    }
    return NULL;
}

int configureSerialPort(const char *port)
{
    int serial_port = open(port, O_RDWR | O_NOCTTY);
    if (serial_port == -1)
    {
        perror("Failed to open the serial port");
        return EXIT_FAILURE;
    }

    struct termios tty;
    if (tcgetattr(serial_port, &tty) != 0)
    {
        perror("Failed to get terminal attributes");
        close(serial_port);
        return EXIT_FAILURE;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    {
        perror("Failed to set terminal attributes");
        close(serial_port);
        return EXIT_FAILURE;
    }

    return serial_port;
}

void initializeBuffer(float *buffer, int length, float value)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = value;
    }
}

void sendCommandToMachine(int serial_port, const char *command)
{
    write(serial_port, command, strlen(command));
    write(serial_port, "\n", 1);
}

int processCommand(char *op, int n, char *cmd)
{
    return format_command(op, n, cmd);
}

void readSensorData(int serial_port, Machine *machine)
{
    char buffer[100];
    int n = read(serial_port, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';

        char unit[20];
        int value;

        if (extract_data(buffer, "TEMP", unit, &value))
        {
            machine->temp_current = value;
        }

        if (extract_data(buffer, "HUM", unit, &value))
        {
            machine->hum_current = value;
        }

        if (machine->buffer_index == 0)
        {
            initializeBuffer(machine->temp_buffer, machine->buffer_lenght, machine->temp_current);
            initializeBuffer(machine->hum_buffer, machine->buffer_lenght, machine->hum_current);
        }

        machine->buffer_index = (machine->buffer_index + 1) % machine->buffer_lenght;

        int temp_values[50];
        int start_index = (machine->buffer_index - machine->median_window_lenght + machine->buffer_lenght) % machine->buffer_lenght;
        for (int i = 0; i < machine->median_window_lenght; i++)
        {
            temp_values[i] = (int)machine->temp_buffer[(start_index + i) % machine->buffer_lenght];
        }
        int temp_median = 0;
        median(temp_values, machine->median_window_lenght, &temp_median);

        int hum_values[50];
        for (int i = 0; i < machine->median_window_lenght; i++)
        {
            hum_values[i] = (int)machine->hum_buffer[(start_index + i) % machine->buffer_lenght];
        }
        int hum_median = 0;
        median(hum_values, machine->median_window_lenght, &hum_median);

        // Passando o argumento machine para updateState
        updateState(machine);

        printf("Máquina %d - Temperatura: %f, Humidade: %f\n",
               machine->id, machine->temp_current, machine->hum_current);
    }

    // Passando o argumento machine para alertCheck
    alertCheck(machine);
}

void updateState(Machine *machine)
{
    if (strcmp(machine->state, "OP") == 0)
    {
        printf("Máquina %d em operação: %s\n", machine->id, "Operação em andamento");
    }
    else if (strcmp(machine->state, "ON") == 0)
    {
        printf("Máquina %d está disponível\n", machine->id);
    }
    else if (strcmp(machine->state, "OFF") == 0)
    {
        printf("Máquina %d está inoperável\n", machine->id);
    }
    else
    {
        printf("Máquina %d está em estado desconhecido: %s\n", machine->id, machine->state);
    }
}

void loadOperationsFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening operations file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        int machine_id, operation_id;
        char state[4]; // To store the state of the machine during the operation

        // Parse the machine ID, operation ID, and state
        if (sscanf(line, "%d,%d,%3s", &machine_id, &operation_id, state) == 3)
        {
            // Find the machine with the given ID
            Machine *machine = selectMachine(machine_id);
            if (machine != NULL)
            {
                // Create a new operation with the parsed data
                if (machine->operation_count < 32)
                {
                    Operation new_operation = {
                        operation_id,
                        "",        // Initialize empty state
                        time(NULL) // Use current time as timestamp
                    };
                    strncpy(new_operation.state, state, sizeof(new_operation.state) - 1);

                    // Add the operation to the machine's operations list
                    machine->operations[machine->operation_count] = new_operation;
                    machine->operation_count++;

                    printf("Operation ID %d with state %s loaded for machine ID %d.\n", operation_id, state, machine_id);
                }
                else
                {
                    printf("Error: Too many operations for machine ID %d.\n", machine_id);
                }
            }
            else
            {
                printf("Error: Machine with ID %d not found.\n", machine_id);
            }
        }
    }

    fclose(file);
}

void alertCheck(Machine *machine)
{
    if (machine->temp_current < machine->temp_min)
    {
        printf("ALERTA! Temperatura abaixo do limite mínimo (%.2f). Temperatura atual: %.2f\n", machine->temp_min, machine->temp_current);
    }
    else if (machine->temp_current > machine->temp_max)
    {
        printf("ALERTA! Temperatura acima do limite máximo (%.2f). Temperatura atual: %.2f\n", machine->temp_max, machine->temp_current);
    }

    if (machine->hum_current < machine->hum_min)
    {
        printf("ALERTA! Humidade abaixo do limite mínimo (%.2f). Humidade atual: %.2f\n", machine->hum_min, machine->hum_current);
    }
    else if (machine->hum_current > machine->hum_max)
    {
        printf("ALERTA! Humidade acima do limite máximo (%.2f). Humidade atual: %.2f\n", machine->hum_max, machine->hum_current);
    }
}
