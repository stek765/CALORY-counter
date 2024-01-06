#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>


void clearScreen();
void opzioneUno();
void Calcolo(char*, float, int*);
void Choose_Piano(char*, float, float);
void Crea_Piano(char*, float, float);
void Aggiungi(char*, float, float);
void apri_file(char*);
void kcaltot(char*);

// - - - - - - - -
// MAIN
// - - - - - - - -

int main() {

    clearScreen();
     
    setlocale(LC_NUMERIC, "C"); // Imposta il separatore decimale su punto
    int scelta;
    char filename[50];
   
    while(scelta != -1){
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("\nMENÙ: \n\n");
        printf("1. REGISTRA ALIMENTO\n");
        printf("2. CALCOLA KCAL TOTALI");
        printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

        printf("\n\n\n(attendo comando, -1 per terminare): ");
        int result = scanf("%d", &scelta);
        // Pulisci il terminale
        clearScreen();
        switch (scelta) {
            case 1:
                opzioneUno();
                break;
            case 2: 
                printf("Inserisci il nome del file (.txt verrà aggiunto automaticamente): ");
                scanf("%s", filename);
                
                FILE *file;
                char estensione[] = ".txt";

                strcat(filename, estensione);  
                kcaltot(filename);
                break;
            default:
                printf("\nScelta non valida. Riprova.\n");
        }
}       

return 0; 
}

// - - - - - - - - 

void opzioneUno() {
    
    printf("\n~ REGISTRA ALIMENTO:");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    sleep(1);
    char Nome[20];
    float Quantità;
    int Answer;
    int risultato[3];

    printf("\nInserisci NOME alimento: ");
    scanf("%s", Nome);

    printf("\nInserisci QUANTITÀ[g]: ");
    scanf("%f", &Quantità);
    
    printf("\n\n\nDevi CALCOLARE Calorie?\n\n");
    printf("1. SÌ \n");
    printf("0. NO \n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n\n(attendo comando): ");
    scanf("%d", &Answer);

    
   
    if(Answer == 0){
        main();
        return;
    }
    if(Answer != 1 && Answer != 0){
        printf("\nScelta non valida. Riprova.\n");
        return;
    }

    clearScreen();

    Calcolo(Nome, Quantità, risultato);
    float tot = 0;
    for (int j = 0; j < 3; j++)
        tot += risultato[j];
        // Pulisci il terminale
    clearScreen();
    printf("\n~ REGISTRA ALIMENTO:");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n- Alimento: %s \n- Quantità: %.2f\n- Valori nutrizionali ricalcolati: (Prot = %d, Carb = %d, Gras = %d) \t in TOTALE = %.2f kcal. \n", Nome, Quantità, risultato[0],risultato[1], risultato[2], tot);
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    sleep(20);

 

    Choose_Piano(Nome, Quantità, tot); 
}

void Choose_Piano(char* Nome, float Quantità, float tot){
    // Pulisci il terminale
    clearScreen();
    printf("\n~ REGISTRA ALIMENTO:");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    int Answer; 
    printf("\nHai già un Piano?\n");
    printf("1. SÌ \n");
    printf("0. NO \n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n(attendo comando): ");
    scanf("%d", &Answer);

    if(Answer == 0){
        Crea_Piano(Nome, Quantità, tot);
        main();
        return;
    }
    if(Answer != 1 && Answer != 0){
        printf("\nScelta non valida. Riprova.\n");
        main();
        return;
        }
    
    Aggiungi(Nome, Quantità, tot);    
}   

void Aggiungi(char* Nome, float Quantità, float tot){
    char nomeFile[50];

    printf("Inserisci il nome del file (.txt verrà aggiunto automaticamente): ");
    scanf("%s", nomeFile);

    FILE *file;
    char estensione[] = ".txt";

    strcat(nomeFile, estensione);  

    file = fopen(nomeFile, "a"); // Apri il file in modalità append

    if (file == NULL) {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }

    fprintf(file, "%-30s %-30.2f %-30.2f\n", Nome, Quantità, tot);
    
    fclose(file);
    apri_file(nomeFile);
}

