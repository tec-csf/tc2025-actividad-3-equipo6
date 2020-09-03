//Jesús González| A01370190 -> Trabajo hecho con Julio Villazón
//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Process
{
    char * g_id;
    int p_id;
    int runTime;
} process_t;

typedef struct node
{
    process_t proc;
    struct node *next;
} node_t;

node_t *head = NULL;
int nProc;

void enqueue(char *, int, int);
process_t dequeue();

int main(int arc, const char *argv[])
{
    srand(time(NULL));
    head = malloc(sizeof(node_t));
    head->next = NULL;
    int nProc, nCPU, Quantum = 10;
    int *cpu, *cAux, *cFinal;
    process_t *proc, *pAux, *pFinal;

    printf("Ingresar la cantidad de procesos: ");
    scanf("%d", &nProc);

    proc = (process_t *)malloc(sizeof(process_t) * nProc);
    pAux = proc;
    pFinal = proc + nProc;

    printf("Escribe las pandillas por orden alfabetico(A-F). \n\n");
    for(;pAux < pFinal; ++pAux)
    {
        printf("Asignale una id al proceso: ");
        scanf("%d", &pAux->p_id);

        pAux->g_id = (char *) malloc(sizeof(char));
        printf("Escribe la pandilla a la que pertenece el proceso %d (Letras entre A-F): ", pAux->p_id);
        scanf("%s", pAux->g_id);

        printf("Tiempo cpu (en milisegundos) proceso %d: ", pAux->p_id);
        scanf("%d", &pAux->runTime);


        enqueue(pAux->g_id, pAux->p_id, pAux->runTime);
        printf("Adding process to gang %s with running time of %d \n", pAux->g_id, pAux->runTime);
        printf("\n");
    }




    printf("Ingresar la cantidad de cpu's: ");
    scanf("%d", &nCPU);

    cpu = (int *)malloc(sizeof(int) * nCPU);
    cAux = cpu;
    cFinal = cpu + nCPU;

    pAux = proc;

    while (1)
    {
        bool done = true;

        for (; pAux < pFinal; ++pAux)
        {
            printf("The Quantum is %d\n", Quantum);
            for(; cAux < cFinal; ++cAux){
                process_t cpu_p = dequeue();
                printf("Running process %d of Gang %s with %d runing time\n", cpu_p.p_id, cpu_p.g_id, cpu_p.runTime);
                if (cpu_p.runTime > 0)
                {
                    done = false; // There is a pending process
                    if (cpu_p.runTime > Quantum)
                    {
                        cpu_p.runTime -= Quantum;
                        if (cpu_p.runTime > 0)
                        {
                            enqueue(cpu_p.g_id, cpu_p.p_id, cpu_p.runTime);
                        }
                    }
                    else
                    {
                        cpu_p.runTime = 0;
                        printf("Process %d of Gang %s finished execution \n", cpu_p.p_id, cpu_p.g_id);
                        nProc -= 1;
                        proc = (process_t *)realloc(proc, sizeof(process_t) * nProc);
                    }
                }
            }
            cAux = cpu;
            pAux = proc;
            pFinal = proc + nProc;
            printf("\n");
        }

        // If all processes are done
        if (done == true)
            break;
    }

    free(proc);
    free(cpu);
    free(head);
    return 0;
}

void enqueue(char * g_id, int p_id, int val)
{
    node_t *current = head;

    if (nProc == 0)
    {
        head->proc.g_id = g_id;
        head->proc.p_id = p_id;
        head->proc.runTime = val;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = malloc(sizeof(node_t));
        current->next->proc.runTime = val;
        current->next->proc.g_id = g_id;
        current->next->proc.p_id = p_id;
        current->next->next = NULL;
    }
    nProc++;
}


process_t dequeue()
{
    process_t returningValue;
    if (nProc == 1)
    {
        returningValue = head->proc;
    }
    else
    {
        node_t *nextProc = head->next;
        returningValue = head->proc;
        free(head);
        head = nextProc;
    }
    nProc--;
    return returningValue;
}