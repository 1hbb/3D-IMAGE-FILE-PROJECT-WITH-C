#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

FILE *output;


struct noktalar
{
    float x;
    float y;
    float z;
    int r;
    int g;
    int b;
};
struct noktalar1
{
    float  x1;
    float y1;
    float  z1;
};
int kordinatSayisiBul(char *chpt)
{
    int i=0;
    int kelimeSayisi=0;
    while (chpt[i]!='\0')
    {
        if(chpt[i]==' ')
            kelimeSayisi++;
        i++;
    }
    return kelimeSayisi+1;
}
int alanlar_bosluk(char *chpt)
{
    int sayac=0;
    for(int j=0; j<strlen(chpt); j++)
    {
        if(chpt[j]==' ')
        {
            sayac++;
        }
    }
    return sayac;
}
void secim_1()
{

    fprintf(output,"\n\n\n\n************************************\n\n\n\n");

    fprintf(output,"\n\n <<<<<<<<<<<<<<<<<< CHOICE1 >>>>>>>>>>>>>> \n\n");
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    struct noktalar1 nkt;

    //Dont' forget you change this directory with one you want to traverse!
    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];
    TCHAR fileName[MAX_PATH];
    char arr[100];
    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }
    else
    {
        do
        {
            //ignore current and parent directories
            if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;

            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }
            else
            {
                //list the Files

                memset(fileName, 0x00, sizeof(fileName));
                _stprintf(fileName, TEXT("%s\\%s"), directory, FindFileData.cFileName);

                FILE *fptr = fopen(fileName, "r");
                char *ara=strstr(fileName,"output.nkt"); // output dosyaı kontrole girmemesi için
                char karakter;
                int satirsayisi=1;
                char alanlar[100];
                char noktalar[100];
                char data_format[100];

                while(feof(fptr)==NULL)//dosya sonuna gelene kadar
                {
                    karakter=fgetc(fptr);
                    if(karakter=='\n')
                    {
                        satirsayisi++;
                        if(satirsayisi==3)
                        {
                            fgets(alanlar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==4)
                        {
                            fgets(noktalar,100,fptr);
                            satirsayisi++;
                        }
                        if(satirsayisi==5)
                        {
                            fgets(data_format,100,fptr);

                            satirsayisi++;
                        }

                    }


                }


                int ilk_kosul=0;
                if(alanlar[8]=='x'&&alanlar[10]=='y'&&alanlar[12]=='z')
                {
                    ilk_kosul=0;
                    if(alanlar[14]!='\0')// EGER X Y Z R G B DEN OLUSUYORSA
                    {
                        if(alanlar[14]!='r'||alanlar[16]!='g'||alanlar[18]!='b')
                        {
                            ilk_kosul=1;
                        }
                    }
                }
                else
                {
                    ilk_kosul=1;
                }

                if(ilk_kosul==1 && ara==NULL)// EGER ALANLAR KISMI HATALIYSA YAZDİRİRYOR
                {
                    fprintf(output," There is error in the %s part of the %s\n",alanlar,fileName);

                }


                if(ilk_kosul==0)
                {
                    int kordinat_sayisi;
                    char data_tipi_ascii[]= "DATA ascii\n";//FGETS FONKSİYONU ALT SATIRA GECMEYİDE BİR KARAKTER OLARAK ALİYOR
                    int sonuc=(strcmp(data_format,data_tipi_ascii));

                    kordinat_sayisi=alanlar_bosluk(alanlar);

                    int noktalarinKordinatSayisi=0;
                    char arr[satirsayisi];
                    int flag=0;
                    if(sonuc==0) // EGER DATA ASCİİ MODUNDA İSE
                    {

                        FILE *fptr1 = fopen(fileName, "r");
                        char *str, *str2;
                        str = fgets(arr,100,fptr1);// ilk satýrý aldým
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);




                        int istenen_nokta_sayisi=atoi(9+noktalar);

                        if(satirsayisi-5!=istenen_nokta_sayisi && ara==NULL)
                        {

                            fprintf(output,"\n The number of requested and given points in the %s file is not the same \n",fileName);
                            flag=1;
                        }
                        else
                        {
                            int j;
                            for(j=0; j<satirsayisi-6 ; j++)
                            {
                                str=fgets(arr,100,fptr1);
                                noktalarinKordinatSayisi=kordinatSayisiBul(str);// verilen kordinatla noktalarin kordinatini karsilastiriyor
                                if(noktalarinKordinatSayisi!=kordinat_sayisi && ara==NULL)
                                {

                                    fprintf(output," \n There is missing line in the %d. of the %s file.\n",j+1,fileName);

                                    flag=1;

                                }
                            }

                        }

                        //do here whatever you want to do..

                        fclose(fptr);
                        fclose(fptr1);

                    }
                    else//BİNARY MODUNDA ACİP AYNİ İSLEMLERİ YAPICAK
                    {
                        if(kordinat_sayisi==3)
                        {

                            FILE *fptr1 = fopen(fileName, "rb");



                            fgets(arr,100,fptr1);// ilk satýrý aldým
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar1 nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];

                            while(!feof(fptr1))
                            {
                                fread(&nkt[say],sizeof(struct noktalar1),1,fptr1);
                                xdeger[say]=nkt[say].x1;
                                ydeger[say]=nkt[say].y1;
                                zdeger[say]=nkt[say].z1;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir && ara==NULL)
                            {

                                fprintf(output," \n The number of requested and given points in the %s file is not the same \n ",fileName);
                                flag=1;
                            }
                            fclose(fptr3);

                        }
                        if(kordinat_sayisi==6)
                        {
                            FILE *fptr2;
                            fptr2=fopen(fileName,"rb");
                            char arr1[1000];
                            fgets(arr1,100,fptr2);// ilk satýrý aldým
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];
                            int rdeger[10000];
                            int gdeger[10000];
                            int bdeger[10000];
                            while(!feof(fptr2))
                            {
                                fread(&nkt[say],sizeof(struct noktalar),1,fptr2);
                                xdeger[say]=nkt[say].x;
                                ydeger[say]=nkt[say].y;
                                zdeger[say]=nkt[say].z;
                                rdeger[say]=nkt[say].r;
                                gdeger[say]=nkt[say].g;
                                bdeger[say]=nkt[say].b;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir && ara==NULL)
                            {

                                fprintf(output,"\n The number of requested and given points in the %s file is not the same \n ",fileName);
                                flag=1;
                            }


                        }




                        //do here whatever you want to do..

                        fclose(fptr);
                    }

                }
            }

        }
        while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }

}
void secim_2()
{
    fprintf(output,"\n\n\n\n************************************\n\n\n\n");

    fprintf(output,"\n\n <<<<<<<<<<<<<<<<<<<  CHOICE 2  >>>>>>>>>>>>>>>>>>> \n\n");

    float en_buyuk_nokta[2][3];
    float en_kucuk_nokta[2][3];
    int en_buyuk_renk[2][3];
    int en_kucuk_renk[2][3];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    struct noktalar1 nkt;

    //Dont' forget you change this directory with one you want to traverse!
    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];
    TCHAR fileName[MAX_PATH];
    char arr[100];
    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }
    else
    {
        do
        {
            //ignore current and parent directories
            if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;

            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }
            else
            {
                //list the Files


                memset(fileName, 0x00, sizeof(fileName));
                _stprintf(fileName, TEXT("%s\\%s"), directory, FindFileData.cFileName);
                FILE *fptr = fopen(fileName, "r");

                char karakter;
                int satirsayisi=1;
                char alanlar[100];
                char noktalar[100];
                char data_format[100];
                while(feof(fptr)==NULL)//dosya sonuna gelene kadar
                {
                    karakter=fgetc(fptr);
                    if(karakter=='\n')
                    {
                        satirsayisi++;
                        if(satirsayisi==3)
                        {
                            fgets(alanlar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==4)
                        {
                            fgets(noktalar,100,fptr);
                            satirsayisi++;
                        }
                        if(satirsayisi==5)
                        {
                            fgets(data_format,100,fptr);

                            satirsayisi++;
                        }
                    }


                }

                int ilk_kosul=0;
                if(alanlar[8]=='x'&&alanlar[10]=='y'&&alanlar[12]=='z')
                {
                    ilk_kosul=0;
                    if(alanlar[14]!='\0')
                    {
                        if(alanlar[14]!='r'||alanlar[16]!='g'||alanlar[18]!='b')
                        {
                            ilk_kosul=1;
                        }
                    }
                }
                else
                {
                    ilk_kosul=1;
                }

                if(ilk_kosul==0)
                {
                    int kordinat_sayisi;
                    char data_tipi_ascii[]= "DATA ascii\n";//FGETS FONKSİYONU ALT SATIRA GECMEYİDE BİR KARAKTER OLARAK ALİYOR
                    int sonuc=(strcmp(data_format,data_tipi_ascii));

                    kordinat_sayisi=alanlar_bosluk(alanlar);

                    int noktalarinKordinatSayisi=0;
                    char arr[satirsayisi];
                    int flag=0;
                    if(sonuc==0) // EGER DATA ASCİİ MODUNDA İSE
                    {

                        FILE *fptr1 = fopen(fileName, "r");
                        char *str, *str2;
                        str = fgets(arr,100,fptr1);// ilk satýrý aldým
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);





                        int istenen_nokta_sayisi=atoi(9+noktalar);

                        if(satirsayisi-5!=istenen_nokta_sayisi)
                        {


                            flag=1;
                        }
                        else
                        {
                            int j;
                            for(j=0; j<satirsayisi-6 ; j++)
                            {
                                str=fgets(arr,100,fptr1);
                                noktalarinKordinatSayisi=kordinatSayisiBul(str);
                                if(noktalarinKordinatSayisi!=kordinat_sayisi)
                                {

                                    flag=1;

                                }
                            }

                        }





                        if(flag==0)
                        {
                            if(kordinat_sayisi==6)
                            {
                                FILE *fptr2 = fopen(fileName, "r");
                                struct noktalar nkt;
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                int rdeger[satirsayisi-5];
                                int gdeger[satirsayisi-5];
                                int bdeger[satirsayisi-5];
                                char arr1[satirsayisi];
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr2,"%f %f %f %d %d %d",&nkt.x,&nkt.y,&nkt.z,&nkt.r,&nkt.g,&nkt.b);
                                    xdeger[i]=nkt.x;
                                    ydeger[i]=nkt.y;
                                    zdeger[i]=nkt.z;
                                    rdeger[i]=nkt.r;
                                    gdeger[i]=nkt.g;
                                    bdeger[i]=nkt.b;
                                }

                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );

                                en_buyuk=en_kucuk;
                                for(int n=0; n<satirsayisi-5; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satirsayisi-5; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );

                                        if(en_buyuk<=tut)
                                        {
                                            en_buyuk=tut;
                                            en_buyuk_nokta[0][0]=xdeger[n];
                                            en_buyuk_nokta[0][1]=ydeger[n];
                                            en_buyuk_nokta[0][2]=zdeger[n];
                                            en_buyuk_nokta[1][0]=xdeger[m];
                                            en_buyuk_nokta[1][1]=ydeger[m];
                                            en_buyuk_nokta[1][2]=zdeger[m];
                                            en_buyuk_renk[0][0]=rdeger[n];
                                            en_buyuk_renk[0][1]=gdeger[n];
                                            en_buyuk_renk[0][2]=bdeger[n];
                                            en_buyuk_renk[1][0]=rdeger[m];
                                            en_buyuk_renk[1][1]=gdeger[m];
                                            en_buyuk_renk[1][2]=bdeger[m];

                                        }

                                        if(en_kucuk>=tut)
                                        {
                                            en_kucuk=tut;
                                            en_kucuk_nokta[0][0]=xdeger[n];//en kucuk x noktasini 1 matrisin x ine atadim
                                            en_kucuk_nokta[0][1]=ydeger[n];
                                            en_kucuk_nokta[0][2]=zdeger[n];
                                            en_kucuk_nokta[1][0]=xdeger[m];
                                            en_kucuk_nokta[1][1]=ydeger[m];
                                            en_kucuk_nokta[1][2]=zdeger[m];
                                            en_kucuk_renk[0][0]=rdeger[n];
                                            en_kucuk_renk[0][1]=rdeger[n];
                                            en_kucuk_renk[0][2]=rdeger[n];
                                            en_kucuk_renk[1][0]=rdeger[n];
                                            en_kucuk_renk[1][1]=rdeger[n];
                                            en_kucuk_renk[1][2]=rdeger[n];

                                        }


                                    }

                                }


                                fprintf(output,"Two points with the greatest distance between in %s file",fileName);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_buyuk_nokta[0][0],en_buyuk_nokta[0][1],en_buyuk_nokta[0][2],en_buyuk_renk[0][0],en_buyuk_renk[0][1],en_buyuk_renk[0][2] );
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_buyuk_nokta[1][0],en_buyuk_nokta[1][1],en_buyuk_nokta[1][2],en_buyuk_renk[1][0],en_buyuk_renk[1][1],en_buyuk_renk[1][2]);
                                fprintf(output,"\n---------------------------------------\n");
                                fprintf(output,"Two points with the least distance between in %s file",fileName);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_kucuk_nokta[0][0],en_kucuk_nokta[0][1],en_kucuk_nokta[0][2],en_kucuk_renk[0][0],en_kucuk_renk[0][1],en_kucuk_renk[0][2]);
                                fprintf(output, "\n%f %f %f %d %d %d \n",en_kucuk_nokta[1][0],en_kucuk_nokta[1][1],en_kucuk_nokta[1][2],en_kucuk_renk[1][0],en_kucuk_renk[1][1],en_kucuk_renk[1][2]);
                                fprintf(output,"\n--------------------------------------\n");







                                fclose(fptr2);
                            }


                            if(kordinat_sayisi==3)
                            {
                                struct noktalar1 nkt;
                                FILE *fptr3 = fopen(fileName, "r");
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                char arr2[satirsayisi];
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr3,"%f %f %f ",&nkt.x1,&nkt.y1,&nkt.z1);
                                    xdeger[i]=nkt.x1;
                                    ydeger[i]=nkt.y1;
                                    zdeger[i]=nkt.z1;

                                }


                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );
                                en_buyuk=en_kucuk;

                                for(int n=0; n<satirsayisi-5; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satirsayisi-5; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );

//printf("   %f\n",tut);
                                        if(en_buyuk<=tut)
                                        {
                                            en_buyuk=tut;
                                            en_buyuk_nokta[0][0]=xdeger[n];
                                            en_buyuk_nokta[0][1]=ydeger[n];
                                            en_buyuk_nokta[0][2]=zdeger[n];
                                            en_buyuk_nokta[1][0]=xdeger[m];
                                            en_buyuk_nokta[1][1]=ydeger[m];
                                            en_buyuk_nokta[1][2]=zdeger[m];

                                        }
                                        if(en_kucuk>=tut)
                                        {
                                            en_kucuk=tut;
                                            en_kucuk_nokta[0][0]=xdeger[n];
                                            en_kucuk_nokta[0][1]=ydeger[n];
                                            en_kucuk_nokta[0][2]=zdeger[n];
                                            en_kucuk_nokta[1][0]=xdeger[m];
                                            en_kucuk_nokta[1][1]=ydeger[m];
                                            en_kucuk_nokta[1][2]=zdeger[m];

                                        }

                                    }

                                }

                                fprintf(output,"Two points with the greatest distance between in %s file",fileName);

                                fprintf(output,"\n%f %f %f \n",en_buyuk_nokta[0][0],en_buyuk_nokta[0][1],en_buyuk_nokta[0][2]);
                                fprintf(output,"\n%f %f %f \n",en_buyuk_nokta[1][0],en_buyuk_nokta[1][1],en_buyuk_nokta[1][2]);
                                fprintf(output,"----------------------------------------\n");
                                fprintf(output,"Two points with the least distance between in %s file",fileName);
                                fprintf(output,"\n%f %f %f \n",en_kucuk_nokta[0][0],en_kucuk_nokta[0][1],en_kucuk_nokta[0][2]);
                                fprintf(output,"\n%f %f %f \n",en_kucuk_nokta[1][0],en_kucuk_nokta[1][1],en_kucuk_nokta[1][2]);
                                fprintf(output,"\n-----------------------------------------\n");





                                fclose(fptr3);
                            }
                        }




                        //do here whatever you want to do..
                        fclose(fptr);
                        fclose(fptr1);
                    }
                    else//BİNARY MODUNDA ACİP AYNİ İSLEMLERİ YAPICAK
                    {

                        if(kordinat_sayisi==3)
                        {

                            FILE *fptr1 = fopen(fileName, "rb");


                            //char *str, *str2;
                            fgets(arr,100,fptr1);// ilk satýrý aldým
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar1 nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];

                            while(!feof(fptr1))
                            {
                                fread(&nkt[say],sizeof(struct noktalar1),1,fptr1);
                                xdeger[say]=nkt[say].x1;
                                ydeger[say]=nkt[say].y1;
                                zdeger[say]=nkt[say].z1;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {

                                flag=1;
                            }
                            fclose(fptr3);
                            if(flag==0)
                            {

                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );
                                en_buyuk=en_kucuk;

                                for(int n=0; n<satir; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satir; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );

//printf("   %f\n",tut);
                                        if(en_buyuk<=tut)
                                        {
                                            en_buyuk=tut;
                                            en_buyuk_nokta[0][0]=xdeger[n];
                                            en_buyuk_nokta[0][1]=ydeger[n];
                                            en_buyuk_nokta[0][2]=zdeger[n];
                                            en_buyuk_nokta[1][0]=xdeger[m];
                                            en_buyuk_nokta[1][1]=ydeger[m];
                                            en_buyuk_nokta[1][2]=zdeger[m];

                                        }
                                        if(en_kucuk>=tut)
                                        {
                                            en_kucuk=tut;
                                            en_kucuk_nokta[0][0]=xdeger[n];
                                            en_kucuk_nokta[0][1]=ydeger[n];
                                            en_kucuk_nokta[0][2]=zdeger[n];
                                            en_kucuk_nokta[1][0]=xdeger[m];
                                            en_kucuk_nokta[1][1]=ydeger[m];
                                            en_kucuk_nokta[1][2]=zdeger[m];

                                        }

                                    }

                                }



                                fprintf(output,"\n----------------------------------------\n");
                                fprintf(output,"Two points with the greatest distance between in %s binary file",fileName);
                                fprintf(output,"\n%f %f %f \n",en_buyuk_nokta[0][0],en_buyuk_nokta[0][1],en_buyuk_nokta[0][2]);
                                fprintf(output,"\n%f %f %f \n",en_buyuk_nokta[1][0],en_buyuk_nokta[1][1],en_buyuk_nokta[1][2] );
                                fprintf(output,"------------------------------------------\n");
                                fprintf(output,"Two points with the least distance between in %s binary file",fileName);
                                fprintf(output,"\n%f %f %f \n",en_kucuk_nokta[0][0],en_kucuk_nokta[0][1],en_kucuk_nokta[0][2]);
                                fprintf(output,"\n%f %f %f \n",en_kucuk_nokta[1][0],en_kucuk_nokta[1][1],en_kucuk_nokta[1][2]);
                                fprintf(output,"------------------------------------------\n");



                            }


                        }
                        if(kordinat_sayisi==6)
                        {
                            FILE *fptr2;
                            fptr2=fopen(fileName,"rb");
                            char arr1[1000];
                            fgets(arr1,100,fptr2);// ilk satýrý aldým
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            int istenen_nokta_sayisi=atoi(9+noktalar);

                            struct noktalar nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];
                            int rdeger[10000];
                            int gdeger[10000];
                            int bdeger[10000];
                            while(!feof(fptr2))
                            {
                                fread(&nkt[say],sizeof(struct noktalar),1,fptr2);
                                xdeger[say]=nkt[say].x;
                                ydeger[say]=nkt[say].y;
                                zdeger[say]=nkt[say].z;
                                rdeger[say]=nkt[say].r;
                                gdeger[say]=nkt[say].g;
                                bdeger[say]=nkt[say].b;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {

                                flag=1;
                            }

                            if(flag==0)
                            {


                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );

                                en_buyuk=en_kucuk;
                                for(int n=0; n<satir; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satir; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );

                                        if(en_buyuk<=tut)
                                        {
                                            en_buyuk=tut;
                                            en_buyuk_nokta[0][0]=xdeger[n];
                                            en_buyuk_nokta[0][1]=ydeger[n];
                                            en_buyuk_nokta[0][2]=zdeger[n];
                                            en_buyuk_nokta[1][0]=xdeger[m];
                                            en_buyuk_nokta[1][1]=ydeger[m];
                                            en_buyuk_nokta[1][2]=zdeger[m];
                                            en_buyuk_renk[0][0]=rdeger[n];
                                            en_buyuk_renk[0][1]=gdeger[n];
                                            en_buyuk_renk[0][2]=bdeger[n];
                                            en_buyuk_renk[1][0]=rdeger[m];
                                            en_buyuk_renk[1][1]=gdeger[m];
                                            en_buyuk_renk[1][2]=bdeger[m];

                                        }

                                        if(en_kucuk>=tut)
                                        {
                                            en_kucuk=tut;
                                            en_kucuk_nokta[0][0]=xdeger[n];//en kucuk x noktasini 1 matrisin x ine atadim
                                            en_kucuk_nokta[0][1]=ydeger[n];
                                            en_kucuk_nokta[0][2]=zdeger[n];
                                            en_kucuk_nokta[1][0]=xdeger[m];
                                            en_kucuk_nokta[1][1]=ydeger[m];
                                            en_kucuk_nokta[1][2]=zdeger[m];
                                            en_kucuk_renk[0][0]=rdeger[n];
                                            en_kucuk_renk[0][1]=rdeger[n];
                                            en_kucuk_renk[0][2]=rdeger[n];
                                            en_kucuk_renk[1][0]=rdeger[n];
                                            en_kucuk_renk[1][1]=rdeger[n];
                                            en_kucuk_renk[1][2]=rdeger[n];

                                        }


                                    }

                                }


                                fprintf(output,"\n--------------------------------------------\n");
                                fprintf(output,"Two points with the greatest distance between in %s binary file",fileName);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_buyuk_nokta[0][0],en_buyuk_nokta[0][1],en_buyuk_nokta[0][2],en_buyuk_renk[0][0],en_buyuk_renk[0][1],en_buyuk_renk[0][2]);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_buyuk_nokta[1][0],en_buyuk_nokta[1][1],en_buyuk_nokta[1][2],en_buyuk_renk[1][0],en_buyuk_renk[1][1],en_buyuk_renk[1][2]);
                                fprintf(output,"--------------------------------------------\n");
                                fprintf(output,"Two points with the least distance between in %s binary file",fileName);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_kucuk_nokta[0][0],en_kucuk_nokta[0][1],en_kucuk_nokta[0][2],en_kucuk_renk[0][0],en_kucuk_renk[0][1],en_kucuk_renk[0][2]);
                                fprintf(output,"\n%f %f %f %d %d %d \n",en_kucuk_nokta[1][0],en_kucuk_nokta[1][1],en_kucuk_nokta[1][2],en_kucuk_renk[1][0],en_kucuk_renk[1][1],en_kucuk_renk[1][2]);
                                fprintf(output,"---------------------------------------------\n");





                                fclose(fptr2);

                            }

                        }




                        //do here whatever you want to do..

                        fclose(fptr);
                    }
                }

            }

        }
        while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }

}
void secim_5()
{

    fprintf(output,"\n\n\n\n************************************\n\n\n\n");

    fprintf(output,"\n\n <<<<<<<<<<<<<<<<<< CHOICE 5>>>>>>>>>>>>>>>>>> \n\n");

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    struct noktalar1 nkt;

    //Dont' forget you change this directory with one you want to traverse!
    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];
    TCHAR fileName[MAX_PATH];

    char arr[100];
    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }
    else
    {
        do
        {
            //ignore current and parent directories
            if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;

            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }
            else
            {
                //list the Files


                memset(fileName, 0x00, sizeof(fileName));
                _stprintf(fileName, TEXT("%s\\%s"), directory, FindFileData.cFileName);
                FILE *fptr = fopen(fileName, "r");

                char karakter;
                int satirsayisi=1;
                char alanlar[100];
                char noktalar[100];
                char data_format[100];
                while(feof(fptr)==NULL)//dosya sonuna gelene kadar
                {
                    karakter=fgetc(fptr);
                    if(karakter=='\n')
                    {
                        satirsayisi++;
                        if(satirsayisi==3)
                        {
                            fgets(alanlar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==4)
                        {
                            fgets(noktalar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==5)
                        {
                            fgets(data_format,100,fptr);

                            satirsayisi++;
                        }

                    }


                }

                int ilk_kosul=0;
                if(alanlar[8]=='x'&&alanlar[10]=='y'&&alanlar[12]=='z')
                {
                    ilk_kosul=0;
                    if(alanlar[14]!='\0')
                    {
                        if(alanlar[14]!='r'||alanlar[16]!='g'||alanlar[18]!='b')
                        {
                            ilk_kosul=1;
                        }
                    }
                }
                else
                {
                    ilk_kosul=1;
                }

                if(ilk_kosul==0)
                {
                    int kordinat_sayisi;
                    char data_tipi_ascii[]= "DATA ascii\n";//FGETS FONKSİYONU ALT SATIRA GECMEYİDE BİR KARAKTER OLARAK ALİYOR
                    int sonuc=(strcmp(data_format,data_tipi_ascii));

                    kordinat_sayisi=alanlar_bosluk(alanlar);

                    char arr[satirsayisi];
                    int flag=0;
                    if(sonuc==0) // EGER DATA ASCİİ MODUNDA İSE
                    {

                        FILE *fptr1 = fopen(fileName, "r");
                        char *str, *str2;
                        str = fgets(arr,100,fptr1);// ilk satýrý aldým
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);





                        int istenen_nokta_sayisi=atoi(9+noktalar);

                        int noktalarinKordinatSayisi=0;
                        if(satirsayisi-5!=istenen_nokta_sayisi)
                        {

                            flag=1;
                        }
                        else
                        {
                            int j;
                            for(j=0; j<satirsayisi-6 ; j++)
                            {
                                str=fgets(arr,100,fptr1);
                                noktalarinKordinatSayisi=kordinatSayisiBul(str);
                                if(noktalarinKordinatSayisi!=kordinat_sayisi)
                                {

                                    flag=1;

                                }
                            }

                        }
                        if(flag==0)
                        {

                            if(kordinat_sayisi==6)
                            {
                                FILE *fptr2 = fopen(fileName, "r");
                                struct noktalar nkt;
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                int rdeger[satirsayisi-5];
                                int gdeger[satirsayisi-5];
                                int bdeger[satirsayisi-5];
                                char arr1[satirsayisi];
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr2,"%f %f %f %d %d %d",&nkt.x,&nkt.y,&nkt.z,&nkt.r,&nkt.g,&nkt.b);
                                    xdeger[i]=nkt.x;
                                    ydeger[i]=nkt.y;
                                    zdeger[i]=nkt.z;
                                    rdeger[i]=nkt.r;
                                    gdeger[i]=nkt.g;
                                    bdeger[i]=nkt.b;
                                }

                                float tut;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                for(int n=0; n<satirsayisi-5; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satirsayisi-5; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );
                                        toplam_uzaklik=toplam_uzaklik+tut;
                                    }

                                }
                                uzaklik_ort=toplam_uzaklik/((satirsayisi-5)*(satirsayisi-6)/2);//sayilrdan sonraki satir sayisi /1 eksigi;
                                fprintf(output,"\n %s file \n",fileName);

                                fprintf(output,"\n Average ditance(between all of the points) is %f\n",uzaklik_ort);
                                fprintf(output,"\n\n---------------------------------------\n\n");




                                fclose(fptr2);
                            }


                            if(kordinat_sayisi==3)
                            {



                                struct noktalar1 nkt;
                                FILE *fptr3 = fopen(fileName, "r");
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                char arr2[satirsayisi];
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr3,"%f %f %f ",&nkt.x1,&nkt.y1,&nkt.z1);
                                    xdeger[i]=nkt.x1;
                                    ydeger[i]=nkt.y1;
                                    zdeger[i]=nkt.z1;

                                }

                                double tut;
                                double toplam_uzaklik=0;
                                double uzaklik_ort=0;
                                for(int n=0; n<satirsayisi-5; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satirsayisi-5; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );
                                        toplam_uzaklik=toplam_uzaklik+tut;

                                    }

                                }


                                uzaklik_ort=toplam_uzaklik/(((double)(satirsayisi-5)*(satirsayisi-6))/2);//yazzilardan sonraki satir sayisi /1 eksigi;
                                fprintf(output,"\n %s file \n",fileName);

                                fprintf(output,"\n Average ditance(between all of the points) is %f\n",uzaklik_ort);
                                fprintf(output,"\n\n-----------------------------------------\n\n");

                                fclose(fptr3);
                            }

                        }




                        //do here whatever you want to do..
                        fclose(fptr);
                        fclose(fptr1);
                    }
                    else//BİNARY MODUNDA ACİP AYNİ İSLEMLERİ YAPICAK
                    {

                        if(kordinat_sayisi==3)
                        {

                            FILE *fptr1 = fopen(fileName, "rb");


                            //char *str, *str2;
                            fgets(arr,100,fptr1);// ilk satýrý aldým
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar1 nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];

                            while(!feof(fptr1))
                            {
                                fread(&nkt[say],sizeof(struct noktalar1),1,fptr1);
                                xdeger[say]=nkt[say].x1;
                                ydeger[say]=nkt[say].y1;
                                zdeger[say]=nkt[say].z1;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {

                                flag=1;
                            }
                            fclose(fptr3);
                            if(flag==0)
                            {

                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );
                                en_buyuk=en_kucuk;

                                for(int n=0; n<satir; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satir; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );
                                        toplam_uzaklik=toplam_uzaklik+tut;

                                    }

                                }
                                uzaklik_ort=toplam_uzaklik/(((double) (satir)*(satir-1)/2));
                                fprintf(output,"\n %s file \n",fileName);

                                fprintf(output,"\n Average ditance(between all of the points) is %f\n",uzaklik_ort);
                                fprintf(output,"\n\n--------------------------------------\n\n");


                            }


                        }
                        if(kordinat_sayisi==6)
                        {
                            FILE *fptr2;
                            fptr2=fopen(fileName,"rb");
                            char arr1[1000];
                            fgets(arr1,100,fptr2);// ilk satýrý aldým
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];
                            int rdeger[10000];
                            int gdeger[10000];
                            int bdeger[10000];
                            while(!feof(fptr2))
                            {
                                fread(&nkt[say],sizeof(struct noktalar),1,fptr2);
                                xdeger[say]=nkt[say].x;
                                ydeger[say]=nkt[say].y;
                                zdeger[say]=nkt[say].z;
                                rdeger[say]=nkt[say].r;
                                gdeger[say]=nkt[say].g;
                                bdeger[say]=nkt[say].b;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {

                                flag=1;
                            }

                            if(flag==0)
                            {


                                float tut;
                                float en_kucuk;
                                float en_buyuk=0;
                                float toplam_uzaklik=0;
                                float uzaklik_ort=0;
                                en_kucuk=sqrt( pow((xdeger[0]-xdeger[1]),2)+ pow((ydeger[0]-ydeger[1]),2)+pow((zdeger[0]-zdeger[1]),2) );

                                en_buyuk=en_kucuk;
                                for(int n=0; n<satir; n++)
                                {
                                    tut=0;
                                    for(int m=n+1; m<satir; m++)
                                    {
                                        tut =sqrt( pow((xdeger[n]-xdeger[m]),2)+ pow((ydeger[n]-ydeger[m]),2)+pow((zdeger[n]-zdeger[m]),2) );
                                        toplam_uzaklik=toplam_uzaklik+tut;

                                    }
                                }
                                uzaklik_ort=toplam_uzaklik/(((double) (satir)*(satir-1)/2));
                                fprintf(output,"\n %s file \n",fileName);

                                fprintf(output,"\n Average ditance(between all of the points) is %f \n",uzaklik_ort);
                                fprintf(output,"\n\n-------------------------------------------\n\n");
                                fclose(fptr2);

                            }

                        }




                        //do here whatever you want to do..

                        fclose(fptr);
                    }

                }
            }

        }
        while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
}
void secim_4()
{
    fprintf(output,"\n\n\n\n************************************\n\n\n\n");

    fprintf(output,"\n\n <<<<<<<<<<<<<<<<<<<<<CHOICE 4 >>>>>>>>>>>>>>>>>>>>>>>\n\n");
    float x=0,y=0,z=0;
    float yaricap=0;
    float girilenler[3];
    printf("Enter the center coordinate of the sphere(x y z):");
    scanf("%f",&x);
    scanf("%f",&y);
    scanf("%f",&z);
    printf("Enter the radius of the sphere:");
    scanf("%f",&yaricap);
    fprintf(output,"\n Given coordinates from user cx= %f cy= %f cz= %f cr=%f \n",x,y,z,yaricap);
    girilenler[0]=x;
    girilenler[1]=y;
    girilenler[2]=z;
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    //struct noktalar1 nkt;

    //Dont' forget you change this directory with one you want to traverse!
    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];
    TCHAR fileName[MAX_PATH];
    char arr[100];
    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }
    else
    {
        do
        {
            //ignore current and parent directories
            if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;

            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }
            else
            {
                //list the Files


                memset(fileName, 0x00, sizeof(fileName));
                _stprintf(fileName, TEXT("%s\\%s"), directory, FindFileData.cFileName);
                FILE *fptr = fopen(fileName, "r");

                char karakter;
                int satirsayisi=1;
                char alanlar[100];
                char noktalar[100];
                char data_format[100];
                while(feof(fptr)==NULL)//dosya sonuna gelene kadar
                {
                    karakter=fgetc(fptr);
                    if(karakter=='\n')
                    {
                        satirsayisi++;
                        if(satirsayisi==3)
                        {
                            fgets(alanlar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==4)
                        {
                            fgets(noktalar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==5)
                        {
                            fgets(data_format,100,fptr);
                            satirsayisi++;
                        }

                    }


                }

                int ilk_kosul=0;
                if(alanlar[8]=='x'&&alanlar[10]=='y'&&alanlar[12]=='z')
                {
                    ilk_kosul=0;
                    if(alanlar[14]!='\0')
                    {
                        if(alanlar[14]!='r'||alanlar[16]!='g'||alanlar[18]!='b')
                        {
                            ilk_kosul=1;
                        }
                    }
                }
                else
                {
                    ilk_kosul=1;
                }

                if(ilk_kosul==0)
                {

                    int kordinat_sayisi;
                    char data_tipi_ascii[]= "DATA ascii\n";//FGETS FONKSİYONU ALT SATIRA GECMEYİDE BİR KARAKTER OLARAK ALİYOR
                    int sonuc=(strcmp(data_format,data_tipi_ascii));

                    kordinat_sayisi=alanlar_bosluk(alanlar);

                    char arr[satirsayisi];
                    int flag=0;
                    if(sonuc==0) // EGER DATA ASCİİ MODUNDA İSE
                    {

                        FILE *fptr1 = fopen(fileName, "r");
                        char *str, *str2;
                        str = fgets(arr,100,fptr1);// ilk satýrý aldým
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);



                        int noktalarinKordinatSayisi=0;

                        int istenen_nokta_sayisi=atoi(9+noktalar);

                        if(satirsayisi-5!=istenen_nokta_sayisi)
                        {

                            flag=1;
                        }
                        else
                        {
                            int j;
                            for(int j=0; j<satirsayisi-6 ; j++)
                            {
                                str=fgets(arr,100,fptr1);
                                noktalarinKordinatSayisi=kordinatSayisiBul(str);
                                if(noktalarinKordinatSayisi!=kordinat_sayisi)
                                {

                                    flag=1;

                                }
                            }

                        }






                        if(flag==0)
                        {
                            if(kordinat_sayisi==6)
                            {
                                FILE *fptr2 = fopen(fileName, "r");
                                struct noktalar nkt;
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                int rdeger[satirsayisi-5];
                                int gdeger[satirsayisi-5];
                                int bdeger[satirsayisi-5];
                                char arr1[satirsayisi];
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr2,"%f %f %f %d %d %d",&nkt.x,&nkt.y,&nkt.z,&nkt.r,&nkt.g,&nkt.b);
                                    xdeger[i]=nkt.x;
                                    ydeger[i]=nkt.y;
                                    zdeger[i]=nkt.z;
                                    rdeger[i]=nkt.r;
                                    gdeger[i]=nkt.g;
                                    bdeger[i]=nkt.b;
                                }

                                float tut;
                                tut=0;
                                fprintf(output,"Points inside the sphere in %s file \n ",fileName);
                                fprintf(output,"%s\n ",alanlar);
                                fprintf(output,"%s\n ",noktalar);
                                fprintf(output,"%s\n ",data_format);
                                for(int m=0; m<satirsayisi-5; m++)
                                {
                                    tut =sqrt( pow((xdeger[m]-girilenler[0]),2)+ pow((ydeger[m]-girilenler[1]),2)+pow((zdeger[m]-girilenler[0]),2) );
                                    if(tut<=yaricap)
                                    {

                                        fprintf(output,"\n %f %f %f %d %d %d\n",xdeger[m],ydeger[m],zdeger[m],rdeger[m],gdeger[m],bdeger[m]);
                                    }

                                }
                                fprintf(output,"\n\n--------------------------------------------------\n\n");


                                fclose(fptr2);

                            }


                            if(kordinat_sayisi==3)
                            {



                                struct noktalar1 nkt;
                                FILE *fptr3 = fopen(fileName, "r");
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                char arr2[satirsayisi];
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr3,"%f %f %f ",&nkt.x1,&nkt.y1,&nkt.z1);
                                    xdeger[i]=nkt.x1;
                                    ydeger[i]=nkt.y1;
                                    zdeger[i]=nkt.z1;

                                }

                                float tut;
                                tut=0;
                                fprintf(output,"Points inside the sphere in %s file \n ",fileName);
                                fprintf(output,"%s\n ",alanlar);
                                fprintf(output,"%s\n ",noktalar);
                                fprintf(output,"%s\n ",data_format);
                                for(int m=0; m<satirsayisi-5; m++)
                                {
                                    tut =sqrt( pow((xdeger[m]-girilenler[0]),2)+ pow((ydeger[m]-girilenler[1]),2)+pow((zdeger[m]-girilenler[0]),2) );
                                    if(tut<=yaricap)
                                    {

                                        fprintf(output,"\n %f %f %f\n",xdeger[m],ydeger[m],zdeger[m]);
                                    }



                                }
                                fprintf(output,"\n\n--------------------------------------------------\n\n");

                                fclose(fptr3);

                            }

                        }




                        //do here whatever you want to do..
                        fclose(fptr);
                        fclose(fptr1);
                    }
                    else//BİNARY MODUNDA ACİP AYNİ İSLEMLERİ YAPICAK
                    {

                        if(kordinat_sayisi==3)
                        {

                            FILE *fptr1 = fopen(fileName, "rb");


                            //char *str, *str2;
                            fgets(arr,100,fptr1);// ilk satýrý aldým
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar1 nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];

                            while(!feof(fptr1))
                            {
                                fread(&nkt[say],sizeof(struct noktalar1),1,fptr1);
                                xdeger[say]=nkt[say].x1;
                                ydeger[say]=nkt[say].y1;
                                zdeger[say]=nkt[say].z1;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {


                                flag=1;
                            }
                            fclose(fptr3);
                            if(flag==0)
                            {

                                float tut=0;
                                fprintf(output,"Points inside the sphere in %s file \n ",fileName);
                                fprintf(output,"%s\n ",alanlar);
                                fprintf(output,"%s\n ",noktalar);
                                fprintf(output,"%s\n ",data_format);
                                for(int m=0; m<satir; m++)
                                {
                                    tut =sqrt( pow((xdeger[m]-girilenler[0]),2)+ pow((ydeger[m]-girilenler[1]),2)+pow((zdeger[m]-girilenler[0]),2) );
                                    if(tut<=yaricap)
                                    {

                                        fprintf(output,"\n %f %f %f\n",xdeger[m],ydeger[m],zdeger[m]);
                                    }

                                }
                                fprintf(output,"\n\n--------------------------------------------------\n\n");

                            }


                        }
                        if(kordinat_sayisi==6)
                        {
                            FILE *fptr2;
                            fptr2=fopen(fileName,"rb");
                            char arr1[1000];
                            fgets(arr1,100,fptr2);// ilk satýrý aldým
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            int istenen_nokta_sayisi=atoi(9+noktalar);


                            struct noktalar nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];
                            int rdeger[10000];
                            int gdeger[10000];
                            int bdeger[10000];
                            while(!feof(fptr2))
                            {
                                fread(&nkt[say],sizeof(struct noktalar),1,fptr2);
                                xdeger[say]=nkt[say].x;
                                ydeger[say]=nkt[say].y;
                                zdeger[say]=nkt[say].z;
                                rdeger[say]=nkt[say].r;
                                gdeger[say]=nkt[say].g;
                                bdeger[say]=nkt[say].b;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {


                                flag=1;
                            }

                            if(flag==0)
                            {

                                float tut=0;
                                fprintf(output,"Points inside the sphere in %s file\n ",fileName);
                                fprintf(output,"%s\n ",alanlar);
                                fprintf(output,"%s\n ",noktalar);
                                fprintf(output,"%s\n ",data_format);
                                for(int m=0; m<satir; m++)
                                {
                                    tut =sqrt( pow((xdeger[m]-girilenler[0]),2)+ pow((ydeger[m]-girilenler[1]),2)+pow((zdeger[m]-girilenler[0]),2) );
                                    if(tut<=yaricap)
                                    {

                                        fprintf(output,"%f %f %f %d %d %d \n",xdeger[m],ydeger[m],zdeger[m],rdeger[m],gdeger[m],bdeger[m]); //hata
                                    }

                                }
                                fprintf(output,"\n\n -------------------------------------------------- \n\n");


                            }

                        }




                        //do here whatever you want to do..

                        fclose(fptr);
                    }

                }
            }

        }
        while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
}
void secim_3()
{

    float kup_kordinat[8][3];


    fprintf(output,"\n\n\n\n************************************\n\n\n\n");
    fprintf(output,"\n\n<<<<<<<<<<<<<<<<<<<<< CHOICE 3 >>>>>>>>>>>>>>>>>>>>>\n\n");

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    struct noktalar1 nkt;

    //Dont' forget you change this directory with one you want to traverse!
    TCHAR  *directory = TEXT(".");
    TCHAR patter[MAX_PATH];
    TCHAR fileName[MAX_PATH];
    char arr[100];
    memset(patter, 0x00, MAX_PATH);
    _stprintf(patter, TEXT("%s\\*.nkt"), directory);
    hFind = FindFirstFile(patter, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed (%d)\n", GetLastError());
        return 1;
    }
    else
    {
        do
        {
            //ignore current and parent directories
            if(_tcscmp(FindFileData.cFileName, TEXT("."))==0 || _tcscmp(FindFileData.cFileName, TEXT(".."))==0)
                continue;

            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                //ignore directories
            }
            else
            {
                //list the Files


                memset(fileName, 0x00, sizeof(fileName));
                _stprintf(fileName, TEXT("%s\\%s"), directory, FindFileData.cFileName);
                FILE *fptr = fopen(fileName, "r");

                char karakter;
                int satirsayisi=1;
                char alanlar[100];
                char noktalar[100];
                char data_format[100];
                while(feof(fptr)==NULL)//dosya sonuna gelene kadar
                {
                    karakter=fgetc(fptr);
                    if(karakter=='\n')
                    {
                        satirsayisi++;
                        if(satirsayisi==3)
                        {
                            fgets(alanlar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==4)
                        {
                            fgets(noktalar,100,fptr);
                            satirsayisi++;

                        }
                        if(satirsayisi==5)
                        {
                            fgets(data_format,100,fptr);

                            satirsayisi++;
                        }

                    }


                }
                int ilk_kosul=0;
                if(alanlar[8]=='x'&&alanlar[10]=='y'&&alanlar[12]=='z')
                {
                    ilk_kosul=0;
                    if(alanlar[14]!='\0')
                    {
                        if(alanlar[14]!='r'||alanlar[16]!='g'||alanlar[18]!='b')
                        {
                            ilk_kosul=1;
                        }
                    }
                }
                else
                {
                    ilk_kosul=1;
                }

                if(ilk_kosul==0)
                {
                    int kordinat_sayisi;
                    char data_tipi_ascii[]= "DATA ascii\n";//FGETS FONKSİYONU ALT SATIRA GECMEYİDE BİR KARAKTER OLARAK ALİYOR
                    int sonuc=(strcmp(data_format,data_tipi_ascii));

                    kordinat_sayisi=alanlar_bosluk(alanlar);

                    char arr[satirsayisi];
                    int flag=0;
                    if(sonuc==0) // EGER DATA ASCİİ MODUNDA İSE
                    {

                        FILE *fptr1 = fopen(fileName, "r");
                        char *str, *str2;
                        str = fgets(arr,100,fptr1);// ilk satýrý aldým
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);
                        str = fgets(arr,100,fptr1);


                        int istenen_nokta_sayisi=atoi(9+noktalar);
                        int noktalarinKordinatSayisi=0;



                        if(satirsayisi-5!=istenen_nokta_sayisi)
                        {


                            flag=1;

                        }



                        if(flag==0)
                        {
                            if(kordinat_sayisi==6)
                            {
                                FILE *fptr2 = fopen(fileName, "r");
                                struct noktalar nkt;
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                int rdeger[satirsayisi-5];
                                int gdeger[satirsayisi-5];
                                int bdeger[satirsayisi-5];
                                char arr1[satirsayisi];
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                fgets(arr1,100,fptr2);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr2,"%f %f %f %d %d %d",&nkt.x,&nkt.y,&nkt.z,&nkt.r,&nkt.g,&nkt.b);
                                    xdeger[i]=nkt.x;
                                    ydeger[i]=nkt.y;
                                    zdeger[i]=nkt.z;
                                    rdeger[i]=nkt.r;
                                    gdeger[i]=nkt.g;
                                    bdeger[i]=nkt.b;
                                }

                                double en_buyuk_sayi=0;
                                double buyukler[3];
                                double enBuyukKordinat=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(xdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=xdeger[i];
                                        buyukler[0]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(ydeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=ydeger[i];
                                        buyukler[1]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;

                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(zdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=zdeger[i];
                                        buyukler[2]=en_buyuk_sayi;
                                    }
                                }

                                en_buyuk_sayi=buyukler[0];
                                for(int i=1; i<3; i++)
                                {
                                    if(buyukler[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=buyukler[i];
                                    }
                                }

                                kup_kordinat[0][0]=0;
                                kup_kordinat[0][1]=0;
                                kup_kordinat[0][2]=0;

                                kup_kordinat[1][0]=0;
                                kup_kordinat[1][1]=en_buyuk_sayi;
                                kup_kordinat[1][2]=0;

                                kup_kordinat[2][0]=en_buyuk_sayi;
                                kup_kordinat[2][1]=0;
                                kup_kordinat[2][2]=0;

                                kup_kordinat[3][0]=en_buyuk_sayi;
                                kup_kordinat[3][1]=en_buyuk_sayi;
                                kup_kordinat[3][2]=0;

                                kup_kordinat[4][0]=en_buyuk_sayi;
                                kup_kordinat[4][1]=en_buyuk_sayi;
                                kup_kordinat[4][2]=en_buyuk_sayi;

                                kup_kordinat[5][0]=0;
                                kup_kordinat[5][1]=0;
                                kup_kordinat[5][2]=en_buyuk_sayi;

                                kup_kordinat[6][0]=0;
                                kup_kordinat[6][1]=en_buyuk_sayi;
                                kup_kordinat[6][2]=en_buyuk_sayi;

                                kup_kordinat[7][0]=en_buyuk_sayi;
                                kup_kordinat[7][1]=0;
                                kup_kordinat[7][2]=en_buyuk_sayi;
                                fprintf(output,"\nCoordinates of the smallest cube from %s file \n ",fileName);
                                for(int m=0; m<8; m++)
                                {
                                    for(int n=0; n<3; n++)
                                    {

                                        fprintf(output,"%lf ",kup_kordinat[m][n]);
                                    }

                                    fprintf(output,"\n");
                                }
                                fprintf(output,"\n ----------------------------------- \n");
                                fclose(fptr2);

                            }


                            if(kordinat_sayisi==3)
                            {



                                struct noktalar1 nkt;
                                FILE *fptr3 = fopen(fileName, "r");
                                float xdeger[satirsayisi-5];
                                float ydeger[satirsayisi-5];
                                float zdeger[satirsayisi-5];
                                char arr2[satirsayisi];
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                fgets(arr2,100,fptr3);
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    fscanf(fptr3,"%f %f %f ",&nkt.x1,&nkt.y1,&nkt.z1);
                                    xdeger[i]=nkt.x1;
                                    ydeger[i]=nkt.y1;
                                    zdeger[i]=nkt.z1;

                                }

                                double en_buyuk_sayi=0;
                                double buyukler[3];
                                double enBuyukKordinat=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(xdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=xdeger[i];
                                        buyukler[0]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(ydeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=ydeger[i];
                                        buyukler[1]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;

                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(zdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=zdeger[i];
                                        buyukler[2]=en_buyuk_sayi;
                                    }
                                }

                                en_buyuk_sayi=buyukler[0];
                                for(int i=1; i<3; i++)
                                {
                                    if(buyukler[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=buyukler[i];
                                    }
                                }

                                kup_kordinat[0][0]=0;
                                kup_kordinat[0][1]=0;
                                kup_kordinat[0][2]=0;

                                kup_kordinat[1][0]=0;
                                kup_kordinat[1][1]=en_buyuk_sayi;
                                kup_kordinat[1][2]=0;

                                kup_kordinat[2][0]=en_buyuk_sayi;
                                kup_kordinat[2][1]=0;
                                kup_kordinat[2][2]=0;

                                kup_kordinat[3][0]=en_buyuk_sayi;
                                kup_kordinat[3][1]=en_buyuk_sayi;
                                kup_kordinat[3][2]=0;

                                kup_kordinat[4][0]=en_buyuk_sayi;
                                kup_kordinat[4][1]=en_buyuk_sayi;
                                kup_kordinat[4][2]=en_buyuk_sayi;

                                kup_kordinat[5][0]=0;
                                kup_kordinat[5][1]=0;
                                kup_kordinat[5][2]=en_buyuk_sayi;

                                kup_kordinat[6][0]=0;
                                kup_kordinat[6][1]=en_buyuk_sayi;
                                kup_kordinat[6][2]=en_buyuk_sayi;

                                kup_kordinat[7][0]=en_buyuk_sayi;
                                kup_kordinat[7][1]=0;
                                kup_kordinat[7][2]=en_buyuk_sayi;
                                fprintf(output,"\nCoordinates of the smallest cube from %s file\n",fileName);
                                for(int m=0; m<8; m++)
                                {
                                    for(int n=0; n<3; n++)
                                    {

                                        fprintf(output,"%lf ",kup_kordinat[m][n]);
                                    }

                                    fprintf(output,"\n");
                                }
                                fprintf(output,"\n --------------------------------------\n");


                                fclose(fptr3);

                            }



                        }




                        //do here whatever you want to do..
                        fclose(fptr);
                        fclose(fptr1);
                    }
                    else//BİNARY MODUNDA ACİP AYNİ İSLEMLERİ YAPICAK
                    {

                        if(kordinat_sayisi==3)
                        {

                            FILE *fptr1 = fopen(fileName, "rb");


                            //char *str, *str2;
                            fgets(arr,100,fptr1);// ilk satýrý aldým
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            fgets(arr,100,fptr1);
                            int istenen_nokta_sayisi=atoi(9+noktalar);

                            printf(" "); //hata veren kisim
                            struct noktalar1 nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];

                            while(!feof(fptr1))
                            {
                                fread(&nkt[say],sizeof(struct noktalar1),1,fptr1);
                                xdeger[say]=nkt[say].x1;
                                ydeger[say]=nkt[say].y1;
                                zdeger[say]=nkt[say].z1;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {


                                flag=1;
                            }
                            fclose(fptr3);
                            if(flag==0)
                            {

                                double en_buyuk_sayi=0;
                                double buyukler[3];
                                double enBuyukKordinat=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(xdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=xdeger[i];
                                        buyukler[0]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(ydeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=ydeger[i];
                                        buyukler[1]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;

                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(zdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=zdeger[i];
                                        buyukler[2]=en_buyuk_sayi;
                                    }
                                }

                                en_buyuk_sayi=buyukler[0];
                                for(int i=1; i<3; i++)
                                {
                                    if(buyukler[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=buyukler[i];
                                    }
                                }

                                kup_kordinat[0][0]=0;
                                kup_kordinat[0][1]=0;
                                kup_kordinat[0][2]=0;

                                kup_kordinat[1][0]=0;
                                kup_kordinat[1][1]=en_buyuk_sayi;
                                kup_kordinat[1][2]=0;

                                kup_kordinat[2][0]=en_buyuk_sayi;
                                kup_kordinat[2][1]=0;
                                kup_kordinat[2][2]=0;

                                kup_kordinat[3][0]=en_buyuk_sayi;
                                kup_kordinat[3][1]=en_buyuk_sayi;
                                kup_kordinat[3][2]=0;

                                kup_kordinat[4][0]=en_buyuk_sayi;
                                kup_kordinat[4][1]=en_buyuk_sayi;
                                kup_kordinat[4][2]=en_buyuk_sayi;

                                kup_kordinat[5][0]=0;
                                kup_kordinat[5][1]=0;
                                kup_kordinat[5][2]=en_buyuk_sayi;

                                kup_kordinat[6][0]=0;
                                kup_kordinat[6][1]=en_buyuk_sayi;
                                kup_kordinat[6][2]=en_buyuk_sayi;

                                kup_kordinat[7][0]=en_buyuk_sayi;
                                kup_kordinat[7][1]=0;
                                kup_kordinat[7][2]=en_buyuk_sayi;
                                fprintf(output,"\n Coordinates of the smallest cube from %s file\n",fileName);
                                for(int m=0; m<8; m++)
                                {
                                    for(int n=0; n<3; n++)
                                    {

                                        fprintf(output,"%lf ",kup_kordinat[m][n]);
                                    }

                                    fprintf(output,"\n");
                                }
                                fprintf(output,"\n --------------------------------------\n");



                            }


                        }
                        if(kordinat_sayisi==6)
                        {
                            FILE *fptr2;
                            fptr2=fopen(fileName,"rb");
                            char arr1[1000];
                            fgets(arr1,100,fptr2);// ilk satýrý aldým
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            fgets(arr1,100,fptr2);
                            int istenen_nokta_sayisi=atoi(9+noktalar);

                            printf(" ");
                            struct noktalar nkt[10000];
                            int say=0;
                            FILE *fptr3 = fopen(fileName, "rb");
                            float xdeger[10000];
                            float ydeger[10000];
                            float zdeger[10000];
                            int rdeger[10000];
                            int gdeger[10000];
                            int bdeger[10000];
                            while(!feof(fptr2))
                            {
                                fread(&nkt[say],sizeof(struct noktalar),1,fptr2);
                                xdeger[say]=nkt[say].x;
                                ydeger[say]=nkt[say].y;
                                zdeger[say]=nkt[say].z;
                                rdeger[say]=nkt[say].r;
                                gdeger[say]=nkt[say].g;
                                bdeger[say]=nkt[say].b;
                                say++;
                            }
                            int satir=0;
                            satir=say-1;

                            if(istenen_nokta_sayisi!=satir)
                            {

                                flag=1;
                            }

                            if(flag==0)
                            {


                                double en_buyuk_sayi=0;
                                double buyukler[3];
                                double enBuyukKordinat=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(xdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=xdeger[i];
                                        buyukler[0]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;
                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(ydeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=ydeger[i];
                                        buyukler[1]=en_buyuk_sayi;
                                    }
                                }
                                en_buyuk_sayi=0;

                                for(int i=0; i<satirsayisi-5; i++)
                                {
                                    if(zdeger[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=zdeger[i];
                                        buyukler[2]=en_buyuk_sayi;
                                    }
                                }

                                en_buyuk_sayi=buyukler[0];
                                for(int i=1; i<3; i++)
                                {
                                    if(buyukler[i]>en_buyuk_sayi)
                                    {
                                        en_buyuk_sayi=buyukler[i];
                                    }
                                }

                                kup_kordinat[0][0]=0;
                                kup_kordinat[0][1]=0;
                                kup_kordinat[0][2]=0;

                                kup_kordinat[1][0]=0;
                                kup_kordinat[1][1]=en_buyuk_sayi;
                                kup_kordinat[1][2]=0;

                                kup_kordinat[2][0]=en_buyuk_sayi;
                                kup_kordinat[2][1]=0;
                                kup_kordinat[2][2]=0;

                                kup_kordinat[3][0]=en_buyuk_sayi;
                                kup_kordinat[3][1]=en_buyuk_sayi;
                                kup_kordinat[3][2]=0;

                                kup_kordinat[4][0]=en_buyuk_sayi;
                                kup_kordinat[4][1]=en_buyuk_sayi;
                                kup_kordinat[4][2]=en_buyuk_sayi;

                                kup_kordinat[5][0]=0;
                                kup_kordinat[5][1]=0;
                                kup_kordinat[5][2]=en_buyuk_sayi;

                                kup_kordinat[6][0]=0;
                                kup_kordinat[6][1]=en_buyuk_sayi;
                                kup_kordinat[6][2]=en_buyuk_sayi;

                                kup_kordinat[7][0]=en_buyuk_sayi;
                                kup_kordinat[7][1]=0;
                                kup_kordinat[7][2]=en_buyuk_sayi;
                                fprintf(output,"\nCoordinates of the smallest cube from %s file\n",fileName);
                                for(int m=0; m<8; m++)
                                {
                                    for(int n=0; n<3; n++)
                                    {

                                        fprintf(output,"%lf ",kup_kordinat[m][n]);
                                    }

                                    fprintf(output,"\n");
                                }
                                fprintf(output,"\n ----------------------------------------\n ");


                            }

                        }




                        //do here whatever you want to do..

                        fclose(fptr);


                    }

                }
            }

        }
        while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }



}
int main(int argc, char *argv[])
{
    output=fopen("output.nkt","a+");
    int secim=0;
    while(secim!=6)
    {

        printf("\nSecim 1 --> File Control \n");
        printf("SECIM 2 --> The farest and nearest points. \n");
        printf("SECIM 3 --> Coordinates of the smallest cube to take into all points. \n");
        printf("SECIM 4 --> Points inside the sphere where you will enter the coordinates\n");
        printf("SECIM 5 --> Average of all points distance from each other.\n");
        printf("SECIM 6 --> EXIT\n");
        printf("CHOICE:");
        scanf("%d",&secim);
        switch(secim)
        {
        case 1:
            secim_1();
            break;
        case 2:
            printf("This process take may take some time...");
            secim_2();
            break;
        case 3:
            secim_3();
            break;
        case 4:
            secim_4();
            break;
        case 5:
            printf("This process take may take some time...");
            secim_5();
            break;

        }

    }
    fclose(output);



    return 0;
}