void Crea_Piano(char* Nome, float Quantità, float tot){
    char nomeFile[50];

    printf("Come vuoi chiamare il file: ");
    scanf("%s", nomeFile);

    FILE *file;
    char estensione[] = ".txt";

    strcat(nomeFile, estensione);  

    file = fopen(nomeFile, "w");

    if (file == NULL) {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }

    fprintf(file, "%-30s %-30s %-30s\n", "NOME",  "QUANTITA",  "KCAL TOTALI");
    fprintf(file, "------------------------------ ------------------------------ ------------------------------ \n");
    fprintf(file, "\n");
    fprintf(file, "%-30s %-30.2f %-30.2f\n", Nome, Quantità, tot);

    fclose(file);
    apri_file(nomeFile);
}

void kcaltot(char* nomeFile) {
    FILE *file;
    char riga[100]; // Assumiamo che ogni riga del file sia lunga al massimo 100 caratteri
    float totaleKcal = 0;

    file = fopen(nomeFile, "r"); // Apri il file in modalità lettura

    if (file == NULL) {
        printf("Errore nell'apertura del file.\n");
        exit(1);
    }

    // Leggi ogni riga del file
    while (fgets(riga, sizeof(riga), file) != NULL) {
        float kcal;
        // Usa sscanf per estrarre la quantità di kcal dalla riga
        if (sscanf(riga, "%*s %*f %f", &kcal) == 1) {
            totaleKcal += kcal;
        }
    }

    // Chiudi il file
    fclose(file);

    printf("\n il TOTALE delle kcal è -> %.2f\n", totaleKcal);
}

void Calcolo(char* Nome, float Quantità, int* risultato){
    char protStr[20], carbStr[20], grasStr[20];
    float prot, carb, gras;

    printf("\nInserisci VALORI NUTRIZIONALI [g] (utilizza il sito FatSecret e in caso di valori decimali usa il punto):\n");
    printf("\nPROTEINE -> ");
    scanf("%s", protStr);

    // Converto la stringa in float utilizzando sscanf
    if (sscanf(protStr, "%f", &prot) != 1) {
        printf("\nInput non valido per le proteine. Riprova.\n");
        return;
    }

    printf("\nCARBOIDRATI -> ");
    scanf("%s", carbStr);

    // Converto la stringa in float utilizzando sscanf
    if (sscanf(carbStr, "%f", &carb) != 1) {
        printf("\nInput non valido per i carboidrati. Riprova.\n");
        return;
    }

    printf("\nGRASSI -> ");
    scanf("%s", grasStr);

    // Converto la stringa in float utilizzando sscanf
    if (sscanf(grasStr, "%f", &gras) != 1) {
        printf("\nInput non valido per i grassi. Riprova.\n");
        return;
    }
    
    int i;
    risultato[0] = (9* ((prot*Quantità)/100));
    risultato[1] = (4* ((carb*Quantità)/100));
    risultato[2] = (4* ((gras*Quantità)/100));
}

void clearScreen() {
    // Controllo del sistema operativo
    #ifdef _WIN32
        system("cls");  // Comando per pulire il terminale su Windows
    #else
        system("clear");  // Comando per pulire il terminale su Linux/macOS
    #endif
}

void apri_file(char* nomeFile){
    #ifdef _WIN32
    // Windows
    char comando[100];
    sprintf(comando, "start notepad %s", nomeFile);
    system(comando);
    #elif __APPLE__
    // macOS
    char comando[100];
    sprintf(comando, "open %s", nomeFile);
    system(comando);
    #else
    // Altri sistemi operativi (Linux, Unix, ecc.)
    printf("Impossibile aprire automaticamente il file. Apri manualmente con il tuo editor di testo preferito.\n");
    #endif
}
