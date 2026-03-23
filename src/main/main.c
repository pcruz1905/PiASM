#include "machmanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototype for the manager loop
#define PORT "/dev/ttyS3"
#define MAX_MACHINES 32

void initializeMachinesFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return;
    }

    int machineIndex = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && machineIndex < MAX_MACHINES)
    {
        Machine newMachine = {0};

        // Parse the line for machine setup information
        if (scanf(line, "%d,%99[^,],%f,%f,%f,%f,%d,%d",
                  &newMachine.id,
                  newMachine.name,
                  &newMachine.temp_min,
                  &newMachine.temp_max,
                  &newMachine.hum_min,
                  &newMachine.hum_max,
                  &newMachine.buffer_lenght,
                  &newMachine.median_window_lenght) == 8)
        {

            // Initialize default values for buffers
            newMachine.buffer_index = 0;
            initializeBuffer(newMachine.temp_buffer, newMachine.buffer_lenght, 0.0);
            initializeBuffer(newMachine.hum_buffer, newMachine.buffer_lenght, 0.0);

            machines[machineIndex++] = newMachine;
            printf("Machine '%s' with ID %d initialized from file.\n", newMachine.name, newMachine.id);
        }
        else
        {
            printf("Invalid line format: %s", line);
        }
    }

    if (machineIndex == MAX_MACHINES)
    {
        printf("Maximum number of machines reached. Additional machines in the file were ignored.\n");
    }

    fclose(file);
}

void addMachine()
{
    for (int i = 0; i < MAX_MACHINES; i++)
    {
        if (machines[i].id == 0)
        { // Find an empty slot
            Machine newMachine = {0};
            printf("Enter machine name: ");
            scanf("%[^\n]", newMachine.name);

            printf("Enter machine ID: ");
            scanf("%d", &newMachine.id);

            printf("Enter machine initial state (ON, OFF, OP): ");
            scanf(" %[^\n]", newMachine.state);

            newMachine.buffer_lenght = 50;       // Default buffer length
            newMachine.median_window_lenght = 5; // Default median window length
            machines[i] = newMachine;

            printf("Machine '%s' added with ID %d.\n", newMachine.name, newMachine.id);
            return;
        }
    }
    printf("Error: Maximum number of machines reached.\n");
}

void removeMachine()
{
    int id;
    printf("Enter the machine ID to remove: ");
    scanf("%d", &id);

    for (int i = 0; i < MAX_MACHINES; i++)
    {
        if (machines[i].id == id)
        {
            machines[i] = (Machine){0}; // Reset the machine
            printf("Machine with ID %d removed.\n", id);
            return;
        }
    }
    printf("Error: Machine with ID %d not found.\n", id);
}

