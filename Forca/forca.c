#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void consultar(){
    char v;
    int nloses,nwins,nplays;
    float winrate,mediachutes;

    FILE *arq=fopen("estatisticas.bin","rb");

    if(arq==NULL){
        printf("Sem estatisticas!\nAperte qualquer tecla para voltar");
        scanf(" %c",&v);

    }else{
        fread(&nwins,sizeof(int),1,arq);
        fread(&nloses,sizeof(int),1,arq);
        fread(&nplays,sizeof(int),1,arq);
        fread(&winrate,sizeof(float),1,arq);
        fread(&mediachutes,sizeof(float),1,arq);

        printf("N° de jogos: %d\n",nplays);
        printf("N° de vitorias: %d\n",nwins);
        printf("N° de derrotas: %d\n",nloses);
        printf("Taxa de vitoria: %.2f%%\n",winrate*100);
        printf("N° medio de chutes: %.2f\n\n",mediachutes);

        printf("Aperte qualquer tecla para voltar\n");
        scanf(" %c",&v);
            fclose(arq);
            system("cls");
            return;


    }

}



void registrar(int ganhou,int nchutes){
    int nloses=0,nwins=0,nplays=1,ver=1;
    float winrate,mediachutes;
/*
ORDEM:
NWINS
NWLOSES
NPLAYS
%WIN
N MEDIO DE CHUTES

*/
    FILE *arq=fopen("estatisticas.bin","rb");

    if(arq==NULL)
        ver=0;
        else
        fclose(arq);


//PRIMEIRO REGISTRO
    if(!ver){
        arq=fopen("estatisticas.bin","wb");

        if(ganhou){
            nwins++;

            winrate=1;
            fwrite(&nwins,sizeof(int),1,arq);
            fwrite(&nloses,sizeof(int),1,arq);
        }else{
            nloses++;

            winrate=0;
            fwrite(&nwins,sizeof(int),1,arq);
            fwrite(&nloses,sizeof(int),1,arq);
        }

        mediachutes=nchutes;

        fwrite(&nplays,sizeof(int),1,arq);
        fwrite(&winrate,sizeof(float),1,arq);
        fwrite(&mediachutes,sizeof(float),1,arq);

        fclose(arq);
    }


//JA JOGOU ANTES

    if(ver){
        arq=fopen("estatisticas.bin","rb");

        fread(&nwins,sizeof(int),1,arq);
        fread(&nloses,sizeof(int),1,arq);
        fread(&nplays,sizeof(int),1,arq);
        fread(&winrate,sizeof(float),1,arq);
        fread(&mediachutes,sizeof(float),1,arq);

        fclose(arq);

        arq=fopen("estatisticas.bin","wb");



        if(ganhou){
            nwins++;
            fwrite(&nwins,sizeof(int),1,arq);
            fwrite(&nloses,sizeof(int),1,arq);
        }else{
            nloses++;
            fwrite(&nwins,sizeof(int),1,arq);
            fwrite(&nloses,sizeof(int),1,arq);
        }

        nplays++;
        fwrite(&nplays,sizeof(int),1,arq);

        winrate=(float)nwins/nplays;
        fwrite(&winrate,sizeof(float),1,arq);

        mediachutes=((mediachutes*(nplays-1))+nchutes)/nplays;
        fwrite(&mediachutes,sizeof(float),1,arq);

        fclose(arq);


    }





}

void jogar(){
    system("cls");
    srand(time(NULL));


    int l=rand()%309,cont=1,erros=0,ip,i,nchutes=0;
    bool ver=false,continua=true,ganhou=true;
    char linha[30],escolhida[30],*ptr,c,erradas[6];
    char progresso[30],v;

    FILE *arq=fopen("formatadas1.txt","r");
//SELECIONAR A PALAVRA
    while(fgets(linha,sizeof(linha),arq)!=NULL){
        if(l==cont){
            strcpy(escolhida,linha);
            break;
        }
        cont++;
    }
    escolhida[strcspn(escolhida,"\n")]='\0';
    for(i=0;escolhida[i]!='\0';i++){
        if(escolhida[i]==' '){
            progresso[i]=' ';
        }else{
            progresso[i]='_';
        }
    }
    progresso[i]='\0';
    fclose(arq);
//TAMANHO
    for(ptr=escolhida;*ptr!='\0';ptr++){
        if(*ptr!=' '){
            printf("_");
        }
        else{
            printf(" ");
        }
    }
    printf("\n");

while(continua){
//CHUTE
    printf("Chute: ");
    printf("\n");
    scanf(" %c",&c);
    nchutes++;

//PROCURAR
    for(ptr=escolhida;*ptr!='\0';ptr++){
        if(*ptr==c){
            ver=true;
            break;
        }
    }
//ACERTO
    if(ver){
        system("cls");

        for(ptr=escolhida,ip=0;*ptr!='\0';ptr++,ip++){

            if(*ptr!=' '){
                if(*ptr==c){
                    printf("%c",c);
                    progresso[ip]=c;
                }else{
                    printf("%c",progresso[ip]);

                }

            }else{
                printf(" ");
                progresso[ip]=' ';
            }
        }
        progresso[ip]='\0';
        printf("\n");

        for(i=0;progresso[i]!='\0';i++){
            if(progresso[i]=='_'){
                ganhou=false;
                break;
            }
        }
        if(ganhou){
            printf("ganhou!\n");

            //registrar stats
            registrar(ganhou,nchutes);



            continua=false;

        }
        ganhou=true;
    }


//ERRO

    if(!ver){
        system("cls");
        erros++;

        if(erros<6){
            erradas[erros-1]=c;
            printf("%s\n",progresso);



        }
        if(erros>=6){
            printf("Perdeu :(\n");
            printf("%s",escolhida);
            printf("\n");

            //registrar stats
            registrar(ganhou,nchutes);

            continua=false;
        }
    }

    ver=false;

    if(erros>0){
            printf("Chutes errados:\n");
            for(i=0;i<erros;i++){
                printf("%c ",erradas[i]);
            }
            printf("\n");
        }

}
printf("\n");
}



int main(){
int o=0;
while(o<3){

    printf("-------Forca-------\n");
    printf("1- Jogar\n");
    printf("2- Estatisticas(em breve)\n");
    printf("3- Sair\n\n");
    printf("Opcao: ");
    scanf("%d",&o);
    getchar();

    if(o==1){
        jogar();
    }

    if(o==2){

        consultar();
    }



}





return 0;
}
