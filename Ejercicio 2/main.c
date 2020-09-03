//Jesús González| A01370190 -> Trabajo hecho con Julio Villazón
//Julio Villazón | A01370190 -> Trabajo hecho con Jesus Gonzalez

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

typedef struct
{
    int min;
    int max;
    int count;
    char * hist;
} Urn;


int traverseDirectoryTree(char *, int);
int is_regular_file(const char *);
int get_file_size(const char *);
int allocateUrn(Urn *,int, int, int);
void createUrns(Urn *, int, int);
int populateNewUrns(Urn *, char *, int, int, int);
void printAllStruc(Urn *, int);
void generateHist(Urn *, int, int);


int main(int arc, const char * argv[])
{
    char path[256];
    int nUrns = 1;
    int urnSize;
    int nFiles;

    Urn *urns;
    Urn *uAux;
    Urn *final;

    printf("Ingrese el tamaño de la urna: ");
    scanf("%d", &urnSize);
    printf("Ingrese la ruta del directorio: ");
    scanf("%s", path);

    urns = (Urn *)malloc(sizeof(Urn));
    final = urns;
    uAux = urns;
    uAux->count = 0;
    uAux->min = 0;
    uAux->max = urnSize - 1;
    
    int maxSize = traverseDirectoryTree(path, urnSize);

    nUrns = allocateUrn(urns, maxSize, urnSize, nUrns);

    urns = (Urn *)realloc(urns, sizeof(Urn) * nUrns);

    createUrns(urns, urnSize, nUrns);
    
    nFiles = populateNewUrns(urns, path, urnSize, nUrns, nFiles);

    generateHist(urns, nFiles, nUrns);

    printAllStruc(urns, nUrns);

    for (; uAux < final; ++uAux)
    {
        free(uAux->hist);
    }
    free(urns);
    
    return 0;
}

int traverseDirectoryTree(char * path, int urnSize)
{
    DIR *d;
    struct dirent *dir;
    int maxSize = urnSize;

    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *currentPath = realpath(dir->d_name, NULL);            
            if (is_regular_file(currentPath))
            {
                int fSize = get_file_size(dir->d_name);
                //printf("%s -> %d\n", dir->d_name, fSize);
                if (maxSize < fSize)
                    maxSize = fSize;
                //printf("MaX: %d \n", maxSize);
            }
            else
            {
                // Aqui es donde se revisarian los directorios que existen en el directorio dado por el usuario. El problema es que creemos que el readdir nos esta dando los nombres tanto del directorio actual (.) y el directorio anterior (..) por lo que se esta ciclano la funcion recursiva buscando en el mismo directorio muchos veces.
                // maxSize = traverseDirectoryTree(currentPath, urnSize);
            }
        }
        closedir(d);
        return maxSize;
    }
}

int is_regular_file(const char * path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int get_file_size(const char * file_name)
{
    struct stat sb;
    if (stat(file_name, &sb) != 0)
    {
        fprintf(stderr, "'stat' failed for '%s': %s.\n",
                file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

int allocateUrn(Urn * urn, int maxSize, int urnSize, int nUrns)
{
    Urn * final = urn + nUrns - 1;
    int diff = maxSize - final->max;
    int newUrns = (diff + urnSize - 1) / urnSize;
    nUrns += newUrns;
    //printf("DIFF: %d, NEW URNS: %d, TOTAL URNS: %d\n", diff, newUrns, nUrns);

    return nUrns;
}

void createUrns(Urn * urns, int urnSize, int nUrns)
{
    Urn * uAux = urns;
    Urn * final = uAux + nUrns;
    int count = 0;
    for (; uAux < final; ++uAux)
    {
        if (uAux->max == 0)
        {
            uAux->count = 0;
            uAux->min = urnSize * count;
            uAux->max = uAux->min + urnSize - 1;
        }
        count++;
    }
}

int populateNewUrns(Urn *urns, char *path, int urnSize, int nUrns, int nFiles)
{
    DIR *d;
    struct dirent *dir;
    Urn *uAux = urns;
    Urn *final = urns + nUrns;
    d = opendir(path);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *currentPath = realpath(dir->d_name, NULL);
            if (is_regular_file(currentPath))
            {
                int fSize = get_file_size(dir->d_name);

                uAux = urns;
                final = uAux + nUrns;
                for (; uAux < final; ++uAux)
                {
                    if (fSize >= uAux->min && fSize <= uAux->max)
                    {
                        uAux->count++;
                    }
                }
            }
            else
            {
                // traverseDirectoryTree(urns, final, currentPath);
            }
            ++nFiles;
        }

        closedir(d);
        return nFiles;
    }
}

void generateHist(Urn * urns, int nFiles, int nUrns){
    int histSize;
    int resize = nFiles * 0.1;
    Urn * uAux = urns;
    Urn * final = urns + nUrns;

    for (; uAux < final; ++uAux)
    {
        if (uAux->count != 0)
        {
            histSize = resize * uAux->count;
            uAux->hist = (char *)malloc(sizeof(char) * histSize);
            while (histSize != 0)
            {
                strcat(uAux->hist, "*");
                --histSize;
            }
        }
    }
}

void printAllStruc(Urn * uAux, int nUrns)
{
    Urn *aux = uAux;
    Urn *final = aux + nUrns;

    printf("Urna \t\t Número de archivos \t Histograma\n\n");
    for (; aux < final; ++aux)
    {
        if(aux->count != 0)
            printf("%d - %d \t %2d \t\t\t %s\n", aux->min, aux->max, aux->count, aux->hist);
    }

    printf("\n\n");
}