void manageMachine()
{
    printf("Available machines:\n");
    for (int i = 0; i < MAX_MACHINES; i++)
    {
        if (machines[i].id != 0)
        {
            printf("ID: %d, Name: %s\n", machines[i].id, machines[i].name);
        }
    }

    int id;
    printf("Enter the ID of the machine to manage: ");
    scanf("%d", &id);

    Machine *machine = selectMachine(id);
    if (machine == NULL)
    {
        printf("Error: Machine with ID %d not found.\n", id);
        return;
    }

    int serial_port = configureSerialPort(PORT);
    if (serial_port < 0)
    {
        printf("Error: Could not configure serial port.\n");
        return;
    }

    char command[100];
    if (processCommand(machine->state, machine->id, command))
    {
        sendCommandToMachine(serial_port, command);
        readSensorData(serial_port, machine);
    }
    else
    {
        printf("Error: Failed to process command.\n");
    }

    // pclose(serial_port); // Always close the serial port when done
}
void assign_operation()
{
    printf("Informe o ID da máquina: ");

    scanf("%d", &machine_id);

    printf("Informe o ID da operação: ");

    scanf("%d", &operation_id);

    for (int i = 0; i < 32; i++)
    {
        if (machines[i].id == machine_id)
        {
            if (machines[i].state != '1')
            { // '1' representa o estado ON
                printf("Erro: A máquina %d não está disponível para operar.\n", machine_id);
                return;
            }

            int valid_operation = 0;
            for (int j = 0; j < machines[i].operation_count; j++)
            {
                if (machines[i].operations_ids[j] == operation_id)
                {
                    valid_operation = 1;
                    break;
                }
            }

            if (!valid_operation)
            {
                printf("Erro: A operação %d não é válida para a máquina %d.\n", operation_id, machine_id);
                return;
            }

            machines[i].state = '2'; // '2' representa OP
            machines[i].current_operation = operation_id;

            char command[20];
            if (format_command("OP", operation_id, command))
            {
                int serial_port = configureSerialPort(PORT);
                if (serial_port != EXIT_FAILURE)
                {
                    if (sendCommandToMachine(serial_port, command) != -1)
                    {
                        printf("Operação %d atribuída à máquina %d com sucesso.\n", operation_id, machine_id);
                    }
                    else
                    {
                        printf("Erro ao enviar comando para a máquina %d.\n", machine_id);
                    }
                    close(serial_port);
                }
                else
                {
                    printf("Erro ao configurar a porta serial para a máquina %d.\n", machine_id);
                }
            }
            else
            {
                printf("Erro ao formatar comando para a operação %d.\n", operation_id);
            }

            return;
        }
    }

    printf("Erro: Máquina %d não encontrada.\n", machine_id);
}

void show_machine_state()
{
    printf("Informe o ID da máquina: ");
    scanf("%d", &machine_id);

    for (int i = 0; i < 32; i++)
    {
        if (machines[i].id == machine_id)
        {

            char command[20];
            if (format_command(machines[i].state == '1' ? "ON" : machines[i].state == '2' ? "OP"
                                                                                          : "OFF",
                               0, command))
            {
                int serial_port = configureSerialPort(PORT);
                if (serial_port != EXIT_FAILURE)
                {

                    if (sendCommandToMachine(serial_port, command) != -1)
                    {
                        printf("Estado da máquina %d exibido por 2 segundos.\n", machine_id);
                        sleep(2);                                           // Wait for 2 seconds
                        sendCommandToMachine(serial_port, "OFF,0,0,0,0,0"); // Reset display
                    }
                    else
                    {
                        printf("Erro ao enviar comando para a máquina %d.\n", machine_id);
                    }
                    close(serial_port);
                }
                else
                {
                    printf("Erro ao configurar a porta serial para a máquina %d.\n", machine_id);
                }
            }
            else
            {
                printf("Erro ao formatar comando para o estado da máquina %d.\n", machine_id);
            }
            return;
        }
    }

    printf("Erro: Máquina %d não encontrada.\n", machine_id);
}

void displayMenu()
{
    printf("\n--- Machine Manager ---\n");
    printf("1. Add Machine\n");
    printf("2. Remove Machine\n");
    printf("3. Manage Machines\n");
    printf("4. Show a Machine State\n");
    printf("5. Assing a Operation\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int main()
{

    initializeMachinesFromFile("machines.txt");

    loadOperationsFromFile("operations.txt");

    for (int i = 0; i < 32; i++)
    {
        if (machines[i].id != 0)
        {
            printf("Machine ID: %d\n", machines[i].id);
            for (int j = 0; j < machines[i].operation_count; j++)
            {
                printf("  Operation ID %d, State: %s, Timestamp: %s",
                       machines[i].operations[j].id,
                       machines[i].operations[j].state,
                       ctime(&machines[i].operations[j].timestamp)); // Convert timestamp to string
            }
        }
    }

    while (1)
    {
        displayMenu();

        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addMachine();
            break;
        case 2:
            removeMachine();
            break;
        case 3:
            manageMachine();
            break;
        case 4:
            show_machine_state();
            break;
        case 5:
            assign_operation();
            break;
        case 6:
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
