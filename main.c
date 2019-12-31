#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <dirent.h>
struct dosyaisimleri   //dosya adlaryny tutmak için struct.
{
    char dosyaadi[30];
};
struct dosyabasliklari  //dosya baslyklaryny tutmak için struct.
{
    char basliklar[25];
};
struct benimdosyam
{
    FILE *x;
};
struct noktasayisi
{
    char noktasayidizisi[9];
};
struct tutucu
{
    char nTutucu[150];
};
struct tutucu1
{
    char nTutucu1[150];
};
struct binaryNoktalar
{
    float x,y,z;
    int r,g,b;
};
struct binaryNoktalar1
{
    float x,y,z;
};

int main()
{
    struct dosyaisimleri dosya[100];//5 dosyanyn da adyny  çekmemiz için tanymalanan struct.
    struct dosyabasliklari baslik[5];// dosyanyn içindeki 5 satyrlyk ba?ly?y çekebilimek için .
    struct benimdosyam dosyalarim[100];
    struct benimdosyam dosyalarimB[100];
    char bdizi[200];
    int dosyasayisi=0;
    DIR *p;
    struct dirent *pp;
    p = opendir (".");
    int i=0;//dosya adlaryny bulup structa kopyalamak için (i).
    if (p != NULL)
    {
        while ((pp = readdir (p))!=NULL)
        {
            int length = strlen(pp->d_name);
            if (strncmp(pp->d_name + length - 4, ".nkt", 4) == 0)//dosya isimlerinin son 4 karakterini sorguluyoruz.
            {
                strcpy(dosya[i].dosyaadi,pp->d_name);//dosya adlarini elimizde tutuyoruz
                //printf("%s\n",dosya[i].dosyaadi);

                if(!strcmp(dosya[i].dosyaadi,"Output.nkt"))
                {
                       continue;
                }
                i++;
            }
        }
        dosyasayisi=i;
        (void) closedir (p);
    }
    FILE *Output;
    int kontrol=0;
    int Secim;//kullanycydan i?lem almak için (secim)
    for(int i=0; i<20; i++)
    {
        printf("bir islem seciniz(1-2-3-4-5):");
        scanf("%d",&Secim);

        switch(Secim)
        {
        case 1 :
            kontrol++;
            printf("*****************SECIM 1******************\n\n\n");
            for(int j=0; j<dosyasayisi; j++)
            {
                dosyalarim[j].x=fopen(dosya[j].dosyaadi,"r");
                dosyalarimB[j].x=fopen(dosya[j].dosyaadi,"rb");
                Output=fopen("Output.nkt","a");
                printf("***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                fprintf(Output,"***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                for(int k=0; k<5; k++)
                {
                    fgets(baslik[k].basliklar,"%s",dosyalarim[j].x);//satir satir basliklari aldyk.
                    //printf("%s\n",baslik[k].basliklar);
                }
                //BASLIK KARSILASTIRMASI
                char *ilkSatir="# Noktalar dosya format";
                char *ikinciSatir ="VERSION 1";
                char *ucuncuSatir1 ="ALANLAR x y z r g b";
                char *ucuncuSatir2="ALANLAR x y z";
                char *besincisatir1="DATA ascii";
                char *besincisatir2="DATA binary";
                int cesit=0;
                if(strlen(baslik[0].basliklar)>24 || strncmp(baslik[0].basliklar,ilkSatir,23)!=0 )//# Noktalar dosya format kontrolü
                {
                    printf("yanlis baslik bilgisi ---> %s\n",baslik[0].basliklar);
                    fprintf(Output,"yanlis baslik bilgisi ---> %s\n",baslik[0].basliklar);
                    cesit-=10;
                }

                if(strlen(baslik[1].basliklar)>10 || strncmp(baslik[1].basliklar,ikinciSatir,9)!=0)//VERSION 1 kontrolü
                {
                    printf("yanlis baslik bilgisi ---> %s\n",baslik[1].basliklar);
                    fprintf(Output,"yanlis baslik bilgisi ---> %s\n",baslik[1].basliklar);
                    cesit-=10;
                }
                if((strlen(baslik[2].basliklar)>20 || strncmp(baslik[2].basliklar,ucuncuSatir1,19)!=0) && (strlen(baslik[2].basliklar)>14 || strncmp(baslik[2].basliklar,ucuncuSatir2,13)!=0)) //ALANLAR x y z kontrolü
                {
                    printf("yanlis baslik bilgisi---->%s \n",baslik[2].basliklar);
                    fprintf(Output,"yanlis baslik bilgisi---->%s \n",baslik[2].basliklar);
                    cesit-=10;
                }
                else if(strncmp(baslik[2].basliklar,ucuncuSatir1,19)==0)
                    cesit+=2;
                if((strlen(baslik[4].basliklar)>11 || strncmp(baslik[4].basliklar,besincisatir1,10)!=0) && (strlen(baslik[4].basliklar)>12 || strncmp(baslik[4].basliklar,besincisatir2,11)!=0))// data ascii ve data binary kontrol
                {
                    printf("%s ----->Basligi hatalidir,okuma yapilmayacaktir!\n",baslik[4].basliklar);
                    fprintf(Output,"%s ----->Basligi hatalidir,okuma yapilmayacaktir!\n",baslik[4].basliklar);
                    cesit-=10;
                }
                else if(strncmp(baslik[4].basliklar,besincisatir2,11)==0)
                    cesit+=1;
                // printf("%d\n",cesit);
                //BASLIK KARSILASTIRMASI BITTI
                if(cesit<0)//armut=baslyk yanlys
                {
                    printf("*****Bu dosyadan bilgi alinmayacaktir.\n\n\n");
                    fprintf(Output,"*****Bu dosyadan bilgi alinmayacaktir.\n\n\n");
                    continue;
                }
                else if(cesit==1) //DATA BINARY ALANLAR XYZ
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar1 binary;
                    struct binaryNoktalar1 *binary1;
                    binary1=(struct binaryNoktalar1 *)malloc(sizeof(struct binaryNoktalar1)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        int a=0;
                        double x,y,z;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar1), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar1)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f\n", binary.x,binary.y,binary.z);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    //nokta sayisi okunacak.
                    char sayi[6];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //printf("baslikta verilen satir sayisi:%d\n",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    //printf("kodda okunan satir sayisi:%d\n",sayiSayacBinary);
                    //nokta bilgisi karsilastirma --baslik icin.
                    if(sayiSayacBinary!=sayinokta)
                    {
                        printf("%s dosyasindaki nokta sayisi gecerli degildir.\n",dosya[j].dosyaadi);
                        fprintf(Output,"%s dosyasindaki nokta sayisi gecerli degildir.\n",dosya[j].dosyaadi);
                    }

                    else
                    {
                        printf("^^^^Bu dosya uyumludur^^^^\n\n\n");
                        fprintf(Output,"^^^^Bu dosya uyumludur^^^^\n\n\n");
                    }
                    free(binary1);
                }
                else if(cesit==3)
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar binary;
                    struct binaryNoktalar *binary1;
                    binary1=(struct binaryNoktalar *)malloc(sizeof(struct binaryNoktalar)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        int r,g,b,a=0;
                        double x,y,z;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1[a].r=binary.r;
                                binary1[a].g=binary.g;
                                binary1[a].b=binary.b;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f %d %d %d\n", binary.x,binary.y,binary.z,binary.r,binary.g,binary.b);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    //bilgiler elimizde tutulacak.
                    for(int i=0; i<sayiSayacBinary; i++)
                    {
                        //printf("%f %f %f %d %d %d\n",binary1[i].x,binary1[i].y,binary1[i].z,binary1[i].r,binary1[i].g,binary1[i].b);
                        if(!((binary1[i].b>=0 && binary1[i].b<=255)||(binary1[i].r>=0 && binary1[i].r<=255)||(binary1[i].g>=0 && binary1[i].g<=255)))
                        {
                            printf("%d. noktanin r-g-b degerleri verilmemistir.\n",i+1);
                            fprintf(Output,"%d. noktanin r-g-b degerleri verilmemistir.\n",i+1);
                        }
                    }
                    //nokta sayisi belirlenecek
                    char sayi[6];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    //nokta bilgisi karsilastirma --baslik icin.
                    if(sayiSayacBinary!=sayinokta)
                    {
                        printf("%s dosyasindaki nokta sayisi gecerli degildir.\n",dosya[j].dosyaadi);
                        fprintf(Output,"%s dosyasindaki nokta sayisi gecerli degildir.\n",dosya[j].dosyaadi);
                    }

                    else
                    {
                        printf("^^^^Bu dosya uyumludur^^^^\n");
                        fprintf(Output,"^^^^Bu dosya uyumludur^^^^\n");
                    }

                    free(binary1);
                }
                else if(cesit==2)//ASCI X Y Z R G B
                {
                    //nokta sayisi belirlenecek
                    char sayi[6];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    int noktaTutanMatris1[sayinokta][3];
                    int s=0,i=0;
                    double noktasayac=-1;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                            noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            noktasayac++;
                            i++;
                        }
                        s++;
                    }
                    /*for(int i=0; i<noktasayac; i++)
                           printf("%s\n",noktaTutucu[i].nTutucu);*/
                    const char *ayrac=" ";
                    int h,h1;
                    for(int i=0; i<noktasayac; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        h1=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;
                            }
                            else if(h>=3)
                            {
                                noktaTutanMatris1[i][h1]=atof(kelime);
                                kelime=strtok(NULL,ayrac);
                                h++;
                                h1++;
                            }
                            else if(h==6)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    /*for(int i=0; i<noktasayac; i++)
                         printf("%d.nokta - %f %f %f %d %d %d \n",i+1,noktaTutanMatris[i][0],noktaTutanMatris[i][1],noktaTutanMatris[i][2],noktaTutanMatris1[i][0],noktaTutanMatris1[i][1],noktaTutanMatris1[i][2]);
                    */
                    for(int i=0; i<noktasayac; i++)
                    {
                        if(!((noktaTutanMatris1[i][3]>=0 && noktaTutanMatris1[i][3]<256)||(noktaTutanMatris1[i][4]>=0 && noktaTutanMatris1[i][4]<256)||(noktaTutanMatris1[i][5]>=0 && noktaTutanMatris1[i][5]<256)))//rgb değeri kontrol edildi.
                        {
                            printf("%d. noktanin r-g-b degerleri dogru verilmemistir.\n",i+1);
                            fprintf(Output,"%d. noktanin r-g-b degerleri dogru verilmemistir.\n",i+1);
                        }

                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        int boslukSayac=1;
                        while (kelime != NULL)
                        {
                            kelime = strtok(NULL,ayrac);
                            boslukSayac++;

                        }
                        if((boslukSayac!=5) && (noktaTutanMatris1[i][0]==0 && noktaTutanMatris1[i][1]==0 && noktaTutanMatris1[i][2]==0))
                        {
                            printf("%d. noktanin r-g-b degerleri dogru verilmemistir.\n",i+1);
                            fprintf(Output,"%d. noktanin r-g-b degerleri dogru verilmemistir.\n",i+1);
                        }

                    }

                    if(noktasayac!=sayinokta)
                    {
                        printf("%s dosyasindaki nokta sayisi uyumlu degildir okuma yapilmayacaktir.!\n",dosya[j].dosyaadi);
                        fprintf(Output,"%s dosyasindaki nokta sayisi uyumlu degildir okuma yapilmayacaktir.!\n",dosya[j].dosyaadi);
                    }
                    else
                    {
                        printf("^^^^Bu dosya uyumludur^^^^\n");
                        fprintf(Output,"^^^^Bu dosya uyumludur^^^^\n");
                    }

                    free(noktaTutucu);
                }
                else if(cesit==0)//ASCI X Y Z
                {
                    //nokta sayisi belirlenecek
                    char sayi[7];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    // int noktaTutanMatris1[sayinokta][3];
                    int s=0,i=0;
                    double noktasayac=-1;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                            noktasayac++;
                            noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            i++;
                        }
                        s++;
                    }
                    /*for(int i=0; i<sayinokta; i++)
                          printf("%s\n",noktaTutucu[i].nTutucu);*/
                    const char *ayrac=" ";
                    int h;
                    for(int i=0; i<noktasayac; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;

                            }
                            else if(h==3)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    /* for(int i=0; i<noktasayac; i++)
                           printf("%d.nokta - %f %f %f\n",i+1,noktaTutanMatris[i][0],noktaTutanMatris[i][1],noktaTutanMatris[i][2]);*/
                    if(noktasayac!=sayinokta)
                    {
                        printf("%s dosyasındaki nokta sayisi uyumlu degildir okuma yapilmayacaktir.!\n",dosya[j].dosyaadi);
                        fprintf(Output,"%s dosyasındaki nokta sayisi uyumlu degildir okuma yapilmayacaktir.!\n",dosya[j].dosyaadi);
                    }
                    else
                    {
                        printf("^^^^Bu dosya uyumludur^^^^\n");
                        fprintf(Output,"^^^^Bu dosya uyumludur^^^^\n");
                    }

                    free(noktaTutucu);
                }
                fclose(Output);
                fclose(dosyalarim[j].x);
                fclose(dosyalarimB[j].x);
            }
            break;

        case 2 :
            if(kontrol==0)
            {
                printf("!!!!!!!!!!!!!!!Dosyadan bilgi cekmelisiniz!!!!!!!!!!!!\n\n");
                continue;
            }
            printf("*****************SECIM 2******************\n\n\n");
            for(int j=0; j<dosyasayisi; j++)
            {
                dosyalarim[j].x=fopen(dosya[j].dosyaadi,"r");
                dosyalarimB[j].x=fopen(dosya[j].dosyaadi,"rb");
                Output=fopen("Output.nkt","a");
                printf("/----DOSYA ADI : %s ----\\\n",dosya[j].dosyaadi);
                fprintf(Output,"/----DOSYA ADI : %s ----\\\n",dosya[j].dosyaadi);
                for(int k=0; k<5; k++)
                {
                    fgets(baslik[k].basliklar,"%s",dosyalarim[j].x);//satir satir basliklari aldyk.
                    //printf("%s\n",baslik[k].basliklar);
                }
                //BASLIK KARSILASTIRMASI
                char *ilkSatir="# Noktalar dosya format";
                char *ikinciSatir ="VERSION 1";
                char *ucuncuSatir1 ="ALANLAR x y z r g b";
                char *ucuncuSatir2="ALANLAR x y z";
                char *besincisatir1="DATA ascii";
                char *besincisatir2="DATA binary";
                int cesit=0;
                if(strlen(baslik[0].basliklar)>24 || strncmp(baslik[0].basliklar,ilkSatir,23)!=0 )//# Noktalar dosya format kontrolü
                    cesit-=10;

                if(strlen(baslik[1].basliklar)>10 || strncmp(baslik[1].basliklar,ikinciSatir,9)!=0)//VERSION 1 kontrolü
                    cesit-=10;
                if((strlen(baslik[2].basliklar)>20 || strncmp(baslik[2].basliklar,ucuncuSatir1,19)!=0) && (strlen(baslik[2].basliklar)>14 || strncmp(baslik[2].basliklar,ucuncuSatir2,13)!=0)) //ALANLAR x y z kontrolü
                    cesit-=10;
                else if(strncmp(baslik[2].basliklar,ucuncuSatir1,19)==0)
                    cesit+=2;
                if((strlen(baslik[4].basliklar)>11 || strncmp(baslik[4].basliklar,besincisatir1,10)!=0) && (strlen(baslik[4].basliklar)>12 || strncmp(baslik[4].basliklar,besincisatir2,11)!=0))// data ascii ve data binary kontrol
                    cesit-=10;
                else if(strncmp(baslik[4].basliklar,besincisatir2,11)==0)
                    cesit+=1;
                // printf("%d\n",cesit);
                //BASLIK KARSILASTIRMASI BITTI
                if(cesit<0)//armut=baslyk yanlys
                    continue;
                else if(cesit==1) //DATA BINARY ALANLAR XYZ
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar1 binary;
                    struct binaryNoktalar1 *binary1;
                    binary1=(struct binaryNoktalar1 *)malloc(sizeof(struct binaryNoktalar1)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int a=0;
                        sscanf(bdizi,"%f %f %f ",&x,&y,&z);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar1), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar1)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f\n", binary.x,binary.y,binary.z);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    double enUzakNokta=0;
                    double enYakiNokta=500;
                    double sonucYU=0;
                    double x1,x2,y1,y2,z1,z2;
                    int yakinSatir1=0,yakinSatir2=0,uzakSatir1=0,uzakSatir2=0;
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        for(int f=d+1; f<sayiSayacBinary; f++)
                        {
                            x1=binary1[d].x;
                            x2=binary1[f].x;
                            y1=binary1[d].y;
                            y2=binary1[f].y;
                            z1=binary1[d].z;
                            z2=binary1[f].z;

                            sonucYU=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            if(sonucYU>enUzakNokta)
                            {
                                enUzakNokta=sonucYU;
                                uzakSatir1=d;
                                uzakSatir2=f;
                            }
                            if(sonucYU<enYakiNokta)
                            {
                                enYakiNokta=sonucYU;
                                yakinSatir1=d;
                                yakinSatir2=f;
                            }

                        }

                    }
                    printf("^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,binary1[yakinSatir1].x,binary1[yakinSatir1].y,binary1[yakinSatir1].z,yakinSatir2+1,binary1[yakinSatir2].x,binary1[yakinSatir2].y,binary1[yakinSatir2].z);
                    printf("^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,binary1[uzakSatir1].x,binary1[uzakSatir1].y,binary1[uzakSatir1].z,uzakSatir2+1,binary1[uzakSatir2].x,binary1[uzakSatir2].y,binary1[uzakSatir2].z);
                    fprintf(Output,"^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,binary1[yakinSatir1].x,binary1[yakinSatir1].y,binary1[yakinSatir1].z,yakinSatir2+1,binary1[yakinSatir2].x,binary1[yakinSatir2].y,binary1[yakinSatir2].z);
                    fprintf(Output,"^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,binary1[uzakSatir1].x,binary1[uzakSatir1].y,binary1[uzakSatir1].z,uzakSatir2+1,binary1[uzakSatir2].x,binary1[uzakSatir2].y,binary1[uzakSatir2].z);
                    free(binary1);
                }
                else if(cesit==3)//DATA BINARY ALANLAR XYZ RGB
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar binary;
                    struct binaryNoktalar *binary1;
                    binary1=(struct binaryNoktalar *)malloc(sizeof(struct binaryNoktalar)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int r,g,b,a=0;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1[a].r=binary.r;
                                binary1[a].g=binary.g;
                                binary1[a].b=binary.b;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f %d %d %d\n", binary.x,binary.y,binary.z,binary.r,binary.g,binary.b);
                            }
                        }
                    }
                    double enUzakNokta=0;
                    double enYakiNokta=500;
                    double sonucYU=0;
                    double x1,x2,y1,y2,z1,z2;
                    int yakinSatir1=0,yakinSatir2=0,uzakSatir1=0,uzakSatir2=0;
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        for(int f=d+1; f<sayiSayacBinary; f++)
                        {
                            x1=binary1[d].x;
                            x2=binary1[f].x;
                            y1=binary1[d].y;
                            y2=binary1[f].y;
                            z1=binary1[d].z;
                            z2=binary1[f].z;

                            sonucYU=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            if(sonucYU>enUzakNokta)
                            {
                                enUzakNokta=sonucYU;
                                uzakSatir1=d;
                                uzakSatir2=f;
                            }
                            if(sonucYU<enYakiNokta)
                            {
                                enYakiNokta=sonucYU;
                                yakinSatir1=d;
                                yakinSatir2=f;
                            }

                        }

                    }
                    printf("^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,binary1[yakinSatir1].x,binary1[yakinSatir1].y,binary1[yakinSatir1].z,yakinSatir2+1,binary1[yakinSatir2].x,binary1[yakinSatir2].y,binary1[yakinSatir2].z);
                    printf("^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,binary1[uzakSatir1].x,binary1[uzakSatir1].y,binary1[uzakSatir1].z,uzakSatir2+1,binary1[uzakSatir2].x,binary1[uzakSatir2].y,binary1[uzakSatir2].z);
                    fprintf(Output,"^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,binary1[yakinSatir1].x,binary1[yakinSatir1].y,binary1[yakinSatir1].z,yakinSatir2+1,binary1[yakinSatir2].x,binary1[yakinSatir2].y,binary1[yakinSatir2].z);
                    fprintf(Output,"^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,binary1[uzakSatir1].x,binary1[uzakSatir1].y,binary1[uzakSatir1].z,uzakSatir2+1,binary1[uzakSatir2].x,binary1[uzakSatir2].y,binary1[uzakSatir2].z);
                    free(binary1);

                }
                else if(cesit==0 || cesit==2)//ASCI X Y Z
                {
                    //nokta sayisi belirlenecek
                    char sayi[7];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    // int noktaTutanMatris1[sayinokta][3];
                    int s=0;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            for(int i=0; i<sayinokta; i++)
                            {
                                fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                                noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            }
                        }
                        s++;
                    }
                    const char *ayrac=" ";
                    int h;
                    for(int i=0; i<sayinokta; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;

                            }
                            else if(h==3)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    double enUzakNokta=0;
                    double enYakiNokta=500;
                    double sonucYU=0;
                    double x1,x2,y1,y2,z1,z2;
                    int yakinSatir1=0,yakinSatir2=0,uzakSatir1=0,uzakSatir2=0;
                    for(int d=0; d<sayinokta; d++)
                    {
                        for(int f=d+1; f<sayinokta; f++)
                        {
                            x1=noktaTutanMatris[d][0];
                            x2=noktaTutanMatris[f][0];
                            y1=noktaTutanMatris[d][1];
                            y2=noktaTutanMatris[f][1];
                            z1=noktaTutanMatris[d][2];
                            z2=noktaTutanMatris[f][2];
                            sonucYU=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            if(sonucYU>enUzakNokta)
                            {
                                enUzakNokta=sonucYU;
                                uzakSatir1=d;
                                uzakSatir2=f;
                            }
                            if(sonucYU<enYakiNokta)
                            {
                                enYakiNokta=sonucYU;
                                yakinSatir1=d;
                                yakinSatir2=f;
                            }
                        }

                    }
                    printf("^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,noktaTutanMatris[yakinSatir1][0],noktaTutanMatris[yakinSatir1][1],noktaTutanMatris[yakinSatir1][2],yakinSatir2+1,noktaTutanMatris[yakinSatir2][0],noktaTutanMatris[yakinSatir2][1],noktaTutanMatris[yakinSatir2][2]);
                    printf("^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,noktaTutanMatris[uzakSatir1][0],noktaTutanMatris[uzakSatir1][1],noktaTutanMatris[uzakSatir1][2],uzakSatir2+1,noktaTutanMatris[uzakSatir2][0],noktaTutanMatris[uzakSatir2][1],noktaTutanMatris[uzakSatir2][2]);
                    fprintf(Output,"^^Birbirine en yakin noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",yakinSatir1+1,noktaTutanMatris[yakinSatir1][0],noktaTutanMatris[yakinSatir1][1],noktaTutanMatris[yakinSatir1][2],yakinSatir2+1,noktaTutanMatris[yakinSatir2][0],noktaTutanMatris[yakinSatir2][1],noktaTutanMatris[yakinSatir2][2]);
                    fprintf(Output,"^^Birbirine en uzak noktalar %d.(x=%f,y=%f,z=%f) ve %d.(x=%f,y=%f,z=%f) noktalaridir^^\n\n",uzakSatir1+1,noktaTutanMatris[uzakSatir1][0],noktaTutanMatris[uzakSatir1][1],noktaTutanMatris[uzakSatir1][2],uzakSatir2+1,noktaTutanMatris[uzakSatir2][0],noktaTutanMatris[uzakSatir2][1],noktaTutanMatris[uzakSatir2][2]);
                    free(noktaTutucu);
                }
                fclose(Output);
                fclose(dosyalarim[j].x);
                fclose(dosyalarimB[j].x);
            }
            break;
        case 3 :
            if(kontrol==0)
            {
                printf("!!!!!!!!!!!!!!!Dosyadan bilgi cekmelisiniz!!!!!!!!!!!!\n\n");
                continue;
            }
            printf("*****************SECIM 3******************\n\n\n");
            for(int j=0; j<dosyasayisi; j++)
            {
                dosyalarim[j].x=fopen(dosya[j].dosyaadi,"r");
                dosyalarimB[j].x=fopen(dosya[j].dosyaadi,"rb");
                Output=fopen("Output.nkt","a");
                printf("/----DOSYA ADI : %s ----\\\n",dosya[j].dosyaadi);
                fprintf(Output,"/----DOSYA ADI : %s ----\\\n",dosya[j].dosyaadi);

                for(int k=0; k<5; k++)
                {
                    fgets(baslik[k].basliklar,"%s",dosyalarim[j].x);//satir satir basliklari aldyk.
                    //printf("%s\n",baslik[k].basliklar);
                }
                //BASLIK KARSILASTIRMASI
                char *ilkSatir="# Noktalar dosya format";
                char *ikinciSatir ="VERSION 1";
                char *ucuncuSatir1 ="ALANLAR x y z r g b";
                char *ucuncuSatir2="ALANLAR x y z";
                char *besincisatir1="DATA ascii";
                char *besincisatir2="DATA binary";
                int cesit=0;
                if(strlen(baslik[0].basliklar)>24 || strncmp(baslik[0].basliklar,ilkSatir,23)!=0 )//# Noktalar dosya format kontrolü
                    cesit-=10;

                if(strlen(baslik[1].basliklar)>10 || strncmp(baslik[1].basliklar,ikinciSatir,9)!=0)//VERSION 1 kontrolü
                    cesit-=10;
                if((strlen(baslik[2].basliklar)>20 || strncmp(baslik[2].basliklar,ucuncuSatir1,19)!=0) && (strlen(baslik[2].basliklar)>14 || strncmp(baslik[2].basliklar,ucuncuSatir2,13)!=0)) //ALANLAR x y z kontrolü
                    cesit-=10;
                else if(strncmp(baslik[2].basliklar,ucuncuSatir1,19)==0)
                    cesit+=2;
                if((strlen(baslik[4].basliklar)>11 || strncmp(baslik[4].basliklar,besincisatir1,10)!=0) && (strlen(baslik[4].basliklar)>12 || strncmp(baslik[4].basliklar,besincisatir2,11)!=0))// data ascii ve data binary kontrol
                    cesit-=10;
                else if(strncmp(baslik[4].basliklar,besincisatir2,11)==0)
                    cesit+=1;
                //printf("%d\n",cesit);
                //BASLIK KARSILASTIRMASI BITTI
                if(cesit<0)//armut=baslyk yanlys
                    continue;
                else if(cesit==1) //DATA BINARY ALANLAR XYZ
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar1 binary;
                    struct binaryNoktalar1 *binary1;
                    binary1=(struct binaryNoktalar1 *)malloc(sizeof(struct binaryNoktalar1)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs+=1;
                        double x,y,z;
                        int a=0;
                        sscanf(bdizi,"%f %f %f ",&x,&y,&z);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar1), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar1)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f\n", binary.x,binary.y,binary.z);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    double x1,y1,z1;
                    double xBuyuk=-1000,xKucuk=1000,yBuyuk=-1000,yKucuk=1000,zBuyuk=-1000,zKucuk=10000;
                    double xOrta=0,yOrta=0,zOrta=0,yariCapX=0,yariCapY=0,yariCapZ=0,yariCap=0;
                    double kose[8][3];
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        x1=binary1[d].x;
                        y1=binary1[d].y;
                        z1=binary1[d].z;
                        if(x1>xBuyuk)
                        {
                            xBuyuk=x1;
                        }
                        if(xKucuk>x1)
                        {
                            xKucuk=x1;
                        }
                        if(y1>yBuyuk)
                        {
                            yBuyuk=y1;
                        }
                        if(yKucuk>y1)
                        {
                            yKucuk=y1;
                        }
                        if(z1>zBuyuk)
                        {
                            zBuyuk=z1;
                        }
                        if(zKucuk>z1)
                        {
                            zKucuk=z1;
                        }
                    }
                    xOrta=(xBuyuk+xKucuk)/2;
                    yOrta=(yBuyuk+yKucuk)/2;
                    zOrta=(zBuyuk+zKucuk)/2;
                    yariCapX=(xBuyuk-xOrta);
                    yariCapY=(yBuyuk-yOrta);
                    yariCapZ=(yBuyuk-yOrta);
                    if(yariCapX>yariCapY && yariCapX>yariCapZ)
                        yariCap=yariCapX;
                    else
                    {
                        if(yariCapY>yariCapZ)
                            yariCap=yariCapY;
                        else
                            yariCap=yariCapZ;
                    }
                    kose[0][0]=(xOrta-yariCap);
                    kose[0][1]=(yOrta-yariCap);
                    kose[0][2]=(zOrta-yariCap);

                    kose[1][0]=(xOrta-yariCap);
                    kose[1][1]=(yOrta-yariCap);
                    kose[1][2]=(zOrta+yariCap);

                    kose[2][0]=(xOrta-yariCap);
                    kose[2][1]=(yOrta+yariCap);
                    kose[2][2]=(zOrta-yariCap);

                    kose[3][0]=(xOrta-yariCap);
                    kose[3][1]=(yOrta+yariCap);
                    kose[3][2]=(zOrta+yariCap);

                    kose[4][0]=(xOrta+yariCap);
                    kose[4][1]=(yOrta-yariCap);
                    kose[4][2]=(zOrta-yariCap);

                    kose[5][0]=(xOrta+yariCap);
                    kose[5][1]=(yOrta-yariCap);
                    kose[5][2]=(zOrta+yariCap);

                    kose[6][0]=(xOrta+yariCap);
                    kose[6][1]=(yOrta+yariCap);
                    kose[6][2]=(zOrta-yariCap);

                    kose[7][0]=(xOrta+yariCap);
                    kose[7][1]=(yOrta+yariCap);
                    kose[7][2]=(zOrta+yariCap);
                    //koseler bulundu yazdirilacak.
                    for(int i=0; i<8; i++)
                    {
                        printf("Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                        fprintf(Output,"Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                    }
                    free(binary1);
                }
                else if(cesit==3)//DATA BINARY ALANLAR XYZ RGB
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar binary;
                    struct binaryNoktalar *binary1;
                    binary1=(struct binaryNoktalar *)malloc(sizeof(struct binaryNoktalar)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int r,g,b,a=0;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1[a].r=binary.r;
                                binary1[a].g=binary.g;
                                binary1[a].b=binary.b;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f %d %d %d\n", binary.x,binary.y,binary.z,binary.r,binary.g,binary.b);
                            }
                        }
                    }
                    double x1,y1,z1;
                    double xBuyuk=-1000,xKucuk=1000,yBuyuk=-1000,yKucuk=1000,zBuyuk=-1000,zKucuk=10000;
                    double xOrta=0,yOrta=0,zOrta=0,yariCapX=0,yariCapY=0,yariCapZ=0,yariCap=0;
                    double kose[8][3];
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        x1=binary1[d].x;
                        y1=binary1[d].y;
                        z1=binary1[d].z;
                        if(x1>xBuyuk)
                        {
                            xBuyuk=x1;
                        }
                        if(xKucuk>x1)
                        {
                            xKucuk=x1;
                        }
                        if(y1>yBuyuk)
                        {
                            yBuyuk=y1;
                        }
                        if(yKucuk>y1)
                        {
                            yKucuk=y1;
                        }
                        if(z1>zBuyuk)
                        {
                            zBuyuk=z1;
                        }
                        if(zKucuk>z1)
                        {
                            zKucuk=z1;
                        }
                    }
                    xOrta=(xBuyuk+xKucuk)/2;
                    yOrta=(yBuyuk+yKucuk)/2;
                    zOrta=(zBuyuk+zKucuk)/2;
                    yariCapX=(xBuyuk-xOrta);
                    yariCapY=(yBuyuk-yOrta);
                    yariCapZ=(yBuyuk-yOrta);
                    if(yariCapX>yariCapY && yariCapX>yariCapZ)
                        yariCap=yariCapX;
                    else
                    {
                        if(yariCapY>yariCapZ)
                            yariCap=yariCapY;
                        else
                            yariCap=yariCapZ;
                    }
                    kose[0][0]=(xOrta-yariCap);
                    kose[0][1]=(yOrta-yariCap);
                    kose[0][2]=(zOrta-yariCap);

                    kose[1][0]=(xOrta-yariCap);
                    kose[1][1]=(yOrta-yariCap);
                    kose[1][2]=(zOrta+yariCap);

                    kose[2][0]=(xOrta-yariCap);
                    kose[2][1]=(yOrta+yariCap);
                    kose[2][2]=(zOrta-yariCap);

                    kose[3][0]=(xOrta-yariCap);
                    kose[3][1]=(yOrta+yariCap);
                    kose[3][2]=(zOrta+yariCap);

                    kose[4][0]=(xOrta+yariCap);
                    kose[4][1]=(yOrta-yariCap);
                    kose[4][2]=(zOrta-yariCap);

                    kose[5][0]=(xOrta+yariCap);
                    kose[5][1]=(yOrta-yariCap);
                    kose[5][2]=(zOrta+yariCap);

                    kose[6][0]=(xOrta+yariCap);
                    kose[6][1]=(yOrta+yariCap);
                    kose[6][2]=(zOrta-yariCap);

                    kose[7][0]=(xOrta+yariCap);
                    kose[7][1]=(yOrta+yariCap);
                    kose[7][2]=(zOrta+yariCap);
                    //koseler bulundu yazdirilacak.
                    for(int i=0; i<8; i++)
                    {
                        printf("Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                        fprintf(Output,"Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                    }
                    free(binary1);

                }
                else if(cesit==0 || cesit==2)//ASCI X Y Z
                {
                    //nokta sayisi belirlenecek
                    char sayi[7];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    // int noktaTutanMatris1[sayinokta][3];
                    int s=0;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            for(int i=0; i<sayinokta; i++)
                            {
                                fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                                noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            }
                        }
                        s++;
                    }
                    const char *ayrac=" ";
                    int h;
                    for(int i=0; i<sayinokta; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;

                            }
                            else if(h==3)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    double x1,y1,z1;
                    double xBuyuk=-1000,xKucuk=1000,yBuyuk=-1000,yKucuk=1000,zBuyuk=-1000,zKucuk=10000;
                    double xOrta=0,yOrta=0,zOrta=0,yariCapX=0,yariCapY=0,yariCapZ=0,yariCap=0;
                    double kose[8][3];
                    for(int d=0; d<sayinokta; d++)
                    {
                        x1=noktaTutanMatris[d][0];
                        y1=noktaTutanMatris[d][1];
                        z1=noktaTutanMatris[d][2];
                        if(x1>xBuyuk)
                        {
                            xBuyuk=x1;
                        }
                        if(xKucuk>x1)
                        {
                            xKucuk=x1;
                        }
                        if(y1>yBuyuk)
                        {
                            yBuyuk=y1;
                        }
                        if(yKucuk>y1)
                        {
                            yKucuk=y1;
                        }
                        if(z1>zBuyuk)
                        {
                            zBuyuk=z1;
                        }
                        if(zKucuk>z1)
                        {
                            zKucuk=z1;
                        }
                    }
                    xOrta=(xBuyuk+xKucuk)/2;
                    yOrta=(yBuyuk+yKucuk)/2;
                    zOrta=(zBuyuk+zKucuk)/2;
                    yariCapX=(xBuyuk-xOrta);
                    yariCapY=(yBuyuk-yOrta);
                    yariCapZ=(yBuyuk-yOrta);
                    if(yariCapX>yariCapY && yariCapX>yariCapZ)
                        yariCap=yariCapX;
                    else
                    {
                        if(yariCapY>yariCapZ)
                            yariCap=yariCapY;
                        else
                            yariCap=yariCapZ;
                    }
                    kose[0][0]=(xOrta-yariCap);
                    kose[0][1]=(yOrta-yariCap);
                    kose[0][2]=(zOrta-yariCap);

                    kose[1][0]=(xOrta-yariCap);
                    kose[1][1]=(yOrta-yariCap);
                    kose[1][2]=(zOrta+yariCap);

                    kose[2][0]=(xOrta-yariCap);
                    kose[2][1]=(yOrta+yariCap);
                    kose[2][2]=(zOrta-yariCap);

                    kose[3][0]=(xOrta-yariCap);
                    kose[3][1]=(yOrta+yariCap);
                    kose[3][2]=(zOrta+yariCap);

                    kose[4][0]=(xOrta+yariCap);
                    kose[4][1]=(yOrta-yariCap);
                    kose[4][2]=(zOrta-yariCap);

                    kose[5][0]=(xOrta+yariCap);
                    kose[5][1]=(yOrta-yariCap);
                    kose[5][2]=(zOrta+yariCap);

                    kose[6][0]=(xOrta+yariCap);
                    kose[6][1]=(yOrta+yariCap);
                    kose[6][2]=(zOrta-yariCap);

                    kose[7][0]=(xOrta+yariCap);
                    kose[7][1]=(yOrta+yariCap);
                    kose[7][2]=(zOrta+yariCap);
                    //koseler bulundu yazdirilacak.
                    for(int i=0; i<8; i++)
                    {
                        printf("Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                        fprintf(Output,"Kupun %d. kosesi : ( %f , %f , %f )\n",i+1,kose[i][0],kose[i][1],kose[i][2]);
                    }
                    free(noktaTutucu);
                }
                fclose(Output);
                fclose(dosyalarim[j].x);
                fclose(dosyalarimB[j].x);
            }
            break;
        case 4 :
        {
            if(kontrol==0)
            {
                printf("!!!!!!!!!!!!!!!Dosyadan bilgi cekmelisiniz!!!!!!!!!!!!\n\n");
                continue;
            }
            printf("*****************SECIM 4******************\n\n\n");
            double x1,y1,z1,data=0;
            double yariCap,mX,mY,mZ;
            printf("\n\nYaricap giriniz:");
            scanf("%lf",&yariCap);
            printf("\n ^^Merkez koordinatlarini giriniz^^");
            printf("\nX=");
            scanf("%lf",&mX);
            printf("\nY=");
            scanf("%lf",&mY);
            printf("\nZ=");
            scanf("%lf",&mZ);
            for(int j=0; j<dosyasayisi; j++)
            {
                dosyalarim[j].x=fopen(dosya[j].dosyaadi,"r");
                dosyalarimB[j].x=fopen(dosya[j].dosyaadi,"rb");
                Output=fopen("Output.nkt","a");
                printf("***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                fprintf(Output,"***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                for(int k=0; k<5; k++)
                {
                    fgets(baslik[k].basliklar,"%s",dosyalarim[j].x);//satir satir basliklari aldyk.
                    //printf("%s\n",baslik[k].basliklar);
                }
                //BASLIK KARSILASTIRMASI
                char *ilkSatir="# Noktalar dosya format";
                char *ikinciSatir ="VERSION 1";
                char *ucuncuSatir1 ="ALANLAR x y z r g b";
                char *ucuncuSatir2="ALANLAR x y z";
                char *besincisatir1="DATA ascii";
                char *besincisatir2="DATA binary";
                int cesit=0;
                if(strlen(baslik[0].basliklar)>24 || strncmp(baslik[0].basliklar,ilkSatir,23)!=0 )//# Noktalar dosya format kontrolü
                    cesit-=10;

                if(strlen(baslik[1].basliklar)>10 || strncmp(baslik[1].basliklar,ikinciSatir,9)!=0)//VERSION 1 kontrolü
                    cesit-=10;
                if((strlen(baslik[2].basliklar)>20 || strncmp(baslik[2].basliklar,ucuncuSatir1,19)!=0) && (strlen(baslik[2].basliklar)>14 || strncmp(baslik[2].basliklar,ucuncuSatir2,13)!=0)) //ALANLAR x y z kontrolü
                    cesit-=10;
                else if(strncmp(baslik[2].basliklar,ucuncuSatir1,19)==0)
                    cesit+=2;
                if((strlen(baslik[4].basliklar)>11 || strncmp(baslik[4].basliklar,besincisatir1,10)!=0) && (strlen(baslik[4].basliklar)>12 || strncmp(baslik[4].basliklar,besincisatir2,11)!=0))// data ascii ve data binary kontrol
                    cesit-=10;
                else if(strncmp(baslik[4].basliklar,besincisatir2,11)==0)
                    cesit+=1;
                //printf("%d\n",cesit);
                //BASLIK KARSILASTIRMASI BITTI
                if(cesit<0)//armut=baslyk yanlys
                    continue;
                else if(cesit==1) //DATA BINARY ALANLAR XYZ
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar1 binary;
                    struct binaryNoktalar1 *binary1;
                    binary1=(struct binaryNoktalar1 *)malloc(sizeof(struct binaryNoktalar1)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int a=0;
                        sscanf(bdizi,"%f %f %f ",&x,&y,&z);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar1), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar1)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f\n", binary.x,binary.y,binary.z);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    printf("\n--Kurenin icinde kalan noktalar--\n\n");
                    fprintf(Output,"\n--Kurenin icinde kalan noktalar--\n\n");
                    for(int i=0; i<sayiSayacBinary; i++)
                    {
                        x1=binary1[i].x;
                        y1=binary1[i].y;
                        z1=binary1[i].z;
                        // printf("%f %f %f\n",x1,y1,z1);
                        data=sqrt(pow((mX-x1),2)+pow((mY-y1),2)+pow((mZ-z1),2));
                        if(data<=yariCap)
                        {
                            printf("%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                            fprintf(Output,"%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                        }
                    }
                    free(binary1);
                }
                else if(cesit==3)//DATA BINARY ALANLAR XYZ RGB
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar binary;
                    struct binaryNoktalar *binary1;
                    binary1=(struct binaryNoktalar *)malloc(sizeof(struct binaryNoktalar)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int r,g,b,a=0;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1[a].r=binary.r;
                                binary1[a].g=binary.g;
                                binary1[a].b=binary.b;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f %d %d %d\n", binary.x,binary.y,binary.z,binary.r,binary.g,binary.b);
                            }
                        }
                    }
                    printf("\n--Kurenin icinde kalan noktalar--\n\n");
                    fprintf(Output,"\n--Kurenin icinde kalan noktalar--\n\n");
                    for(int i=0; i<sayiSayacBinary; i++)
                    {
                        x1=binary1[i].x;
                        y1=binary1[i].y;
                        z1=binary1[i].z;
                        // printf("%f %f %f\n",x1,y1,z1);
                        data=sqrt(pow((mX-x1),2)+pow((mY-y1),2)+pow((mZ-z1),2));
                        if(data<=yariCap)
                        {
                            printf("%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                            fprintf(Output,"%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                        }
                    }
                    free(binary1);

                }
                else if(cesit==0 || cesit==2)//ASCI X Y Z
                {
                    //nokta sayisi belirlenecek
                    char sayi[7];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    // int noktaTutanMatris1[sayinokta][3];
                    int s=0;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            for(int i=0; i<sayinokta; i++)
                            {
                                fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                                noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            }
                        }
                        s++;
                    }
                    const char *ayrac=" ";
                    int h;
                    for(int i=0; i<sayinokta; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;

                            }
                            else if(h==3)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    printf("\n--Kurenin icinde kalan noktalar--\n\n");
                    fprintf(Output,"\n--Kurenin icinde kalan noktalar--\n\n");
                    for(int i=0; i<sayinokta; i++)
                    {
                        x1=noktaTutanMatris[i][0];
                        y1=noktaTutanMatris[i][1];
                        z1=noktaTutanMatris[i][2];
                        // printf("%f %f %f\n",x1,y1,z1);
                        data=sqrt(pow((mX-x1),2)+pow((mY-y1),2)+pow((mZ-z1),2));
                        if(data<=yariCap)
                        {
                            printf("%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                            fprintf(Output,"%d. nokta ( %f , %f , %f )\n",i+1,x1,y1,z1);
                        }
                    }
                    free(noktaTutucu);
                }
                fclose(Output);
                fclose(dosyalarim[j].x);
                fclose(dosyalarimB[j].x);
            }
        }

        break;
        case 5 :
            if(kontrol==0)
            {
                printf("!!!!!!!!!!!!!!!Dosyadan bilgi cekmelisiniz!!!!!!!!!!!!\n\n");
                continue;
            }
            printf("*****************SECIM 5******************\n\n\n");
            for(int j=0; j<dosyasayisi; j++)
            {
                dosyalarim[j].x=fopen(dosya[j].dosyaadi,"r");
                dosyalarimB[j].x=fopen(dosya[j].dosyaadi,"rb");
                Output=fopen("Output.nkt","a+");
                printf("***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                fprintf(Output,"***----DOSYA ADI : %s ----***\n",dosya[j].dosyaadi);
                for(int k=0; k<5; k++)
                {
                    fgets(baslik[k].basliklar,"%s",dosyalarim[j].x);//satir satir basliklari aldyk.
                    //printf("%s\n",baslik[k].basliklar);
                }
                //BASLIK KARSILASTIRMASI
                char *ilkSatir="# Noktalar dosya format";
                char *ikinciSatir ="VERSION 1";
                char *ucuncuSatir1 ="ALANLAR x y z r g b";
                char *ucuncuSatir2="ALANLAR x y z";
                char *besincisatir1="DATA ascii";
                char *besincisatir2="DATA binary";
                int cesit=0;
                if(strlen(baslik[0].basliklar)>24 || strncmp(baslik[0].basliklar,ilkSatir,23)!=0 )//# Noktalar dosya format kontrolü
                    cesit-=10;

                if(strlen(baslik[1].basliklar)>10 || strncmp(baslik[1].basliklar,ikinciSatir,9)!=0)//VERSION 1 kontrolü
                    cesit-=10;
                if((strlen(baslik[2].basliklar)>20 || strncmp(baslik[2].basliklar,ucuncuSatir1,19)!=0) && (strlen(baslik[2].basliklar)>14 || strncmp(baslik[2].basliklar,ucuncuSatir2,13)!=0)) //ALANLAR x y z kontrolü
                    cesit-=10;
                else if(strncmp(baslik[2].basliklar,ucuncuSatir1,19)==0)
                    cesit+=2;
                if((strlen(baslik[4].basliklar)>11 || strncmp(baslik[4].basliklar,besincisatir1,10)!=0) && (strlen(baslik[4].basliklar)>12 || strncmp(baslik[4].basliklar,besincisatir2,11)!=0))// data ascii ve data binary kontrol
                    cesit-=10;
                else if(strncmp(baslik[4].basliklar,besincisatir2,11)==0)
                    cesit+=1;
                //printf("%d\n",cesit);
                //BASLIK KARSILASTIRMASI BITTI
                if(cesit<0)//armut=baslyk yanlys
                    continue;
                else if(cesit==1) //DATA BINARY ALANLAR XYZ
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar1 binary;
                    struct binaryNoktalar1 *binary1;
                    binary1=(struct binaryNoktalar1 *)malloc(sizeof(struct binaryNoktalar1)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int a=0;
                        sscanf(bdizi,"%f %f %f ",&x,&y,&z);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar1), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar1)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f\n", binary.x,binary.y,binary.z);
                            }
                        }
                    }
                    // printf("%d",sayiSayacBinary);//NOKTA SAYISI BELIRLENDi
                    double x1,x2,y1,y2,z1,z2;
                    double toplamBinary=0,ortalamaBinary=0;
                    int sonucSayac=0;
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        for(int f=d+1; f<sayiSayacBinary; f++)
                        {
                            x1=binary1[d].x;
                            x2=binary1[f].x;
                            y1=binary1[d].y;
                            y2=binary1[f].y;
                            z1=binary1[d].z;
                            z2=binary1[f].z;
                            toplamBinary+=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            sonucSayac++;
                        }
                    }
                    ortalamaBinary=toplamBinary/sonucSayac;
                    printf("\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaBinary);
                    fprintf(Output,"\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaBinary);
                    free(binary1);
                }
                else if(cesit==3)//DATA BINARY ALANLAR XYZ RGB
                {
                    //ILK ONCE NOKTALARI TESPIT ETTIK
                    int satirs=0,sayiSayacBinary=0;
                    struct binaryNoktalar binary;
                    struct binaryNoktalar *binary1;
                    binary1=(struct binaryNoktalar *)malloc(sizeof(struct binaryNoktalar)*10);
                    for(;fgets(bdizi,200,dosyalarimB[j].x);)
                    {
                        satirs++;
                        double x,y,z;
                        int r,g,b,a=0;
                        sscanf(bdizi,"%f %f %f %d %d %d",&x,&y,&z,&r,&g,&b);
                        if(satirs==5)
                        {
                            while(!feof(dosyalarimB[j].x))
                            {
                                size_t sayi = fread(&binary, sizeof(struct binaryNoktalar), 1, dosyalarimB[j].x);
                                if (sayi < 1)
                                    break;   // okunamadiysa (dosya bitmisse vb.) donguyu durdur
                                binary1[a].x=binary.x;
                                binary1[a].y=binary.y;
                                binary1[a].z=binary.z;
                                binary1[a].r=binary.r;
                                binary1[a].g=binary.g;
                                binary1[a].b=binary.b;
                                binary1=realloc(binary1,sizeof(struct binaryNoktalar)*10*10);
                                sayiSayacBinary++;
                                a++;
                                //printf("%f %f %f %d %d %d\n", binary.x,binary.y,binary.z,binary.r,binary.g,binary.b);
                            }
                        }
                    }
                    double x1,x2,y1,y2,z1,z2;
                    double toplamBinaryRGB=0,ortalamaBinaryRGB=0;
                    int sonucSayac=0;
                    for(int d=0; d<sayiSayacBinary; d++)
                    {
                        for(int f=d+1; f<sayiSayacBinary; f++)
                        {
                            x1=binary1[d].x;
                            x2=binary1[f].x;
                            y1=binary1[d].y;
                            y2=binary1[f].y;
                            z1=binary1[d].z;
                            z2=binary1[f].z;
                            toplamBinaryRGB+=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            sonucSayac++;
                        }

                    }
                    ortalamaBinaryRGB=toplamBinaryRGB/sonucSayac;
                    printf("\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaBinaryRGB);
                    fprintf(Output,"\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaBinaryRGB);
                    free(binary1);

                }
                else if(cesit==0 || cesit==2)//ASCI X Y Z
                {
                    //nokta sayisi belirlenecek
                    char sayi[7];
                    char noktakopya[strlen(baslik[3].basliklar)];
                    int k=0;
                    strcpy(noktakopya,baslik[3].basliklar);
                    for(int i=9; i<=strlen(baslik[3].basliklar); i++)
                    {
                        sayi[k]=noktakopya[i];
                        k++;
                    }
                    //printf("%s",sayi);
                    int sayinokta=atoi(sayi);
                    //  printf("%d",sayinokta);//NOKTA SAYYSY ALINMI?TIR .
                    struct tutucu *noktaTutucu;
                    noktaTutucu = (struct tutucu *) malloc (sayinokta);
                    double noktaTutanMatris[sayinokta][3];
                    // int noktaTutanMatris1[sayinokta][3];
                    int s=0;
                    while(!feof(dosyalarim[j].x))//noktalar , matrise atylmak üzere ilk önce char dizisine atylyyor.
                    {
                        if(s>4)
                        {
                            for(int i=0; i<sayinokta; i++)
                            {
                                fgets(noktaTutucu[i].nTutucu,"%s",dosyalarim[j].x);
                                noktaTutucu=realloc(noktaTutucu,sayinokta*1000);
                            }
                        }
                        s++;
                    }
                    const char *ayrac=" ";
                    int h;
                    for(int i=0; i<sayinokta; i++)
                    {
                        char *kelime = strtok(noktaTutucu[i].nTutucu,ayrac);
                        h=0;
                        while (kelime != NULL)
                        {
                            if(h<3)
                            {
                                noktaTutanMatris[i][h]=atof(kelime);
                                kelime = strtok(NULL,ayrac);
                                h++;

                            }
                            else if(h==3)
                            {
                                kelime=NULL;
                            }
                        }
                    }
                    double x1,x2,y1,y2,z1,z2;
                    double toplamAscii=0,ortalamaAscii=0;
                    double sonucSayac=0;
                    for(int d=0; d<sayinokta; d++)
                    {
                        for(int f=d+1; f<sayinokta; f++)
                        {
                            x1=noktaTutanMatris[d][0];
                            x2=noktaTutanMatris[f][0];
                            y1=noktaTutanMatris[d][1];
                            y2=noktaTutanMatris[f][1];
                            z1=noktaTutanMatris[d][2];
                            z2=noktaTutanMatris[f][2];
                            toplamAscii+=sqrt(pow((x2-x1),2)+pow((y2-y1),2)+pow((z2-z1),2));
                            sonucSayac++;
                        }
                    }
                    ortalamaAscii=toplamAscii/sonucSayac;
                    printf("\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaAscii);
                    fprintf(Output,"\nNoktalarin birbirine olan uzakliklarin ortalamasi = %f\n\n",ortalamaAscii);
                    free(noktaTutucu);
                }
                fclose(Output);
                fclose(dosyalarim[j].x);
                fclose(dosyalarimB[j].x);
            }
            break;
        default  :
            printf( "yanlis secim\n\n" );
            break;
        }
    }
    return 0;
}

