//Pislari Vadim

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define N 40000
#define M 430000
//dimension-dimensiunea arenei
//var[N]-memorizeaza marimea fiecarui spatiul alocat
//nr-indicele pentru var[]
//start,marime,precedent-setarea primelor 3 indici de start, marime, precedent
//out[M]-fiecare output a comenzii ALLOC
//cite_alocari-indica cite alocari sau efectuat
//sp_lib[N]-indica spatiul liber dupa fiecare bloc daca el exista
// valoare[M]-indica valoarea octetilor alocati de la FILL

uint32_t *arena, dimension, var[N] = {0}, nr = 0, start = 0, marime = 2,
        precedent = 1, out[M] = {0}, p_free = 0, out_s=0, cite_alocari=0,
        sp_lib[N] = {0}, valoare[M]={0}, var_ind[N]={0};
unsigned char *biti;

//afiseaza spatiile si tab-urile pentru dump
void spatiere (uint32_t *ind){
    if( (*ind + 1) % 16 == 0) {
            if((*ind) + 1 != dimension) {
                    printf("\n");
                    printf("%07X0\t",(*ind / 16) + 1);
            }
    }
    else if( (*ind + 1) % 8 == 0) printf(" ");
    (*ind)++;
}

//afiseaza spatiile alocat
void indici (uint32_t adaos, uint32_t *ind ,uint32_t index){
      //variabila contor
      uint32_t j;
        for(j = 0;j != 4; j++){
            if(*ind >= dimension) break;
            printf("%02X ",biti[adaos + j + 12 * index]);
            spatiere(ind);
        }
}

//pentru free
void setarea_zero(uint32_t i){
     arena[3 * i - 3] = 0;
     arena[3 *i - 2] = 0;
     arena[3 * i - 1] = 0;
}

//pentru finalize si pentru verificare daca arena e libera
void variabilele_zero(){
     uint32_t l;
       for(l = 0;l != dimension;l++){
          arena[l] = 0;
          var[l]=0;
          valoare[l]=0;
          sp_lib[l]=0;
          out[l]=0;
       }
       cite_alocari=0;
       nr=0; start = 0;marime = 2;precedent = 1; p_free=0; out_s=0;
}

void parse_command(char* cmd)
{
    const char* delims = " \n";

    char* cmd_name = strtok(cmd, delims);
    if (!cmd_name) {
        goto invalid_command;
    }

    if (strcmp(cmd_name, "INITIALIZE") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        //INITIALIZE

        dimension = size;
        arena = calloc(dimension, sizeof(uint32_t));
        biti =  (unsigned char *) arena;

    } else if (strcmp(cmd_name, "FINALIZE") == 0) {
        // FINALIZE
	variabilele_zero();
        free(arena);

    } else if (strcmp(cmd_name, "DUMP") == 0) {
        // DUMP
        uint32_t ind = 0, j, index = 0, poz = 0, nr_de_alocare = 0;

        while(ind < dimension){
            if(ind == 0)printf("%07X0\t",ind / 16);

            //afisarea a 4 biti a fiecarui indice
            indici(0, &ind, index);
            indici(4, &ind, index);
            indici(8, &ind, index);

            index++;

            //reprezentarea octetilor in arena
            for(j = 0; j!= var[nr_de_alocare]; j++){
                    if(ind >= dimension) break;
                    printf("%02X ", valoare[poz]);
                    poz++;
                    spatiere(&ind);
            }
            nr_de_alocare++;
        }
        if(ind != 0)printf("\n");

    } else if (strcmp(cmd_name, "ALLOC") == 0) {
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);

        // TODO - ALLOC
        uint32_t i = 0, lib = 0 ,j ,tr = 0, auxil = 0;

        // verificarea daca toate elementele sunt 0
        for(i=0; i <= cite_alocari;i++){
            if(arena[3 * i + 2] != 0)auxil = 1;
        }
        // setarea tuturor variabelelor pe 0 daca verificarea a fost cu succes
        if(auxil == 0){
	    variabilele_zero();
        }

        // calcularea spatiilor libere
        for(i = 0;i != cite_alocari; i++){
            if(arena[3 * i + 2] == 0){
                    lib++;
                    if(tr == 0)j = i;
                    tr = 1;
            }
            else {
                    tr = 0;
                    lib = 0;
            }
            if(arena[3 * i + 2] == 0 &&  arena[3 * i + 5] != 0){

                sp_lib[j] = 12*lib;
                while(lib != 0){
                    lib--;
                   sp_lib[j] = sp_lib[j] + var[i - lib];
                }
            }
        }

        uint32_t aux = 0;
        //se verifica daca in toate spatiile libere incape macar un element
        //daca nu se continua functia
        for(i = 0;i != cite_alocari; i++){
            if (size + 12 <= sp_lib[i]){
                    if(i>0) if(var[i - 1] > arena[3 * i - 1] && arena[3 * i + 2]==0){
                        var[i] = var[i]+var[i - 1] - arena [3 * i -1];
                        arena[3 * i - 3]=arena[3 * i - 3]-var[i - 1] + arena [3 * i -1];
                        out[i + 1] =out[i + 1]-var[i - 1] + arena [3 * i -1];
                        var[i - 1] = arena[3 * i - 1];
                    }
                    if (i != 0) {
                        uint32_t aux1 = arena[3 * i - 3];
                        arena[3 * i] = arena[3 * i - 3];
                        arena[3 * i - 3]=aux1 - sp_lib[i];
                    }
                    else arena[3 * i] = sp_lib[i];
                    uint32_t whi = 0;
                    if(i != 0){
                        while(arena[3 * (i + whi) + 2] == 0) whi++;
                        arena[3 * (i + whi) + 1]=arena[3 * i-3];
                    }
                    arena[3 * i + 2] = size;
                    if(i == 0)printf("12\n");
                        else printf("%d\n",arena[3 * i - 3] + 12);
                    //daca marimea alocata ii mai mare decit
                    //spatiul liber a fostului bloc alocat
                    if(size > var[i]){
                        var[i + 1] = var[i + 1] + var[i] - size;
                        var[i] = size;
                        sp_lib[i] = 0;
                        aux = 1;
                        break;
                    }
                    // verificarea daca in spatiul liber se mai poate face inca
                    // o alocare de minim 1 (a 2-a)
                    // daca da se muta cu cite un index spre stinga
                    if(sp_lib[i] - size - 12 > 12 && i != 0 &&
                      (arena[i + 5] != 0 || i+2 == cite_alocari)){
                        cite_alocari++;
                        for(j = cite_alocari; j != i; j--){
                            var[j + 1] = var[j];
                            out[j + 1] = out[j];
                        }
                        var[i+1] = var[i] - size - 12;
                        out[i+2]=out[i+1]+var[i+1];
                        var[i] = size;
                        arena[3 * i + 6] = arena[3 * i + 3];
                        arena[3 * i + 7] = arena[3 * i + 4];
                        arena[3 * i + 8] = arena[3 * i + 5];
                        arena[3 * i + 3] = 0;
                        arena[3 * i + 4] = arena[3 * i - 3];
                        arena[3 * i + 5] = 0;
                        start = start + 3;
                        precedent = precedent + 3;
                        marime = marime + 3;
                        nr++;
                        p_free++;
                    }
                aux = 1;
                sp_lib[i] = 0;
                break;
            }
        }

        //daca sa executat partea de mai sus se termina functia
        if(aux == 1)return;

        //se verifica daca sizeul nu depaseste marimea arenei
        if(aux == 0 && size + 12 + out_s > dimension){
            printf("0\n");
            return;
        }

        //urmatoarea parte se executa daca nu exista spatii libere in care
        // sar putea aloca, se insereaza dupa ultimul element din arena

        if(var[nr] > arena[3 * nr + 2]) var[nr] = arena[3 * nr + 2];
        cite_alocari++;
        var[nr] = size;
        //Setarea valoarea de start
        if(start == 0) arena[start] = 0;
        else if (start == 3 && nr != 0) arena[start - 3] = 12 + var[nr - 1];
        else{
                if(arena [start - 6] == 0){
                        int32_t aux=0;
                        while (sp_lib[aux] == 0) aux++;
                        arena[start - 3] = sp_lib[aux] + 12 + var[nr-1];
                }
                else arena[start - 3] = arena[start-  6] + 12 + var[nr - 1];
        }

        //Setarea valoarea precedenta
        if (precedent == 1 || precedent == 4) arena[precedent] = 0;
        else arena[precedent] = arena[start-3] - 12 - var[nr-1];
        arena[marime] = size;
        nr++;
        precedent = precedent + 3;
        marime = marime + 3;
        start = start + 3;
        //outputul comenzii alloc
        p_free++;
        out_s = out_s + 12 + size;
        out[p_free] = out_s-size;
        printf("%d\n",out[p_free]);

    } else if (strcmp(cmd_name, "FREE") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        // FREE
        int32_t i,j;
        for(i = 0; i <= p_free; i++){
            if (out[i] == index){
                // daca se elibereaza ultimul element
                // toate valorile se micsoreaza
                if(i == cite_alocari){
                    int aux = 1;
                    if(i > 1)while(arena[3 * (i - aux) - 1] == 0 && 3 * (i - aux) - 1 >= 0){
                            aux++;
                    }
                    start = start - 3 * aux;
                    precedent = precedent - 3 * aux;
                    marime = marime - 3 * aux;
                    nr=nr - aux;
                    j=nr;
                    p_free = p_free - aux;
                    cite_alocari = cite_alocari - aux;
		    setarea_zero(i);
                    if(i > 1)arena[3 * i - 6] = arena[3 * i - 3];

                    if (p_free!=0)out_s = out[p_free]+arena[3*p_free-1];
                    else out_s=0;

                    if(nr != 0 && i > 1) var[nr - 1] = arena[3 * (i - aux) - 1];
                    return;
                }

                //modificarea indexului start a alocarii precedente
                //se cauta alocarea precedenta
                int32_t aux_sc = 1;
                if(i > 1) while(arena[3 * (i - aux_sc) - 1] == 0 ){
                    aux_sc++;
                    if(3 * (i - aux_sc) - 1<0)break;
                }
                if(3 * (i - aux_sc) - 3 >=0) arena[3 * (i - aux_sc) - 3] = arena[3 * i - 3];

                //modificarea indexului precedente a alocarii urmatoare
                //se cauta alocarea urmatoare
                int32_t aux_ad = 1;
                while(arena[3 * (i + aux_ad) - 1]==0 ){
                    aux_ad++;
                }

                arena[3 * (i + aux_ad) - 2] = arena[3 * i - 2];

                //se seteaza la 0 bitii a alocarii eliberate
                for(j=var_ind[i-1]; j!= var_ind[i - 1] + var[i]; j++){
                valoare[j] = 0;
                }

                //indexii alocarii eliberate se seteaza la 0
		setarea_zero(i);
            }
        }


    } else if (strcmp(cmd_name, "FILL") == 0) {
        char* index_str = strtok(NULL, delims);
        if (!index_str) {
            goto invalid_command;
        }
        uint32_t index = atoi(index_str);
        char* size_str = strtok(NULL, delims);
        if (!size_str) {
            goto invalid_command;
        }
        uint32_t size = atoi(size_str);
        char* value_str = strtok(NULL, delims);
        if (!value_str) {
            goto invalid_command;
        }
        uint32_t value = atoi(value_str);
        //FILL
        uint32_t i,j;
        var_ind[0] = 0;

        //calcularea de unde se va incepe fill
        for(i = 1; i != p_free; i++){
            var_ind[i] = var_ind[i-1]+var[i-1];
        }
            for(i = 0; i <= p_free; i++){
                //identificarea pozitiei de la ALLOC
                if (out[i] == index){
                    for(j=var_ind[i-1]; j != var_ind[i-1] + size;j++){
                        valoare[j] = value;
                }
            }
        }

    }else {
        goto invalid_command;
    }

    return;

invalid_command:
    printf("Invalid command: %s\n", cmd);
    exit(1);
}

int main(void)
{
    ssize_t read;
    char* line = NULL;
    size_t len;
    int i=0;
    while ((read = getline(&line, &len, stdin)) != -1) {
        printf("%s", line);
        parse_command(line);
        i++;
    }

    free(line);

    return 0;
}


